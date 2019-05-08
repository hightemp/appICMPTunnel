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

/*!@file tcl/fiber.h
 * @author uael
 *
 * @addtogroup tcl.fiber @{
 */
#ifndef __TCL_FIBER_H
# define __TCL_FIBER_H

#include "tcl/waitq.h"

/*!@public
 *
 * @brief
 * Fiber creation attributes structure declaration.
 */
typedef struct fiber_attr fiber_attr_t;


/*!@public
 *
 * @brief
 * Fiber creation attributes structure definition.
 */
struct fiber_attr {

	/*! The fiber priority. */
	int8_t prio;

	/*!@private Padding. */
	uint8_t __padding;

	/*! Desired stack size, 1024 by default. */
	uint16_t stack_size;
};

__api int fiber_attr_init(fiber_attr_t *attr, int prio, uint16_t stack_size);

__api int fiber_create(fiber_t *fiber, const fiber_attr_t *attr,
					   routine_t *routine, void *arg);

__api int fiber_cancel(fiber_t fiber);

__api int fiber_join(fiber_t fiber, void **retval);

__api int fiber_setschedprio(fiber_t fiber, int prio);

__api NORETURN void fiber_exit(void *retval);

__api fiber_t fiber_self(void);

/*!@private
 *
 * @brief
 * TODO
 *
 * @param waitq
 */
__api void fiber_lock(waitq_t *waitq);

/*!@private
 *
 * @brief
 * TODO
 *
 * @param waitq
 */
__api void fiber_unlock(waitq_t *waitq);

/*!@private
 *
 * @brief
 * TODO
 */
__api void fiber_yield(void);

#endif /* __TCL_FIBER_H */
/*!@} */
