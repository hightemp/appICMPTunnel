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

#define LOG_TAG "tcl_semaphore"

#include "tcl/log.h"
#include "tcl/sema.h"

int sema_init(sema_t *sema, unsigned value)
{
	return event_init(&sema->event, value, EVENT_SEMAPHORE);
}

void sema_destroy(sema_t *sema)
{
	event_destroy(&sema->event);
}

void sema_wait(sema_t *sema)
{
	event_value_t value;

	event_read(&sema->event, &value);
}

bool sema_trywait(sema_t *sema)
{
	event_value_t value;
	
	return event_tryread(&sema->event, &value);
}

int sema_post(sema_t *sema)
{
	return event_write(&sema->event, 1ULL);
}
