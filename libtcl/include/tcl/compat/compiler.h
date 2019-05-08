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

/*!@file tcl/compat/compiler.h
 * @author uael
 *
 * @addtogroup tcl.compat @{
 */
#ifndef __TCL_COMPAT_COMPILER_H
# define __TCL_COMPAT_COMPILER_H

#include "tcl/compat/preprocessor.h"
#include "tcl/compat/verno.h"

/*
 * Borland C/C++
 *
 * Version: 0xVRR : V = Version, RR = Revision
 * Example: 0x551 = Borland C++ 5.51
 */
#if defined(__BORLANDC__) || defined(__CODEGEARC__)
# if defined(__BORLANDC__)
#   define CC_BORLAND VERNO_0X_VRR(__BORLANDC__)
# elif defined(__CODEGEARC__)
#   define CC_BORLAND VERNO_0X_VVRP(__CODEGEARC__)
# endif
#endif

/*
 * clang C/C++
 */
#if defined(__clang__)
# define CC_CLANG VERNO(__clang_major__,__clang_minor__,__clang_patchlevel__)
#endif

/*
 * Comeau C++
 *
 * Version: VRR : V = Version, RR = Revision
 * Example: 230 = Comeau C++ 2.30
 */
#if defined(__COMO__)
# if defined(__COMO_VERSION__)
#   define CC_COMEAU VERNO_0X_VRP(__COMO_VERSION__)
# else
#   define CC_COMEAU 0
# endif
#endif

/*
 * Cray C/C++
 *
 * Version: V : V = Version
 * Example: ?
 */
#if defined(_CRAYC)
# if defined(_RELEASE_MAJOR) && defined(_RELEASE_MINOR)
#   define CC_CRAY VERNO(_RELEASE_MAJOR, _RELEASE_MINOR, 0)
# else
#   define CC_CRAY 0
# endif
#endif

/*
 * Compaq C++
 *
 * Version: VVRRTPPP : VV = Version, RR = Revision, T = Type, PPP = Patch
 * Example: 60090001 = DEC C/C++ 6.0-001
 */
#if defined(__DECC) || defined(__DECCXX)
# if defined(__DECC_VER)
#   define CC_DECC VERNO_10_VVRR0PP00(__DECC_VER)
# elif defined(__DECCXX_VER)
#   define CC_DECC VERNO_10_VVRR0PP00(__DECCXX_VER)
# else
#   define CC_DECC 0
# endif
#else
# if defined(VAXC) || defined(__VAXC)
#   define CC_DECC 0
# else
#   if defined(__osf__) && defined(__LANGUAGE_C__) && !defined(__GNUC__)
#     define CC_DECC 0
#   endif
# endif
#endif

/*
 * Diab C/C++
 *
 * Version: VRPP : V = Version, R = Revision, PP = Patch
 * Example: 4426 = Diab C/C++ 4.4q
 */
#if defined(__DCC__) && defined(__VERSION_NUMBER__)
# define CC_DIAB VERNO_10_VRPP(__VERSION_NUMBER__)
#endif

/*
 * Digital Mars (Symatec C++, Zortech C++)
 *
 * Version: 0xVRP : V = Version, R = Revision, P = Patch
 * Example: 0x720 = Digital Mars 7.2
 */
#if defined(__DMC__)
# define CC_DMC VERNO_0X_VRP(__DMC__)
#else
# if defined(__SC__) || defined(__ZTC__)
#   define CC_DMC 0
# endif
#endif

/*
 * Dignus Systems/C++
 */
#if defined(__SYSC__)
# define CC_SYSC VERNO_10_VRRPP(__SYSC_VER__)
#endif

/*
 * EDG C++ Frontend
 */
#if defined(__EDG__)
# define CC_EDG VERNO_10_VRR(__EDG_VERSION__)
#endif

/*
 * EKOpath
 */
#if defined(__PATHCC__)
# define CC_PATH VERNO(__PATHCC__,__PATHCC_MINOR__,__PATHCC_PATCHLEVEL__)
#endif

/*
 * GNU C/C++
 *
 * Version: VVRRPP : VV = Version, RR = Revision, PP = Patch
 * Example: 030200 = GCC 3.0.2
 */
#if defined(__GNUC__)
# if defined(__GNUC_PATCHLEVEL__)
#   define CC_GCC VERNO(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
# else
#   define CC_GCC VERNO(__GNUC__, __GNUC_MINOR__, 0)
# endif
#endif

/*
 * Green Hills C/C++
 */
#if defined(__ghs) || defined(__ghs__)
# if defined(__GHS_VERSION_NUMBER__)
#   define CC_GHS VERNO_10_VRP(__GHS_VERSION_NUMBER__)
# elif defined(__ghs)
#   define CC_GHS VERNO_10_VRP(__ghs)
# else
#   define CC_GHS 0
# endif
#endif

