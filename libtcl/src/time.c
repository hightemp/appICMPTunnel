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

#define LOG_TAG "bt_tcl_time"

#include "tcl/time.h"
#include "tcl/log.h"
#include "tcl/string.h"

#ifdef OS_MAC
# include <mach/clock.h>
# include <mach/mach.h>
#elif defined(CC_MSVC)
# define __BILLION (1E9)

static BOOL __first_time = 1;
static LARGE_INTEGER __counts_per_sec;
#endif

static int __gettime(struct timespec *ts)
{
#if defined(OS_MAC)
	clock_serv_t cclock;
	mach_timespec_t mts;

	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	if (clock_get_time(cclock, &mts))
		return -1;
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
	return 0;
#elif defined(CC_MSVC)
	LARGE_INTEGER count;

	if (__first_time) {
		__first_time = 0;
		if (0 == QueryPerformanceFrequency(&__counts_per_sec))
			__counts_per_sec.QuadPart = 0;
	}

	if (!ts || __counts_per_sec.QuadPart <= 0
		|| !QueryPerformanceCounter(&count)) {
		return -1;
	}

	ts->tv_sec = count.QuadPart / __counts_per_sec.QuadPart;
	ts->tv_nsec = (long)(
		((count.QuadPart % __counts_per_sec.QuadPart) * __BILLION) /
			__counts_per_sec.QuadPart);

	return 0;
#else
	return clock_gettime(CLOCK_REALTIME, ts);
#endif
}

period_ms_t now(void)
{
	struct timespec ts;

	if (__gettime(&ts) == -1) {
		LOG_ERROR("Unable to get current time: %s", strerror(errno));
		return 0;
	}
	return (period_ms_t) ((ts.tv_sec * 1000LL) + (ts.tv_nsec / 1000000LL));
}
