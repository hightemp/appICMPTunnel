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

/*!@file tcl/compat/types.h
 * @author uael
 *
 * @addtogroup tcl.compat @{
 */
#ifndef __TCL_COMPAT_TYPES_H
# define __TCL_COMPAT_TYPES_H

#include "tcl/compat/defs.h"

#ifndef NULL
# ifdef __cplusplus
#   define NULL nullptr
# else
#   define NULL ((void *)0)
# endif
#endif

typedef float f32_t;
typedef double f64_t;

#ifdef HAS_INTTYPES_H
# include <inttypes.h>
#endif
#ifdef HAS_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAS_STDINT_H
# if defined(__cplusplus)
#   include <cstdint>
typedef std::int8_t i8_t;
typedef std::int16_t i16_t;
typedef std::int32_t i32_t;
typedef std::int64_t i64_t;
typedef std::uint8_t u8_t;
typedef std::uint16_t u16_t;
typedef std::uint32_t u32_t;
typedef std::uint64_t u64_t;
# else
#   include <stdint.h>
typedef int8_t i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;
# endif
#else
typedef signed char i8_t;
typedef signed short i16_t;
typedef signed int i32_t;
typedef signed long long i64_t;
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long long u64_t;
#endif

#if defined(OS_WIN) || !defined(HAS_STDINT_H)
# if CPU_64 && !defined(WIN32)
typedef i64_t isize_t;
#   if !defined(_SIZE_T_DEFINED)
typedef u64_t usize_t;
#   else
typedef size_t usize_t;
#   endif
#   if !defined(SSIZE_MAX)
#     define ISIZE_MAX I64_MAX
#   else
#     define ISIZE_MAX SSIZE_MAX
#   endif
#   if !defined(SIZE_MAX)
#     define USIZE_MAX U64_MAX
#   else
#     define USIZE_MAX SIZE_MAX
#   endif
# else
typedef i32_t isize_t;
#   if !defined(_SIZE_T_DEFINED)
typedef u32_t usize_t;
#   else
typedef size_t usize_t;
#   endif
#   if !defined(SSIZE_MAX)
#     define ISIZE_MAX I32_MAX
#   else
#     define ISIZE_MAX SSIZE_MAX
#   endif
#   if !defined(SIZE_MAX)
#     define USIZE_MAX U32_MAX
#   else
#     define USIZE_MAX SIZE_MAX
#   endif
# endif
#else
typedef ssize_t isize_t;
typedef size_t usize_t;
# define ISIZE_MAX SSIZE_MAX
# define USIZE_MAX SIZE_MAX
#endif

#ifdef HAS_STDBOOL_H
# if defined(__cplusplus) && __cplusplus >= 201103L
#   include <cstdbool>
# else
#   include <stdbool.h>
# endif
#else
# if !defined(__cplusplus) && !defined(__bool_true_false_are_defined)
#   define bool unsigned char
#   define true 1
#   define false 0
#   define __bool_true_false_are_defined 1
# endif
#endif

#include <limits.h>

#ifdef INT8_MIN
# define I8_MIN INT8_MIN
#else
# define I8_MIN (-(I8_MAX) - 1)
#endif

#ifdef INT16_MIN
# define I16_MIN INT16_MIN
#else
# define I16_MIN (-(I16_MAX) - 1)
#endif

#ifdef INT32_MIN
# define I32_MIN INT32_MIN
#else
# define I32_MIN (-(I32_MAX) - 1)
#endif

#ifdef INT64_MIN
# define I64_MIN INT64_MIN
#else
# define I64_MIN (-(I64_MAX) - 1)
#endif

#ifdef INT8_MAX
# define I8_MAX INT8_MAX
#else
# define I8_MAX 127
#endif

#ifdef INT16_MAX
# define I16_MAX INT16_MAX
#else
# define I16_MAX 32767
#endif

#ifdef INT32_MAX
# define I32_MAX INT32_MAX
#else
# define I32_MAX 2147483647
#endif

#ifdef INT64_MAX
# define I64_MAX INT64_MAX
#else
# define I64_MAX 9223372036854775807LL
#endif

#ifdef UINT8_MAX
# define U8_MAX UINT8_MAX
#else
# define U8_MAX 0xffU
#endif

#ifdef UINT16_MAX
# define U16_MAX UINT16_MAX
#else
# define U16_MAX 0xffffU
#endif

#ifdef UINT32_MAX
# define U32_MAX UINT32_MAX
#else
# define U32_MAX 0xffffffffUL
#endif

#ifdef UINT64_MAX
# define U64_MAX UINT64_MAX
#else
# define U64_MAX 0xffffffffffffffffULL
#endif

#endif /* !__TCL_COMPAT_TYPES_H */
/*!@} */
