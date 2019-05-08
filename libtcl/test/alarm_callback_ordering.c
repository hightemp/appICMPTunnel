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

#define LOG_TAG "test_alarm_callback_ordering"

#include "alarm.h"

int main(void)
{
	alarm_t alarms[100];
	char alarm_name[50];

	sema_init(&semaphore, 0);
	strcpy(alarm_name, "alarm_test.test_callback_ordering[000]");
	for (int i = 0; i < 100; i++) {
		alarm_name[34] = (char)(i % 1000 / 100 + '0');
		alarm_name[35] = (char)(i % 100 / 10 + '0');
		alarm_name[36] = (char)(i % 10 + '0');
		alarm_init(alarms + i, alarm_name);
	}

	for (int i = 0; i < 100; i++) {
		alarm_set(alarms + i, 100, ordered_cb, INT_TO_PTR(i));
	}

	for (int i = 1; i <= 100; i++) {
		sema_wait(&semaphore);
		ASSERT_GE(cb_counter, i);
	}
	ASSERT_EQ(cb_counter, 100);
	ASSERT_EQ(cb_misordered_counter, 0);

	for (int i = 0; i < 100; i++)
		alarm_destroy(alarms + i);
	alarm_cleanup();
	return 0;
}
