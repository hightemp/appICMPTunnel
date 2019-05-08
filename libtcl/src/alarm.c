/*
 * Copyright (C) 2014 Google, Inc.
 * Copyright (C) 2018 Tempow
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "alarm"

#include "tcl/alarm.h"
#include "tcl/log.h"
#include "tcl/task.h"
#include "tcl/string.h"

#define WORKER_RT_PRIORITY 1
#define CALLBACK_WORKER_PRIORITY_HIGH (-19)

static bool __init_started = false;
static bool __is_ready = false;
static LIST_HEAD(__alarms);
static mutex_t __alarms_mutex;
static bool __dispatcher_active = false;
static task_t __dispatcher;
static worker_t __default_callback_worker;
static blocking_queue_t __default_callback_queue;
static sema_t __alarm_expired;

#ifdef HAS_TIMER
static timer_t __timer = NULL;
static timer_t __wakeup_timer = NULL;
static bool __timer_set = false;

static bool __timer_init(const clockid_t clock_id, timer_t *timer)
{
	struct sigevent sigevent;
	pthread_attr_t thread_attr;
	struct sched_param param;

	pthread_attr_init(&thread_attr);
	pthread_attr_setschedpolicy(&thread_attr, SCHED_FIFO);
	param.sched_priority = THREAD_RT_PRIORITY;
	pthread_attr_setschedparam(&thread_attr, &param);
	bzero(&sigevent, sizeof(sigevent));
	sigevent.sigev_notify = SIGEV_THREAD;
	sigevent.sigev_notify_function = (void (*)(union sigval))sema_post;
	sigevent.sigev_notify_attributes = &thread_attr;
	sigevent.sigev_value.sival_ptr = &__alarm_expired;
	if (timer_create(clock_id, &sigevent, timer) == -1) {
		LOG_ERROR("Unable to create timer with clock %d: %s", clock_id,
			strerror(errno));
		if (clock_id == CLOCK_BOOTTIME_ALARM) {
			LOG_ERROR("The kernel might not have support for "
				"timer_create(CLOCK_BOOTTIME_ALARM): "
				"https://lwn.net/Articles/429925/"
				"See following patches: "
				"https://git.kernel.org/cgit/linux/kernel/git/torvalds/"
				"linux.git/log/?qt=grep&q=CLOCK_BOOTTIME_ALARM");
		}
		return false;
	}
	return true;
}
#endif

FORCEINLINE
static bool __schedule_needed(alarm_t *alarm)
{
	return !list_empty(&__alarms) &&
		list_first_entry(&__alarms, alarm_t, list_alarm) == alarm;
}

FORCEINLINE
static void __unschedule(alarm_t *alarm)
{
	list_del_init(&alarm->list_alarm);
	if (blocking_queue_remove(alarm->queue, alarm)) {
		/* We shouldn't have repeated alarm instances */
		assert(blocking_queue_remove(alarm->queue, alarm) == false);
	}
}

static void __schedule_root(void)
{
#ifdef HAS_TIMER
# ifdef HAS_WAKELOCK
	const bool timer_was_set = __timer_set;
# endif
	/* If used in a zeroed state, disarms the timer. */
	struct itimerspec timer_time;
	struct itimerspec end_of_time;
	struct itimerspec wakeup_time;
	struct itimerspec time_to_expire;
	period_ms_t next_deadline;
	int64_t next_expiration;

	bzero(&timer_time, sizeof(struct itimerspec));
	if (list_empty(&__alarms))
		goto done;

	next_deadline = list_first_entry(&__alarms, alarm_t, list_alarm)->deadline;
	next_expiration = (int64_t)(next_deadline - now());
	if (next_expiration < TIMER_INTERVAL_FOR_WAKELOCK_IN_MS) {
# ifdef HAS_WAKELOCK
		if (!__timer_set) {
			if (!wakelock_acquire()) {
				LOG_ERROR(LOG_TAG, "Unable to acquire wake lock");
				goto done;
			}
		}
# endif
		timer_time.it_value.tv_sec = (int64_t)(next_deadline / 1000);
		timer_time.it_value.tv_nsec = (int64_t)(next_deadline % 1000)
			* 1000000LL;
		bzero(&end_of_time, sizeof(end_of_time));
		end_of_time.it_value.tv_sec = (time_t)(1LL << (sizeof(time_t) * 8 - 2));
		if (__timer_settime(__wakeup_timer, TIMER_ABSTIME, &end_of_time,
			NULL) < 0)
			LOG_ERROR("Unable to set wakeup timer: %s", strerror(errno));
	} else {
		bzero(&wakeup_time, sizeof(wakeup_time));
		wakeup_time.it_value.tv_sec = (int64_t)(next_deadline / 1000);
		wakeup_time.it_value.tv_nsec = (int64_t)(next_deadline % 1000)
			* 1000000LL;
		if (__timer_settime(__wakeup_timer, TIMER_ABSTIME, &wakeup_time,
			NULL) < 0)
			LOG_ERROR("Unable to set wakeup timer: %s", strerror(errno));
	}
done:
	__timer_set = timer_time.it_value.tv_sec || timer_time.it_value.tv_nsec;
# ifdef HAS_WAKELOCK
	if (timer_was_set && !__timer_set) {
		wakelock_release();
	}
# endif
	if (__timer_settime(__timer, TIMER_ABSTIME, &timer_time, NULL) == -1)
		LOG_ERROR("Unable to set timer: %s", strerror(errno));
	if (__timer_set) {
		timer_gettime(__timer, &time_to_expire);
		if (!time_to_expire.it_value.tv_sec &&
			!time_to_expire.it_value.tv_nsec) {
			LOG_DEBUG("Alarm expiration too close for posix timers, "
				"switching to guns");
			sema_post(&__alarm_expired);
		}
	}
#endif
}

