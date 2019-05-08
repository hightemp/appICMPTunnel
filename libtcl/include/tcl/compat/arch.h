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

/*!@file tcl/compat/arch.h
 * @author uael
 *
 * @addtogroup tcl.compat @{
 */
#ifndef __TCL_COMPAT_ARCH_H
# define __TCL_COMPAT_ARCH_H

#include "tcl/compat/verno.h"

#if defined(__alpha__) \
  || defined(__alpha) \
  || defined(_M_ALPHA)
# if defined(__alpha_ev4__)
#   define ARCH_ALPHA VERNO(4, 0, 0)
# elif defined(__alpha_ev5__)
#   define ARCH_ALPHA VERNO(5, 0, 0)
# elif defined(__alpha_ev6__)
#   define ARCH_ALPHA VERNO(6, 0, 0)
# else
#   define ARCH_ALPHA 0
# endif
#endif

#if defined(__arm__) \
  || defined(__TARGET_ARCH_ARM) \
  || defined(__TARGET_ARCH_THUMB) \
  || defined(_ARM) \
  || defined(_M_ARM_) \
  || defined(_M_ARM64) \
  || defined(__arm) \
  || defined(__arm64) \
  || defined(__aarch64__) \
  || defined(__thumb__)
# if defined(__ARM_ARCH) && __ARM_ARCH > 1
#   define ARCH_ARM VERNO(__ARM_ARCH, 0, 0)
# elif defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM > 1
#   define ARCH_ARM VERNO(__TARGET_ARCH_ARM, 0, 0)
# elif defined(__TARGET_ARCH_THUMB) && __TARGET_ARCH_THUMB > 1
#   define ARCH_ARM VERNO(__TARGET_ARCH_THUMB, 0, 0)
# elif defined(_M_ARM) && _M_ARM > 1
#   define ARCH_ARM VERNO(_M_ARM, 0, 0)
# elif defined(__ARM64_ARCH_8__) \
  || defined(__aarch64__) \
  || defined(__CORE_CORTEXAV8__)
#   define ARCH_ARM VERNO(8, 0, 0)
# elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || \
       defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || \
       defined(__ARM_ARCH_7S__) || defined(_ARM_ARCH_7) || \
       defined(__CORE_CORTEXA__)
#   define ARCH_ARM VERNO(7, 0, 0)
# elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || \
       defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_6Z__) || \
       defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6ZK__) || \
       defined(__ARM_ARCH_6M__)
#   define ARCH_ARM VERNO(6, 0, 0)
# elif defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5E__) || \
       defined(__ARM_ARCH_5T__) || defined(__ARM_ARCH_5TE__) || \
       defined(__ARM_ARCH_5TEJ__)
#   define ARCH_ARM VERNO(5, 0, 0)
# elif defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_4T__)
#   define ARCH_ARM VERNO(4, 0, 0)
# elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
#   define ARCH_ARM VERNO(3, 0, 0)
# elif defined(__ARM_ARCH_2__)
#   define ARCH_ARM VERNO(2, 0, 0)
# endif
#endif

#if defined(__bfin__) || defined(__BFIN__) || defined(bfin) || defined(BFIN)
# define ARCH_BLACKFIN 0
#endif

#if defined(__convex__)
# if defined(__convex_c1__)
#   define ARCH_CONVEX VERNO(1, 0, 0)
# elif defined(__convex_c2__)
#   define ARCH_CONVEX VERNO(2, 0, 0)
# elif defined(__convex_c32__)
#   define ARCH_CONVEX VERNO(3, 2, 0)
# elif defined(__convex_c34__)
#   define ARCH_CONVEX VERNO(3, 4, 0)
# elif defined(__convex_c38__)
#   define ARCH_CONVEX VERNO(3, 8, 0)
# else
#   define ARCH_CONVEX 0
# endif
#endif

#if defined(__ia64__) || defined(_IA64) || \
    defined(__IA64__) || defined(__ia64) || \
    defined(_M_IA64) || defined(__itanium__)
