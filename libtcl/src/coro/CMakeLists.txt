#
# Copyright 2018 Tempow
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set(CORO_X86_64 OFF)
set(CORO_X86_32 OFF)
set(CORO_UCONTEXT OFF)
set(CORO_WIN OFF)
set(CORO_SJLJ OFF)

message(STATUS "Processor - ${CMAKE_SYSTEM_PROCESSOR}")

if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
  set(CORO_X86_64 ON)
  set(tcl_SRCS ${tcl_SRCS}
    ${tcl_SRC_DIR}/coro/coro.c
    ${tcl_SRC_DIR}/coro/pool.c
    ${tcl_SRC_DIR}/coro/coro-x86_64.c)

  message(STATUS "Selecting `x86_64' coroutine implementation.")
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "x86")
  set(CORO_X86_32 ON)
  set(tcl_SRCS ${tcl_SRCS}
    ${tcl_SRC_DIR}/coro/coro.c
    ${tcl_SRC_DIR}/coro/pool.c
    ${tcl_SRC_DIR}/coro/coro-x86_32.c)

  message(STATUS "Selecting `x86_32' coroutine implementation.")
elseif (MSVC)
  set(CORO_WIN ON)
  set(tcl_SRCS ${tcl_SRCS}
    ${tcl_SRC_DIR}/coro/coro.c
    ${tcl_SRC_DIR}/coro/coro-win.c)

  message(STATUS "Selecting `windows' coroutine implementation.")
elseif (HAS_SETJMP_H)
  set(CORO_SJLJ ON)
  set(tcl_SRCS ${tcl_SRCS} ${tcl_SRC_DIR}/coro/coro-sjlj.c)

  message(STATUS "Selecting `setjmp/longjmp' coroutine implementation.")
elseif (HAS_UCONTEXT_H)
  set(CORO_UCONTEXT ON)
  set(tcl_SRCS ${tcl_SRCS}
    ${tcl_SRC_DIR}/coro/coro.c
    ${tcl_SRC_DIR}/coro/pool.c
    ${tcl_SRC_DIR}/coro/coro-ucontext.c)

  message(STATUS "Selecting `ucontext' coroutine implementation.")
else ()
  message(FATAL_ERROR "Unable to implement coroutine in your system.")
endif ()

set(CORO_X86_64 ${CORO_X86_64} PARENT_SCOPE)
set(CORO_X86_32 ${CORO_X86_32} PARENT_SCOPE)
set(CORO_UCONTEXT ${CORO_UCONTEXT} PARENT_SCOPE)
set(CORO_WIN ${CORO_WIN} PARENT_SCOPE)
set(CORO_SJLJ ${CORO_SJLJ} PARENT_SCOPE)

set(tcl_SRCS ${tcl_SRCS} PARENT_SCOPE)
