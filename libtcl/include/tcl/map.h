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

/*!@file tcl/map.h
 * @author uael
 *
 * @addtogroup tcl.ds @{
 */
#ifndef __TCL_MAP_H
# define __TCL_MAP_H

#include "tcl/compat.h"
#include "tcl/string.h"

/*!@public
 *
 * @brief
 * The map structure type declaration.
 */
typedef struct map map_t;

/*!@public
 *
 * @brief
 * The map iterator structure type declaration.
 */
typedef struct map_it map_it_t;

/*!@public
 *
 * @brief
 * TODO
 */
typedef size_t (hash_fn_t)(const void *key);

/*!@public
 *
 * @brief
 * TODO
 */
typedef bool (hash_eq_t)(const void *x, const void *y);

/*!@public
 *
 * @brief
 * map item destruction callback, passed to `map_destroy' to destroy every
 * item before freed.
 */
typedef void (map_dtor_t)(void *item);

/*!@public
 *
 * @brief
 * The map structure definition.
 */
struct map {

	/*! TODO. */
	size_t capacity;

	/*! TODO. */
	size_t length;

	/*! TODO. */
	size_t occupied;

	/*! TODO. */
	size_t upbound;

	/*! TODO. */
	uint8_t *flags;

	/*! TODO. */
	const void **keys;

	/*! TODO. */
	const void **values;

	/*! TODO. */
	hash_fn_t *hash;

	/*! TODO. */
	hash_eq_t *eq;
};

/*!@public
 *
 * @brief
 * The map iterator structure definition.
 */
struct map_it {

	/*! TODO. */
	map_t const *map;

	/*! TODO. */
	size_t idx;
};

/*!@public
 *
 * @brief
 * TODO
 *
 * @param it
 * @param map
 */
__api void map_it_init(map_it_t *it, map_t const *map);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param it
 * @return
 */
__api bool map_it_hasnext(map_it_t *it);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param it
 */
__api void map_it_next(map_it_t *it);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param it
 * @return
 */
__api void *map_it_key(map_it_t *it);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param it
 * @return
 */
__api void *map_it_value(map_it_t *it);

#define MAP_FOREACH(IT, MAP) \
	for (map_it_init(IT, MAP); map_it_hasnext(IT); map_it_next(IT))

/*!@public
 *
 * @brief
 * TODO
 *
 * @param map
 * @param hash
 * @param eq
 */
__api void map_init(map_t *map, hash_fn_t *hash, hash_eq_t *eq);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param map
 * @param idtor
 */
__api void map_destroy(map_t *map, map_dtor_t *idtor);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param map
 * @param idtor
 */
__api void map_clear(map_t *map, map_dtor_t *idtor);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param map
 * @return
 */
__api size_t map_length(const map_t *map);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param map
 * @param key
 * @return
 */
__api bool map_contains(const map_t *map, const void *key);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param map
 * @param key
 * @param idx
 * @return
 */
__api bool map_indexof(const map_t *map, const void *key, size_t *idx);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param map
 * @param idx
 * @return
 */
__api bool map_populated(const map_t *map, size_t idx);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param map
 * @param key
 * @return
 */
__api void *map_get(const map_t *map, const void *key);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param map
 * @param key
 * @param value
 * @return
 */
__api int map_set(map_t *map, const void *key, const void *value);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param map
 * @param x
 */
__api bool map_remove(map_t *map, const void *key);

#endif /* __TCL_MAP_H */
/*!@} */
