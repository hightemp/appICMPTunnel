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

#include "tcl/set.h"

#define __ac_bit_empty 0b00000010
#define __ac_bit_deleted 0b00000001
#define __ac_bit_any 0b11111111

#define __ac_isempty(flag, i) (flag[i]&__ac_bit_empty)
#define __ac_isdel(flag, i) (flag[i]&__ac_bit_deleted)
#define __ac_iseither(flag, i) (flag[i]&__ac_bit_any)
#define __ac_set_isempty_false(flag, i) (flag[i]&=~__ac_bit_empty)
#define __ac_set_isboth_false(flag, i) (flag[i]=0)
#define __ac_set_isdel_true(flag, i) (flag[i]|=__ac_bit_deleted)

#ifndef ROUNDUP32
# define ROUNDUP32(x) \
	(--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, \
		(x)|=(x)>>8, (x)|=(x)>>16, ++(x))
#endif

static const double __ac_HASH_UPPER = 0.77;

static size_t CONST __hash(const void *key)
{
	return (size_t)key;
}

static bool CONST __eq(const void *x, const void *y)
{
	return x == y;
}

FORCEINLINE void set_init(set_t *set, hash_fn_t *hash, hash_eq_t *eq)
{
	bzero(set, sizeof(set_t));
	set->hash = hash ? hash : __hash;
	set->eq = eq ? eq : __eq;
}

FORCEINLINE void set_destroy(set_t *set)
{
	if (set->flags) {
		free((void *)set->keys);
		free(set->flags);
	}
}

FORCEINLINE void set_clear(set_t *set)
{
	if (set->flags) {
		memset(set->flags, __ac_bit_empty, set->capacity);
		set->length = set->occupied = 0;
	}
}

FORCEINLINE size_t set_length(const set_t *set)
{
	return set->length;
}

static FORCEINLINE bool __getidx(const set_t *set, const void *key,
	size_t *idx)
{
	size_t k, i, last, mask, step;

	if (set->capacity) {
		step = 0;
		mask = set->capacity - 1;
		k = set->hash(key);
		i = k & mask;
		last = i;
		while (!__ac_isempty(set->flags, i) &&
			(__ac_isdel(set->flags, i) || !set->eq(set->keys[i], key))) {
			i = (i + (++step)) & mask;
			if (i == last)
				return false;
		}
		if (!__ac_iseither(set->flags, i)) {
			if (idx) *idx = i;
			return true;
		}
	}
	return false;
}

bool set_contains(const set_t *set, const void *key)
{
	return __getidx(set, key, NULL);
}

static int __resize(set_t *h, size_t new_n_buckets)
{
	uint8_t *new_flags = 0;
	size_t j = 1;
	{
		ROUNDUP32(new_n_buckets);
		if (new_n_buckets < 4)new_n_buckets = 4;
		if (h->length >= (size_t)(new_n_buckets * __ac_HASH_UPPER + 0.5))
			j = 0;
		else {
			new_flags = (uint8_t *)malloc(new_n_buckets);
			if (!new_flags)return -1;
			memset(new_flags, __ac_bit_empty, new_n_buckets);
			if (h->capacity < new_n_buckets) {
				const void **new_keys = (const void **)realloc((void *)h->keys,
					new_n_buckets * sizeof(const void *));
				if (!new_keys) {
					free(new_flags);
					return -1;
				}
				h->keys = new_keys;
			}
		}
	}
	if (j) {
		for (j = 0; j != h->capacity; ++j) {
			if (__ac_iseither(h->flags, j) == 0) {
				const void *key = h->keys[j];
				size_t new_mask;
				new_mask = new_n_buckets - 1;
				__ac_set_isdel_true(h->flags, j);
				while (1) {
					size_t k, i, step = 0;
					k = h->hash(key);
					i = k & new_mask;
					while (!__ac_isempty(new_flags, i))
						i = (i + (++step)) & new_mask;
					__ac_set_isempty_false(new_flags, i);
					if (i < h->capacity && __ac_iseither(h->flags, i) == 0) {
						{
							const void *tmp = h->keys[i];
							h->keys[i] = key;
							key = tmp;
						}
						__ac_set_isdel_true(h->flags, i);
					} else {
						h->keys[i] = key;
						break;
					}
				}
			}
		}
		if (h->capacity > new_n_buckets) {
			h->keys = (const void **)realloc((void *)h->keys,
				new_n_buckets * sizeof(const void *));
		}
		free(h->flags);
		h->flags = new_flags;
		h->capacity = new_n_buckets;
		h->occupied = h->length;
		h->upbound = (size_t)(h->capacity * __ac_HASH_UPPER + 0.5);
	}
	return 0;
}

int set_put(set_t *set, const void *key)
{
	size_t x;
	if (set->occupied >= set->upbound) {
		if (set->capacity > (set->length << 1)) {
			if (__resize(set, set->capacity - 1) < 0) {
				return -1;
			}
		} else if (__resize(set, set->capacity + 1) < 0) {
			return -1;
		}
	}
	{
		size_t k, i, site, last, mask = set->capacity - 1, step = 0;
		x = site = set->capacity;
		k = set->hash(key);
		i = k & mask;
		last = i;
		while (!__ac_isempty(set->flags, i) && (__ac_isdel(set->flags, i) ||
			!set->eq(set->keys[i], key))) {
			if (__ac_isdel(set->flags, i))site = i;
			i = (i + (++step)) & mask;
			if (i == last) {
				x = site;
				break;
			}
		}
		if (x == set->capacity) {
			if (__ac_isempty(set->flags, i) && site != set->capacity)
				x = site;
			else x = i;
		}
	}
	set->keys[x] = key;
	if (__ac_isempty(set->flags, x)) {
		__ac_set_isboth_false(set->flags, x);
		++set->length;
		++set->occupied;
	}
	else if (__ac_isdel(set->flags, x)) {
		__ac_set_isboth_false(set->flags, x);
		++set->length;
	}
	return 0;
}

bool set_remove(set_t *set, const void *key)
{
	size_t idx;

	if (!__getidx(set, key, &idx))
		return false;
	if (!__ac_iseither(set->flags, idx)) {
		__ac_set_isdel_true(set->flags, idx);
		--set->length;
		return true;
	}
	return false;
}
