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

#include "tcl/queue.h"

FORCEINLINE void queue_init(fifo_t *queue, size_t isize)
{
	vector_init(&queue->base, isize);
}

FORCEINLINE void queue_destroy(fifo_t *queue, fifo_dtor_t *idtor)
{
	vector_destroy(&queue->base, idtor);
}

FORCEINLINE void queue_clear(fifo_t *queue, fifo_dtor_t *idtor)
{
	vector_clear(&queue->base, idtor);
}

FORCEINLINE size_t queue_length(fifo_t *queue)
{
	return vector_length(&queue->base);
}

FORCEINLINE void *queue_peek(fifo_t *queue)
{
	return vector_begin(&queue->base);
}

FORCEINLINE void queue_ensure(fifo_t *queue, size_t n)
{
	vector_ensure(&queue->base, n);
}

FORCEINLINE void queue_grow(fifo_t *queue, size_t n_added)
{
	vector_grow(&queue->base, n_added);
}

FORCEINLINE void *queue_npush(fifo_t *queue, size_t n)
{
	return vector_npush_back(&queue->base, n);
}

FORCEINLINE size_t queue_npop(fifo_t *queue, size_t n, void *out)
{
	return vector_npop_front(&queue->base, n, out);
}

FORCEINLINE void *queue_push(fifo_t *queue)
{
	return queue_npush(queue, 1);
}

FORCEINLINE bool queue_pop(fifo_t *queue, void *out)
{
	return queue_npop(queue, 1, out) == 1;
}

FORCEINLINE size_t queue_index_of(fifo_t *queue, void *item) {
	return vector_indexof(&queue->base, item);
}

FORCEINLINE bool queue_pop_at(fifo_t *queue, size_t idx, void *out) {
	return vector_pop_at(&queue->base, idx, out);
}
