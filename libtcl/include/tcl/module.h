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

/*!@file tcl/module.h
 * @author uael
 *
 * @addtogroup tcl.module @{
 */
#ifndef __TCL_MODULE_H
# define __TCL_MODULE_H

#include "tcl/future.h"

#define MODULE_STATE_NONE (0)
#define MODULE_STATE_INITIALIZED (1 << 0)
#define MODULE_STATE_STARTED  (1 << 1)

#ifdef TCL_LOGGING
#define ITER_OVER_DEPENDENCIES(fn) do \
{ \
	int i = -1; \
	++nested; \
	while (module->dependencies[++i]) { \
		if (!fn(*module->dependencies[i])) { \
			--nested; \
			return (false); \
		} \
	} \
	--nested; \
} while (0)
#else
#define ITER_OVER_DEPENDENCIES(fn) do \
{ \
	int i = -1; \
	while (module->dependencies[++i]) { \
		if (!fn(*module->dependencies[i])) { \
			return (false); \
		} \
	} \
} while (0)
#endif



typedef bool (*module_lifecycle_fn)(future_t *future);

/*!@public
 *
 * @brief
 * The module structure declaration.
 */
typedef struct module module_t;

/*!@public
 *
 * @brief
 * The module structure definition.
 */
struct module {

	/*! no comments, var name is too explicit */
	uint8_t state;

#ifdef TCL_LOGGING
	/*! no comments, var name is too explicit */
	char *name;
#endif

	/*! TODO. */
	module_lifecycle_fn init;

	/*! TODO. */
	module_lifecycle_fn start_up;

	/*! TODO. */
	module_lifecycle_fn shut_down;

	/*! TODO. */
	module_lifecycle_fn clean_up;

	/*! TODO. */
	module_t **dependencies[1];
};

bool module_init(module_t *module);

/* will call module_init if needed */
bool module_start_up(module_t *module);

bool module_shut_down(module_t *module);

/* will call module_shut_down if needed */
bool module_clean_up(module_t *module);
#endif /* __TCL_MODULE_H */
/*!@} */
