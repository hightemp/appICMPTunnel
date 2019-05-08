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

#define LOG_TAG "test_reader_small_data"

#include "tcl.h"
#include "tcl/log.h"
#include "tcl/io.h"
#include "tcl/reader.h"

static int pipefd[2];
static sema_t sema;
static char *small_data = "white chocolate lindor truffles";

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

static void expect_data(eager_reader_t *reader, void *context)
{
	char *data = (char *)context;
	int length = (int)strlen(data);

	for (int i = 0; i < length; i++) {
		uint8_t byte;
		ASSERT_EQ((size_t)1, eager_reader_read(reader, &byte, 1));
		printf("read: %c\n", byte);
		ASSERT_EQ(data[i], byte);
	}
	sema_post(&sema);
}

void test(void)
{
	eager_reader_t reader;
	worker_t read_worker;

	tcl_init();
	eager_reader_init(&reader, pipefd[0], DFT_ALLOCATOR, 32, UINT_MAX);
	worker_init(&read_worker, "read");

	eager_reader_register(&reader, &read_worker, expect_data, small_data);
	write(pipefd[1], small_data, strlen(small_data));
	sema_wait(&sema);
	eager_reader_unregister(&reader);

	worker_destroy(&read_worker);
	eager_reader_destroy(&reader);
	tcl_cleanup();
}
