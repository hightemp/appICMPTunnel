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

static void setup(void);
static void teardown(void);
static void test(void);

#define  SETUP setup
#define  TEARDOWN teardown
#define  TEST test

#include "test.h"

#include "tcl/io.h"
#include "tcl/reader.h"

static int pipefd[2];
static sema_t sema;

static void setup(void)
{
	pipe(pipefd);
	sema_init(&sema, 0);
}

static void teardown(void)
{
	sema_destroy(&sema);
	close(pipefd[0]);
	close(pipefd[1]);
}

void test(void)
{
	eager_reader_t reader;

	eager_reader_init(&reader, pipefd[0], DFT_ALLOCATOR, 32, UINT_MAX);
	eager_reader_destroy(&reader);
}