/*
 * HP ANSI C / aC++
 *
 * Version: VVRRPP : VV = Version, RR = Revision, PP = Patch
 * Example: 12100 = A.01.21
 *
 * The __HP_aCC was introduced in version A.01.15 (and A.03.13), where
 * it is set to 1. Beginning with version A.01.21 (and A.03.25) __HP_aCC
 * is set as above.
 *
 * The C compiler defines the __HP_cc macro, and the C++ compiler the
 * __HP_aCC macro.
 */
#if defined(__HP_aCC)
# if (__HP_aCC > 1)
#   define CC_HPCC VERNO_10_VVRRPP(__HP_aCC)
# else
#   define CC_HPCCVERNO_10_VVRRPP(11500)
# endif
#else
# if defined(__HP_cc)
#   define CC_HPCC VERNO_10_VVRRPP(__HP_cc)
# else
#   if defined(__cplusplus) && __cplusplus >= 199707L && defined(__hpux)
#     define CC_HPCC 0
#   endif
# endif
#endif

/*
 * IAR C/C++
 */
#if defined(__IAR_SYSTEMS_ICC__)
# define CC_IAR VERNO_10_VVRR(__VER__)
#endif

/*
 * IBM xlC
 *
 * Version: 0xVVRR : VV = Version, RR = Revision
 * Example: 0x0500 = IBM xlC 5.0
 */
#if defined(__IBMCPP__) || defined(__xlC__) || defined(__xlc__)
# if defined(__COMPILER_VER__)
#   define CC_XLC VERNO_0X_VRRPPPP(__COMPILER_VER__)
# elif defined(__xlC__)
#   define CC_XLC VERNO_0X_VVRR(__xlC__)
# elif defined(__xlc__)
#   define CC_XLC VERNO_0X_VVRR(__xlc__)
# elif defined(__IBMC__)
#   define CC_XLC VERNO_10_VRP(__IBMC__)
# elif defined(__IBMCPP__)
#   define CC_XLC VERNO_10_VRP(__IBMCPP__)
# elif defined(_AIX) && !defined(__GNUC__)
#   define CC_XLC 0
# endif
#endif

/*
 * Intel C++
 *
 * Version: VRP : V = Version, R = Revision, P = Patch
 * Example: 500 = ICC 5.0
 */
#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || \
  defined(__ECC)
# if defined(__INTEL_COMPILER) && (__INTEL_COMPILER == 9999)
#   define CC_INTEL VERNO(12, 1, 0)
# elif defined(__INTEL_COMPILER) && defined(__INTEL_COMPILER_UPDATE)
#   define CC_INTEL VERNO( \
    VERNO_MAJOR(VERNO_10_VVRR(__INTEL_COMPILER)), \
    VERNO_MINOR(VERNO_10_VVRR(__INTEL_COMPILER)), \
    __INTEL_COMPILER_UPDATE \
  )
# elif defined(__INTEL_COMPILER)
#   define CC_INTEL VERNO_10_VVRR(__INTEL_COMPILER)
# else
#   define CC_INTEL 0
# endif
#endif

/*
 * KAI C++
 *
 * Version: 0xVRPP : V = Version, R = Revision, PP = Patch
 * Example: 0x4004 = KAI C++ 4.0d
 *
 * Please note that Intel has discontinued development of the KAI C++
 * compiler in spring 2002:
 *
 *   http://developer.intel.com/software/products/kcc/announcement.htm
 */
#if defined(__KCC)
# if defined(__KCC_VERSION)
#   define CC_KAI VERNO_0X_VRPP(__KCC_VERSION)
# else
#   define CC_KAI 0
# endif
#endif

/*
 * llvm
 */
#if defined(__llvm__)
# define CC_LLVM 0
#endif

/*
 * MetaWare High C/C++
 */
#if defined(__HIGHC__)
# define CC_HIGHC 0
#endif

/*
 * Metrowerks CodeWarrior
 *
 * Version: 0xVRPP : V = Version, R = Revision, PP = Patch
 * Example: 0x2200 = Metrowerks C/C++ 2.2
 *
 * Versions prior to CodeWarrior 7 sets __MWERKS__ to 1.
 */
#if defined(__MWERKS__) || defined(__CWCC__)
# if defined(__CWCC__)
#   define CC_MWERKS VERNO_0X_VRPP(__CWCC__)
# elif (__MWERKS__ >= 0x4200)
#   define CC_MWERKS VERNO_0X_VRPP(__MWERKS__)
# elif (__MWERKS__ >= 0x3204)
#   define CC_MWERKS VERNO(9, (__MWERKS__) % 100 - 1, 0)
# elif (__MWERKS__ >= 0x3200)
#   define CC_MWERKS VERNO(9, (__MWERKS__) % 100, 0)
# elif (__MWERKS__ >= 0x3000)
#   define CC_MWERKS VERNO(8, (__MWERKS__) % 100, 0)
# else
#   define CC_MWERKS 0
# endif
#endif

/*
 * Microtec C/C++
 */
#if defined(_MRI)
# define CC_MRI 0
#endif

