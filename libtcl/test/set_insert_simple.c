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

#include "tcl/set.h"

static size_t hash_set_fn00(const void *key)
{
	size_t hash_key = (size_t)key;
	return hash_key;
}

int main(void)
{
	set_t set;
	struct { const char *key, *value; } data[] = {
		{ "0" },
		{ "1" },
		{ "2" },
		{ "3" },
	};
	const size_t data_sz = sizeof(data) / sizeof(data[0]);
	size_t i;

	set_init(&set, hash_set_fn00, NULL);

	for (i = 0; i < data_sz; i++) {
		ASSERT_EQ(i, set_length(&set));
		ASSERT_EQ(0, set_put(&set, data[i].key));
		ASSERT_TRUE(set_contains(&set, data[i].key));
		ASSERT_EQ(i + 1, set_length(&set));
	}

	set_destroy(&set);
	return 0;
}
