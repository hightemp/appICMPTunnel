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

#pragma once

/*!@file tcl/log.h
 * @author uael
 *
 * @addtogroup tcl.log @{
 */
#ifndef __TCL_LOG_H
# define __TCL_LOG_H

#include "tcl/conf.h"
#include <stdio.h>

#ifdef TCL_LOGGING
# if !defined(LOG_TAG)
#   warning "LOG_TAG must be defined"
#   define LOG_TAG __FILE__
# endif /* !defined(LOG_TAG) */
# define LOGWRAP(lvl, fmt, ...) \
	fprintf(stderr, "[%s] %s: %s: %d "fmt"\n", #lvl, LOG_TAG, __func__, \
		__line__ , ##__VA_ARGS__)
# ifdef NDEBUG
#   define LOG_VERBOSE(...) do; while (0)
# else
#   define LOG_VERBOSE(fmt, ...) LOGWRAP(LOG_VERBOSE, fmt, ##__VA_ARGS__)
# endif /* NDEBUG */
# define LOG_DEBUG(fmt, ...) LOGWRAP(LOG_DEBUG, fmt, ##__VA_ARGS__)
# define LOG_INFO(fmt, ...) LOGWRAP(LOG_INFO, fmt, ##__VA_ARGS__)
# define LOG_WARN(fmt, ...) LOGWRAP(LOG_WARN, fmt, ##__VA_ARGS__)
# define LOG_ERROR(fmt, ...) LOGWRAP(LOG_ERROR, fmt, ##__VA_ARGS__)
#else /* !TCL_LOGGING */
# define LOG_VERBOSE(fmt, ...) do {} while (0)
# define LOG_DEBUG(fmt, ...) do {} while (0)
# define LOG_INFO(fmt, ...) do {} while (0)
# define LOG_WARN(fmt, ...) do {} while (0)
# define LOG_ERROR(fmt, ...) do {} while (0)
#endif /* TCL_LOGGING */

#endif /* !__TCL_LOG_H */
/*!@} */
