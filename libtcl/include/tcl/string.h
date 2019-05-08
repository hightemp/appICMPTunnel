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

/*!@file tcl/string.h
 * @author uael
 *
 * @addtogroup tcl.string @{
 */
#ifndef __TCL_STRING_H
# define __TCL_STRING_H

#include "tcl/compat.h"

#include <string.h>

#ifdef HAS_STRINGS_H
# include <strings.h>
#endif

#ifndef HAS_BZERO
void bzero(void *ptr, size_t n);
#endif

#ifndef HAS_STRNLEN
size_t strnlen(const char *s, size_t n);
#endif

#ifndef HAS_STRLCPY
size_t strlcpy(char *__restrict dst, const char *__restrict src, size_t dstsize);
#endif

#ifndef HAS_STRNCPY
char *strncpy(char *dest, const char *src, size_t n);
#endif

#endif /* __TCL_STRING_H */
/*!@} */
