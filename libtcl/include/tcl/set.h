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

/*!@file tcl/set.h
 * @author uael
 *
 * @addtogroup tcl.ds @{
 */
#ifndef __TCL_SET_H
# define __TCL_SET_H

#include "tcl/map.h"

/*!@public
 *
 * @brief
 * The set structure type declaration.
 */
typedef struct set set_t;

/*!@public
 *
 * @brief
 * The set structure definition.
 */
struct set {

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
	hash_fn_t *hash;

	/*! TODO. */
	hash_eq_t *eq;
};

/*!@public
 *
 * @brief
 * TODO
 *
 * @param set
 * @param hash
 * @param eq
 */
__api void set_init(set_t *set, hash_fn_t *hash, hash_eq_t *eq);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param set
 * @param idtor
 */
__api void set_destroy(set_t *set);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param set
 * @param idtor
 */
__api void set_clear(set_t *set);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param set
 * @return
 */
__api size_t set_length(const set_t *set);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param set
 * @param key
 * @return
 */
__api bool set_contains(const set_t *set, const void *key);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param set
 * @param key
 * @param value
 * @return
 */
__api int set_put(set_t *set, const void *key);

/*!@public
 *
 * @brief
 * TODO
 *
 * @param set
 * @param x
 */
__api bool set_remove(set_t *set, const void *key);

#endif /* __TCL_SET_H */
/*!@} */
