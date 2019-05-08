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

/*!@file tcl/compat/os.h
 * @author uael
 *
 * @addtogroup tcl.compat @{
 */
#ifndef __TCL_COMPAT_OS_H
# define __TCL_COMPAT_OS_H

#include "tcl/compat/compiler.h"

#if defined(_AIX) || defined(__TOS_AIX__)
# if defined(_AIX43)
#   define OS_AIX VERNO(4, 3, 0)
# elif defined(_AIX41)
#   define OS_AIX VERNO(4, 1, 0)
# elif defined(_AIX32)
#   define OS_AIX VERNO(3, 2, 0)
# elif defined(_AIX3)
#   define OS_AIX VERNO(3, 0, 0)
# else
#   define OS_AIX 0
# endif
#endif

#if defined(__amigaos__) \
  || defined(AMIGA)
# define OS_AMIGA 0
#endif

#if defined(__ANDROID__)
# define OS_ANDROID 0
#endif

#if defined(__BEOS__)
# define OS_BEOS 0
#endif

#if defined(BSD)
|| defined(_SYSTYPE_BSD)
# include <sys/param.h>
# if defined(BSD4_4)
#   define OS_BSD VERNO(4, 4, 0)
# elif defined(BSD4_3)
#   define OS_BSD VERNO(4, 3, 0)
# elif defined(BSD4_2)
#   define OS_BSD VERNO(4, 2, 0)
# elif defined(BSD)
#   define OS_BSD VERNO_10_VVRR(BSD)
# else
#   define OS_BSD 0
# endif
#endif

#if defined(bsdi) \
  || defined(__bsdi__)
# define OS_BSDI 0
#endif

#if defined(__CYGWIN__) || defined(__CYGWIN32__) || defined(__CYGWIN64__)
# define OS_CYGWIN 0
#endif

#if defined(_UNICOS) \
  || defined(_CRAY)
# define OS_CRAY 0
#endif

#if defined(DGUX) \
  || defined(__DGUX__) \
  || defined(__dgux__)
# define OS_DGUX 0
#endif

#if defined(__DragonFly__)
# define OS_DRAGONFLY 0
#endif

#if defined(__FreeBSD__)
# if defined(__FreeBSD_version)
#   if __FreeBSD_version < 500000
#     define OS_FREEBSD VERNO_10_VRP000(__FreeBSD_version)
#   else
#     define OS_FREEBSD VERNO_10_VRR000(__FreeBSD_version)
#   endif
# else
#   define OS_FREEBSD 0
# endif
#endif

#if defined(__HAIKU__)
# define OS_HAIKU 0
#endif

#if defined(hpux) \
  || defined(_hpux) \
  || defined(__hpux) \
  || defined(_HPUX_SOURCE)
# define OS_HPUX 0
#endif

#if defined(__GNU__)
# define OS_HURD
#endif

#if defined(__APPLE__) && defined(__MACH__) && \
    defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
# define OS_IOS 0
#endif

#if defined(sgi) \
  || defined(__sgi) \
  || defined(mips) \
  || defined(_SGI_SOURCE)
# define OS_IRIX 0
#endif

#if defined(linux) \
  || defined(__linux) \
  || defined(__linux__)
# define OS_LINUX 0
#endif

#if defined(macintosh) \
  || defined(Macintosh) \
  || defined(__MACOS__)
# define OS_MAC VERNO(9, 0, 0)
#endif

#if defined(__MACOSX__) \
  || (defined(__APPLE__) && defined(__MACH__))
# define OS_MAC VERNO(10, 0, 0)
#endif

#if defined(mpeix) \
  || defined(__mpexl)
# define OS_MPEIX 0
#endif

#if defined(MSDOS) \
  || defined(__MSDOS__) \
  || defined(_MSDOS) \
  || defined(__DOS__)
# define OS_MSDOS 0
#endif

#if defined(__MSYS__)
# define OS_MSYS 0
#endif

#if defined(__NetBSD__) || defined(__NETBSD__)
# if defined(__NETBSD__)
#   if defined(__NETBSD_version)
#     if __NETBSD_version < 500000
#       define OS_NETBSD VERNO_10_VRP000(__NETBSD_version)
#     else
#       define OS_NETBSD VERNO_10_VRR000(__NETBSD_version)
#     endif
#   else
#     define OS_NETBSD 0
#   endif
# elif defined(__NetBSD__)
#   if defined(NetBSD0_8)
#     define OS_NETBSD VERNO(0, 8, 0)
#   elif defined(NetBSD0_9)
#     define OS_NETBSD VERNO(0, 9, 0)
#   elif defined(NetBSD1_0)
#     define OS_NETBSD VERNO(1, 0, 0)
#   elif defined(__NetBSD_Version)
#     define OS_NETBSD VERNO_10_VVRR00PP00(__NetBSD_Version)
#   else
#     define OS_NETBSD 0
#   endif
# endif
#endif

