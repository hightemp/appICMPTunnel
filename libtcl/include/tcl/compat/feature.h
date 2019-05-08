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

/*!@file tcl/compat/feature.h
 * @author uael
 *
 * @addtogroup tcl.compat @{
 */
#ifndef __TCL_COMPAT_FEATURE_H
# define __TCL_COMPAT_FEATURE_H

#include "tcl/compat/platform.h"

#define VERSION_C89 198900L
#define VERSION_C90 199000L
#define VERSION_C94 199409L
#define VERSION_C99 199901L
#define VERSION_C11 201112L

#if defined(__STDC_VERSION__)
# if (__STDC_VERSION__ - 0 > 1)
#   define STD_C __STDC_VERSION__
# else
#   if (CC_SUNPRO - 0 >= 0x420)
#     define STD_C VERSION_C94
#   else
#     define STD_C VERSION_C90
#   endif
# endif
#else
# if defined(__STDC__) \
  || defined(CC_MSVC) \
  || defined(CC_BORLAND)
#   define STD_C VERSION_C89
# endif
#endif

#if (STD_C - 0 >= VERSION_C89)
# define STD_C89
#endif
#if (STD_C - 0 >= VERSION_C90)
# define STD_C90
#endif
#if (STD_C - 0 >= VERSION_C94)
# define STD_C94
#endif
#if (STD_C - 0 >= VERSION_C99)
# define STD_C99
#endif
#if (STD_C - 0 >= VERSION_C11)
# define STD_C11
#endif

#define VERSION_CXX98 199711L

#if defined(__cplusplus)
# if __cplusplus - 0 == 1
#   define STD_CXX VERSION_CXX98
# else
#   define STD_CXX __cplusplus
# endif
#endif

#if defined(STD_CXX) && STD_CXX >= VERSION_CXX98
# define STD_CXX98
#endif

#if defined(OS_UNIX)
# include <unistd.h>
#endif

#define VERSION_POSIX_1990 199009L
#define VERSION_POSIX_1992 199209L
#define VERSION_POSIX_1993 199309L
#define VERSION_POSIX_1996 199506L
#define VERSION_POSIX_2001 200112L

#if defined(_POSIX_VERSION)
# if _POSIX_VERSION >= VERSION_POSIX_1990
#   define STD_POSIX_1990
# endif
# if _POSIX_VERSION >= VERSION_POSIX_1992
#   define STD_POSIX_1992
# endif
# if _POSIX_VERSION >= VERSION_POSIX_1993
#   define STD_POSIX_1993
# endif
# if _POSIX_VERSION >= VERSION_POSIX_1996
#   define STD_POSIX_1996
# endif
# if _POSIX_VERSION >= VERSION_POSIX_2001
#   define STD_POSIX_2001
# endif
#   define STD_POSIX _POSIX_VERSION
#endif

#define VERSION_XOPEN_1989 300
#define VERSION_XOPEN_1992 400
#define VERSION_XOPEN_1995 450
#define VERSION_XOPEN_1998 500
#define VERSION_XOPEN_2003 600

#if defined(_XOPEN_VERSION)
# if (_XOPEN_VERSION - 0 >= 3) || defined(_XOPEN_XPG3)
#   define STD_XOPEN_1989
# endif
# if (_XOPEN_VERSION - 0 >= 4) || defined(_XOPEN_XPG4)
#   define STD_XOPEN_1992
# endif
# if (_XOPEN_VERSION - 0 > 4) || \
	(defined(_XOPEN_UNIX) && (_XOPEN_VERSION - 0 == 4))
#   define STD_XOPEN_1995
# endif
# if (_XOPEN_VERSION - 0 >= VERSION_XOPEN_1998)
#   define STD_XOPEN_1998
# endif
# if (_XOPEN_VERSION - 0 >= VERSION_XOPEN_2003)
#   define STD_XOPEN_2003
# endif
#endif

#if defined(STD_XOPEN_1998)
# define STD_XOPEN _XOPEN_VERSION
#else
# if defined(STD_XOPEN_1995)
#   define STD_XOPEN VERSION_XOPEN_1995
# else
#   if defined(STD_XOPEN_1992)
#     define STD_XOPEN VERSION_XOPEN_1992
#   else
#     if defined(STD_XOPEN_1989)
#       define STD_XOPEN VERSION_XOPEN_1989
#     endif
#   endif
# endif
#endif

