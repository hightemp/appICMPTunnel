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

#define LOG_TAG "test_alarm_set_short_long"

#include "alarm.h"

int main(void)
{
	alarm_t alarm[2];

	alarm_init(alarm, "alarm_test.test_set_short_long_0");
	alarm_init(alarm + 1, "alarm_test.test_set_short_long_1");

	sema_init(&semaphore, 0);
	alarm_set(alarm, 10, cb, NULL);
	alarm_set(alarm + 1, 10 + TIMER_INTERVAL_FOR_WAKELOCK_IN_MS + EPSILON_MS,
		cb, NULL);

	ASSERT_EQ(cb_counter, 0);

	sema_wait(&semaphore);

	ASSERT_EQ(cb_counter, 1);

	sema_wait(&semaphore);

	ASSERT_EQ(cb_counter, 2);

	alarm_destroy(alarm);
	alarm_destroy(alarm + 1);
	alarm_cleanup();
	return 0;
}
