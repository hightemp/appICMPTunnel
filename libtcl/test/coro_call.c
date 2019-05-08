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

static int counter = 0;

static void *call(void *arg)
{
	(void)arg;
	++counter;
	ASSERT(counter == 2);
	return NULL;
}

int main(void)
{
	coro_t coro;
	char *result;

	coro_init(&coro, call, 32);
	++counter;
	ASSERT(counter == 1);
	(void)(result = coro_resume(&coro, NULL));
	++counter;
	ASSERT(counter == 3);
	ASSERT(!result);
	ASSERT_NULL(coro);
	return 0;
}
