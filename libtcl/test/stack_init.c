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

	stack_init(&stack, sizeof(int));
	ASSERT_NULL(stack.buffer);
	ASSERT_EQ(sizeof(int), stack.isize);
	ASSERT_EQ(0, stack.capacity);
	ASSERT_EQ(0, stack.length);
	stack_destroy(&stack, NULL);
	ASSERT_NULL(stack.buffer);
	ASSERT_EQ(sizeof(int), stack.isize);
	ASSERT_EQ(0, stack.capacity);
	ASSERT_EQ(0, stack.length);
	return 0;
}