#if defined(__ANDROID__)
# include <sys/cdefs.h>
#elif defined(__APPLE__)
# include <TargetConditionals.h>
#elif defined(__linux__)
# include <features.h>
#endif

#if defined (_MSC_VER)
# define LIBC_MSVCRT 1
#elif defined(__BIONIC__)
# define LIBC_BIONIC 1
# define LIBC_BSD 1
#elif defined(__UCLIBC__)
# define LIBC_UCLIBC 1
#elif defined(__KLIBC__)
# define LIBC_KLIBC 1
# define LIBC_BSD 1
#elif defined(__CRTL_VER)
# define LIBC_VMS 1
# define LIBC_BSD 1
#elif defined(__LIBREL__)
# define LIBC_ZOS 1
# define LIBC_BSD 1
#elif defined(__GLIBC__) || defined(__GNU_LIBRARY__)
# define LIBC_GLIBC 1
#else
# define LIBC_BSD 1
#endif

#ifndef __has_attribute
# define __has_attribute(x) (0)
#endif

#ifndef __has_builtin
# define __has_builtin(x) (0)
#endif

#ifndef __has_declspec_attribute
# define __has_declspec_attribute(x) 0
#endif

#ifndef __has_feature
# define __has_feature(x) (0)
#endif

#ifndef __has_include
# define __has_include(x) (0)
#endif

#ifndef __has_extension
# define __has_extension(x) __has_feature(x)
#endif

#if !defined(__has_attribute__)
# define __has_attribute__(x) (__has_attribute(x) || __HAS_ATTRIBUTE_##x)
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined(CC_CRAY))
#   define __HAS_ATTRIBUTE_alias 1
# else
#   define __HAS_ATTRIBUTE_alias 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined(CC_INTEL)) || \
     (defined(CC_CRAY)) || \
     (defined(CC_SUNPRO)) || \
     (defined(CC_DIAB))
#   define __HAS_ATTRIBUTE_aligned 1
# else
#   define __HAS_ATTRIBUTE_aligned 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 1))
#   define __HAS_ATTRIBUTE_always_inline 1
# else
#   define __HAS_ATTRIBUTE_always_inline 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_cdecl 1
# else
#   define __HAS_ATTRIBUTE_cdecl 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 3))
#   define __HAS_ATTRIBUTE_cleanup 1
# else
#   define __HAS_ATTRIBUTE_cleanup 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 3))
#   define __HAS_ATTRIBUTE_common 1
# else
#   define __HAS_ATTRIBUTE_common 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined(CC_INTEL))
#   define __HAS_ATTRIBUTE_const 1
# else
#   define __HAS_ATTRIBUTE_const 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_constructor 1
# else
#   define __HAS_ATTRIBUTE_constructor 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 1)) || \
     (defined(CC_CRAY))
#   define __HAS_ATTRIBUTE_deprecated 1
# else
#   define __HAS_ATTRIBUTE_deprecated 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 4, 5)) || \
     (defined(CC_CRAY))
