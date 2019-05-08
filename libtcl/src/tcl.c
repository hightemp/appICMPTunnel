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

#define LOG_TAG "tcl"

#include "tcl/log.h"
#include "tcl.h"

static int __init;

int tcl_init(void)
{
	if (__init)
		return -1;
#ifdef CC_MSVC
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;

		wVersionRequested = MAKEWORD(2, 2);
		if ((err = WSAStartup(wVersionRequested, &wsaData))) {
			LOG_ERROR("WSAStartup failed with error: %d", err);
			return -1;
		}
	}
#endif
	mutex_global_init();
	__init = 1;
	return 0;
}

void tcl_cleanup(void)
{
	if (!__init)
		return;
#ifdef CC_MSVC
	WSACleanup();
#endif
	mutex_global_cleanup();
	alarm_cleanup();
	__init = 0;
}