# define ARCH_IA64 0
#endif

#if defined(__m68k__) || defined(M68000)
# if defined(__mc68060__) || defined(mc68060) || defined(__mc68060)
#   define ARCH_M68K VERNO(6,0,0)
# elif defined(__mc68040__) || defined(mc68040) || defined(__mc68040)
#   define ARCH_M68K VERNO(4,0,0)
# elif defined(__mc68030__) || defined(mc68030) || defined(__mc68030)
#   define ARCH_M68K VERNO(3,0,0)
# elif defined(__mc68020__) || defined(mc68020) || defined(__mc68020)
#   define ARCH_M68K VERNO(2,0,0)
# elif defined(__mc68010__) || defined(mc68010) || defined(__mc68010)
#   define ARCH_M68K VERNO(1,0,0)
# elif defined(__mc68000__) || defined(mc68000) || defined(__mc68000)
#   define ARCH_M68K 0
# else
#   define ARCH_M68K 0
# endif
#endif

#if defined(__mips__) || defined(__mips) || defined(__MIPS__) || \
    defined(_M_MRX000)
# if defined(_M_MRX000)
#   if (_M_MRX000 >= 10000)
#     define ARCH_MIPS VERNO(4, 0, 0)
#   else
#     define ARCH_MIPS VERNO(3, 0, 0)
#   endif
# else
#   if defined(_MIPS_ARCH_MIPS64) || (defined(__mips) && __mips - 0 >= 64) \
		|| (defined(_MIPS_ISA) && defined(_MIPS_ISA_MIPS64) && \
        __MIPS_ISA - 0 >= _MIPS_ISA_MIPS64)
#     define ARCH_MIPS VERNO(4, 0, 0)
#   elif defined(_MIPS_ARCH_MIPS32) || (defined(__mips) && __mips - 0 >= 32) \
		|| (defined(_MIPS_ISA) && defined(_MIPS_ISA_MIPS32) && \
         __MIPS_ISA - 0 >= _MIPS_ISA_MIPS32)
#     define ARCH_MIPS VERNO(2, 0, 0)
#   elif defined(_MIPS_ARCH_MIPS4) || (defined(__mips) && __mips - 0 >= 4) \
		|| (defined(_MIPS_ISA) && defined(_MIPS_ISA_MIPS4) && \
        __MIPS_ISA - 0 >= _MIPS_ISA_MIPS4)
#     define ARCH_MIPS VERNO(4, 0, 0)
#   elif defined(_MIPS_ARCH_MIPS3) || (defined(__mips) && __mips - 0 >= 3) \
		|| (defined(_MIPS_ISA) && defined(_MIPS_ISA_MIPS3) && \
         __MIPS_ISA - 0 >= _MIPS_ISA_MIPS3)
#     define ARCH_MIPS VERNO(3, 0, 0)
#   elif defined(_MIPS_ARCH_MIPS2) || (defined(__mips) && __mips - 0 >= 2) \
		|| (defined(_MIPS_ISA) && defined(_MIPS_ISA_MIPS2) && \
         __MIPS_ISA - 0 >= _MIPS_ISA_MIPS2)
#     define ARCH_MIPS VERNO(2, 0, 0)
#   elif defined(_MIPS_ARCH_MIPS1) || (defined(__mips) && __mips - 0 >= 1) \
		|| (defined(_MIPS_ISA) && defined(_MIPS_ISA_MIPS1) && \
         __MIPS_ISA - 0 >= _MIPS_ISA_MIPS1)
#     define ARCH_MIPS VERNO(1, 0, 0)
#   else
#     define ARCH_MIPS 0
#   endif
# endif
#endif

#if defined(__hppa__) || defined(__hppa) || defined(__HPPA__)
# if defined(_PA_RISC1_0)
#   define ARCH_PARISC VERNO(1,0,0)
# elif defined(_PA_RISC1_1) || defined(__HPPA11__) || defined(__PA7100__)
#   define ARCH_PARISC VERNO(1,1,0)
# elif defined(_PA_RISC2_0) || defined(__RISC2_0__) || defined(__HPPA20__) || \
       defined(__PA8000__)
