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

#include "tcl/string.h"

#ifndef HAS_BZERO
void bzero(void *ptr, size_t n)
{
	memset(ptr, 0, n);
}
#endif /* !HAS_BZERO */

#ifndef HAS_STRNLEN
size_t strnlen(const char *s, size_t n)
{
	const char *p;

	p = s;
	while (*p && n--)
		p++;
	return (size_t) (p - s);
}
#endif /* !HAS_STRNLEN */

#ifndef HAS_STRNCPY
char *strncpy(char *s1, const char *s2, size_t n)
{
	size_t size = strnlen(s2, n);
	if (size != n)
		memset(s1 + size, '\0', n - size);
	return memcpy(s1, s2, size);
}

#endif /* !HAS_STRNCPY */

#ifndef HAS_STRLCPY
size_t strlcpy(char *__restrict dst, const char *__restrict src, size_t siz)
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;

	/* Copy as many bytes as will fit */
	if (n != 0) {
		while (--n != 0) {
			if ((*d++ = *s++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
			*d = '\0';        /* NUL-terminate dst */
		while (*s++);
	}

	return (s - src - 1);    /* count does not include NUL */
}
#endif /* !HAS_STRLCPY */
