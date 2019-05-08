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

/*!@file tcl/waitq.h
 * @author uael
 *
 * @addtogroup tcl.ds @{
 */
#ifndef __TCL_WAITQ_H
# define __TCL_WAITQ_H

#include "tcl/queue.h"
#include "tcl/coro.h"

/*!@public
 *
 * @brief
 * Represent a fiber as a coroutine.
 */
typedef coro_t fiber_t;

/*!@public
 *
 * @brief
 * The wait queue structure declaration.
 */
typedef union waitq waitq_t;

/*!@public
 *
 * @brief
 * The wait queue structure definition.
 */
union waitq {

	/*!@private
	 * Base of the wait queue.
	 */
	fifo_t base;

	/*! Public accessors. */
	struct {

		/*! Dynamic buffer to store items. */
		const fiber_t *buffer;

		/*! The queue length, represent the number of items stored in
		 * `buffer'.
		 */
		const size_t length;
	};
};

__api void waitq_init(waitq_t *waitq);

__api void waitq_destroy(waitq_t *waitq);

__api void waitq_push(waitq_t *waitq, fiber_t fiber);

__api fiber_t waitq_pop(waitq_t *waitq);

#endif /* !__TCL_WAITQ_H */
/*!@} */
