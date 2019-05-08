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

/*!@file tcl/vector.h
 * @author uael
 *
 * @addtogroup tcl.ds @{
 */
#ifndef __TCL_VECTOR_H
# define __TCL_VECTOR_H

#include "tcl/compat.h"
#include "tcl/string.h"

/*!@public
 *
 * @brief
 * The vector structure type declaration.
 */
typedef struct vector vector_t;

/*!@public
 *
 * @brief
 * Vector item destruction callback, passed to `vector_destroy' to destroy every
 * item before freed.
 */
typedef void (vector_dtor_t)(void *item);

/*!@public
 *
 * @brief
 * The vector structure definition.
 */
struct vector {

	/*! Dynamic buffer to store items. */
	void *buffer;

	/*! The vector length, represent the number of items stored in `buffer'. */
	size_t length;

	/*! The vector capacity, or size of the allocated `buffer'. */
	size_t capacity;

	/*! Size of an item which is passed to `vector_init'. */
	size_t isize;
};

/*!@public
 *
 * @brief
 * Initialize `vector' where `isize' represent the item size.
 *
 * @param vector The vector to initialize.
 * @param isize  The size of an item.
 */
__api void vector_init(vector_t *vector, size_t isize);

/*!@public
 *
 * @brief
 * Destroy `vector'. An optional callback `idtor' will be called on
 * each item before freed if present.
 *
 * @param vector The vector to destroy.
 * @param idtor  The item callback destructor.
 */
__api void vector_destroy(vector_t *vector, vector_dtor_t *idtor);

/*!@public
 *
 * @brief
 * Clear `vector' using the optional destructor `idtor'.
 * The vector is still allocated and his capacity does not change, there is no
 * truncation.
 *
 * @param vector The vector to clear.
 * @param idtor  The item callback destructor.
 */
__api void vector_clear(vector_t *vector, vector_dtor_t *idtor);

/*!@public
 *
 * @brief
 * Get `vector' actual length.
 *
 * @param vector The vector.
 * @return       The vector length.
 */
__api size_t vector_length(vector_t *vector);

/*!@public
 *
 * @brief
 * Get a pointer that point to the begin of `vector'.
 * If there is no element in `vector', return `NULL'.
 *
 * @param vector The vector.
 * @return       The vector begin pointer or `NULL'.
 */
__api void *vector_begin(vector_t *vector);

/*!@public
 *
 * @brief
 * Get a pointer that point to the end of `vector'.
 * The end is the element where `vector_length' point to.
 * If there is no element in `vector', return `NULL'.
 *
 * @param vector The vector.
 * @return       The vector end pointer or `NULL'.
 */
__api void *vector_end(vector_t *vector);

/*!@public
 *
 * @brief
 * Get a pointer that point to the back of `vector'.
 * The back is the element where `vector_length' - 1 point to.
 * If there is no element in `vector', return `NULL'.
 *
 * @param vector The vector.
 * @return       The vector back pointer or `NULL'.
 */
__api void *vector_back(vector_t *vector);

/*!@public
 *
 * @brief
 * Get a pointer that point to the `idx' of `vector'.
 * The `idx' is the element where `idx' point to.
 * If there is no element in `vector' or if `idx' is than `vector_length',
 * return `NULL'.
 *
 * @param vector The vector.
 * @param idx    The index where to retrieve an item.
 * @return       The vector `idx' pointer or `NULL'.
 */
__api void *vector_at(vector_t *vector, size_t idx);

/*!@public
 *
 * @brief
 * Make sure `vector' is sufficiently allocated to store `n' number
 * of items.
 * If not reallocate it.
 *
 * @param vector The vector to ensure.
 * @param n      The number of items to ensure.
 */
__api void vector_ensure(vector_t *vector, size_t n);

/*!@public
 *
 * @brief
 * Grow `vector' to make sure he is sufficiently allocated to store
 * `vector_length' + `n' number of additional items.
 * If not reallocate it.
 *
 * @param vector The vector to grow.
 * @param n      The number of additional items to ensure.
 */
__api void vector_grow(vector_t *vector, size_t n);

/*!@public
 *
 * @brief
 * Make place to `n' number of items at the end of `vector' and
 * return a pointer to the begin of this new space.
 * A memory copy of `n' items can then be safely appliqued to this space.
 *
 * @param vector The vector where to push.
 * @param n      The number of items in the new space.
 * @return       A pointer to the begin of this new space.
 */
__api void *vector_npush_back(vector_t *vector, size_t n);

/*!@public
 *
 * @brief
 * Make place to `n' number of items at the begin of `vector' and
 * return a pointer to the begin of this new space.
 * A memory copy of `n' items can then be safely appliqued to this space.
 *
 * @param vector The vector where to push.
 * @param n      The number of items in the new space.
 * @return       A pointer to the begin of this new space.
 */
__api void *vector_npush_front(vector_t *vector, size_t n);

/*!@public
 *
 * @brief
 * Make place to `n' number of items at the index `idx' of `vector'
 * and return a pointer to the begin of this new space.
 * A memory copy of `n' items can then be safely appliqued to this space.
 *
 * @param vector The vector where to push.
 * @param n      The number of items in the new space.
 * @param idx    The index where the new space should begin.
 * @return       A pointer to the begin of this new space.
 */
__api void *vector_npush_at(vector_t *vector, size_t n, size_t idx);

