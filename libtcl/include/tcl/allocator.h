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

/*!@file tcl/allocator.h
 * @author uael
 *
 * @addtogroup tcl.allocator @{
 */
#ifndef __TCL_ALLOCATOR_H
# define __TCL_ALLOCATOR_H

#include "tcl/compat.h"

/*!@public
 *
 * @brief
 * The allocator structure declaration.
 */
typedef struct allocator allocator_t;

/*!@public
 *
 * @brief
 * Declaration of alloc function.
 */
typedef void *(*alloc_t)(size_t size);

/*!@public
 *
 * @brief
 * Declaration of free function.
 */
typedef void (*free_t)(void *ptr);

/*!@public
 *
 * @brief
 * The allocator structure definition.
 */
struct allocator {

	/*! Allocation callback wrapper. */
	alloc_t alloc;

	/*! Allocation freed callback wrapper. */
	free_t  free;
};

/*!@public
 *
 * @brief
 * The default allocator.
 */
__api allocator_t const *DFT_ALLOCATOR;

/*!@public
 *
 * @brief
 * Free the pointer into `pptr' and assign it to `NULL'
 *
 * @param pptr The pointer of the pointer to free.
 */
__api void pfree(void *pptr);

#endif /* !__TCL_ALLOCATOR_H */
/*!@} */
