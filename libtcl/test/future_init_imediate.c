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

static const char *pass_back_data1 =
	"what kind of ice cream truck plays the worst christmas song of all time?";

int main(void)
{
	future_t future;

	future_immediate(&future, pass_back_data1);
	ASSERT_STREQ(pass_back_data1, future_await(&future));
	return 0;
}
