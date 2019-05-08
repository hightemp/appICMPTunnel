/*
 * Copyright (C) 2014 Google, Inc.
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

#include "tcl/buffer.h"

buffer_t *buffer_new(size_t size)
{
	buffer_t *buf = malloc(sizeof(buffer_t) + size);

	buf->root = buf;
	buf->refcount = 1;
	buf->length = size;
	bzero(buf->data, size);
	return buf;
}

buffer_t *buffer_newref(const buffer_t *buf)
{
	return buffer_newslice(buf, buf->length);
}

buffer_t *buffer_newslice(const buffer_t *buf, size_t slice_size)
{
	buffer_t *ret = calloc(1, sizeof(buffer_t));

	ret->root = buf->root;
	ret->refcount = SIZE_MAX;
	ret->length = slice_size;

	++buf->root->refcount;

	return ret;
}

void buffer_free(buffer_t *buf)
{
	if (buf->root != buf) {
		if (--buf->root->refcount == 0)
			free(buf->root);
		free(buf);
	} else if (--buf->refcount == 0)
		free(buf);
}

void *buffer_ptr(const buffer_t *buf)
{
	return buf->root->data + buf->root->length - buf->length;
}

size_t buffer_length(const buffer_t *buf)
{
	return buf->length;
}
