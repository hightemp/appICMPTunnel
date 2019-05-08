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

#include "tcl/vector.h"

FORCEINLINE
void vector_init(vector_t *vector, size_t isize)
{
	vector->buffer = NULL;
	vector->isize = isize;
	vector->capacity = vector->length = 0;
}

FORCEINLINE
void vector_destroy(vector_t *vector, vector_dtor_t *idtor)
{
	char *item;

	if (vector->capacity) {
		if (idtor)
			vector_foreach(vector, item)
				idtor(item);
		free(vector->buffer);
		vector->buffer = NULL;
		vector->capacity = vector->length = 0;
	}
}

FORCEINLINE
void vector_clear(vector_t *vector, vector_dtor_t *idtor)
{
	char *item;

	if (vector->length) {
		if (idtor)
			vector_foreach(vector, item)
				idtor(item);
		vector->length = 0;
	}
}

FORCEINLINE
size_t vector_length(vector_t *vector)
{
	return vector->length;
}

FORCEINLINE
void *vector_begin(vector_t *vector)
{
	if (!vector->length)
		return NULL;
	return vector->buffer;
}

FORCEINLINE
void *vector_end(vector_t *vector)
{
	if (!vector->capacity)
		return NULL;
	return (char *)vector->buffer + (vector->length * vector->isize);
}

FORCEINLINE
void *vector_back(vector_t *vector)
{
	if (!vector->capacity || !vector->length)
		return NULL;
	return (char *)vector->buffer + ((vector->length - 1) * vector->isize);
}

FORCEINLINE
void *vector_at(vector_t *vector, size_t idx)
{
	if (idx >= vector->length)
		return NULL;
	return (char *)vector_begin(vector) + (idx * vector->isize);
}

FORCEINLINE
static void __vector_alloc(vector_t *vector, size_t n)
{
	if (vector->capacity == n)
		return;
	if (!vector->buffer) {
		vector->buffer = calloc(n, vector->isize);
		vector->capacity = n;
	}
	else
	{
		vector->buffer = realloc(vector->buffer, n * vector->isize);
		if (vector->length > (vector->capacity = n))
			vector->length = n;
		bzero((char *)vector->buffer + (vector->length * vector->isize),
			(vector->capacity - vector->length) * vector->isize);
	}
}

#if __has_builtin__(__builtin_popcount)
# define ISPOW2(n) (__builtin_popcount(n) == 1)
#else
# define ISPOW2(n) (((n) != 0) && (((n) & (~(n) + 1)) == (n)))
#endif

FORCEINLINE PURE
static size_t __pow2_next(size_t n)
{
	size_t i;
	size_t j;

	if (n == SIZE_MAX || ISPOW2(n))
		return n;
	i = n;
	if (!(j = i & 0xFFFF0000))
		j = i;
	if (!(i = j & 0xFF00FF00))
		i = j;
	if (!(j = i & 0xF0F0F0F0))
		j = i;
	if (!(i = j & 0xCCCCCCCC))
		i = j;
	if (!(j = i & 0xAAAAAAAA))
		j = i;
	i = j << 1;
	return (i < n) ? (size_t)SIZE_MAX : i;
}

FORCEINLINE
void vector_ensure(vector_t *vector, size_t n)
{
	if (++n < 32 && 32 > vector->capacity)
		__vector_alloc(vector, 32);
	else if (n > vector->capacity)
		__vector_alloc(vector, __pow2_next(n));
}

FORCEINLINE
void vector_grow(vector_t *vector, size_t n)
{
	const size_t final_len = n + vector->length + 1;

	if (final_len < 32 && 32 > vector->capacity)
		__vector_alloc(vector, 32);
	else if (final_len > vector->capacity)
		__vector_alloc(vector, __pow2_next(final_len));
}

FORCEINLINE
void *vector_npush_back(vector_t *vector, size_t n)
{
	char *it;

	vector_grow(vector, n);
	it = vector_end(vector);
	vector->length += n;
	return it;
}

FORCEINLINE
void *vector_npush_front(vector_t *vector, size_t n)
{
	size_t len;
	char *it;

	vector_grow(vector, n);
	it = vector_begin(vector);
	if ((len = vector_length(vector)))
		memmove(it + (n * vector->isize), it, (len + 1) * vector->isize);
	vector->length += n;
	return it;
}

FORCEINLINE
void *vector_npush_at(vector_t *vector, size_t n, size_t idx)
{
	size_t len;
	void *it;

	if (idx == 0)
		return vector_npush_front(vector, n);
	if (idx == (len = vector_length(vector)))
		return vector_npush_back(vector, n);
	if (n == 0 || idx > len)
		return NULL;
	vector_grow(vector, n);
	it = vector_at(vector, idx);
	memmove((char *)it + (n * vector->isize), it,
		(len - idx + 1) * vector->isize);
	vector->length += n;
	return it;
}

FORCEINLINE
size_t vector_npop_back(vector_t *vector, size_t n, void *out)
{
	size_t len;

	if (!(len = vector_length(vector)))
		return 0;
	if (n > len)
		n = len;
	vector->length -= n;
	if (out)
		memcpy(out, vector_end(vector), n * vector->isize);
	return n;
}

FORCEINLINE
size_t vector_npop_front(vector_t *vector, size_t n, void *out)
{
	size_t len;
	char *it;

	if (!n || !(len = vector_length(vector)))
		return 0;
	if (n > len)
		n = len;
	it = vector_begin(vector);
	if (out)
		memcpy(out, it, n * vector->isize);
	if (vector->length -= n)
		memmove(it, it + (n * vector->isize),
			(vector->length + 1) * vector->isize);
	return n;
}

FORCEINLINE
size_t vector_npop_at(vector_t *s, size_t n, size_t idx,
									  void *out)
{
	size_t len;
	char *it;

	if (idx >= (len = vector_length(s)))
		return 0;
	if (idx == len - 1)
		return vector_npop_back(s, n, out);
	if (idx == 0)
		return vector_npop_front(s, n, out);
	if (n > len)
		n = len;
	it = vector_at(s, idx);
	if (out)
		memcpy(out, it, n * s->isize);
	len = len - idx - n + 1;
	memmove(it, it + (n * s->isize), len * s->isize);
	s->length -= n;
	return n;
}

FORCEINLINE
void *vector_push_back(vector_t *vector)
{
	return vector_npush_back(vector, 1);
}

FORCEINLINE
void *vector_push_front(vector_t *vector)
{
	return vector_npush_front(vector, 1);
}

FORCEINLINE
void *vector_push_at(vector_t *vector, size_t i)
{
	return vector_npush_at(vector, 1, i);
}

FORCEINLINE
bool vector_pop_back(vector_t *vector, void *out)
{
	return vector_npop_back(vector, 1, out) == 1;
}

FORCEINLINE
bool vector_pop_front(vector_t *vector, void *out)
{
	return vector_npop_front(vector, 1, out) == 1;
}

FORCEINLINE
bool vector_pop_at(vector_t *vector, size_t idx, void *out)
{
	return vector_npop_at(vector, 1, idx, out) == 1;
}

FORCEINLINE
size_t vector_indexof(vector_t *vector, void *item) {
	size_t i;

	i = 0;
	while (i < vector->length) {
		if (!memcmp(*(void **)vector_at(vector, i), item, vector->isize)) {
			return i;
		}
		++i;
	}
	return (size_t)-1;
}
