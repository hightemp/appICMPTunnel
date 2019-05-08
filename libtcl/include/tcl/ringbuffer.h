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

/*!@file tcl/ringbuffer.h
 * @author uael
 *
 * @addtogroup tcl.ringbuffer @{
 */
#ifndef __TCL_RINGBUFFER_H
# define __TCL_RINGBUFFER_H

#include "tcl/sema.h"

/*!@public
 *
 * @brief
 * The ringbuffer structure declaration.
 */
typedef struct ringbuffer ringbuffer_t;

/*!@public
 *
 * @brief
 * The ringbuffer structure definition.
 */
struct ringbuffer {

	/*! TODO. */
	size_t total;

	/*! TODO. */
	size_t available;

	/*! TODO. */
	uint8_t *base;

	/*! TODO. */
	uint8_t *head;

	/*! TODO. */
	uint8_t *tail;
};

/*!@public
 *
 * @brief
 * TODO
 *
 * @param p
 * @param size
 */
__api void ringbuffer_init(ringbuffer_t *p, size_t size);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param rb
 */
__api void ringbuffer_destroy(ringbuffer_t *rb);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param rb
 * @return
 */
__api size_t ringbuffer_available(const ringbuffer_t *rb);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param rb
 * @return
 */
__api size_t ringbuffer_size(const ringbuffer_t *rb);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param rb
 * @param p
 * @param length
 * @return
 */
__api size_t ringbuffer_insert(ringbuffer_t *rb, const uint8_t *p,
	size_t length);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param rb
 * @param length
 * @return
 */
__api size_t ringbuffer_delete(ringbuffer_t *rb, size_t length);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param rb
 * @param offset
 * @param p
 * @param length
 * @return
 */
__api size_t ringbuffer_peek(const ringbuffer_t *rb, off_t offset, uint8_t *p,
	size_t length);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param rb
 * @param p
 * @param length
 * @return
 */
__api size_t ringbuffer_pop(ringbuffer_t *rb, uint8_t *p, size_t length);

#endif /* __TCL_RINGBUFFER_H */
/*!@} */
