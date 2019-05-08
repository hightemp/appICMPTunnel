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

#pragma once

/*!@file tcl/alarm.h
 * @author uael
 *
 * @addtogroup tcl.alarm @{
 */
#ifndef __TCL_ALARM_H
# define __TCL_ALARM_H

#include "tcl/blocking_queue.h"
#include "tcl/list.h"
#include "tcl/mutex.h"
#include "tcl/worker.h"
#include "tcl/time.h"

#define ALARM_NAME_MAX 16

/*!@public
 *
 * @brief
 * The alarm structure declaration.
 */
typedef struct alarm alarm_t;

/*!@public
 *
 * @brief
 * Procedure which is launched when the alarm is ready.
 */
typedef void (proc_t)(void *context);


/*!@public
 *
 * @brief
 * The alarm structure definition.
 */
struct alarm {

	/*! The alarm cute name. */
	char name[ALARM_NAME_MAX];

	/*! The alarm deadline, must be scheduled after. */
	period_ms_t deadline;

	/*! At which time we set this alarm with `alarm_attach' or `alarm_set'. */
	period_ms_t creation_time;

	/*! The alarm period or relay, depends of `is_periodic'. */
	period_ms_t period;

	/*! If this alarm is periodic. */
	bool is_periodic;

	/*! If this alarm was already set by `alarm_attach' or `alarm_set'. */
	bool is_set;

	/*! The `callback' argument. */
	void *data;

	/*! The alarm procedure. */
	proc_t *callback;

	/*! Must lock the call of `callback'. */
	mutex_t callback_mutex;

	/*! The processing queue to add this alarm to. */
	blocking_queue_t *queue;

	/*! Head of pending alarms list. */
	struct list_head list_alarm;
};

/*!@public
 *
 * @brief
 * Init `alarm' as a non periodic one. This means that she will not be
 * re-scheduled on ready, after she was attached to a queue with `alarm_set'
 * or `alarm_set'.
 * The alarm must be destroyed with `alarm_destroy'.
 *
 * @param alarm The alarm to init.
 * @param name  The alarm name.
 * @return      0 on success, non zero otherwise.
 */
__api int alarm_init(alarm_t *alarm, const char *name);

/*!@public
 *
 * @brief
 * Init `alarm' as a periodic one. This means that she will be re-scheduled
 * on ready, after she was attached to a queue with `alarm_set' or `alarm_set'.
 * The alarm must be destroyed with `alarm_destroy'.
 *
 * @param alarm The alarm to init.
 * @param name  The alarm name.
 * @return      0 on success, non zero otherwise.
 */
__api int alarm_init_periodic(alarm_t *alarm, const char *name);

/*!@public
 *
 * @brief
 * Destroy `alarm', trigger `alarm_cancel'.
 *
 * @param alarm The alarm to destroy.
 */
__api void alarm_destroy(alarm_t *alarm);

/*!@public
 *
 * @brief
 * Check if `alarm' is scheduled or not on his processing queue.
 *
 * @param alarm The alarm to check for.
 * @return      If `alarm' is scheduled or not.
 */
__api bool alarm_is_scheduled(const alarm_t *alarm);

/*!@public
 *
 * @brief
 * Cancel the scheduling of `alarm'.
 *
 * @param alarm The alarm to cancel.
 */
__api void alarm_cancel(alarm_t *alarm);

/*!@public
 *
 * @brief
 * Attach the `alarm' work `cb' which will be called every `interval_ms' with
 * the `data' argument on the `queue' processing queue.
 * `alarm' can only be set once, at least until she was canceled by
 * `alarm_cancel'.
 *
 * @param alarm  The alarm to set.
 * @param period The period between every call of `cb'.
 * @param cb     The alarm work.
 * @param data   The `cb' argument.
 * @param queue  The queue where alarm is queued when ready.
 */
__api void alarm_attach(alarm_t *alarm, period_ms_t period, proc_t *cb,
	void *data, blocking_queue_t *queue);

/*!@public
 *
 * @brief
 * Set the `alarm' work `cb' which will be called every `interval_ms' with
 * the `data' argument on the default alarm processing queue.
 * `alarm' can only be set once, at least until she was canceled by
 * `alarm_cancel'.
 *
 * @param alarm  The alarm to set.
 * @param period The period between every call of `cb'.
 * @param cb     The alarm work.
 * @param data   The `cb' argument.
 */
__api void alarm_set(alarm_t *alarm, period_ms_t period, proc_t *cb,
	void *data);

/*!@public
 *
 * @brief
 * Listen `queue' to dispatch all alarms queued on and schedule them on
 * `worker'.
 * This can be done on multiple queues and threads.
 *
 * @param queue  The queue to listen.
 * @param worker The worker o schedule alarm on.
 */
__api void alarm_register(blocking_queue_t *queue, worker_t *worker);

/*!@public
 *
 * @brief
 * Unregister a queue for listening.
 *
 * @param queue The queue to unlisten.
 */
__api void alarm_unregister(blocking_queue_t *queue);

/*!@public
 *
 * @brief
 * Cleanup the static scope of alarms handling.
 * Must be called before exiting.
 * `tcl_cleanup' trigger the alarm cleanup.
 */
__api void alarm_cleanup(void);

#endif /* !__TCL_ALARM_H */
/*!@} */
