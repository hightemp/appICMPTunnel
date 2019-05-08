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

/*!@file tcl/mutex.h
 * @author uael
 *
 * @addtogroup tcl.mutex @{
 */
#ifndef __TCL_MUTEX_H
# define __TCL_MUTEX_H

#include "tcl/conf.h"
#include "tcl/event.h"
#include "tcl/queue.h"
#include "tcl/fiber.h"

/*!@public
 *
 * @brief
 * The mutex structure declaration.
 */
typedef struct mutex mutex_t;

/*!@public
 *
 * @brief
 * The mutex structure definition.
 */
struct mutex {

#ifdef TCL_THREADING

	pthread_mutex_t mutex;
#else

	event_t ev;
#endif /* TCL_THREADING */
};

/*!@public
 *
 * @brief
 * TODO
 *
 * @param mutex
 * @return
 */
__api int mutex_init(mutex_t *mutex);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param mutex
 */
__api void mutex_destroy(mutex_t *mutex);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param mutex
 */
__api void mutex_lock(mutex_t *mutex);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param mutex
 * @return
 */
__api void mutex_unlock(mutex_t *mutex);

/*!@public
 *
 * @brief
 * TODO
 *
 * @return
 */
__api int mutex_global_init(void);

/*!@public
 *
 * @brief
 * TODO
 */
__api void mutex_global_cleanup(void);

/*!@public
 *
 * @brief
 * TODO
 */
__api void mutex_global_lock(void);

/*!@public
 *
 * @brief
 * TODO
 */
__api void mutex_global_unlock(void);

#endif /* __TCL_MUTEX_H */
/*!@} */
