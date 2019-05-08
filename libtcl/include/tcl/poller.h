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

/*!@file tcl/poller.h
 * @author uael
 *
 * @addtogroup tcl.reactor @{
 */
#ifndef __TCL_POLLER_H
# define __TCL_POLLER_H

#include "tcl/event.h"
#include "tcl/map.h"

#define POLLER_IN (1 << 0)
#define POLLER_OUT (1 << 1)

/*!@public
 *
 * @brief
 * The poller structure declaration.
 */
typedef struct poller poller_t;

/*!@public
 *
 * @brief
 * The poller event structure declaration.
 */
typedef struct pollev pollev_t;

/*!@public
 *
 * @brief
 * The poller structure definition.
 */
struct poller {

#ifdef TCL_THREADING

	/*! TODO */
	int fd;
#else

	/*! TODO */
	map_t events;

	/*! TODO */
	unsigned size;
#endif
};

/*!@public
 *
 * @brief
 * The poller event structure definition.
 */
struct pollev {

	/*! TODO */
	uint32_t events;

	/*! TODO */
	void *ptr;
};

__api int poller_init(poller_t *poller, unsigned size);

__api void poller_destroy(poller_t *poller);

__api int poller_add(poller_t *poller, event_t *event, pollev_t attr);

__api int poller_del(poller_t *poller, event_t *event);

__api int poller_wait(poller_t *poller, pollev_t *events, int size);

#endif /* !__TCL_POLLER_H */
/*!@} */