#   define __HAS_ATTRIBUTE_deprecated_message 1
# else
#   define __HAS_ATTRIBUTE_deprecated_message 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_destructor 1
# else
#   define __HAS_ATTRIBUTE_destructor 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_dllexport 1
# else
#   define __HAS_ATTRIBUTE_dllexport 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_dllimport 1
# else
#   define __HAS_ATTRIBUTE_dllimport 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_eightbit_data 1
# else
#   define __HAS_ATTRIBUTE_eightbit_data 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_exception 1
# else
#   define __HAS_ATTRIBUTE_exception 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 3))
#   define __HAS_ATTRIBUTE_far 1
# else
#   define __HAS_ATTRIBUTE_far 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 4))
#   define __HAS_ATTRIBUTE_fastcall 1
# else
#   define __HAS_ATTRIBUTE_fastcall 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined(CC_CRAY))
#   define __HAS_ATTRIBUTE_format 1
# else
#   define __HAS_ATTRIBUTE_format 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined(CC_CRAY))
#   define __HAS_ATTRIBUTE_format_arg 1
# else
#   define __HAS_ATTRIBUTE_format_arg 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_function_vector 1
# else
#   define __HAS_ATTRIBUTE_function_vector 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_interrupt 1
# else
#   define __HAS_ATTRIBUTE_interrupt 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_interrupt_handler 1
# else
#   define __HAS_ATTRIBUTE_interrupt_handler 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_long_call 1
# else
#   define __HAS_ATTRIBUTE_long_call 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_longcall 1
# else
#   define __HAS_ATTRIBUTE_longcall 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_malloc 1
# else
#   define __HAS_ATTRIBUTE_malloc 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 3))
#   define __HAS_ATTRIBUTE_may_alias 1
# else
#   define __HAS_ATTRIBUTE_may_alias 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_mode 1
# else
#   define __HAS_ATTRIBUTE_mode 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_model 1
# else
#   define __HAS_ATTRIBUTE_model 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_naked 1
# else
#   define __HAS_ATTRIBUTE_naked 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 3))
#   define __HAS_ATTRIBUTE_near 1
# else
#   define __HAS_ATTRIBUTE_near 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_no_check_memory_usage 1
# else
#   define __HAS_ATTRIBUTE_no_check_memory_usage 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_no_instrument_function 1
# else
#   define __HAS_ATTRIBUTE_no_instrument_function 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_nocommon 1
# else
#   define __HAS_ATTRIBUTE_nocommon 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 1))
#   define __HAS_ATTRIBUTE_noinline 1
# else
#   define __HAS_ATTRIBUTE_noinline 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 3))
#   define __HAS_ATTRIBUTE_nonnull 1
# else
#   define __HAS_ATTRIBUTE_nonnull 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_noreturn 1
# else
#   define __HAS_ATTRIBUTE_noreturn 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 3)) || \
     (defined(CC_CRAY))
#   define __HAS_ATTRIBUTE_nothrow 1
# else
#   define __HAS_ATTRIBUTE_nothrow 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 4, 4)) || \
     (defined(CC_CRAY))
#   define __HAS_ATTRIBUTE_optimize 1
# else
#   define __HAS_ATTRIBUTE_optimize 0
# endif
# define __HAS_ATTRIBUTE_optnone 0
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95)) || \
     defined(CC_DIAB) || \
     (defined(CC_INTEL))
#   define __HAS_ATTRIBUTE_packed 1
# else
#   define __HAS_ATTRIBUTE_packed 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_pure 1
# else
#   define __HAS_ATTRIBUTE_pure 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_regparam 1
# else
#   define __HAS_ATTRIBUTE_regparam 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 4))
#   define __HAS_ATTRIBUTE_saveall 1
# else
#   define __HAS_ATTRIBUTE_saveall 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined(CC_INTEL))
#   define __HAS_ATTRIBUTE_section 1
# else
#   define __HAS_ATTRIBUTE_section 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_shared 1
# else
#   define __HAS_ATTRIBUTE_shared 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_signal 1
# else
#   define __HAS_ATTRIBUTE_signal 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_short_call 1
# else
#   define __HAS_ATTRIBUTE_short_call 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_shortcall 1
# else
#   define __HAS_ATTRIBUTE_shortcall 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_sp_switch 1
# else
#   define __HAS_ATTRIBUTE_sp_switch 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_stdcall 1
# else
#   define __HAS_ATTRIBUTE_stdcall 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_tiny_data 1
# else
#   define __HAS_ATTRIBUTE_tiny_data 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined(CC_CRAY))
#   define __HAS_ATTRIBUTE_transparent_union 1
# else
#   define __HAS_ATTRIBUTE_transparent_union 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_trap_exit 1
# else
#   define __HAS_ATTRIBUTE_trap_exit 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined(CC_CRAY)) || \
     (defined(CC_INTEL))
#   define __HAS_ATTRIBUTE_unused 1
# else
#   define __HAS_ATTRIBUTE_unused 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 1)) || \
     (defined(CC_CRAY)) || \
     (defined(CC_INTEL))
