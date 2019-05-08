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

#include "tcl/coro.h"

static char *result[] = { "a", "b", "c", "d" };

static void *yield_return_value(void *arg)
{
	printf("%s\n", (char *) arg);
	printf("%s\n", (char *) coro_yield(result[1]));
	return result[3];
}

int main(void)
{
	coro_t coro;

	coro_init(&coro, yield_return_value, 32);
	printf("%s\n", (char *) coro_resume(&coro, result[0]));
	printf("%s\n", (char *) coro_resume(&coro, result[2]));
	ASSERT_NULL(coro);
	return 0;
}
