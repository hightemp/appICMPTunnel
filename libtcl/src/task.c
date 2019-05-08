/*
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
#define LOG_TAG "tcl_task"

#include "tcl/log.h"
#include "tcl/task.h"
#include "tcl/sema.h"

typedef struct {
	task_t *task;
	sema_t start_sem;
} start_arg_t;

static void *__repeat(void *context)
{
	task_t *task;
	start_arg_t *arg;

	arg = (start_arg_t *)context;
	task = arg->task;
	task->running = true;
	LOG_INFO("task repeat started %p", task);
	sema_post(&arg->start_sem);

	while (task->running) {
		task->work(task->context);
		task_schedule();
	}

	task->joined = true;
	LOG_INFO("task repeat exited %p", task);
	return NULL;
}

static void *__spawn(void *context)
{
	task_t *task;
	start_arg_t *arg;

	arg = (start_arg_t *)context;
	task = arg->task;
	task->running = true;
	LOG_INFO("task spawn started %p", task);
	sema_post(&arg->start_sem);

	task->work(task->context);
	task_schedule();

	task->running = false;
	task->joined = true;
	LOG_INFO("task spawn exited %p", task);
	return NULL;
}

static FORCEINLINE int __init(task_t *task, routine_t *work, void *context,
	bool repeat)
{
	start_arg_t start;
	routine_t *task_work;

	if (sema_init(&start.start_sem, 0))
		return -1;
	start.task = task;
	task->running = false;
	task->joined = false;
	task->work = work;
	task->context = context;
	task_work = repeat ? __repeat : __spawn;
#ifdef TCL_THREADING
	if (pthread_create(&task->pthread, NULL, task_work, &start))
		return -1;
#else
	if (fiber_create(&task->fiber, NULL, task_work, &start))
		return -1;
#endif
	sema_wait(&start.start_sem);
	sema_destroy(&start.start_sem);
	return 0;
}

int task_spawn(task_t *task, routine_t *work, void *context)
{
	return __init(task, work, context, false);
}

int task_repeat(task_t *task, routine_t *work, void *context)
{
	return __init(task, work, context, true);
}

FORCEINLINE void task_destroy(task_t *task)
{
	task_join(task);
}

FORCEINLINE bool task_running(task_t *task)
{
	return !task->joined;
}

FORCEINLINE int task_setpriority(task_t *task, int priority)
{
#ifdef TCL_THREADING
	if (pthread_setschedprio(task->pthread, priority)) {
		LOG_ERROR("Unable to set thread priority %d, %s", priority,
			strerror(errno));
		return 0;
	}
#else
	if (fiber_setschedprio(task->fiber, priority)) {
		LOG_ERROR("Unable to set fiber priority %d, %s", priority,
			strerror(errno));
		return -1;
	}
#endif /* TCL_THREADING */
	return 0;
}

FORCEINLINE void task_stop(task_t *task)
{
	task->running = false;
}

FORCEINLINE void task_join(task_t *task)
{
	if (!task->joined) {
		task_stop(task);
#ifdef TCL_THREADING
		pthread_join(task->pthread, NULL);
#else
		fiber_join(task->fiber, NULL);
#endif /* TCL_THREADING */
	}
}

FORCEINLINE void task_schedule(void)
{
#ifdef TCL_THREADING
	sched_yield();
#else
	fiber_yield();
#endif
}
