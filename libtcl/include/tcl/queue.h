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

#pragma once

/*!@file tcl/queue.h
 * @author uael
 *
 * @addtogroup tcl.ds @{
 */
#ifndef __TCL_QUEUE_H
# define __TCL_QUEUE_H

#include "tcl/vector.h"

/*!@public
 *
 * @brief
 * The fifo structure type declaration.
 */
typedef union fifo fifo_t;

/*!@public
 *
 * @brief
 * QUEUE item destruction callback, passed to `queue_destroy' to destroy every
 * item before freed.
 */
typedef void (fifo_dtor_t)(void *item);

/*!@public
 *
 * @brief
 * The fifo structure definition.
 */
union fifo {

	/*!@private
	 * Base of the queue.
	 */
	vector_t base;

	/*! Public accessors. */
	struct {

		/*! Dynamic buffer to store items. */
		const void *buffer;

		/*! The queue length, represent the number of items stored in
		 * `buffer'.
		 */
		const size_t length;

		/*! The queue capacity, or size of the allocated `buffer'. */
		const size_t capacity;

		/*! Size of an item which is passed to `queue_init'. */
		const size_t isize;
	};
};

/*!@public
 *
 * @brief
 * Initialize `queue' where `isize' represent the item size.
 *
 * @param queue The queue to initialize.
 * @param isize The size of an item.
 */
__api void queue_init(fifo_t *queue, size_t isize);

/*!@public
 *
 * @brief
 * Destroy `queue'. An optional callback `idtor' will be called on
 * each item before freed if present.
 *
 * @param queue The queue to destroy.
 * @param idtor The item callback destructor.
 */
__api void queue_destroy(fifo_t *queue, fifo_dtor_t *idtor);

/*!@public
 *
 * @brief
 * Clear `queue' using the optional destructor `idtor'.
 * The queue is still allocated and his capacity does not change, there is no
 * truncation.
 *
 * @param queue The queue to clear.
 * @param idtor The item callback destructor.
 */
__api void queue_clear(fifo_t *queue, fifo_dtor_t *idtor);

/*!@public
 *
 * @brief
 * Get `queue' actual length.
 *
 * @param queue The queue.
 * @return      The queue length.
 */
__api size_t queue_length(fifo_t *queue);

/*!@public
 *
 * @brief
 * Get a pointer that point to the first inserted item of `queue'.
 * If there is no element in `queue', return `NULL'.
 *
 * @param queue The queue.
 * @return      The queue begin pointer or `NULL'.
 */
__api void *queue_peek(fifo_t *queue);

/*!@public
 *
 * @brief
 * Make sure `queue' is sufficiently allocated to store `n' number
 * of items.
 * If not reallocate it.
 *
 * @param queue The queue to ensure.
 * @param n     The number of items to ensure.
 */
__api void queue_ensure(fifo_t *queue, size_t n);

/*!@public
 *
 * @brief
 * Grow `queue' to make sure he is sufficiently allocated to store
 * `queue_length' + `n' number of additional items.
 * If not reallocate it.
 *
 * @param queue The queue to grow.
 * @param n     The number of additional items to ensure.
 */
__api void queue_grow(fifo_t *queue, size_t n_added);

/*!@public
 *
 * @brief
 * Make place to `n' number of items at the end of `queue' and
 * return a pointer to the begin of this new space.
 * A memory copy of `n' items can then be safely appliqued to this space.
 *
 * @param queue The queue where to push.
 * @param n     The number of items in the new space.
 * @return      A pointer to the begin of this new space.
 */
__api void *queue_npush(fifo_t *queue, size_t n);

/*!@public
 *
 * @brief
 * Remove `n' number of items at the begin of `queue'.
 * `n' can be greater than the actual number of items in `queue'.
 * Copy removed items to the optional pointer `out' if not `NULL'.
 * Return the number of items which have been actually removed.
 *
 * @param queue The queue where to pop.
 * @param n     The number of items to remove.
 * @param out   Nullable pointer to receive removed items.
 * @return      Return the number of items which have been actually removed.
 */
__api size_t queue_npop(fifo_t *queue, size_t n, void *out);

/*!@public
 *
 * @brief
 * Make place to 1 items at the end of `queue' and
 * return a pointer to the begin of the new item.
 * A memory copy of 1 item can then be safely appliqued to this space.
 *
 * @param queue The queue where to push.
 * @return      A pointer to the begin of the new item.
 */
__api void *queue_push(fifo_t *queue);

/*!@public
 *
 * @brief
 * Remove 1 item at the begin of `queue'.
 * Copy removed item to the optional pointer `out' if not `NULL'.
 * Return if an item have been actually removed.
 *
 * @param queue The queue where to pop.
 * @param out   Nullable pointer to receive removed item.
 * @return      Return if an item have been actually removed.
 */
__api bool queue_pop(fifo_t *queue, void *out);

/*!
 * @brief
 *  Get index of an item `item` in a queue `queue`.
 *
 * @param queue The queue where to look for.
 * @param item  The item to find index
 * @return
 */
__api size_t queue_index_of(fifo_t *queue, void *item);

/*!@public
 *
 * @brief
 * Remove 1 item at the index `idx' of `queue'.
 * Copy removed item to the optional pointer `out' if not `NULL'.
 * Return if an item have been actually removed.
 *
 * @param queue The queue where to pop.
 * @param idx   The index where to remove items.
 * @param out   Nullable pointer to receive removed item.
 * @return      Return if an item have been actually removed.
 */
__api bool queue_pop_at(fifo_t *queue, size_t idx, void *out);

#endif /* !__TCL_QUEUE_H */
/*!@} */
