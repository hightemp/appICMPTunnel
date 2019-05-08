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

#include "tcl/ringbuffer.h"

void ringbuffer_init(ringbuffer_t *p, size_t size)
{
	p->base = calloc(size, 1);
	p->head = p->tail = p->base;
	p->total = p->available = size;
}

void ringbuffer_destroy(ringbuffer_t *rb)
{
	free(rb->base);
}

size_t ringbuffer_available(const ringbuffer_t *rb)
{
	return rb->available;
}

size_t ringbuffer_size(const ringbuffer_t *rb)
{
	return rb->total - rb->available;
}

size_t ringbuffer_insert(ringbuffer_t *rb, const uint8_t *p, size_t length)
{
	size_t i;

	if (length > ringbuffer_available(rb))
		length = ringbuffer_available(rb);
	for (i = 0; i != length; ++i) {
		*rb->tail++ = *p++;
		if (rb->tail >= (rb->base + rb->total))
			rb->tail = rb->base;
	}
	rb->available -= length;
	return length;
}

size_t ringbuffer_delete(ringbuffer_t *rb, size_t length)
{
	if (length > ringbuffer_size(rb))
		length = ringbuffer_size(rb);
	rb->head += length;
	if (rb->head >= (rb->base + rb->total))
		rb->head -= rb->total;
	rb->available += length;
	return length;
}

size_t ringbuffer_peek(const ringbuffer_t *rb, off_t offset, uint8_t *p,
	size_t length)
{
	uint8_t *b;
	size_t copied;
	const size_t bytes_to_copy = (offset + length > ringbuffer_size(rb))
		? ringbuffer_size(rb) - offset : length;

	b = ((rb->head - rb->base + offset) % rb->total) + rb->base;
	for (copied = 0; copied < bytes_to_copy; ++copied) {
		*p++ = *b++;
		if (b >= (rb->base + rb->total))
			b = rb->base;
	}

	return bytes_to_copy;
}

size_t ringbuffer_pop(ringbuffer_t *rb, uint8_t *p, size_t length)
{
	const size_t copied = ringbuffer_peek(rb, 0, p, length);

	rb->head += copied;
	if (rb->head >= (rb->base + rb->total))
		rb->head -= rb->total;
	rb->available += copied;
	return copied;
}
