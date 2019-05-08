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

/*!@file tcl/event.h
 * @author uael
 *
 * @addtogroup tcl.event @{
 */
#ifndef __TCL_EVENT_H
# define __TCL_EVENT_H

#include "tcl/waitq.h"
#include "tcl/fiber.h"

/*!@public
 *
 * @brief
 * The event structure declaration.
 */
typedef struct event event_t;

/*!@public
 *
 * @brief
 * The event structure declaration.
 */
#ifdef TCL_THREADING
typedef eventfd_t event_value_t;

# define EVENT_NONE 0
# define EVENT_SEMAPHORE EFD_SEMAPHORE
#else
typedef uint64_t event_value_t;

# define EVENT_NONE 0
# define EVENT_SEMAPHORE (1 << 0)
#endif

#define EVENT_FLAGS_SET (EVENT_NONE | EVENT_SEMAPHORE)

/*!@public
 *
 * @brief
 * The event structure definition.
 */
struct event {

#ifdef TCL_THREADING

	/*! TODO. */
	int fd;
#else

	/*! TODO. */
	waitq_t wq;

	/*! TODO. */
	event_value_t count;

	/*! TODO. */
	unsigned flags;
#endif /* TCL_THREADING */
};

__api int event_init(event_t *event, event_value_t value, unsigned flags);

__api void event_destroy(event_t *event);

__api bool event_tryread(event_t *event, event_value_t *value);

__api int event_read(event_t *event, event_value_t *value);

__api int event_write(event_t *event, event_value_t value);

#endif /* !__TCL_EVENT_H */
/*!@} */