/*!@public
 *
 * @brief
 * Remove `n' number of items at the end of `vector'.
 * `n' can be greater than the actual number of items in `vector'.
 * Copy removed items to the optional pointer `out' if not `NULL'.
 * Return the number of items which have been actually removed.
 *
 * @param vector The vector where to pop.
 * @param n      The number of items to remove.
 * @param out    Nullable pointer to receive removed items.
 * @return       Return the number of items which have been actually removed.
 */
__api size_t vector_npop_back(vector_t *vector, size_t n, void *out);

/*!@public
 *
 * @brief
 * Remove `n' number of items at the begin of `vector'.
 * `n' can be greater than the actual number of items in `vector'.
 * Copy removed items to the optional pointer `out' if not `NULL'.
 * Return the number of items which have been actually removed.
 *
 * @param vector The vector where to pop.
 * @param n      The number of items to remove.
 * @param out    Nullable pointer to receive removed items.
 * @return       Return the number of items which have been actually removed.
 */
__api size_t vector_npop_front(vector_t *vector, size_t n, void *out);

/*!@public
 *
 * @brief
 * Remove `n' number of items at the index `idx' of `vector'.
 * `n' can be greater than the actual number of items in `vector' after `idx'.
 * Copy removed items to the optional pointer `out' if not `NULL'.
 * Return the number of items which have been actually removed.
 *
 * @param vector The vector where to pop.
 * @param n      The number of items to remove.
 * @param idx    The index where to remove items.
 * @param out    Nullable pointer to receive removed items.
 * @return       Return the number of items which have been actually removed.
 */
__api size_t vector_npop_at(vector_t *vector, size_t n, size_t idx,
	void *out);

/*!@public
 *
 * @brief
 * Make place to 1 items at the end of `vector' and
 * return a pointer to the begin of the new item.
 * A memory copy of 1 item can then be safely appliqued to this space.
 *
 * @param vector The vector where to push.
 * @return       A pointer to the begin of the new item.
 */
__api void *vector_push_back(vector_t *vector);

/*!@public
 *
 * @brief
 * Make place to 1 items at the begin of `vector' and
 * return a pointer to the begin of the new item.
 * A memory copy of 1 item can then be safely appliqued to this space.
 *
 * @param vector The vector where to push.
 * @return       A pointer to the begin of the new item.
 */
__api void *vector_push_front(vector_t *vector);

/*!@public
 *
 * @brief
 * Make place to 1 items at the index `idx' of `vector' and
 * return a pointer to the begin of the new item.
 * A memory copy of 1 item can then be safely appliqued to this space.
 *
 * @param vector The vector where to push.
 * @param idx    The index where to add the new item.
 * @return       A pointer to the begin of the new item.
 */
__api void *vector_push_at(vector_t *vector, size_t i);

/*!@public
 *
 * @brief
 * Remove 1 item at the end of `vector'.
 * Copy removed item to the optional pointer `out' if not `NULL'.
 * Return if an item have been actually removed.
 *
 * @param vector The vector where to pop.
 * @param out    Nullable pointer to receive removed item.
 * @return       Return if an item have been actually removed.
 */
__api bool vector_pop_back(vector_t *vector, void *out);

/*!@public
 *
 * @brief
 * Remove 1 item at the begin of `vector'.
 * Copy removed item to the optional pointer `out' if not `NULL'.
 * Return if an item have been actually removed.
 *
 * @param vector The vector where to pop.
 * @param out    Nullable pointer to receive removed item.
 * @return       Return if an item have been actually removed.
 */
__api bool vector_pop_front(vector_t *vector, void *out);

/*!@public
 *
 * @brief
 * Remove 1 item at the index `idx' of `vector'.
 * Copy removed item to the optional pointer `out' if not `NULL'.
 * Return if an item have been actually removed.
 *
 * @param vector The vector where to pop.
 * @param idx    The index where to remove items.
 * @param out    Nullable pointer to receive removed item.
 * @return       Return if an item have been actually removed.
 */
__api bool vector_pop_at(vector_t *vector, size_t idx, void *out);

/*!@public
 *
 * @brief
 * Get index of an item `item' in a vector `vector'.
 *
 * @param vector The vector where to look for.
 * @param item   The item to find index
 * @return
 */
__api size_t vector_indexof(vector_t *vector, void *item);

/*!@public
 *
 * @brief
 * Iterate through `vector'.
 *
 * @param vector The vector to iterate.
 * @paran item   A variable where to stock each items.
 */
#define vector_foreach(vector, item) \
	for (item = vector_begin(vector); \
		item && (void *)item != vector_end(vector); \
		item = (void *)((char *)item + (vector)->isize))

/*!@public
 *
 * @brief
 * Iterate through `vector'.
 *
 * @param vector The vector to iterate.
 * @paran item   A variable where to stock each items.
 */
#define vector_iforeach(vector, idx, item) \
	for (idx = 0, item = vector_begin(vector); \
		item && (void *)item != vector_end(vector); \
		++idx, item = (void *)((char *)item + (vector)->isize))

/*!@public
 *
 * @brief
 * Reverse iterate through `vector'.
 *
 * @param vector The vector to reverse iterate.
 * @paran item   A variable where to stock each items.
 */
#define vector_rforeach(vector, item) \
	for (item = vector_back(vector); \
		item && (void *)item >= vector_begin(vector); \
		item = (void *)((char *)item - (vector)->isize))

#define vec_foreach(vector, idx) \
	for (idx = 0; idx < vector_length(vector); ++idx) \

#endif /* !__TCL_VECTOR_H */
/*!@} */