#   define __HAS_ATTRIBUTE_used 1
# else
#   define __HAS_ATTRIBUTE_used 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 1))
#   define __HAS_ATTRIBUTE_vector_size 1
# else
#   define __HAS_ATTRIBUTE_vector_size 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 3)) || \
     (defined(CC_SYSC) && VERNO_GE(CC_SYSC, 1, 9)) || \
     (defined(CC_HPCC) && VERNO_GE(CC_HPCC, 6, 26))
#   define __HAS_ATTRIBUTE_visibility 1
# else
#   define __HAS_ATTRIBUTE_visibility 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 3, 3)) || \
     (defined(CC_CRAY)) || \
     (defined(CC_HPCC) && VERNO_GE(CC_HPCC, 6, 26))
#   define __HAS_ATTRIBUTE_warn_unused_result 1
# else
#   define __HAS_ATTRIBUTE_warn_unused_result 0
# endif
# if (defined(CC_GCC) && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined(CC_CRAY))
#   define __HAS_ATTRIBUTE_weak 1
# else
#   define __HAS_ATTRIBUTE_weak 0
# endif
#endif

#if !defined(__has_builtin__)
# define __has_builtin__(x) (__has_builtin(x) || __HAS_BUILTIN_##x)
# if defined(CC_GCC)
#   define __HAS_BUILTIN_expect 1
#   define __HAS_BUILTIN___builtin_popcount 1
# else
#   define __HAS_BUILTIN_expect 0
#   define __HAS_BUILTIN___builtin_popcount 0
# endif
#endif

#if !defined(__has_declspec_attribute__)
# define __has_declspec_attribute__(x) \
  (__has_declspec_attribute(x) || __HAS_DECLSPEC_ATTRIBUTE_##x)
# if defined(CC_MSVC) && VERNO_GE(CC_MSVC, 13, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_align 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_align 0
# endif
# if defined(CC_MSVC) && VERNO_GE(CC_MSVC, 13, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_deprecated 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_deprecated 0
# endif
# if defined(CC_COMEAU) || \
     (defined(CC_DMC)) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 12, 0))
#   define __HAS_DECLSPEC_ATTRIBUTE_dllexport 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_dllexport 0
# endif
# if defined(CC_COMEAU) || \
     (defined(CC_DMC)) || \
     ( \
      defined(CC_MSVC) && ( \
        defined(_M_IX86) || \
        defined(_M_IA64) || \
        defined(_M_AMD64) || \
        defined(_M_ARM) || \
        defined(_M_ARM64) \
      ) \
    )
#   define __HAS_DECLSPEC_ATTRIBUTE_dllimport 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_dllimport 0
# endif
# if defined(CC_COMEAU) || \
     (defined(CC_DMC)) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 12, 0))
#   define __HAS_DECLSPEC_ATTRIBUTE_naked 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_naked 0
# endif
# if defined(CC_MSVC) && VERNO_GE(CC_MSVC, 13, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_noinline 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_noinline 0
# endif
# if defined(CC_MSVC) && VERNO_GE(CC_MSVC, 12, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_noreturn 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_noreturn 0
# endif
# if defined(CC_MSVC) && VERNO_GE(CC_MSVC, 12, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_nothrow 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_nothrow 0
# endif
# if defined(CC_MSVC) && VERNO_GE(CC_MSVC, 15, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_restrict 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_restrict 0
# endif
# if defined(CC_MSVC) && VERNO_GE(CC_MSVC, 16, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_safebuffers 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_safebuffers 0
# endif
# if defined(CC_MSVC) && VERNO_GE(CC_MSVC, 11, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_selectany 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_selectany 0
# endif
# if defined(CC_COMEAU) || \
     (defined(CC_DMC)) || \
     (defined(CC_INTEL)) || \
     (defined(CC_BORLAND)) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 12, 0))
#   define __HAS_DECLSPEC_ATTRIBUTE_thread 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_thread 0
# endif
#endif

#if !defined(__has_feature__)
# define __has_feature__(x) (__has_feature(x) || __HAS_FEATURE_##x)
# if defined(STD_C11) || \
     __has_include(<stdalign.h>) || \
     (defined(CC_GCC) && VERNO_GE(CC_GCC, 4, 7))
