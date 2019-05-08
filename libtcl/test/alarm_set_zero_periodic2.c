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

#define LOG_TAG "test/alarm"

#include "alarm.h"

int main(void)
{
	alarm_t alarm;
	alarm_t alarm2;

	alarm_init_periodic(&alarm,
		"1test_set_zero_periodic.test_set_zero_periodic");
	alarm_init_periodic(&alarm2,
		"2test_set_zero_periodic.test_set_zero_periodic");

	sema_init(&semaphore, 0);
	sema_init(&semaphore2, 0);

	alarm_set(&alarm, 25, cb, NULL);
	alarm_set(&alarm2, 25, cb2, NULL);

	for (int i = 1; i <= 10; i++) {
		sema_wait(&semaphore);
		sema_post(&semaphore2);
		ASSERT_GE(cb_counter, i);
		ASSERT_GE(i, cb_counter2);
	}

	while (cb_counter2 != 10) {
		task_schedule();
	}
	ASSERT_EQ(10, cb_counter2);

	alarm_destroy(&alarm);
	alarm_destroy(&alarm2);

	alarm_cleanup();
	return 0;
}
