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

#define CYCLE (500)

static size_t __counter;

static void *hello(void *arg)
{
	int i;

	(void)arg;
	for (i = 0; i < (CYCLE * 10) - 1; ++i) {
		++__counter;
		coro_yield(NULL);
	}
	++__counter;
	return NULL;
}

int main(void)
{
	int i, j;
	coro_t coros[CYCLE];

	for (i = 0; i < CYCLE; ++i)
		coro_init(&coros[i], hello, 32);
	for (i = 0; i < (CYCLE * 10); ++i)
		for (j = 0; j < CYCLE; ++j) {
			++__counter;
			coro_resume(&coros[j], NULL);
		}
	for (i = 0; i < CYCLE; ++i)
		ASSERT_NULL(coros[i]);
	ASSERT_EQ(CYCLE * (CYCLE * 10) * 2, __counter);
	return 0;
}
