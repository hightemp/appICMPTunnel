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

#include "tcl/stack.h"

FORCEINLINE void stack_init(lifo_t *stack, size_t isize)
{
	vector_init(&stack->base, isize);
}

FORCEINLINE void stack_destroy(lifo_t *stack, lifo_dtor_t *idtor)
{
	vector_destroy(&stack->base, idtor);
}

FORCEINLINE void stack_clear(lifo_t *stack, lifo_dtor_t *idtor)
{
	vector_clear(&stack->base, idtor);
}

FORCEINLINE size_t stack_length(lifo_t *stack)
{
	return vector_length(&stack->base);
}

FORCEINLINE void *stack_peek(lifo_t *stack)
{
	return vector_back(&stack->base);
}

FORCEINLINE void stack_ensure(lifo_t *stack, size_t n)
{
	vector_ensure(&stack->base, n);
}

FORCEINLINE void stack_grow(lifo_t *stack, size_t n_added)
{
	vector_grow(&stack->base, n_added);
}

FORCEINLINE void *stack_npush(lifo_t *stack, size_t n)
{
	return vector_npush_back(&stack->base, n);
}

FORCEINLINE size_t stack_npop(lifo_t *stack, size_t n, void *out)
{
	return vector_npop_back(&stack->base, n, out);
}

FORCEINLINE void *stack_push(lifo_t *stack)
{
	return stack_npush(stack, 1);
}

FORCEINLINE bool stack_pop(lifo_t *stack, void *out)
{
	return stack_npop(stack, 1, out) == 1;
}
