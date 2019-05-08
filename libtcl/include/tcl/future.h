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

/*!@file tcl/future.h
 * @author uael
 *
 * @addtogroup tcl.future @{
 */
#ifndef __TCL_FUTURE_H
# define __TCL_FUTURE_H

#include "tcl/sema.h"

#define FUTURE_SUCCESS ((void *)1)
#define FUTURE_FAIL ((void *)0)

/*!@public
 *
 * @brief
 * The future structure declaration.
 */
typedef struct future future_t;

/*!@public
 *
 * @brief
 * The future structure definition.
 */
struct future {

	/*! TODO. */
	bool awaitable;

	/*! TODO. */
	sema_t sema;

	/*! TODO. */
	void const *result;
};

/*!@public
 *
 * @brief
 * TODO(uael): remove it, only new/free, await should free
 *
 * @param future
 * @return
 */
__api int future_init(future_t *future);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param future
 * @param value
 * @return
 */
__api void future_immediate(future_t *future, void const *value);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param future
 * @param value
 */
__api void future_ready(future_t *future, void *value);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param future
 * @return
 */
__api void *future_await(future_t *future);

#endif /* __TCL_FUTURE_H */
/*!@} */
