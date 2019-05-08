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

#include "tcl/queue.h"

int main(void)
{
	fifo_t queue;
	int i;

	queue_init(&queue, sizeof(int));

	*(int *)queue_push(&queue) = 1;
	*(int *)queue_push(&queue) = -2;
	*(int *)queue_push(&queue) = 3;

	ASSERT_EQ(3, queue_length(&queue));

	ASSERT_TRUE(queue_pop(&queue, &i));
	ASSERT_EQ(1, i);
	ASSERT_EQ(2, queue_length(&queue));

	ASSERT_EQ(-2, *(int *)queue_peek(&queue));

	ASSERT_TRUE(queue_pop(&queue, &i));
	ASSERT_EQ(-2, i);
	ASSERT_EQ(1, queue_length(&queue));

	ASSERT_TRUE(queue_pop(&queue, &i));
	ASSERT_EQ(3, i);
	ASSERT_EQ(0, queue_length(&queue));

	ASSERT_FALSE(queue_pop(&queue, &i));
	ASSERT_NULL(queue_peek(&queue));

	queue_destroy(&queue, NULL);
	return 0;
}
