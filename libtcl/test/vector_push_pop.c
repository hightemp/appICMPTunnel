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
	int values[] = { 42, 21, 7, 4096, 3 };
	int *item;
	int i;

	vector_init(&vector, sizeof(int));

	memcpy(vector_npush_back(&vector, 5), values, 5 * sizeof(int));
	ASSERT_EQ(5, vector_length(&vector));
	ASSERT_EQ(32, vector.capacity);

	i = 0;
	vector_foreach(&vector, item) ASSERT_EQ(values[i++], *item);
	ASSERT_EQ(5, i);
	ASSERT_EQ(vector_end(&vector), item);

	ASSERT_TRUE(vector_pop_back(&vector, &i));
	ASSERT_EQ(3, i);

	ASSERT_TRUE(vector_pop_back(&vector, &i));
	ASSERT_EQ(4096, i);

	*(int *) vector_push_back(&vector) = 12;

	ASSERT_TRUE(vector_pop_back(&vector, &i));
	ASSERT_EQ(12, i);

	ASSERT_TRUE(vector_pop_back(&vector, &i));
	ASSERT_EQ(7, i);

	ASSERT_TRUE(vector_pop_back(&vector, &i));
	ASSERT_EQ(21, i);

	ASSERT_TRUE(vector_pop_back(&vector, &i));
	ASSERT_EQ(42, i);

	vector_destroy(&vector, NULL);
	return 0;
}
