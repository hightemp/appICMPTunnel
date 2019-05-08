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

/*!@file tcl/coro.h
 * @author uael
 *
 * @addtogroup tcl.coro @{
 */
#ifndef __TCL_CORO_H
# define __TCL_CORO_H

#include "tcl/compat.h"

typedef void *(routine_t)(void *);

typedef struct coro *coro_t;

__api int coro_init(coro_t *coro, routine_t *fn, size_t stack_size);

__api void coro_kill(coro_t *coro);

__api void coro_setdata(coro_t coro, uintptr_t data);

__api uintptr_t coro_getdata(coro_t coro);

__api void *coro_resume(coro_t *coro, void *arg);

__api void *coro_yield(void *arg);

__api NORETURN void coro_exit(void *retval);

__api coro_t coro_self(void);

#endif /* !__TCL_CORO_H */
/*!@} */
