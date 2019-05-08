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

#define LOG_TAG "bt_tcl_dispatcher"

#include "tcl/log.h"
#include "tcl/dispatcher.h"

void dispatcher_init(dispatcher_t *dispatcher, const char *name)
{
	map_init(&dispatcher->dispatch_table, NULL, NULL);
	strncpy(dispatcher->name, name, DISPATCHER_NAME_MAX - 1);
}

void dispatcher_destroy(dispatcher_t *dispatcher)
{
	map_destroy(&dispatcher->dispatch_table, NULL);
}

void dispatcher_register(dispatcher_t *dispatcher, uintptr_t type,
	blocking_queue_t *queue)
{
	map_remove(&dispatcher->dispatch_table, (void *)type);
	if (queue) map_set(&dispatcher->dispatch_table, (void *)type, queue);
}

void dispatcher_register_default(dispatcher_t *dispatcher,
	blocking_queue_t *queue)
{
	dispatcher->default_queue = queue;
}

bool dispatcher_dispatch(dispatcher_t *dispatcher, uintptr_t type, void *data)
{
	blocking_queue_t *queue;

	if (!(queue = map_get(&dispatcher->dispatch_table, (void *)type)))
		queue = dispatcher->default_queue;
	if (queue)
		blocking_queue_push(queue, data);
	else
		LOG_WARN("has no handler for type (%zd) in data dispatcher named: %s",
			type, dispatcher->name);
	return queue != NULL;
}

