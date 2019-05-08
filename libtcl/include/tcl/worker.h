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

/*!@file tcl/worker.h
 * @author uael
 *
 * @addtogroup tcl.worker @{
 */
#ifndef __TCL_WORKER_H
# define __TCL_WORKER_H

#include "tcl/reactor.h"
#include "tcl/blocking_queue.h"
#include "tcl/stack.h"
#include "tcl/task.h"

#define WORKER_NAME_MAX 16

/*!@public
 *
 * @brief
 * The worker structure declaration.
 */
typedef struct worker worker_t;

/*!@public
 *
 * @brief
 * The worker structure definition.
 */
struct worker {

	char name[WORKER_NAME_MAX];

	blocking_queue_t work_queue;

	reactor_t reactor;

	task_t task;
};

__api int worker_init(worker_t *worker, char const *name);

__api void worker_destroy(worker_t *worker);

__api int worker_setpriority(worker_t *worker, int priority);

__api void worker_join(worker_t *worker);

__api bool worker_post(worker_t *worker, routine_t *work, void *context);

__api void worker_stop(worker_t *worker);

#endif /* __TCL_WORKER_H */
/*!@} */