#   define __HAS_FEATURE_c_alignas 1
# else
#   define __HAS_FEATURE_c_alignas 0
# endif
# if defined(STD_C11) || \
     __has_include(<stdalign.h>) || \
     (defined(CC_GCC) && VERNO_GE(CC_GCC, 4, 7))
#   define __HAS_FEATURE_c_alignof 1
# else
#   define __HAS_FEATURE_c_alignof 0
# endif
# if !defined(__STDC_NO_ATOMICS) && !defined(CC_INTEL) && ( \
     defined(STD_C11) || \
     __has_include(<stdatomic.h>) || \
     (defined(CC_GCC) && VERNO_GE(CC_GCC, 4, 9)))
#   define __HAS_FEATURE_c_atomic 1
# else
#   define __HAS_FEATURE_c_atomic 0
# endif
# if defined(STD_C99) || \
     __has_include(<stdbool.h>) || \
     (defined(CC_GCC) && VERNO_GE(CC_GCC, 4, 9)) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 18, 0))
#   define __HAS_FEATURE_c_bool 1
# else
#   define __HAS_FEATURE_c_bool 0
# endif
# if defined(STD_C99) || \
     __has_include(<complex.h>) || \
     (defined(CC_GCC) && defined(STD_C90))
#   define __HAS_FEATURE_c_complex 1
# else
#   define __HAS_FEATURE_c_complex 0
# endif
# if defined(STD_C11) || \
     (defined(CC_GCC) && VERNO_GE(CC_GCC, 4, 9))
#   define __HAS_FEATURE_c_generic_selections 1
# else
#   define __HAS_FEATURE_c_generic_selections 0
# endif
# if defined(STD_C11) || \
     defined(_Noreturn) || \
     __has_include(<stdnoreturn.h>) || \
     (defined(CC_CLANG) && VERNO_GT(CC_CLANG, 3, 2))
#   define __HAS_FEATURE_c_noreturn 1
# else
#   define __HAS_FEATURE_c_noreturn 0
# endif
# if defined(STD_C11) || \
     (defined(CC_GCC) && VERNO_GE(CC_GCC, 4, 6))
#   define __HAS_FEATURE_c_static_assert 1
# else
#   define __HAS_FEATURE_c_static_assert 0
# endif
# if defined(STD_C11) || \
     (defined(CC_GCC) && VERNO_GE(CC_GCC, 4, 9))
#   define __HAS_FEATURE_c_thread_local 1
# else
#   define __HAS_FEATURE_c_thread_local 0
# endif
# if __has_include(<complex.h>) || \
     defined(STD_C99) || \
     __has_feature__(c_complex) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 18, 0))
#   define __HAS_FEATURE_complex_h 1
# else
#   define __HAS_FEATURE_complex_h 0
# endif
# if __has_include(<direct.h>) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 15, 0))
#   define __HAS_FEATURE_direct_h 1
# else
#   define __HAS_FEATURE_direct_h 0
# endif
# if __has_include(<dirent.h>) || \
     defined(STD_POSIX) || \
     defined(STD_XOPEN)
#   define __HAS_FEATURE_dirent_h 1
# else
#   define __HAS_FEATURE_dirent_h 0
# endif
# if __has_include(<fcntl.h>) || \
     defined(STD_POSIX) || \
     defined(STD_XOPEN) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 15, 0))
#   define __HAS_FEATURE_fcntl_h 1
# else
#   define __HAS_FEATURE_fcntl_h 0
# endif
# if __has_include(<fenv.h>) || \
     defined(STD_C99) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 18, 0))
#   define __HAS_FEATURE_fenv_h 1
# else
#   define __HAS_FEATURE_fenv_h 0
# endif
# if __has_include(<fenv.h>) || \
     defined(STD_C99) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 18, 0))
#   define __HAS_FEATURE_inttypes_h 1
# else
#   define __HAS_FEATURE_inttypes_h 0
# endif
# if defined(CC_MSVC) && VERNO_GE(CC_MSVC, 15, 0)
#   define __HAS_FEATURE_io_h 1
# else
#   define __HAS_FEATURE_io_h 0
# endif
# if __has_include(<iso646.h>) || \
     defined(STD_POSIX) || \
     defined(STD_XOPEN)
