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
#define LOG_TAG "tcl_coro_win"

#include "tcl/log.h"
#include "coro/internal.h"

coro_t __coroalloc(size_t stack_size)
{
	coro_t coro;

	if (!(coro = calloc(1, sizeof(struct coro))))
		return NULL;
	if (!stack_size)
		stack_size = 256 * 1024;
	coro->ssize = stack_size;
	return coro;
}

void __cororelease(coro_t coro)
{
	free(coro);
}

void __coromake(coro_t coro, routine_t *fn)
{
	coro->handle = CreateFiberEx(coro->ssize / 2, coro->ssize, 0,
		(LPFIBER_START_ROUTINE)__coromain, fn);
}

void __coroswitch(coro_t from, coro_t to)
{
	if (!from->handle) {
		if (!(from->handle = ConvertThreadToFiber(0))) {
			if (GetLastError() != ERROR_ALREADY_FIBER)
				return;
			from->handle = GetCurrentFiber();
		}
	}
	SwitchToFiber(to->handle);
}
