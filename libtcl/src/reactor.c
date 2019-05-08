/*
 * Copyright (C) 2014 Google, Inc.
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

#define LOG_TAG "tcl_reactor"

#include "tcl/reactor.h"
#include "tcl/string.h"
#include "tcl/log.h"

#ifndef EFD_SEMAPHORE
# define EFD_SEMAPHORE (1 << 0)
#endif /* !EFD_SEMAPHORE */

#define __MAX_EVENTS 64

static reactor_st_t __run_reactor(reactor_t *reactor, int iterations);

int reactor_init(reactor_t *reactor)
{
	pollev_t event;

	if (poller_init(&reactor->poller, __MAX_EVENTS)) {
		LOG_ERROR("unable to create epoll instance: %s", strerror(errno));
		return -1;
	}
	if (event_init(&reactor->stopev, 0, 0)) {
		LOG_ERROR("unable to create eventfd: %s", strerror(errno));
		return -1;
	}
	bzero(&event, sizeof(event));
	event.events = POLLER_IN;
	event.ptr = NULL;
	if (poller_add(&reactor->poller, &reactor->stopev, event)) {
		LOG_ERROR("unable to register eventfd with epoll set: %s",
			strerror(errno));
		return -1;
	}
	return 0;
}

void reactor_destroy(reactor_t *reactor)
{
	poller_destroy(&reactor->poller);
	event_destroy(&reactor->stopev);
}

reactor_st_t reactor_start(reactor_t *reactor)
{
	return __run_reactor(reactor, 0);
}

reactor_st_t reactor_run_once(reactor_t *reactor)
{
	return __run_reactor(reactor, 1);
}

void reactor_stop(reactor_t *reactor)
{
	event_write(&reactor->stopev, 1UL);
}

reactor_object_t *reactor_register(reactor_t *reactor, event_t *ev,
	void *context, reactor_ready_t *read_ready, reactor_ready_t *write_ready)
{
	reactor_object_t *object;
	pollev_t event;

	object = (reactor_object_t *)malloc(sizeof(reactor_object_t));
	object->reactor = reactor;
	object->ev = ev;
	object->context = context;
	object->read_ready = read_ready;
	object->write_ready = write_ready;
	object->is_processed = false;
	event.events = 0;
	if (read_ready) event.events |= POLLER_IN;
	if (write_ready) event.events |= POLLER_OUT;
	event.ptr = object;
	if (poller_add(&reactor->poller, ev, event)) {
		LOG_ERROR("unable to register ev to epoll set: %s", strerror(errno));
		free(object);
		return NULL;
	}
	mutex_init(&object->lock);
	return object;
}

void reactor_unregister(reactor_object_t *obj)
{
	reactor_t *reactor = obj->reactor;

	poller_del(&reactor->poller, obj->ev);
	obj->read_ready = NULL;
	obj->write_ready = NULL;
	if (obj->is_processed) {
		reactor->object_removed = true;
		return;
	}
	mutex_destroy(&obj->lock);
	free(obj);
}

static reactor_st_t __run_reactor(reactor_t *reactor, int iterations)
{
	int ret, i, j;
	reactor_object_t *object;
	pollev_t events[__MAX_EVENTS];
	
	reactor->is_running = true;
	for (i = 0; iterations == 0 || i < iterations; ++i) {

		ret = poller_wait(&reactor->poller, events, (int)__MAX_EVENTS);
		if (ret < 0) {
			LOG_ERROR("error in epoller_wait: %s", strerror(errno));
			reactor->is_running = false;
			return REACTOR_STATUS_ERROR;
		}

		for (j = 0; j < ret; ++j) {
			/*
			 * The event file descriptor is the only one that registers with
			 * a NULL data pointer. We use the NULL to identify it and break
			 * out of the reactor loop.
			 */
			if (events[j].ptr == NULL) {
				event_value_t value;
				
				event_read(&reactor->stopev, &value);
				reactor->is_running = false;
				return REACTOR_STATUS_STOP;
			}
			object = (reactor_object_t *)events[j].ptr;
			object->is_processed = true;

			/* Downgrade the list lock to an object lock. */
			mutex_lock(&object->lock);
			reactor->object_removed = false;
			if ((events[j].events & POLLER_IN) && object->read_ready)
				object->read_ready(object->context);
			if (!reactor->object_removed && (events[j].events & POLLER_OUT)
				&& object->write_ready)
				object->write_ready(object->context);
			mutex_unlock(&object->lock);

			object->is_processed = false;
			if (reactor->object_removed) {
				mutex_destroy(&object->lock);
				free(object);
			}
		}
	}
	reactor->is_running = false;
	return REACTOR_STATUS_DONE;
}