#if defined(__OpenBSD__)
# if defined(OpenBSD2_0)
#   define OS_OPENBSD VERNO(2,0,0)
# elif defined(OpenBSD2_1)
#   define OS_OPENBSD VERNO(2,1,0)
# elif defined(OpenBSD2_2)
#   define OS_OPENBSD VERNO(2,2,0)
# elif defined(OpenBSD2_3)
#   define OS_OPENBSD VERNO(2,3,0)
# elif defined(OpenBSD2_4)
#   define OS_OPENBSD VERNO(2,4,0)
# elif defined(OpenBSD2_5)
#   define OS_OPENBSD VERNO(2,5,0)
# elif defined(OpenBSD2_6)
#   define OS_OPENBSD VERNO(2,6,0)
# elif defined(OpenBSD2_7)
#   define OS_OPENBSD VERNO(2,7,0)
# elif defined(OpenBSD2_8)
#   define OS_OPENBSD VERNO(2,8,0)
# elif defined(OpenBSD2_9)
#   define OS_OPENBSD VERNO(2,9,0)
# elif defined(OpenBSD3_0)
#   define OS_OPENBSD VERNO(3,0,0)
# elif defined(OpenBSD3_1)
#   define OS_OPENBSD VERNO(3,1,0)
# elif defined(OpenBSD3_2)
#   define OS_OPENBSD VERNO(3,2,0)
# elif defined(OpenBSD3_3)
#   define OS_OPENBSD VERNO(3,3,0)
# elif defined(OpenBSD3_4)
#   define OS_OPENBSD VERNO(3,4,0)
# elif defined(OpenBSD3_5)
#   define OS_OPENBSD VERNO(3,5,0)
# elif defined(OpenBSD3_6)
#   define OS_OPENBSD VERNO(3,6,0)
# elif defined(OpenBSD3_7)
#   define OS_OPENBSD VERNO(3,7,0)
# elif defined(OpenBSD3_8)
#   define OS_OPENBSD VERNO(3,8,0)
# elif defined(OpenBSD3_9)
#   define OS_OPENBSD VERNO(3,9,0)
# elif defined(OpenBSD4_0)
#   define OS_OPENBSD VERNO(4,0,0)
# elif defined(OpenBSD4_1)
#   define OS_OPENBSD VERNO(4,1,0)
# elif defined(OpenBSD4_2)
#   define OS_OPENBSD VERNO(4,2,0)
# elif defined(OpenBSD4_3)
#   define OS_OPENBSD VERNO(4,3,0)
# elif defined(OpenBSD4_4)
#   define OS_OPENBSD VERNO(4,4,0)
# elif defined(OpenBSD4_5)
#   define OS_OPENBSD VERNO(4,5,0)
# elif defined(OpenBSD4_6)
#   define OS_OPENBSD VERNO(4,6,0)
# elif defined(OpenBSD4_7)
#   define OS_OPENBSD VERNO(4,7,0)
# elif defined(OpenBSD4_8)
#   define OS_OPENBSD VERNO(4,8,0)
# elif defined(OpenBSD4_9)
#   define OS_OPENBSD VERNO(4,9,0)
# else
#   define OS_OPENBSD 0
# endif
#endif

#if defined(OS2) \
  || defined(_OS2) \
  || defined(__OS2__) \
  || defined(__TOS_OS2__)
# define OS_OS2 0
#endif

#if defined(__OS400__)
# define OS_OS400 0
#endif

#if defined(__osf__) \
  || defined(__osf) \
  || defined(CC_DECC)
# define OS_OSF 0
#endif

#if defined(__pnacl__)
# define OS_PNACL 0
#endif

#if defined(pyr)
# define OS_PYRAMID 0
#endif

#if defined(__QNX__) \
  || defined(__QNXNTO__)
# ifdef __BLACKBERRY10__
#   define OS_BB VERNO(10, 0, 0)
# endif
# if defined(_NTO_VERSION)
#   define OS_QNX VERNO_10_VVRR(_NTO_VERSION)
# elif defined(__QNX__)
#   define OS_QNX VERNO(4, 0, 0)
# else
#   define OS_QNX 0
# endif
#endif

#if defined(M_I386) \
  || defined(M_XENIX) \
  || defined(_SCO_DS) \
  || defined(_SCO_C_DIALECT) \
  || defined(CC_SCO)
# define OS_SCO 0
#endif

#if defined(_SEQUENT_) \
  || defined(sequent)
# define OS_SEQUENT 0
#endif

#if defined(sinix)
# define OS_SINIX 0
#endif

#if defined(sun) \
  || defined(__sun__) \
  || defined(CC_SUNPRO)
# if defined(__SVR4) || defined(__svr4__)
#   define OS_SOLARIS 0
# else
#   define OS_SUNOS 0
# endif
#endif

#if defined(__SYLLABLE__)
# define OS_SYLLABLE 0
#endif

#if defined(__TIZEN__)
# define OS_TIZEN 0
#endif

#if defined(ultrix) \
  || defined(__ultrix) \
  || defined(__ultrix__)
# define OS_ULTRIX 0
#endif

#if defined(unix) \
  || defined(__unix) \
  || defined(_XOPEN_SOURCE) \
  || defined(_POSIX_SOURCE)
# define OS_UNIX 0
#elif defined(OS_AIX) \
 || defined(OS_OSF) \
 || defined(OS_NETBSD) \
 || defined(OS_QNX) \
 || defined(OS_CYGWIN) \
 || (defined(OS_AMIGA) && defined(CC_GCC))
# ifndef OS_UNIX
#   define OS_UNIX 0
# endif
#endif

#if defined(__USLC__)
|| defined(__UNIXWARE__)
  || (defined(__svr4__) && defined(i386))
# define OS_UNIXWARE 0
#endif

#if defined(VMS) || defined(__VMS)
# if defined(__VMS_VER)
#   define OS_VMS VERNO_10_VVRR00PP00(__VMS_VER)
# else
#   define OS_VMS 0
# endif
#endif

#if defined(_WIN64) \
  || defined(_M_X64) \
  || defined(_M_AMD64) \
  || defined(__WIN32__) \
  || defined(_WIN32) \
  || defined(WIN32) \
  || defined(__TOS_WIN__) \
  ||defined(__WINDOWS__)
# define OS_WIN 0
#endif

#endif /* !__TCL_COMPAT_OS_H */
/*!@} */
