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

static void *generator(void *arg)
{
	static char *adjectives[] = { "small", "clean", NULL, "fast", NULL };
	char **adjective = adjectives;

	(void)arg;
	while (*adjective) {
		coro_yield(*adjective);
		++adjective;
	}
	return adjectives[3];
}

int main(void)
{
	coro_t coro;
	char *lol;

	coro_init(&coro, generator, 64);

	lol = (char *)coro_resume(&coro, NULL);
	ASSERT_STREQ("small", lol);
	printf("%s\n", lol);

	lol = (char *)coro_resume(&coro, NULL);
	ASSERT_STREQ("clean", lol);
	printf("%s\n", lol);

	lol = (char *)coro_resume(&coro, NULL);
	ASSERT_STREQ("fast", lol);
	printf("%s\n", lol);

	ASSERT_NULL(coro);
	return 0;
}