static void __schedule(alarm_t *alarm)
{
	period_ms_t just_now;
	period_ms_t ms_into_period;
	alarm_t *alarm_entry;

	if (alarm->callback) __unschedule(alarm);
	ms_into_period = 0;
	just_now = now();
	if (alarm->is_periodic && alarm->period != 0)
		ms_into_period = ((just_now - alarm->creation_time) % alarm->period);
	alarm->deadline = just_now + (alarm->period - ms_into_period);
	list_for_each_entry(alarm_entry, alarm_t, &__alarms, list_alarm) {
		if (alarm->deadline < alarm_entry->deadline) {
			list_add_tail(&alarm->list_alarm, &alarm_entry->list_alarm);
			return ;
		}
	}
	list_add_tail(&alarm->list_alarm, &__alarms);
}

static void *__schedule_loop(void *context)
{
	alarm_t *alarm;

	(void)context;
#ifdef HAS_TIMER
	sema_wait(&__alarm_expired);
#endif
	if (!__dispatcher_active) {
		task_stop(&__dispatcher);
		return NULL;
	}
	mutex_lock(&__alarms_mutex);
	if (list_empty(&__alarms) ||
		(alarm = list_first_entry(&__alarms, alarm_t, list_alarm))->deadline >
		now()) {
		__schedule_root();
		mutex_unlock(&__alarms_mutex);
		return NULL;
	}
	list_del_init(&alarm->list_alarm);
	if (alarm->is_periodic)
		__schedule(alarm);
	__schedule_root();
	if (alarm->callback)
		blocking_queue_push(alarm->queue, alarm);
	mutex_unlock(&__alarms_mutex);
	return NULL;
}

static void __dispatch_ready(blocking_queue_t *queue)
{
	alarm_t *alarm;
	proc_t *callback;
	void *data;

	if (!__dispatcher_active)
		return ;
	mutex_lock(&__alarms_mutex);
	if (!(alarm = (alarm_t *)blocking_queue_trypop(queue)) ||
		!alarm->callback) {
		/* Alarm was canceled, return */
		mutex_unlock(&__alarms_mutex);
		return;
	}

	/* If the alarm is not periodic, we've fully serviced it now, and can reset
	 * some of its internal state. This is useful to distinguish between expired
	 * __alarms and active ones.
	 */
	callback = alarm->callback;
	data = alarm->data;
	if (!alarm->is_periodic) {
		alarm->deadline = 0;
		alarm->callback = NULL;
		alarm->data = NULL;
	}
	mutex_lock(&alarm->callback_mutex);
	mutex_unlock(&__alarms_mutex);
	callback(data);
	mutex_unlock(&alarm->callback_mutex);
}

static bool __lazyinit(void)
{
	if (__is_ready) return (true);
	if (!__init_started) {
		__init_started = true;
		mutex_init(&__alarms_mutex);
	}
	mutex_lock(&__alarms_mutex);
	if (__is_ready) {
		mutex_unlock(&__alarms_mutex);
		return (true);
	}
#ifdef HAS_TIMER
	if (!__timer_init(CLOCK_ID, &__timer))
		goto error;
	if (!__timer_init(CLOCK_ID_ALARM, &__wakeup_timer))
		goto error1;
#endif
	if (sema_init(&__alarm_expired, 0))
		goto error2;
	if (worker_init(&__default_callback_worker, "alarm_default_callback"))
		goto error3;
	if (worker_setpriority(&__default_callback_worker,
		CALLBACK_WORKER_PRIORITY_HIGH))
		goto error4;
	if (blocking_queue_init(&__default_callback_queue, UINT32_MAX))
		goto error4;
	alarm_register(&__default_callback_queue, &__default_callback_worker);
	__dispatcher_active = true;
	if (task_repeat(&__dispatcher, __schedule_loop, NULL))
		goto error5;
	if (task_setpriority(&__dispatcher, CALLBACK_WORKER_PRIORITY_HIGH))
		goto error6;
	__is_ready = true;
	mutex_unlock(&__alarms_mutex);
	return true;
error6:
	task_destroy(&__dispatcher);
error5:
	__dispatcher_active = false;
	blocking_queue_destroy(&__default_callback_queue, NULL);
error4:
	worker_destroy(&__default_callback_worker);
error3:
	sema_destroy(&__alarm_expired);
error2:
#ifdef HAS_TIMER
	timer_delete(__wakeup_timer);
error1:
	timer_delete(__timer);
error:
#endif
	mutex_unlock(&__alarms_mutex);
	return false;
}