/*
 * Apple MPW C++
 *
 * Version: 0xVVRR : VV = Version, RR = Revision
 * Example: 0x0500 = MPW C++ 5.0
 */
#if defined(__MRC__)
# define CC_MPW VERNO_0X_VVRR(__MRC__)
#else
# if defined(MPW_C) || defined(MPW_CPLUS)
#  define CC_MPW 0
# endif
#endif

/*
 * Palm C/C++
 */
#if defined(_PACC_VER)
# define CC_PALM VERNO_0X_VRRPP000(_PACC_VER)
#endif

/*
 * Portland Group C/C++
 */
#if defined(__PGI)
# if defined(__PGIC__) && defined(__PGIC_MINOR__) && defined(__PGIC_PATCHLEVEL__)
#   define CC_PGI VERNO(__PGIC__, __PGIC_MINOR__, __PGIC_PATCHLEVEL__)
# else
#   define CC_PGI 0
# endif
#endif

/*
 * SGI MIPSpro
 *
 * Version: VRP : V = Version, R = Revision, P = Patch
 * Example: 721 = MIPSpro 7.2.1
 */
#if defined(sgi) || defined(__sgi)
# if defined(_SGI_COMPILER_VERSION)
#   define CC_MIPSPRO VERNO_10_VRP(_SGI_COMPILER_VERSION)
# elif defined(_COMPILER_VERSION)
#   define CC_MIPSPRO VERNO_10_VRP(_COMPILER_VERSION)
# else
#   define CC_MIPSPRO 0
# endif
#endif

/*
 * Norcroft C
 *
 * The __ARMCC_VERSION macro is assigned a floating-point number,
 * so it cannot be used by the preprocessor to compare versions.
 */
#if defined(__CC_NORCROFT)
# define CC_NORCROFT 0
#endif

/*
 * SCO OpenServer
 */
#if defined(_SCO_DS)
# define CC_SCO 0
#endif

/*
 * Sun Forte C/C++ (Workshop Pro)
 *
 * Version: 0xVRP : V = Version, R = Revision, P = Patch
 * Example: 0x500 = Workshop Pro 5.0
 */
#if defined(__SUNPRO_C)
# if (__SUNPRO_C < 0x5100)
#   define CC_SUNPRO VERNO_0X_VRP(__SUNPRO_C)
# else
#   define CC_SUNPRO VERNO_0X_VVRRP(__SUNPRO_C)
# endif
#elif defined(__SUNPRO_CC)
# if (__SUNPRO_CC < 0x5100)
#   define CC_SUNPRO VERNO_0X_VRP(__SUNPRO_CC)
# else
#   define CC_SUNPRO VERNO_0X_VVRRP(__SUNPRO_CC)
# endif
#endif

/*
 * TenDRA
 */
#if defined(__TenDRA__)
# define CC_TENDRA 0
#endif

/*
 * USL C
 *
 * Version: 0xVRRYYYYMM : V = Version, RR = Revision, YYYY = Year, MM = Month
 * Example: 0x302199801 = USL C 3.2
 */
#if defined(__USLC__)
# if defined(__SCO_VERSION__)
#  define CC_USLC VERNO_0X_VRRPPPP(__SCO_VERSION__)
# else
#  define CC_USLC 0
# endif
#endif

/*
 * Microsoft Visual C++
 *
 * Version: VVRR : VV = Version, RR = Revision
 * Example: 1200 = Visual C++ 6.0 (compiler 12.0)
 */
#if defined(_MSC_VER)
# if !defined (_MSC_FULL_VER)
#   define CC_MSVC_BUILD 0
# else
#   if _MSC_FULL_VER / 10000 == _MSC_VER
#     define CC_MSVC_BUILD (_MSC_FULL_VER % 10000)
#   elif _MSC_FULL_VER / 100000 == _MSC_VER
#     define CC_MSVC_BUILD (_MSC_FULL_VER % 100000)
#   else
#     define CC_MSVC_BUILD 0
#   endif
# endif
# if (_MSC_VER > 1900)
#   define CC_MSVC VERNO(_MSC_VER / 100, _MSC_VER % 100, CC_MSVC_BUILD)
# elif (_MSC_VER >= 1900)
#   define CC_MSVC VERNO(_MSC_VER / 100 - 5, _MSC_VER % 100, CC_MSVC_BUILD)
# else
#   define CC_MSVC VERNO(_MSC_VER / 100 - 6, _MSC_VER % 100, CC_MSVC_BUILD)
# endif
# define _CRT_SECURE_NO_WARNINGS 1
# pragma warning(disable: 4081)
# pragma warning(disable: 4141)
#endif

/*
 * Watcom C++
 *
 * Version: VVRR : VV = Version, RR = Revision
 * Example: 1050 = Watcom C++ 10.5
 */
#if defined(__WATCOMC__)
# define CC_WATCOM VERNO_10_VVRR(__WATCOMC__)
#endif

#endif /* !__TCL_COMPAT_COMPILER_H */
/*!@} */