#   define ARCH_PARISC VERNO(2,0,0)
# else
#   define ARCH_PARISC 0
# endif
#endif

#if defined(__powerpc) || defined(__powerpc__) || \
    defined(__POWERPC__) || defined(__ppc__) || \
    defined(_M_PPC) || defined(_ARCH_PPC) || \
    defined(__PPCGECKO__) || defined(__PPCBROADWAY__) || \
    defined(_XENON)
# if defined(__ppc601__) || defined(_ARCH_601)
#   define ARCH_PPC VERNO(6,1,0)
# elif defined(__ppc603__) || defined(_ARCH_603)
#   define ARCH_PPC VERNO(6,3,0)
# elif defined(__ppc604__) || defined(__ppc604__)
#   define ARCH_PPC VERNO(6,4,0)
# else
#   define ARCH_PPC 0
# endif
#endif

#if defined(pyr)
# define ARCH_PYRAMID 0
#endif

#if defined(__THW_RS6000) || defined(_IBMR2) || \
    defined(_POWER) || defined(_ARCH_PWR) || \
    defined(_ARCH_PWR2)
# define ARCH_RS6000 0
#endif

#if defined(__sparc__) || defined(__sparc)
# if defined(__sparcv9)
#   define ARCH_SPARC VERNO(9,0,0)
# elif defined(__sparcv8)
#   define ARCH_SPARC VERNO(8,0,0)
# else
#   define ARCH_SPARC 0
# endif
#endif

#if defined(__sh__)
# if defined(__SH5__)
#   define ARCH_SH VERNO(5,0,0)
# elif defined(__SH4__)
#   define ARCH_SH VERNO(4,0,0)
# elif defined(__sh3__) || defined(__SH3__)
#   define ARCH_SH VERNO(3,0,0)
# elif defined(__sh2__)
#   define ARCH_SH VERNO(2,0,0)
# elif defined(__sh1__)
#   define ARCH_SH VERNO(1,0,0)
# else
#   define ARCH_SH 0
# endif
#endif

#if defined(__370__) || defined(__THW_370__)
# define ARCH_SYS370 0
#endif

#if defined(__s390__) || defined(__s390x__)
# define ARCH_SYS390 0
#endif

#if defined(i386) || defined(__i386__) || \
    defined(__i486__) || defined(__i586__) || \
    defined(__i686__) || defined(__i386) || \
    defined(_M_IX86) || defined(_X86_) || \
    defined(__THW_INTEL__) || defined(__I86__) || \
    defined(__INTEL__)
# define ARCH_X86 0
# if defined(__I86__)
#   define ARCH_X86_32 VERNO(__I86__, 0, 0)
# elif defined(_M_IX86)
#   define ARCH_X86_32 VERNO_10_VV00(_M_IX86)
# elif defined(__i686__) || defined(__athlon__) || defined(__SSE__) || \
       defined(__pentiumpro__)
#   define ARCH_X86_32 VERNO(6,0,0)
# elif defined(__i586__) || defined(__k6__) || defined(__pentium__)
#   define ARCH_X86_32 VERNO(5,0,0)
# elif defined(__i486__) || defined(__80486__)
#   define ARCH_X86_32 VERNO(4,0,0)
# elif defined(__i386__)
#   define ARCH_X86_32 VERNO(3,0,0)
# else
#   define ARCH_X86_32 0
# endif
#endif

#if defined(__x86_64__) || defined(__x86_64) || defined(__amd64__) || \
    defined(__amd64) || defined(_M_X64) || defined(_M_AMD64)
# define ARCH_X86 0
# define ARCH_X86_64 0
#endif

#if defined(__NTC_ZARCH__)
# define ARCH_Z 0
#endif

#endif /* !__TCL_COMPAT_ARCH_H */
/*!@} */
