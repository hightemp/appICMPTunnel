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

#include "tcl/blocking_queue.h"
#include "tcl/reactor.h"
#include "tcl/worker.h"
#include "tcl/string.h"

int blocking_queue_init(blocking_queue_t *queue, unsigned capacity)
{
	if (sema_init(&queue->producer, capacity))
		return -1;
	if (sema_init(&queue->consumer, 0))
		return -1;
	queue->capacity = capacity;
	queue_init(&queue->base, sizeof(void *));
	queue->reactor_event = NULL;
	mutex_init(&queue->lock);
	return 0;
}

void blocking_queue_destroy(blocking_queue_t *queue, fifo_dtor_t *dtor)
{
	blocking_queue_unlisten(queue);
	queue_destroy(&queue->base, dtor);
	sema_destroy(&queue->producer);
	sema_destroy(&queue->consumer);
	mutex_destroy(&queue->lock);
}

bool blocking_queue_empty(blocking_queue_t *queue)
{
	bool empty;

	mutex_lock(&queue->lock);
	empty = queue_length(&queue->base) == 0;
	mutex_unlock(&queue->lock);
	return empty;
}

unsigned blocking_queue_length(blocking_queue_t *queue)
{
	unsigned len;

	mutex_lock(&queue->lock);
	len = (unsigned)queue_length(&queue->base);
	mutex_unlock(&queue->lock);
	return len;
}

unsigned blocking_queue_capacity(blocking_queue_t *queue)
{
	return queue->capacity;
}

void blocking_queue_push(blocking_queue_t *queue, void const *item)
{
	sema_wait(&queue->producer);
	mutex_lock(&queue->lock);
	*(void const **)queue_push(&queue->base) = item;
	mutex_unlock(&queue->lock);
	sema_post(&queue->consumer);
}

void *blocking_queue_pop(blocking_queue_t *queue)
{
	void *item;

	sema_wait(&queue->consumer);
	mutex_lock(&queue->lock);
	item = NULL;
	queue_pop(&queue->base, &item);
	mutex_unlock(&queue->lock);
	sema_post(&queue->producer);
	return item;
}

void *blocking_queue_peek(blocking_queue_t *queue)
{
	void *item;

	sema_wait(&queue->consumer);
	mutex_lock(&queue->lock);
	item = *(void **)queue_peek(&queue->base);
	mutex_unlock(&queue->lock);
	sema_post(&queue->producer);
	return item;
}

void *blocking_queue_back(blocking_queue_t *queue)
{
	void *item;

	sema_wait(&queue->consumer);
	mutex_lock(&queue->lock);
	item = *(void **)vector_back(&queue->base.base);
	mutex_unlock(&queue->lock);
	sema_post(&queue->producer);
	return item;
}

bool blocking_queue_trypush(blocking_queue_t *queue, const void *item)
{
	if (!sema_trywait(&queue->producer))
		return false;
	mutex_lock(&queue->lock);
	*(void const **)queue_push(&queue->base) = item;
	mutex_unlock(&queue->lock);
	sema_post(&queue->consumer);
	return true;
}

void *blocking_queue_trypop(blocking_queue_t *queue)
{
	void *item;

	if (!sema_trywait(&queue->consumer))
		return NULL;
	mutex_lock(&queue->lock);
	item = NULL;
	queue_pop(&queue->base, &item);
	mutex_unlock(&queue->lock);
	sema_post(&queue->producer);
	return item;
}

void *blocking_queue_trypeek(blocking_queue_t *queue)
{
	void *item;

	if (!sema_trywait(&queue->consumer))
		return NULL;
	mutex_lock(&queue->lock);
	item = *(void **)queue_peek(&queue->base);
	mutex_unlock(&queue->lock);
	sema_post(&queue->producer);
	return item;
}

void *blocking_queue_tryback(blocking_queue_t *queue)
{
	void *item;

	if (!sema_trywait(&queue->consumer))
		return NULL;
	mutex_lock(&queue->lock);
	item = *(void **)vector_back(&queue->base.base);
	mutex_unlock(&queue->lock);
	sema_post(&queue->producer);
	return item;
}

void blocking_queue_listen(blocking_queue_t *queue, worker_t *worker,
	listener_t *listener)
{
	blocking_queue_unlisten(queue);
	queue->reactor_event = reactor_register(
		&worker->reactor, &queue->consumer.event, queue,
		(reactor_ready_t *)listener, NULL);
}

void blocking_queue_unlisten(blocking_queue_t *queue)
{
	if (queue->reactor_event) {
		while (queue_length(&queue->base)
			&& queue->reactor_event->reactor->is_running)
			reactor_run_once(queue->reactor_event->reactor);
		reactor_unregister(queue->reactor_event);
		queue->reactor_event = NULL;
	}
}

bool blocking_queue_remove(blocking_queue_t *queue, void *item) {
	bool removed;
	size_t item_idx;

	if (!queue)
		return false;
	removed = false;
	mutex_lock(&queue->lock);
	item_idx = queue_index_of(&queue->base, item);
	if (item_idx != (size_t)-1 && sema_trywait(&queue->consumer)) {
		removed = queue_pop_at(&queue->base, item_idx, NULL);
	}
	mutex_unlock(&queue->lock);
	if (removed) {
		sema_post(&queue->producer);
	}
	return removed;
}
