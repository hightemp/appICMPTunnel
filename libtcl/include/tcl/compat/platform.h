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

#pragma once

/*!@file tcl/compat/platform.h
 * @author uael
 *
 * @addtogroup tcl.compat @{
 */
#ifndef __TCL_COMPAT_PLATFORM_H
# define __TCL_COMPAT_PLATFORM_H

#include "tcl/compat/arch.h"
#include "tcl/compat/cpu.h"
#include "tcl/compat/os.h"

#if defined(OS_IOS)
# include <TargetConditionals.h>
# define PLAT_IOS
# if defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR == 1
#   define PLAT_IOS_SIMULATOR 0
# else
#   define PLAT_IOS_DEVICE 0
# endif
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
# include <_mingw.h>
# if defined(__MINGW64_VERSION_MAJOR) && defined(__MINGW64_VERSION_MINOR)
#   define PLAT_MINGW VERNO(__MINGW64_VERSION_MAJOR, __MINGW64_VERSION_MINOR, 0)
# elif defined(__MINGW32_VERSION_MAJOR) && defined(__MINGW32_VERSION_MINOR)
#   define PLAT_MINGW VERNO(__MINGW32_MAJOR_VERSION, __MINGW32_MINOR_VERSION, 0)
# else
#   define PLAT_MINGW 0
# endif
#endif

#if defined(OS_WIN) && \
    defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
# define PLAT_WIN_DESKTOP 0
#endif

#if defined(OS_WIN) && \
    defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
# define PLAT_WIN_PHONE 0
#endif

#if defined(OS_WIN) && \
    defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_APP
# define PLAT_WIN_STORE 0
#endif

#if defined(PLAT_WIN_PHONE) || defined(PLAT_WIN_STORE)
# define PLAT_WIN_RT 0
#endif

#endif /* !__TCL_COMPAT_PLATFORM_H */
/*!@} */
