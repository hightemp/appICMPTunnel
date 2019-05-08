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

/*!@file tcl/dispatcher.h
 * @author uael
 *
 * @addtogroup tcl.dispatcher @{
 */
#ifndef __TCL_DISPATCHER_H
# define __TCL_DISPATCHER_H

#include "tcl/blocking_queue.h"
#include "tcl/map.h"

#define DISPATCHER_NAME_MAX 16

/*!@public
 *
 * @brief
 * The dispatcher structure declaration.
 */
typedef struct dispatcher dispatcher_t;

/*!@public
 *
 * @brief
 * The dispatcher structure definition.
 */
struct dispatcher {

	/*! TODO. */
	char name[DISPATCHER_NAME_MAX];

	/*! TODO. */
	map_t dispatch_table;

	/*! TODO. */
	blocking_queue_t *default_queue;
};

/*!@public
 *
 * @brief
 * Creates a new data dispatcher object, with the given name.
 * The returned object must be freed by calling |dispatcher_free|.
 * Returns NULL on failure. |name| may not be NULL.
 *
 * @param dispatcher
 * @param name
 */
__api void dispatcher_init(dispatcher_t *dispatcher, const char *name);

/*!@public
 *
 * @brief
 * Frees a data dispatcher object created by |dispatcher_new|.
 * |data_dispatcher| may be NULL.
 *
 * @param dispatcher
 */
__api void dispatcher_destroy(dispatcher_t *dispatcher);

/*!@public
 *
 * @brief
 * Registers |type| and |queue| with the data dispatcher so that data
 * sent under |type| ends up in |queue|. If |type| is already registered,
 * it is replaced. If |queue| is NULL, the existing registration is
 * removed, if it exists. |dispatcher| may not be NULL.
 *
 * @param dispatcher
 * @param type
 * @param queue
 */
__api void dispatcher_register(dispatcher_t *dispatcher, uintptr_t type,
	blocking_queue_t *queue);

/*!@public
 *
 * @brief
 * Registers a default queue to send data to when there is not a specific
 * type/queue relationship registered. If a default queue is already registered,
 * it is replaced. If |queue| is NULL, the existing registration is
 * removed, if it exists. |dispatcher| may not be NULL.
 *
 * @param dispatcher
 * @param queue
 */
__api void dispatcher_register_default(dispatcher_t *dispatcher,
	blocking_queue_t *queue);

/*!@public
 *
 * @brief
 * Dispatches |data| to the queue registered for |type|. If no such registration
 * exists, it is dispatched to the default queue if it exists.
 * Neither |dispatcher| nor |data| may be NULL.
 * Returns true if data dispatch was successful.
 *
 * @param dispatcher
 * @param type
 * @param data
 * @return
 */
__api bool dispatcher_dispatch(dispatcher_t *dispatcher, uintptr_t type,
	void *data);

#endif /* !__TCL_DISPATCHER_H */
/*!@} */
