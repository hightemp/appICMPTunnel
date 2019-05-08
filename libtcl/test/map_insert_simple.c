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

#include "tcl/map.h"

static size_t hash_map_fn00(const void *key)
{
	size_t hash_key = (size_t)key;
	return hash_key;
}

int main(void)
{
	map_t map;
	struct { const char *key, *value; } data[] = {
		{ "0", "zero" },
		{ "1", "one" },
		{ "2", "two" },
		{ "3", "three" },
	};
	const char *value;
	const size_t data_sz = sizeof(data) / sizeof(data[0]);
	size_t i;

	map_init(&map, hash_map_fn00, NULL);

	for (i = 0; i < data_sz; i++) {
		ASSERT_EQ(i, map_length(&map));
		ASSERT_EQ(0, map_set(&map, data[i].key, data[i].value));
		ASSERT(value = map_get(&map, data[i].key));
		ASSERT_STREQ(data[i].value, value);
		ASSERT_EQ(i + 1, map_length(&map));
	}

	map_destroy(&map, NULL);
	return 0;
}
