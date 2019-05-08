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

/*!@file tcl/task.h
 * @author uael
 *
 * @addtogroup tcl.task @{
 */
#ifndef __TCL_TASK_H
# define __TCL_TASK_H

#include "tcl/fiber.h"

/*!@public
 *
 * @brief
 * The task structure declaration.
 */
typedef struct task task_t;

/*!@public
 *
 * @brief
 * The task structure definition.
 */
struct task {
	
	bool running;

	bool joined;

	routine_t *work;

	void *context;

#ifdef TCL_THREADING

	pthread_t pthread;
#else

	fiber_t fiber;
#endif
};

__api int task_spawn(task_t *task, routine_t *work, void *context);

__api int task_repeat(task_t *task, routine_t *work, void *context);

__api void task_destroy(task_t *task);

__api bool task_running(task_t *task);

__api int task_setpriority(task_t *task, int priority);

__api void task_stop(task_t *task);

__api void task_join(task_t *task);

__api void task_schedule(void);

#endif /* __TCL_TASK_H */
/*!@} */
