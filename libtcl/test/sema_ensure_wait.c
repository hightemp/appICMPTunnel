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

#include "tcl/time.h"
#include "tcl/worker.h"

typedef struct {
	sema_t *sema;
	int counter;
} dummy_t;

static void *sleep_then_increment_counter(void *context) {
	dummy_t *helper;

	helper = (dummy_t *)context;
	ASSERT(helper);
	ASSERT(helper->sema);
	usleep(50 * 1000);
	++helper->counter;
	sema_post(helper->sema);
	return NULL;
}

int main(void)
{
	sema_t sema;
	worker_t worker;
	dummy_t dummy;

	ASSERT_EQ(0, sema_init(&sema, 0));
	ASSERT_EQ(0, worker_init(&worker, "SEMA_TEST"));

	ASSERT_FALSE(sema_trywait(&sema));

	dummy.sema = &sema;
	dummy.counter = 0;

	worker_post(&worker, sleep_then_increment_counter, &dummy);
	sema_wait(&sema);
	ASSERT_EQ(1, dummy.counter);

	worker_post(&worker, sleep_then_increment_counter, &dummy);
	sema_wait(&sema);
	ASSERT_EQ(2, dummy.counter);

	worker_post(&worker, sleep_then_increment_counter, &dummy);
	sema_wait(&sema);
	ASSERT_EQ(3, dummy.counter);

	worker_post(&worker, sleep_then_increment_counter, &dummy);
	worker_post(&worker, sleep_then_increment_counter, &dummy);
	sema_wait(&sema);
	sema_wait(&sema);
	ASSERT_EQ(5, dummy.counter);

	sema_destroy(&sema);
	worker_destroy(&worker);
	return 0;
}