#   define __HAS_FEATURE_iso646_h 1
# else
#   define __HAS_FEATURE_iso646_h 0
# endif
# if defined(CC_MSVC) && VERNO_GE(CC_MSVC, 13, 0)
#   define __HAS_FEATURE_malloc_h 1
# else
#   define __HAS_FEATURE_malloc_h 0
# endif
# if __has_include(<stdalign.h>) || \
     defined(STD_C11) || \
     __has_feature__(c_alignas) || \
     __has_feature__(c_alignof)
#   define __HAS_FEATURE_stdalign_h 1
# else
#   define __HAS_FEATURE_stdalign_h 0
# endif
# if __has_include(<stdatomic.h>) || \
     defined(STD_C11) || \
     __has_feature__(c_atomic)
#   define __HAS_FEATURE_stdatomic_h 1
# else
#   define __HAS_FEATURE_stdatomic_h 0
# endif
# if __has_include(<stdbool.h>) || \
     defined(STD_C99) || \
     __has_feature__(c_bool) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 18, 0))
#   define __HAS_FEATURE_stdbool_h 1
# else
#   define __HAS_FEATURE_stdbool_h 0
# endif
# if __has_include(<stdint.h>) || \
     defined(STD_C99) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 17, 0))
#   define __HAS_FEATURE_stdint_h 1
# else
#   define __HAS_FEATURE_stdint_h 0
# endif
# if __has_include(<stdnoreturn.h>) || \
     defined(STD_C11) || \
     __has_feature__(c_noreturn)
#   define __HAS_FEATURE_stdnoreturn_h 1
# else
#   define __HAS_FEATURE_stdnoreturn_h 0
# endif
# if __has_include(<sys/stat.h>) || \
     defined(STD_POSIX) || \
     defined(STD_XOPEN) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 13, 0))
#   define __HAS_FEATURE_sys_stat_h 1
# else
#   define __HAS_FEATURE_sys_stat_h 0
# endif
# if __has_include(<sys/types.h>) || \
     defined(STD_POSIX) || \
     defined(STD_XOPEN) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 13, 0))
#   define __HAS_FEATURE_sys_types_h 1
# else
#   define __HAS_FEATURE_sys_types_h 0
# endif
# if __has_include(<tgmath.h>) || \
     defined(STD_C99)
#   define __HAS_FEATURE_tgmath_h 1
# else
#   define __HAS_FEATURE_tgmath_h 0
# endif
# if (!defined(OS_CYGWIN) && __has_include(<threads.h>)) || \
     defined(STD_C11)
#   define __HAS_FEATURE_threads_h 1
# else
#   define __HAS_FEATURE_threads_h 0
# endif
# if __has_include(<uchar.h>) || \
     defined(STD_C11) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 19, 0))
#   define __HAS_FEATURE_uchar_h 1
# else
#   define __HAS_FEATURE_uchar_h 0
# endif
# if __has_include(<unistd.h>) || \
     defined(STD_POSIX) || \
     defined(STD_XOPEN)
#   define __HAS_FEATURE_unistd_h 1
# else
#   define __HAS_FEATURE_unistd_h 0
# endif
# if __has_include(<wchar.h>) || \
     defined(STD_C90) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 15, 0))
#   define __HAS_FEATURE_wchar_h 1
# else
#   define __HAS_FEATURE_wchar_h 0
# endif
# if __has_include(<wctype.h>) || \
     defined(STD_C90) || \
     (defined(CC_MSVC) && VERNO_GE(CC_MSVC, 15, 0))
#   define __HAS_FEATURE_wctype_h 1
# else
#   define __HAS_FEATURE_wctype_h 0
# endif
# define __HAS_FEATURE_address_sanitizer 0
#endif

#if !defined(__has_extension__)
# define __has_extension__(x) (__has_feature__(x) || __has_extension(x) || \
  __HAS_BUILTIN_##x)
#endif

#endif /* !__TCL_COMPAT_FEATURE_H */
/*!@} */
