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

void __coromake(coro_t ctx, uint16_t stack_size, fn_t *fn)
{
	uintptr_t *stack;

	stack = (uintptr_t *)((char *)ctx + ctx->ssize - 1);

	/* make stack-frame of coro_main 16-byte aligned
	 * for amd64-ABI and some systems (such as Mac OS) require this
	 */
	stack = (void *)((uintptr_t)stack & ~0x0fUL);

#ifdef __SANITIZE_ADDRESS__
	stack[0] = (uintptr_t)__coromain;
	/* here is 16-byte alignment boundary */
	stack[-1] = 0UL; // IP
	stack[-2] = (uintptr_t)fn;
	stack[-3] = 0UL; // BP
	stack[-4] = 0; // BX
	stack[-5] = (uintptr_t)&stack[-3]; // BP
	stack[-6] = stack[-2]; // DI
#else
	stack[0] = (uintptr_t)fn;
	/* here is 16-byte alignment boundary */
	stack[-1] = 0UL; // IP
	stack[-2] = (uintptr_t)__coromain;
	stack[-3] = (uintptr_t)__coromain; // BP
	stack[-4] = 0; // BX
	stack[-5] = (uintptr_t)&stack[-3]; // BP
	stack[-6] = stack[0]; // DI
#endif
	stack[-7] = 0UL; // SI
	ctx->sp = &stack[-7];
}

NOINLINE
void __coroswitch(coro_t from, coro_t to)
{
	asm volatile (
		"pushl %%ebx\n\t"
		"pushl %%ebp\n\t"
		"pushl %%edi\n\t"
		"pushl %%esi\n\t"
		"movl %%esp, %0\n\t"
		"movl %1, %%esp\n\t"
		"popl %%esi\n\t"
		"popl %%edi\n\t"
		"popl %%ebp\n\t"
		"popl %%ebx\n\t"
		:
		: "m"(from->sp), "r"(to->sp)
	);
}
