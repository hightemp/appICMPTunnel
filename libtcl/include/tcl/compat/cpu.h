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

/*!@file tcl/compat/cpu.h
 * @author uael
 *
 * @addtogroup tcl.compat @{
 */
#ifndef __TCL_COMPAT_CPU_H
# define __TCL_COMPAT_CPU_H

#include "tcl/conf.h"

#if defined(__LP64__) || defined(__64BIT__) || defined(_LP64) \
  || defined(__x86_64) || defined(__x86_64__) || defined(__amd64) \
  || defined(__amd64__) || defined(__arm64) || defined(__arm64__) \
  || defined(__sparc64__) || defined(__PPC64__) || defined(__ppc64__) \
  || defined(__powerpc64__) || defined(_M_X64) || defined(_M_AMD64) \
  || defined(_M_IA64) || (defined(__WORDSIZE) && __WORDSIZE == 64)
# define CPU_SIZE (64)
# define CPU_BYTE (8)
# define CPU_ALIGN (7)
# define CPU_32 (0)
# define CPU_64 (1)
# define CPU_SHIFT (6)
#else
# define CPU_SIZE (32)
# define CPU_BYTE (4)
# define CPU_ALIGN (3)
# define CPU_32 (1)
# define CPU_64 (0)
# define CPU_SHIFT (5)
#endif

#endif /* !__TCL_COMPAT_CPU_H */
/*!@} */
