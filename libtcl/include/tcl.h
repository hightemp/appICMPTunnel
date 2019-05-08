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

/*!@file tcl.h
 * @author uael
 */
#ifndef __TCL_H
# define __TCL_H

#include "tcl/conf.h"
#include "tcl/alarm.h"
#include "tcl/allocator.h"
#include "tcl/blocking_queue.h"
#include "tcl/buffer.h"
#include "tcl/compat.h"
#include "tcl/coro.h"
#include "tcl/dispatcher.h"
#include "tcl/event.h"
#include "tcl/fiber.h"
#include "tcl/future.h"
#include "tcl/io.h"
#include "tcl/list.h"
#include "tcl/map.h"
#include "tcl/module.h"
#include "tcl/mutex.h"
#include "tcl/poller.h"
#include "tcl/properties.h"
#include "tcl/queue.h"
#include "tcl/reader.h"
#include "tcl/reactor.h"
#include "tcl/ringbuffer.h"
#include "tcl/sema.h"
#include "tcl/stack.h"
#include "tcl/string.h"
#include "tcl/task.h"
#include "tcl/vector.h"
#include "tcl/waitq.h"
#include "tcl/worker.h"

/*!@public
 *
 * @brief
 * TODO
 */
__api int tcl_init(void);

/*!@public
 *
 * @brief
 * TODO
 */
__api void tcl_cleanup(void);

#endif /* __TCL_H */
