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

#include "tcl/map.h"

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

FORCEINLINE
void map_init(map_t *map, hash_fn_t *hash, hash_eq_t *eq)
{
	bzero(map, sizeof(map_t));
	map->hash = hash ? hash : __hash;
	map->eq = eq ? eq : __eq;
}

FORCEINLINE
void map_destroy(map_t *map, map_dtor_t *idtor)
{
	size_t it;

	if (map->flags) {
		if (idtor) {
			for (it = 0; it < map->capacity; ++it)
				if (!__ac_iseither(map->flags, it)) {
					idtor((void *)map->values[it]);
				}
		}
		free((void *)map->keys);
		free(map->flags);
		free((void *)map->values);
	}
}

FORCEINLINE
void map_clear(map_t *map, map_dtor_t *idtor)
{
	size_t it;

	if (map->flags) {
		if (idtor) {
			for (it = 0; it < map->length; ++it)
				if (!__ac_iseither(map->flags, it)) {
					idtor((void *)map->values[it]);
				}
		}
		memset(map->flags, __ac_bit_empty, map->capacity);
		map->length = map->occupied = 0;
	}
}

FORCEINLINE
size_t map_length(const map_t *map)
{
	return map->length;
}

bool map_indexof(const map_t *map, const void *key, size_t *idx)
{
	size_t k, i, last, mask, step;

	if (map->capacity) {
		step = 0;
		mask = map->capacity - 1;
		k = map->hash(key);
		i = k & mask;
		last = i;
		while (!__ac_isempty(map->flags, i) &&
			(__ac_isdel(map->flags, i) || !map->eq(map->keys[i], key))) {
			i = (i + (++step)) & mask;
			if (i == last)
				return false;
		}
		if (!__ac_iseither(map->flags, i)) {
			if (idx) *idx = i;
			return true;
		}
	}
	return false;
}

FORCEINLINE
bool map_populated(const map_t *map, size_t idx)
{
	return !__ac_iseither(map->flags, idx);
}

bool map_contains(const map_t *map, const void *key)
{
	return map_indexof(map, key, NULL);
}

void *map_get(const map_t *map, const void *key)
{
	size_t idx;

	if (!map_indexof(map, key, &idx))
		return NULL;
	return (void *)map->values[idx];
}

static int __resize(map_t *h, size_t new_n_buckets)
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
				if (1) {
					const void **new_vals = (const void **)realloc(
						(void *)h->values, new_n_buckets * sizeof(const void *));
					if (!new_vals) {
						free(new_flags);
						return -1;
					}
					h->values = new_vals;
				}
			}
		}
	}
	if (j) {
		for (j = 0; j != h->capacity; ++j) {
			if (__ac_iseither(h->flags, j) == 0) {
				const void *key = h->keys[j];
				const void *val;
				size_t new_mask;
				new_mask = new_n_buckets - 1;
				if (1)val = h->values[j];
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
						if (1) {
							const void *tmp = h->values[i];
							h->values[i] = val;
							val = tmp;
						}
						__ac_set_isdel_true(h->flags, i);
					} else {
						h->keys[i] = key;
						if (1)h->values[i] = val;
						break;
					}
				}
			}
		}
		if (h->capacity > new_n_buckets) {
			h->keys = (const void **)realloc((void *)h->keys,
				new_n_buckets * sizeof(const void *));
			if (1)
				h->values = (const void **)realloc((void *)h->values,
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

int map_set(map_t *map, const void *key, const void *value)
{
	size_t x;
	if (map->occupied >= map->upbound) {
		if (map->capacity > (map->length << 1)) {
			if (__resize(map, map->capacity - 1) < 0) {
				return -1;
			}
		} else if (__resize(map, map->capacity + 1) < 0) {
			return -1;
		}
	}
	{
		size_t k, i, site, last, mask = map->capacity - 1, step = 0;
		x = site = map->capacity;
		k = map->hash(key);
		i = k & mask;
		last = i;
		while (!__ac_isempty(map->flags, i) && (__ac_isdel(map->flags, i) ||
			!map->eq(map->keys[i], key))) {
			if (__ac_isdel(map->flags, i))site = i;
			i = (i + (++step)) & mask;
			if (i == last) {
				x = site;
				break;
			}
		}
		if (x == map->capacity) {
			if (__ac_isempty(map->flags, i) && site != map->capacity)
				x = site;
			else x = i;
		}
	}
	map->keys[x] = key;
	if (value) map->values[x] = value;
	if (__ac_isempty(map->flags, x)) {
		__ac_set_isboth_false(map->flags, x);
		++map->length;
		++map->occupied;
	}
	else if (__ac_isdel(map->flags, x)) {
		__ac_set_isboth_false(map->flags, x);
		++map->length;
	}
	return 0;
}

bool map_remove(map_t *map, const void *key)
{
	size_t idx;

	if (!map_indexof(map, key, &idx))
		return false;
	if (!__ac_iseither(map->flags, idx)) {
		__ac_set_isdel_true(map->flags, idx);
		--map->length;
		return true;
	}
	return false;
}

FORCEINLINE
void map_it_init(map_it_t *it, map_t const *map)
{
	it->map = map;
	it->idx = 0;
}

FORCEINLINE
bool map_it_hasnext(map_it_t *it)
{
	for (; it->idx < it->map->capacity; ++it->idx) {
		if (map_populated(it->map, it->idx))
			return true;
	}
	return false;
}

FORCEINLINE
void map_it_next(map_it_t *it)
{
	++it->idx;
}

FORCEINLINE
void *map_it_key(map_it_t *it)
{
	return (void *)it->map->keys[it->idx];
}

FORCEINLINE
void *map_it_value(map_it_t *it)
{
	return (void *)it->map->values[it->idx];
}
