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

/*!@file tcl/compat/preprocessor.h
 * @author uael
 *
 * @addtogroup tcl.compat @{
 */
#ifndef __TCL_COMPAT_PREPROCESSOR_H
# define __TCL_COMPAT_PREPROCESSOR_H

#include "tcl/conf.h"

#ifndef STRINGIFY
# define STRINGIFY_EX(x) #x
# define STRINGIFY(x) STRINGIFY_EX(x)
#endif

#ifndef COUNT_OF
# define COUNT_OF(x) (sizeof(x)/sizeof(*(x)))
#endif

#ifndef containerof
# define containerof(ptr, type, member) \
	((type *) ((char *) (ptr) - offsetof(type, member)))
#endif

#define PP_VA_ARGS(...) __VA_ARGS__
#define PP_EVAL(...) PP_VA_ARGS(PP_VA_ARGS(PP_VA_ARGS(__VA_ARGS__)))

#define PP_PRIMITIVE_JOIN__(x, y) x##y
#define PP_PRIMITIVE_JOIN(x, y) PP_PRIMITIVE_JOIN__(x, y)
#define PP_JOIN(x, y) PP_PRIMITIVE_JOIN(x, y)

#define PP_LPAR (
#define PP_RPAR )
#define PP_VA_PASS(...) PP_LPAR __VA_ARGS__ PP_RPAR
#ifdef _MSC_VER
# define PP_MCALL(macro, ...) \
  PP_EVAL(PP_JOIN(PP_EVAL(macro), PP_VA_PASS(__VA_ARGS__)))
#else
# define PP_MCALL(macro, ...) PP_EVAL(PP_EVAL(macro) PP_VA_PASS(__VA_ARGS__))
#endif

#define PP_VA_NARGS_PEEK(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define PP_VA_NARGS_RSEQ 9, 8, 7, 6, 5, 4, 3, 2, 1
#define PP_VA_NARGS(...) \
	PP_MCALL(PP_VA_NARGS_PEEK, __VA_ARGS__, PP_VA_NARGS_RSEQ)

#endif /* !__TCL_COMPAT_PREPROCESSOR_H */
/*!@} */
