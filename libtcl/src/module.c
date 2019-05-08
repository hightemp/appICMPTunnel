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

#define LOG_TAG "module"

#include "tcl/module.h"
#include "tcl/mutex.h"
#include "tcl/map.h"
#include "tcl/log.h"

static bool call_lifecycle_function(module_lifecycle_fn function) {
	// A NULL lifecycle function means it isn't needed, so assume success
	if (!function)
		return true;

	future_t future;

	if (!function(&future))
		return true;

	// Otherwise fall back to the future
	return future_await(&future) ? true : false;
}

bool module_init(module_t *module) {
#ifdef TCL_LOGGING
	static int nested = 0;
#endif

	assert(module != NULL);
	if ((module->state & MODULE_STATE_INITIALIZED))
	{
		LOG_WARN("Module \"%s\" Already initialized", module->name);
		return true;
	}
	ITER_OVER_DEPENDENCIES(module_init);
	LOG_INFO("Initializing module \"%s\" (nested %d)", module->name, nested);
	if (!call_lifecycle_function(module->init)) {
		LOG_ERROR("Failed to initialize module \"%s\"", module->name);
		return false;
	}
	LOG_INFO("Initialized module \"%s\"", module->name);
	module->state |= MODULE_STATE_INITIALIZED;
	return true;
}

bool module_start_up(module_t *module) {
#ifdef TCL_LOGGING
	static int nested = 0;
#endif

	assert(module != NULL);
	if ((module->state & MODULE_STATE_STARTED)) {
		LOG_WARN("Module \"%s\" Already started", module->name);
		return true;
	}
	if (!(module->state & MODULE_STATE_INITIALIZED) && !module_init(module))
		return false;

	ITER_OVER_DEPENDENCIES(module_start_up);
	LOG_INFO("Starting module \"%s\" (nested %d)", module->name, nested); \
	if (!call_lifecycle_function(module->start_up)) {
		LOG_ERROR("Failed to start up module \"%s\"", module->name);
		return false;
	}
	LOG_INFO("Started module \"%s\"", module->name);
	module->state |= MODULE_STATE_STARTED;
	return true;
}

bool module_shut_down(module_t *module) {
#ifdef TCL_LOGGING
	static int nested = 0;
#endif

	assert(module != NULL);
	if (!(module->state & MODULE_STATE_STARTED)) {
		LOG_WARN("Module \"%s\" was not started", module->name);
		return true;
	}
	LOG_INFO("Shutting down module \"%s\" (nested %d)", module->name, nested);
	if (!call_lifecycle_function(module->shut_down)) {
		LOG_ERROR("Failed to shutdown module \"%s\".", module->name);
		return false;
	}
	LOG_INFO("Shutdown of module \"%s\" completed", module->name);
	ITER_OVER_DEPENDENCIES(module_shut_down);
	module->state &= ~(MODULE_STATE_STARTED);
	return (true);
}


bool module_clean_up(module_t *module) {
#ifdef TCL_LOGGING
	static int nested = 0;
#endif

	assert(module != NULL);
	if ((module->state & MODULE_STATE_NONE)) {
		LOG_WARN("Module \"%s\" Already clean up", module->name);
		return true;
	}
	if ((module->state & MODULE_STATE_STARTED) && !module_shut_down(module)) {
		return false;
	}

	LOG_INFO("Cleaning up module \"%s\"", module->name);
	if (!call_lifecycle_function(module->clean_up)) {
		LOG_ERROR("Failed to cleanup module \"%s\".", module->name);
		return false;
	}
	LOG_INFO("Cleanup of module \"%s\" completed", module->name);
	ITER_OVER_DEPENDENCIES(module_clean_up);
	module->state &= ~(MODULE_STATE_INITIALIZED);
	return true;
}