FORCEINLINE
static int __init(alarm_t *alarm, const char *name,
	bool is_periodic)
{
	if (!__lazyinit())
		return -1;

	bzero(alarm, sizeof(alarm_t));
	strncpy(alarm->name, name, ALARM_NAME_MAX - 1);
	if (mutex_init(&alarm->callback_mutex))
		return -1;
	INIT_LIST_HEAD(&alarm->list_alarm);
	alarm->is_periodic = is_periodic;
	alarm->is_set = false;
	return 0;
}

static void __cancel(alarm_t *alarm)
{
	const bool needs_reschedule = __schedule_needed(alarm);

	alarm->callback = NULL;
	__unschedule(alarm);
	alarm->deadline = 0;
	alarm->data = NULL;
	alarm->queue = NULL;
	alarm->is_set = false;
	if (needs_reschedule) __schedule_root();
}

int alarm_init(alarm_t *alarm, const char *name)
{
	return __init(alarm, name, false);
}

int alarm_init_periodic(alarm_t *alarm, const char *name)
{
	return __init(alarm, name, true);
}

FORCEINLINE
void alarm_destroy(alarm_t *alarm)
{
	alarm_cancel(alarm);
	mutex_destroy(&alarm->callback_mutex);
}

FORCEINLINE
bool alarm_is_scheduled(const alarm_t *alarm)
{
	return (alarm && alarm->callback);
}

FORCEINLINE
void alarm_cancel(alarm_t *alarm)
{
	mutex_lock(&__alarms_mutex);
	__cancel(alarm);
	mutex_unlock(&__alarms_mutex);
}

void alarm_attach(alarm_t *alarm, period_ms_t period, proc_t *cb, void *data,
	blocking_queue_t *queue)
{
	assert(!alarm->is_set);
	/* TODO(tempow) fix this, zero periodic */
	assert(!alarm->is_periodic || period >= 25);

	mutex_lock(&__alarms_mutex);
	alarm->creation_time = now();
	alarm->period = period;
	alarm->queue = queue;
	alarm->callback = cb;
	alarm->data = data;
	__schedule(alarm);
	alarm->is_set = true;
	if (__schedule_needed(alarm))
		__schedule_root();
	mutex_unlock(&__alarms_mutex);
}

FORCEINLINE
void alarm_set(alarm_t *alarm, period_ms_t period, proc_t *cb, void *data)
{
	alarm_attach(alarm, period, cb, data, &__default_callback_queue);
}

FORCEINLINE
void alarm_register(blocking_queue_t *queue, worker_t *worker)
{
	blocking_queue_listen(queue, worker, __dispatch_ready);
}

void alarm_unregister(blocking_queue_t *queue)
{
	alarm_t *alarm_entry;
	alarm_t *tmp;

	blocking_queue_unlisten(queue);

	/* Cancel all __alarms that are using this queue */
	mutex_lock(&__alarms_mutex);
	list_for_each_entry_safe(alarm_entry, tmp, alarm_t, &__alarms, list_alarm) {
		if (alarm_entry->queue == queue)
			__cancel(alarm_entry);
	}
	mutex_unlock(&__alarms_mutex);
}

void alarm_cleanup(void)
{
	alarm_t *alarm_entry;
	alarm_t *tmp;

	if (!__is_ready)
		return;

	__is_ready = false;
	__dispatcher_active = false;
	sema_post(&__alarm_expired);
	task_destroy(&__dispatcher);
	mutex_lock(&__alarms_mutex);
	blocking_queue_destroy(&__default_callback_queue, NULL);
	worker_destroy(&__default_callback_worker);
#ifdef HAS_TIMER
	if (timer_delete(__wakeup_timer))
		LOG_ERROR("timer_delete wakeup_timer %s", strerror(errno));
	__wakeup_timer = NULL;
	if (timer_delete(__timer))
		LOG_ERROR("timer_delete timer %s", strerror(errno));
	__timer = NULL;
#endif
	sema_destroy(&__alarm_expired);
	list_for_each_entry_safe(alarm_entry, tmp, alarm_t, &__alarms, list_alarm)
		alarm_destroy(alarm_entry);
	INIT_LIST_HEAD(&__alarms);
	__init_started = false;
	mutex_unlock(&__alarms_mutex);
	mutex_destroy(&__alarms_mutex);
}
