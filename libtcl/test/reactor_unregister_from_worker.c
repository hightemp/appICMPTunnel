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

#include "tcl/reactor.h"

static pthread_t worker;

static void *reactor_worker(void *ptr)
{
	reactor_t *reactor;

	reactor = (reactor_t *)ptr;
	reactor_start(reactor);
	return NULL;
}

static void spawn_reactor_worker(reactor_t *reactor)
{
	int ret;

	ret = pthread_create(&worker, NULL, reactor_worker, reactor);
	ASSERT_EQ(0, ret);
}

static void join_reactor_worker(void)
{
	pthread_join(worker, NULL);
}

int main(void)
{
	event_t ev;
	reactor_t reactor;
	reactor_object_t *object;

	ASSERT_EQ(0, reactor_init(&reactor));
	event_init(&ev, 0, 0);;
	object = reactor_register(&reactor, &ev, NULL, NULL, NULL);
	spawn_reactor_worker(&reactor);
	usleep(50 * 1000);
	reactor_unregister(object);
	reactor_stop(&reactor);
	join_reactor_worker();

	event_destroy(&ev);
	reactor_destroy(&reactor);
	return 0;
}
