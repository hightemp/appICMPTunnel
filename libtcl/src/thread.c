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

#define LOG_TAG "tcl_worker"

#include "tcl/log.h"
#include "tcl/allocator.h"
#include "tcl/worker.h"
#include "tcl/sema.h"
#include "tcl/string.h"

#define DEFAULT_WORK_QUEUE_CAPACITY 128

typedef struct {
	worker_t *worker;
	sema_t start_sem;
	int error;
} start_arg_t;

typedef struct {
	routine_t *func;
	void *context;
} work_item_t;

static void __work_ready(blocking_queue_t *queue)
{
	work_item_t *item;

	item = blocking_queue_pop(queue);
	item->func(item->context);
	free(item);
}

static void *__run_worker(void *context)
{
	start_arg_t *arg;
	worker_t *worker;
	work_item_t *item;
	unsigned count;

	arg = (start_arg_t *)context;
	worker = arg->worker;

	blocking_queue_listen(&worker->work_queue, worker, __work_ready);
	sema_post(&arg->start_sem);
	LOG_INFO("worker name %s started", worker->name);
	reactor_start(&worker->reactor);
	blocking_queue_unlisten(&worker->work_queue);

	/*
	 * Make sure we dispatch all queued work items before exiting the worker.
     * This allows a caller to safely tear down by enqueuing a teardown
     * work item and then joining the worker.
	 */
	count = 0;
	while ((item = blocking_queue_trypop(&worker->work_queue))
		   && count <= blocking_queue_capacity(&worker->work_queue)) {
		item->func(item->context);
		free(item);
		++count;
	}

	if (count > blocking_queue_capacity(&worker->work_queue))
		LOG_DEBUG("Growing event queue on shutdown.");

	LOG_INFO("worker name %s exited", worker->name);
	return NULL;
}

int worker_init(worker_t *worker, char const *name)
{
	start_arg_t start;

	if (sema_init(&start.start_sem, 0))
		return -1;
	start.worker = worker;
	start.error = 0;
	strncpy(worker->name, name, WORKER_NAME_MAX - 1);
	blocking_queue_init(&worker->work_queue, DEFAULT_WORK_QUEUE_CAPACITY);
	reactor_init(&worker->reactor);
	task_spawn(&worker->task, __run_worker, &start);
	sema_wait(&start.start_sem);
	sema_destroy(&start.start_sem);
	if (start.error) {
		blocking_queue_destroy(&worker->work_queue, pfree);
		reactor_destroy(&worker->reactor);
	}
	return 0;
}

void worker_destroy(worker_t *worker)
{
	worker_stop(worker);
	worker_join(worker);
	task_destroy(&worker->task);
	blocking_queue_destroy(&worker->work_queue, pfree);
	reactor_destroy(&worker->reactor);
}

int worker_setpriority(worker_t *worker, int priority)
{
	return task_setpriority(&worker->task, priority);
}

void worker_join(worker_t *worker)
{
	task_join(&worker->task);
}

bool worker_post(worker_t *worker, routine_t *work, void *context)
{
	work_item_t *item;

	if (!(item = (work_item_t *)malloc(sizeof(work_item_t))))
		return false;
	item->func = work;
	item->context = context;
	blocking_queue_push(&worker->work_queue, item);
	return true;
}

void worker_stop(worker_t *worker)
{
	reactor_stop(&worker->reactor);
}
