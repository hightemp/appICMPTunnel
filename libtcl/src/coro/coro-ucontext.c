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

#include "coro/internal.h"

#include <ucontext.h>

static void __uctxmain(int arg1, int arg2)
{
	__coromain((void*(*)(void*))(
		((uintptr_t)(unsigned int)arg1) +
			((uintptr_t)(unsigned int)arg2 << (sizeof(int) * 8))));
}

void __coromake(coro_t coro, routine_t *fn)
{
	if (getcontext(&coro->ctx) < 0) {
		abort();
	}
	coro->ctx.uc_stack.ss_sp = (void *)coro + sizeof(struct coro);
	coro->ctx.uc_stack.ss_size = coro->ssize - sizeof(struct coro);
	coro->ctx.uc_link = NULL;
	makecontext(&coro->ctx, (void(*)())__uctxmain, 2,
		(int)fn, (int)((uintptr_t)fn >> (sizeof(int) * 8)));
}

void __coroswitch(coro_t from, coro_t to)
{
	if (swapcontext(&from->ctx, &to->ctx) < 0)
		abort();
}
