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

void __coromake(coro_t ctx, routine_t *fn)
{
	uintptr_t *stack;

	stack = (uintptr_t *)((char *)ctx + ctx->ssize - sizeof(uintptr_t));

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
	stack[-7] = 0; // R12
	stack[-8] = 0; // R13
	stack[-9] = 0; // R14
	stack[-10] = 0; // R15
	ctx->sp = &stack[-10];
}

NOINLINE
void __coroswitch(coro_t from, coro_t to)
{
	asm volatile (
		"pushq %%rbx\n\t"
		"pushq %%rbp\n\t"
		"pushq %%rdi\n\t"
		"pushq %%r12\n\t"
		"pushq %%r13\n\t"
		"pushq %%r14\n\t"
		"pushq %%r15\n\t"
		"movq %%rsp, %0\n\t"
		"movq %1, %%rsp\n\t"
		"popq %%r15\n\t"
		"popq %%r14\n\t"
		"popq %%r13\n\t"
		"popq %%r12\n\t"
		"popq %%rdi\n\t"
		"popq %%rbp\n\t"
		"popq %%rbx\n\t"
		:
		: "m"(from->sp), "r"(to->sp)
	);
}
