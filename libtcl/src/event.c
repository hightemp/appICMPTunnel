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

#define LOG_TAG "bt_tcl_event"

#include "tcl/event.h"
#include "tcl/log.h"

int event_init(event_t *event, event_value_t value,
	unsigned flags)
{
#ifdef TCL_THREADING
	if ((event->fd = eventfd((unsigned)value, (int)flags)) < 0)
		return -1;
#else
	if (flags & ~EVENT_FLAGS_SET) {
		errno = EINVAL;
		return -1;
	}
	event->count = value;
	event->flags = flags;
	waitq_init(&event->wq);
#endif /* TCL_THREADING */
	return 0;
}

void event_destroy(event_t *event)
{
#ifdef TCL_THREADING
	if (event->fd >= 0)
		close(event->fd);
#else
	waitq_destroy(&event->wq);
#endif /* TCL_THREADING */
}

bool event_tryread(event_t *event, event_value_t *value)
{
#ifdef TCL_THREADING
	int flags;
	bool rc;

	if ((flags = fcntl(event->fd, F_GETFL)) < 0)
		return false;
	if (fcntl(event->fd, F_SETFL, flags | O_NONBLOCK) < 0)
		return false;
	rc = true;
	if (eventfd_read(event->fd, value) < 0)
    	rc = false;
	fcntl(event->fd, F_SETFL, flags);

	return rc;
#else
	event_value_t ucnt;

	if (!event->count)
		return false;
	ucnt = (event->flags & EVENT_SEMAPHORE) ? 1 : event->count;
	event->count -= ucnt;
	if (value) *value = ucnt;
	return true;
#endif /* TCL_THREADING */
}

int event_read(event_t *event, event_value_t *value)
{
#ifdef TCL_THREADING
	return eventfd_read(event->fd, value);
#else
	int res;
	event_value_t ucnt;

	if (event->count > 0) res = sizeof(ucnt);
	else {
		fiber_lock(&event->wq);
		while (true) {
			if (event->count > 0) {
				res = sizeof(ucnt);
				break;
			}
			fiber_yield();
		}
	}
	ucnt = (event->flags & EVENT_SEMAPHORE) ? 1 : event->count;
	if ((event->count -= ucnt))
		fiber_unlock(&event->wq);
	if (value) *value = ucnt;
	return res;
#endif /* TCL_THREADING */
}

int event_write(event_t *event, event_value_t value)
{
#ifdef TCL_THREADING
	return eventfd_write(event->fd, value);
#else
	event_value_t ucnt;

	ucnt = value;
	if (ucnt == U64_MAX) {
		errno = EINVAL;
		return -1;
	}
	if (U64_MAX - event->count <= ucnt) {
		fiber_lock(&event->wq);
		while (true) {
			if (U64_MAX - event->count > ucnt)
				break;
			fiber_yield();
		}
	}
	event->count += ucnt;
	fiber_unlock(&event->wq);
	return 0;
#endif /* TCL_THREADING */
}
