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

#include "tcl/future.h"
#include "tcl/worker.h"

static const char *pass_back_data0 =
	"fancy a sandwich? it's a fancy sandwich";

static void *post_to_future(void *context)
{
	future_ready((future_t *)context, (void *)pass_back_data0);
	return NULL;
}

int main(void)
{
	future_t future;
	worker_t worker;

	ASSERT_EQ(0, future_init(&future));
	ASSERT_EQ(0, worker_init(&worker, "ZOB"));

	worker_post(&worker, post_to_future, &future);
	ASSERT_STREQ(pass_back_data0, future_await(&future));

	worker_destroy(&worker);
	return 0;
}
