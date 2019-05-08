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

#define LOG_TAG "tcl_mutex"

#include "tcl/log.h"
#include "tcl/mutex.h"
#include "tcl/fiber.h"

int mutex_init(mutex_t *mutex)
{
#ifdef TCL_THREADING
	int st;

	if ((st = pthread_mutex_init(&mutex->mutex, NULL)))
		return st;
#else
	event_init(&mutex->ev, 1, EVENT_SEMAPHORE);
#endif /* TCL_THREADING */
	return 0;
}

void mutex_destroy(mutex_t *mutex)
{
#ifdef TCL_THREADING
	pthread_mutex_destroy(&mutex->mutex);
#else
# ifndef NDEBUG
	event_value_t val;

	assert(event_tryread(&mutex->ev, &val));
# endif
	event_destroy(&mutex->ev);
#endif /* TCL_THREADING */
}

void mutex_lock(mutex_t *mutex)
{
#ifdef TCL_THREADING
	pthread_mutex_lock(&mutex->mutex);
#else
	event_value_t val;

	do event_read(&mutex->ev, &val); while (val != 1);
#endif /* TCL_THREADING */
}

void mutex_unlock(mutex_t *mutex)
{
#ifdef TCL_THREADING
	pthread_mutex_unlock(&mutex->mutex);
#else
	event_write(&mutex->ev, 1UL);
#endif /* TCL_THREADING */
}

static mutex_t __global_lock;

int mutex_global_init(void)
{
#ifdef TCL_THREADING
	int st;
	pthread_mutexattr_t attr;

	if ((st = pthread_mutexattr_init(&attr)))
		return st;
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
	if ((st = pthread_mutex_init(&__global_lock.mutex, &attr)))
		return st;
	return 0;
#else
	return mutex_init(&__global_lock);
#endif /* TCL_THREADING */
}

void mutex_global_cleanup(void)
{
	mutex_destroy(&__global_lock);
}

void mutex_global_lock(void)
{
	mutex_lock(&__global_lock);
}

void mutex_global_unlock(void)
{
	mutex_unlock(&__global_lock);
}
