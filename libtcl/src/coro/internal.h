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

/*!@file coro/internal.h
 * @author uael
 */
#ifndef __CORO_INTERNAL_H
# define __CORO_INTERNAL_H

#include "tcl/coro.h"

#if defined(CORO_X86_64) || defined (CORO_X86_32) || \
	defined (CORO_ARM) || defined (CORO_UCONTEXT) || defined(CORO_WIN)

# ifdef CORO_UCONTEXT
#   include <ucontext.h>
# endif

__ext coro_t __coroalloc(size_t stack_size);
__ext void __cororelease(coro_t coro);
__ext void __coromake(coro_t from, routine_t *fn);
__ext void __coroswitch(coro_t from, coro_t to);
__ext void __coromain(routine_t *fn);

struct coro {

# if defined(CORO_UCONTEXT)

	/*! Unix ucontext. */
	ucontext_t ctx;
# elif defined(CORO_WIN)

	/*! Windows fiber handle ptr. */
	LPVOID handle;
# else

	/*! Stack pointer origin. */
	void *sp;
# endif

	/*! Stack size. */
	size_t ssize;

	/*! Coroutine linked list. */
	coro_t caller;

	/*! Last ret. */
	void *ret;

	/*! User data. */
	uintptr_t data;

	unsigned int flag;

#ifdef RUNNING_ON_VALGRIND

	int valgrind_id;
#elif !defined(CORO_UCONTEXT)

	char __padding[4];
#endif
};

#endif

#endif /* !__CORO_INTERNAL_H */
