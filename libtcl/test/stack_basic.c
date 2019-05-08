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

#include "test.h"

#include "tcl/stack.h"

int main(void)
{
	lifo_t stack;
	int i;

	stack_init(&stack, sizeof(int));

	*(int *)stack_push(&stack) = 1;
	*(int *)stack_push(&stack) = -2;
	*(int *)stack_push(&stack) = 3;

	ASSERT_EQ(3, stack_length(&stack));

	ASSERT_TRUE(stack_pop(&stack, &i));
	ASSERT_EQ(3, i);
	ASSERT_EQ(2, stack_length(&stack));

	ASSERT_EQ(-2, *(int *)stack_peek(&stack));

	ASSERT_TRUE(stack_pop(&stack, &i));
	ASSERT_EQ(-2, i);
	ASSERT_EQ(1, stack_length(&stack));

	ASSERT_TRUE(stack_pop(&stack, &i));
	ASSERT_EQ(1, i);
	ASSERT_EQ(0, stack_length(&stack));

	ASSERT_FALSE(stack_pop(&stack, &i));
	ASSERT_NULL(stack_peek(&stack));

	stack_destroy(&stack, NULL);
	return 0;
}
