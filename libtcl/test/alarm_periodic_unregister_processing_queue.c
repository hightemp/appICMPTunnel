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

#define LOG_TAG "test_alarm_periodic_unregister_processing_queue"

#include "alarm.h"


int main(void)
{
	alarm_t alarms[5];
	blocking_queue_t queue;
	worker_t worker;

	sema_init(&semaphore, 0);
	blocking_queue_init(&queue, UINT32_MAX);
	worker_init(&worker, "timers.periodic_unregister_processing_queue.worker");

	alarm_register(&queue, &worker);
	char alarm_name[50];

	strcpy(alarm_name, "alarm_test.test_callback_ordering_on_queue[000]");
	for (int i = 0; i < 5; i++) {
		alarm_name[43] = (char)(i % 1000 / 100 + '0');
		alarm_name[44] = (char)(i % 100 / 10+ '0');
		alarm_name[45] = (char)(i % 10 + '0');
		alarm_init_periodic(alarms + i, alarm_name);
	}

	// Schedule each of the timers with different period
	for (int i = 0; i < 5; i++) {
		alarm_attach(alarms + i, (period_ms_t)25 + i,
			cb, INT_TO_PTR(i), &queue);
	}

	for (int i = 1; i <= 20; i++) {
		sema_wait(&semaphore);

		ASSERT_GE(cb_counter, i);
	}

	// Test that all timers are still scheduled
	for (int i = 0; i < 5; i++) {
		ASSERT_TRUE(alarm_is_scheduled(alarms + i));
	}

	alarm_unregister(&queue);

	int saved_cb_counter = cb_counter;

	// Test that none of the timers are scheduled
	for (int i = 0; i < 5; i++) {
		ASSERT_FALSE(alarm_is_scheduled(alarms + i));
	}

	// Sleep for 500ms and test again that the cb_counter hasn't been modified
	usleep(500 * 1000);
	ASSERT_TRUE(cb_counter == saved_cb_counter);

	for (int i = 0; i < 5; i++) {
		alarm_destroy(alarms + i);
	}

	blocking_queue_destroy(&queue, NULL);
	worker_destroy(&worker);
	sema_destroy(&semaphore);

	alarm_cleanup();

	return 0;
}
