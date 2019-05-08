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

/*!@file tcl/reactor.h
 * @author uael
 *
 * @addtogroup tcl.reactor @{
 */
#ifndef __TCL_REACTOR_H
# define __TCL_REACTOR_H

#include "tcl/fiber.h"
#include "tcl/set.h"
#include "tcl/poller.h"
#include "tcl/mutex.h"

/*!@public
 *
 * @brief
 * The reactor structure declaration.
 */
typedef struct reactor reactor_t;

/*!@public
 *
 * @brief
 * The reactor object structure declaration.
 */
typedef struct reactor_object reactor_object_t;

/*!@private
 *
 * @brief
 * Reactor status.
 */
typedef enum reactor_st reactor_st_t;

/*!@public
 *
 * @brief
 * TODO
 */
typedef void (reactor_ready_t)(void *context);

/*!@public
 *
 * @brief
 * The reactor structure definition.
 */
struct reactor {

	/*! TODO */
	poller_t poller;

	/*! TODO */
	event_t stopev;

	/*! indicates whether `run_worker' is valid. */
	bool is_running;

	/*! TODO */
	bool object_removed;
};

/*!@public
 *
 * @brief
 * The reactor object structure definition.
 */
struct reactor_object {

	/*! the event to monitor for events. */
	event_t *ev;

	/*! a context that's passed back to the *_ready functions. */
	void *context;

	/*! protects the lifetime of this object and all variables. */
	reactor_t *reactor;

	/*! protects the lifetime of this object and all variables. */
	mutex_t lock;

	/*! function to call when the file descriptor becomes readable. */
	reactor_ready_t *read_ready;

	/*! function to call when the file descriptor becomes writeable. */
	reactor_ready_t *write_ready;

	/*! TODO. */
	bool is_processed;
};

/*!@public
 *
 * @brief
 * The reactor status definition
 */
enum reactor_st {

	/*! `reactor_stop' was called. */
	REACTOR_STATUS_STOP,

	/*! there was an error during the operation. */
	REACTOR_STATUS_ERROR,

	/*! the reactor completed its work (for the _run_once* variants). */
	REACTOR_STATUS_DONE,
};

/*!@public
 *
 * @brief
 * Creates a new reactor object. Returns -1 on failure. The returned object
 * must be destroyed by calling `reactor_destroy'.
 *
 * @param reactor The reactor to initialize.
 * @return 0 on success -1 otherwise.
 */
__api int reactor_init(reactor_t *reactor);

/*!@public
 *
 * @brief
 * Frees a reactor object created with `reactor_init'.
 *
 * @param reactor The reactor to destroy.
 */
__api void reactor_destroy(reactor_t *reactor);

/*!@public
 *
 * @brief
 * Starts the reactor. This function blocks the caller until `reactor_stop' is
 * called from another worker or in a callback. `reactor' may not be NULL.
 *
 * @param reactor The reactor to start.
 * @return        The start status.
 */
__api reactor_st_t reactor_start(reactor_t *reactor);

/*!@public
 *
 * @brief
 * Runs one iteration of the reactor. This function blocks until at least one
 * registered object becomes ready. `reactor' may not be NULL.
 *
 * @param reactor The reactor to run once.
 * @return        The run once status.
 */
__api reactor_st_t reactor_run_once(reactor_t *reactor);

/*!@public
 *
 * @brief
 * Immediately unblocks the reactor. This function is safe to call from any
 * worker. `reactor' may not be NULL.
 *
 * @param reactor The reactor to stop.
 */
__api void reactor_stop(reactor_t *reactor);

/*!@public
 *
 * @brief
 * Registers a file descriptor with the reactor. The file descriptor, `fd', must
 * be valid when this function is called and its ownership is not transferred to
 * the reactor. The `context' variable is a user-defined opaque handle that is
 * passed back to the `read_ready' and `write_ready' functions. It is not copied
 * or even dereferenced by the reactor so it may contain any value including
 * NULL. The `read_ready' and `write_ready' arguments are optional and may be
 * NULL. This function returns an opaque object that represents the file
 * descriptor's registration with the reactor. When the caller is no longer
 * interested in events on the `fd', it must free the returned object by calling
 * `reactor_unregister'.
 *
 * @param reactor     Where to register.
 * @param ev          The event to monitor for events.
 * @param context     A context that's passed back to the *_ready functions.
 * @param read_ready  Function to call when the fd becomes readable.
 * @param write_ready Function to call when the fd becomes writeable.
 * @return            A fresh reactor object on success, NULL otherwise.
 */
__api reactor_object_t *reactor_register(reactor_t *reactor, event_t *ev,
	void *context, reactor_ready_t *read_ready, reactor_ready_t *write_ready);

/*!@public
 *
 * @brief
 * Unregisters a previously registered file descriptor with its reactor. `obj'
 * may not be NULL. `obj' is invalid after calling this function so the caller
 * must drop all references to it.
 *
 * @param obj The reactor object to unregister.
 */
__api void reactor_unregister(reactor_object_t *obj);

#endif /* !__TCL_REACTOR_H */
/*!@} */
