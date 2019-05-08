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

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(tcl C)
set(CMAKE_C_STANDARD 99)

set(tcl_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(tcl_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(tcl_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

option(TCL_CHECK "Enable testing" ON)
option(TCL_THREADING "Enable Threading" ON)
if (CMAKE_BUILD_TYPE MATCHES "Release")
  option(TCL_LOGGING "Enable Logging" OFF)
else ()
  option(TCL_LOGGING "Enable Logging" ON)
endif ()

set(CMAKE_REQUIRED_QUIET_OLD ${CMAKE_REQUIRED_QUIET})
set(CMAKE_REQUIRED_QUIET ON)

include(CheckIncludeFile)
message(STATUS "Checking include file")
check_include_file(sys/eventfd.h HAS_SYS_EVENTFD_H)
check_include_file(sys/epoll.h HAS_SYS_EPOLL_H)
check_include_file(sys/time.h HAS_SYS_TIME_H)
check_include_file(sys/types.h HAS_SYS_TYPES_H)
check_include_file(sys/prctl.h HAS_SYS_PRCTL_H)
check_include_file(sys/_system_properties.h HAS_SYS__SYSTEM_PROPERTIES_H)
check_include_file(pthread.h HAS_PTHREAD_H)
check_include_file(unistd.h HAS_UNISTD_H)
check_include_file(time.h HAS_TIME_H)
check_include_file(fcntl.h HAS_FCNTL_H)
check_include_file(signal.h HAS_SIGNAL_H)
check_include_file(ucontext.h HAS_UCONTEXT_H)
check_include_file(setjmp.h HAS_SETJMP_H)
check_include_file(inttypes.h HAS_INTTYPES_H)
check_include_file(stdbool.h HAS_STDBOOL_H)
check_include_file(stdint.h HAS_STDINT_H)
check_include_file(strings.h HAS_STRINGS_H)
message(STATUS "Checking include file - done")

include(CheckFunctionExists)
message(STATUS "Checking function")
check_function_exists(bzero HAS_BZERO)
check_function_exists(free HAS_FREE)
check_function_exists(realloc HAS_REALLOC)
check_function_exists(strnlen HAS_STRNLEN)
check_function_exists(strlcpy HAS_STRLCPY)
check_function_exists(strncpy HAS_STRNCPY)
check_function_exists(wakelock_acquire HAS_WAKELOCK_ACQUIRE)
check_function_exists(wakelock_release HAS_WAKELOCK_RELEASE)
message(STATUS "Checking function - done")

include(CheckSymbolExists)
message(STATUS "Checking symbole")
check_symbol_exists(sigaltstack "signal.h" HAS_SIGALTSTACK)
check_symbol_exists(mmap "sys/mman.h" HAS_MMAP)
check_symbol_exists(munmap "sys/mman.h" HAS_MUNMAP)
check_symbol_exists(mprotect "sys/mman.h" HAS_MPROTECT)
check_symbol_exists(sysconf "unistd.h" HAS_SYSCONF)
check_symbol_exists(getpagesize "unistd.h" HAS_GETPAGESIZE)
check_symbol_exists(usleep "unistd.h" HAS_USLEEP)
message(STATUS "Checking symbole - done")

set(CMAKE_REQUIRED_QUIET ${CMAKE_REQUIRED_QUIET_OLD})

message(STATUS "Looking for concurrent method")

if (TCL_THREADING AND (NOT HAS_SYS_EVENTFD_H OR NOT HAS_SYS_EPOLL_H
  OR NOT HAS_PTHREAD_H OR NOT HAS_UNISTD_H OR NOT HAS_SYS_PRCTL_H
  OR NOT HAS_FCNTL_H))
  set(TCL_THREADING OFF)
endif ()

set(WORKER_LIB)
if (TCL_THREADING)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads QUIET)
  if (Threads_FOUND)
    set(WORKER_LIB Threads::Threads)
  else ()
    set(TCL_THREADING OFF)
  endif ()
endif ()

if (TCL_THREADING)
  message(STATUS "Looking for concurrent method - pthread")
 else ()
  message(STATUS "Looking for concurrent method - fiber")
endif ()

file(GLOB tcl_SRCS ${tcl_SRCS} ${tcl_SRC_DIR}/[a-z_]*.c)
file(GLOB_RECURSE tcl_SRCS ${tcl_SRCS} ${tcl_SRC_DIR}/*.h)
file(GLOB_RECURSE tcl_HDRS ${tcl_HDRS} ${tcl_INC_DIR}/tcl/*.h)
set(tcl_HDR ${tcl_INC_DIR}/tcl.h ${CMAKE_BINARY_DIR}/include/tcl/conf.h)

add_subdirectory(${tcl_SRC_DIR}/coro)

configure_file(${tcl_INC_DIR}/tcl/conf.h.in
  ${CMAKE_BINARY_DIR}/include/tcl/conf.h @ONLY)

add_library(tcl ${tcl_SRCS} ${tcl_HDRS} ${tcl_HDR})
target_include_directories(tcl
  PUBLIC ${tcl_INC_DIR} ${CMAKE_BINARY_DIR}/include
  PRIVATE ${tcl_SRC_DIR})

target_link_libraries(tcl PUBLIC ${WORKER_LIB})

target_compile_definitions(tcl
  PRIVATE TCL_COMPILE)
if (MSVC)
  target_link_libraries(tcl PRIVATE ws2_32)
  target_compile_options(tcl
    PUBLIC /MT$<$<CONFIG:Debug>:d> /Oy /W3
    PUBLIC /D_CRT_SECURE_NO_WARNINGS
    PUBLIC /O$<$<CONFIG:Debug>:d>$<$<CONFIG:Release>:x>)
  if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(tcl PUBLIC /Ox)
  else ()
    target_compile_options(tcl PUBLIC /Od)
  endif ()

  foreach (flag_var
    CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if (${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif ()
  endforeach ()

else ()
  target_compile_options(tcl
    PUBLIC -Wall -Werror -Wextra -fomit-frame-pointer
    PUBLIC -Wno-implicit-function-declaration
    PUBLIC -Wno-missing-field-initializers
    PUBLIC -Wno-pointer-to-int-cast
    PRIVATE -ansi -Wshadow -Wswitch-default -Wunreachable-code
    PRIVATE -Wstrict-prototypes -Wmissing-prototypes
    PRIVATE -Wunreachable-code -Winit-self
    PRIVATE -Wmissing-declarations -Wfloat-equal -Wbad-function-cast -Wundef
    PRIVATE -Waggregate-return -Wold-style-definition -Wredundant-decls)
  if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(tcl PUBLIC -O2 -g)
  else ()
    target_compile_options(tcl PUBLIC -O0 -g3)
    if (CMAKE_BUILD_TYPE MATCHES San)
      target_link_libraries(tcl
        PUBLIC "-fsanitize=address -fno-omit-frame-pointer")
      target_compile_options(tcl PUBLIC "-fsanitize=address")
    endif ()
  endif ()
endif ()

if (TCL_CHECK AND EXISTS ${tcl_TEST_DIR})
  enable_testing()

  if (NOT TARGET check)
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --timeout 4)
  endif ()
  add_subdirectory(${tcl_TEST_DIR})
endif ()

install(FILES ${tcl_HDRS} DESTINATION include/tcl)
install(FILES ${tcl_HDR} DESTINATION include)
