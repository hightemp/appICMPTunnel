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

#include "tcl/blocking_queue.h"

static const char *DUMMY_DATA_STRING = "Dummy data string";

int main(void)
{
	blocking_queue_t blocking_queue;
	const char *str;

	ASSERT_EQ(0, blocking_queue_init(&blocking_queue, 1));
	ASSERT_EQ(0, blocking_queue_length(&blocking_queue));

	ASSERT_TRUE(blocking_queue_trypush(&blocking_queue, DUMMY_DATA_STRING));
	ASSERT_EQ(1, blocking_queue_length(&blocking_queue));

	ASSERT(str = blocking_queue_trypop(&blocking_queue));
	ASSERT_STREQ(DUMMY_DATA_STRING, str);
	ASSERT_EQ(0, blocking_queue_length(&blocking_queue));

	blocking_queue_destroy(&blocking_queue, NULL);
	return 0;
}
