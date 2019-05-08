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

static coro_t a;
static coro_t b;

static void *call_b(void *arg)
{
	(void)arg;
	printf("coro b\n");
	return NULL;
}

static void *call_a(void *arg)
{
	(void)arg;
	printf("begin coro a\n");
	coro_resume(&b, NULL);
	printf("end coro a\n");
	return NULL;
}

int main(void)
{
	coro_init(&a, call_a, 32);
	coro_init(&b, call_b, 32);
	printf("begin main\n");
	coro_resume(&a, NULL);
	printf("end main\n");
	ASSERT_NULL(a);
	ASSERT_NULL(b);
	return 0;
}
