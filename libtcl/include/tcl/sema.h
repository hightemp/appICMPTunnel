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

/*!@file tcl/sema.h
 * @author uael
 *
 * @addtogroup tcl.sema @{
 */
#ifndef __TCL_SEMA_H
# define __TCL_SEMA_H

#include "tcl/event.h"

/*!@public
 *
 * @brief
 * The semaphore opaque structure.
 */
typedef struct sema sema_t;

/*!@public
 *
 * @brief
 * The sema structure definition
 */
struct sema {

	/*! TODO. */
	event_t event;
};

/*!@public
 *
 * @brief
 * TODO
 *
 * @param value
 * @return
 */
__api int sema_init(sema_t *sema, unsigned value);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param sema
 */
__api void sema_destroy(sema_t *sema);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param sema
 */
__api void sema_wait(sema_t *sema);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param sema
 * @return
 */
__api bool sema_trywait(sema_t *sema);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param sema
 */
__api int sema_post(sema_t *sema);

#endif /* __TCL_SEMA_H */
/*!@} */
