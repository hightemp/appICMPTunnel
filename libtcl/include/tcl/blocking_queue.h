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

/*!@file tcl/blocking_queue.h
 * @author uael
 *
 * @addtogroup tcl.ds @{
 *
 * @brief
 * The blocking queue is a concurrent version of the lock free one fifo data
 * structure. This blocking queue apply the producer/consumer pattern which is
 * described here `https://en.wikipedia.org/wiki/Producer-consumer_problem',
 * using two semaphores, one to produce the other to consume.
 *
 * This implementation also provide a way to listen push/pop operation on a
 * worker using the reactor pattern, see `blocking_queue_listen' and
 * `blocking_queue_unlisten' for more information.
 */
#ifndef __TCL_BLOCKING_QUEUE_H
# define __TCL_BLOCKING_QUEUE_H

#include "tcl/queue.h"
#include "tcl/sema.h"
#include "tcl/reactor.h"

struct worker;

/*!@public
 *
 * @brief
 * The blocking queue structure declaration.
 */
typedef struct blocking_queue blocking_queue_t;

/*!@public
 *
 * @brief
 * The blocking queue listener function callback.
 */
typedef void (listener_t)(blocking_queue_t *blocking_queue);

/*!@public
 *
 * @brief
 * The blocking queue structure definition.
 */
struct blocking_queue {

	/*! Use a fifo as base to stock items. */
	fifo_t base;

	/*! The `producer' capacity. */
	uint32_t capacity;

	/*! The producer semaphore, wait on push, post on pop. */
	sema_t producer;

	/*! The consumer semaphore, wait on pop, post on push. */
	sema_t consumer;

	/*! Used when listening operations on a worker, see
	 * `blocking_queue_unlisten'.
	 */
	reactor_object_t *reactor_event;

	/*! Used to lock operations on the `base' fifo. */
	mutex_t lock;
};

/*!@public
 *
 * @brief
 * Init the `queue' blocking queue with `capacity' for capacity, which represent
 * the maximum items at once in the queue.
 * `queue' must be destroyed with `blocking_queue_destroy'.
 *
 * @param queue    The blocking queue to init.
 * @param capacity The capacity to init the queue with.
 * @return         0 on success, non-zero otherwise.
 */
__api int blocking_queue_init(blocking_queue_t *queue, unsigned capacity);

/*!@public
 *
 * @brief
 * Destroy the given `queue' blocking queue.
 *
 * @param queue The blocking queue to destroy.
 * @param dtor  The callback which is called on every remaining items.
 */
__api void blocking_queue_destroy(blocking_queue_t *queue, fifo_dtor_t *dtor);

/*!@public
 *
 * @brief
 * Check if the blocking queue `queue' is empty.
 *
 * @param queue The blocking queue to check for empty.
 * @return      true if empty, false otherwise.
 */
__api bool blocking_queue_empty(blocking_queue_t *queue);

/*!@public
 *
 * @brief
 * Get the length of the blocking queue `queue'.
 *
 * @param queue The blocking queue.
 * @return      The length of the blocking queue.
 */
__api unsigned blocking_queue_length(blocking_queue_t *queue);

/*!@public
 *
 * @brief
 * Get the capacity of the blocking queue `queue'.
 *
 * @param queue The blocking queue.
 * @return      The capacity of the blocking queue.
 */
__api unsigned blocking_queue_capacity(blocking_queue_t *queue);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param queue
 * @param ev
 */
__api void blocking_queue_push(blocking_queue_t *queue, void const *item);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param queue
 * @return
 */
__api void *blocking_queue_pop(blocking_queue_t *queue);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param queue
 * @return
 */
__api void *blocking_queue_peek(blocking_queue_t *queue);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param queue
 * @return
 */
__api void *blocking_queue_back(blocking_queue_t *queue);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param queue
 * @param node
 * @return
 */
__api bool blocking_queue_trypush(blocking_queue_t *queue, void const *data);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param queue
 * @return
 */
__api void *blocking_queue_trypop(blocking_queue_t *queue);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param queue
 * @return
 */
__api void *blocking_queue_trypeek(blocking_queue_t *queue);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param queue
 * @return
 */
__api void *blocking_queue_tryback(blocking_queue_t *queue);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param queue
 * @param worker
 * @param listener
 */
__api void blocking_queue_listen(blocking_queue_t *queue,
	struct worker *worker, listener_t *listener);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param queue
 */
__api void blocking_queue_unlisten(blocking_queue_t *queue);

/*!@public
 *
 * @brief
 * TODO
 * 
 * @param queue 
 * @param item 
 * @return 
 */
__api bool blocking_queue_remove(blocking_queue_t *queue, void *item);

#endif /* !__TCL_BLOCKING_QUEUE_H */
/*!@} */
