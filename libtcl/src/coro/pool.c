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

#include "coro/internal.h"
#include "tcl/string.h"

#ifdef RUNNING_ON_VALGRIND
# include <valgrind/valgrind.h>
#endif

#if defined(_POSIX_MAPPED_FILES) && defined(HAS_MMAP) && defined(HAS_MUNMAP)
# include <sys/mman.h>
# define __CORO_MMAP
# ifndef MAP_ANONYMOUS
#   ifdef MAP_ANON
#     define MAP_ANONYMOUS MAP_ANON
#   else
#     undef __CORO_MMAP
#   endif
# endif
# include <limits.h>
#else
# undef __CORO_MMAP
#endif

#if defined(__CORO_MMAP) && defined(_POSIX_MEMORY_PROTECTION) && \
	defined(HAS_MPROTECT)
# ifndef __CORO_GUARD
#   define __CORO_GUARD 4
# endif
#else
# undef __CORO_GUARD
# define __CORO_GUARD 0
#endif

FORCEINLINE CONST
static size_t __getpagesize(void)
{
#ifdef PAGE_SIZE
	return (size_t)PAGE_SIZE;
#elif defined(PAGESIZE)
	return (size_t)PAGESIZE;
#elif defined(HAS_GETPAGESIZE)
	return (size_t)getpagesize();
#elif defined(HAS_SYSCONF) && defined(_SC_PAGESIZE)
	return (size_t)sysconf(_SC_PAGESIZE);
#else
	return 4096;
#endif
}

coro_t __coroalloc(size_t stack_size)
{
	size_t ssize;
	coro_t coro = NULL;

	if (!stack_size)
		stack_size = 256 * 1024;
	stack_size = (stack_size * sizeof(void *) + __getpagesize() - 1)
		/ __getpagesize() * __getpagesize();
	ssize = stack_size + (__CORO_GUARD * __getpagesize());
#ifdef __CORO_MMAP
	coro = mmap(0, ssize, PROT_READ | PROT_WRITE | PROT_EXEC,
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (coro == (void *)-1) {
		coro = mmap (0, ssize, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (coro == (void *)-1)
			return NULL;
	}
# if __CORO_GUARD
	if (mprotect(coro, __CORO_GUARD * (size_t)__getpagesize(), PROT_NONE)) {
		munmap((char *)coro - __CORO_GUARD * __getpagesize(), ssize);
		return NULL;
	}
	coro = (coro_t)((char *)coro + __CORO_GUARD * __getpagesize());
# endif
#else
	if (!(coro = malloc(ssize)))
		return NULL;
#endif /* __CORO_MMAP */
	bzero(coro, sizeof(struct coro));
	coro->ssize = stack_size;
#ifdef RUNNING_ON_VALGRIND
	coro->valgrind_id = VALGRIND_STACK_REGISTER((char *)coro,
		((char *)coro) + ssize - __CORO_GUARD * __getpagesize());
#endif /* RUNNING_ON_VALGRIND */
	return coro;
}

void __cororelease(coro_t coro)
{
#ifdef RUNNING_ON_VALGRIND
	VALGRIND_STACK_DEREGISTER(coro->valgrind_id);
#endif /* RUNNING_ON_VALGRIND */
#ifdef __CORO_MMAP
	munmap((void*)((char *)coro - __CORO_GUARD * __getpagesize()),
		coro->ssize + __CORO_GUARD * __getpagesize());
#else
	free(coro);
#endif /* __CORO_MMAP */
}
