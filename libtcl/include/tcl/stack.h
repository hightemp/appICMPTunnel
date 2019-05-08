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

/*!@file tcl/stack.h
 * @author uael
 *
 * @addtogroup tcl.ds @{
 */
#ifndef __TCL_STACK_H
# define __TCL_STACK_H

#include "tcl/vector.h"

/*!@public
 *
 * @brief
 * The lifo structure type declaration.
 */
typedef union lifo lifo_t;

/*!@public
 *
 * @brief
 * stack item destruction callback, passed to `stack_destroy' to destroy every
 * item before freed.
 */
typedef void (lifo_dtor_t)(void *item);

/*!@public
 *
 * @brief
 * The lifo structure definition.
 */
union lifo {

	/*!@private
	 * Base of the stack.
	 */
	vector_t base;

	/*! Public accessors. */
	struct {

		/*! Dynamic buffer to store items. */
		const void *buffer;

		/*! The stack length, represent the number of items stored in
		 * `buffer'.
		 */
		const size_t length;

		/*! The stack capacity, or size of the allocated `buffer'. */
		const size_t capacity;

		/*! Size of an item which is passed to `stack_init'. */
		const size_t isize;
	};
};

/*!@public
 *
 * @brief
 * Initialize `stack' where `isize' represent the item size.
 *
 * @param stack The stack to initialize.
 * @param isize The size of an item.
 */
__api void stack_init(lifo_t *stack, size_t isize);

/*!@public
 *
 * @brief
 * Destroy `stack'. An optional callback `idtor' will be called on
 * each item before freed if present.
 *
 * @param stack The stack to destroy.
 * @param idtor The item callback destructor.
 */
__api void stack_destroy(lifo_t *stack, lifo_dtor_t *idtor);

/*!@public
 *
 * @brief
 * Clear `stack' using the optional destructor `idtor'.
 * The stack is still allocated and his capacity does not change, there is no
 * truncation.
 *
 * @param stack The stack to clear.
 * @param idtor The item callback destructor.
 */
__api void stack_clear(lifo_t *stack, lifo_dtor_t *idtor);

/*!@public
 *
 * @brief
 * Get `stack' actual length.
 *
 * @param stack The stack.
 * @return      The stack length.
 */
__api size_t stack_length(lifo_t *stack);

/*!@public
 *
 * @brief
 * Get a pointer that point to the last inserted item of `stack'.
 * If there is no element in `stack', return `NULL'.
 *
 * @param stack The stack.
 * @return      The stack begin pointer or `NULL'.
 */
__api void *stack_peek(lifo_t *stack);

/*!@public
 *
 * @brief
 * Make sure `stack' is sufficiently allocated to store `n' number
 * of items.
 * If not reallocate it.
 *
 * @param stack The stack to ensure.
 * @param n     The number of items to ensure.
 */
__api void stack_ensure(lifo_t *stack, size_t n);

/*!@public
 *
 * @brief
 * Grow `stack' to make sure he is sufficiently allocated to store
 * `stack_length' + `n' number of additional items.
 * If not reallocate it.
 *
 * @param stack The stack to grow.
 * @param n     The number of additional items to ensure.
 */
__api void stack_grow(lifo_t *stack, size_t n_added);

/*!@public
 *
 * @brief
 * Make place to `n' number of items at the end of `stack' and
 * return a pointer to the begin of this new space.
 * A memory copy of `n' items can then be safely appliqued to this space.
 *
 * @param stack The stack where to push.
 * @param n     The number of items in the new space.
 * @return      A pointer to the begin of this new space.
 */
__api void *stack_npush(lifo_t *stack, size_t n);

/*!@public
 *
 * @brief
 * Remove `n' number of items at the end of `stack'.
 * `n' can be greater than the actual number of items in `stack'.
 * Copy removed items to the optional pointer `out' if not `NULL'.
 * Return the number of items which have been actually removed.
 *
 * @param stack The stack where to pop.
 * @param n     The number of items to remove.
 * @param out   Nullable pointer to receive removed items.
 * @return      Return the number of items which have been actually removed.
 */
__api size_t stack_npop(lifo_t *stack, size_t n, void *out);

/*!@public
 *
 * @brief
 * Make place to 1 items at the end of `stack' and
 * return a pointer to the begin of the new item.
 * A memory copy of 1 item can then be safely appliqued to this space.
 *
 * @param stack The stack where to push.
 * @return      A pointer to the begin of the new item.
 */
__api void *stack_push(lifo_t *stack);

/*!@public
 *
 * @brief
 * Remove 1 item at the end of `stack'.
 * Copy removed item to the optional pointer `out' if not `NULL'.
 * Return if an item have been actually removed.
 *
 * @param stack The stack where to pop.
 * @param out   Nullable pointer to receive removed item.
 * @return      Return if an item have been actually removed.
 */
__api bool stack_pop(lifo_t *stack, void *out);

#endif /* !__TCL_STACK_H */
/*!@} */
