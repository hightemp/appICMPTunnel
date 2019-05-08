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

#include "tcl/vector.h"

int main(void)
{
	vector_t vector;

	vector_init(&vector, sizeof(int));
	ASSERT_NULL(vector.buffer);
	ASSERT_EQ(sizeof(int), vector.isize);
	ASSERT_EQ(0, vector.capacity);
	ASSERT_EQ(0, vector.length);
	vector_destroy(&vector, NULL);
	ASSERT_NULL(vector.buffer);
	ASSERT_EQ(sizeof(int), vector.isize);
	ASSERT_EQ(0, vector.capacity);
	ASSERT_EQ(0, vector.length);
	return 0;
}
