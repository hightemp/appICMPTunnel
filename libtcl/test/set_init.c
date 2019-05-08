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

	set_init(&set, hash_set_fn00, NULL);
	ASSERT_EQ(0, set.length);
	ASSERT_EQ(0, set.capacity);
	ASSERT_EQ(0, set.occupied);
	ASSERT_EQ(0, set.upbound);
	ASSERT_NULL(set.flags);
	ASSERT_NULL(set.keys);
	ASSERT(set.eq);
	ASSERT_EQ(hash_set_fn00, set.hash);
	return 0;
}
