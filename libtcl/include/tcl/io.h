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

/*!@file tcl/io.h
 * @author uael
 *
 * @addtogroup tcl.io @{
 */
#ifndef __TCL_IO_H
# define __TCL_IO_H

#include "tcl/compat.h"

#include <stdio.h>

#ifdef HAS_FCNTL_H
# include <fcntl.h>
#endif

#ifdef CC_MSVC
# include <io.h>
# define read _read
# define write _write
# define close _close
# define pipe(FDS) _pipe(FDS, 4096, 0)
#endif

#endif /* !__TCL_IO_H */
/*!@} */
