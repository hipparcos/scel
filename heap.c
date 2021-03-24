#include "heap.h"

#include <assert.h>
#include <stddef.h>

struct scel_heap
{
  size_t allocable;
  size_t allocated;
  void *heap;
};

const unsigned SCEL_HEAP_MIN_BYTES = sizeof(struct scel_heap)+sizeof(struct scel_pair);
const unsigned SCEL_HEAP_ALIGNMENT = sizeof(scel_t);

static size_t
heap_align(size_t size)
{
  return (size ^ (SCEL_HEAP_ALIGNMENT - 1)) & ~SCEL_HEAP_ALIGNMENT;
}

size_t
scel_heap_size(size_t inbytes)
{
  if (inbytes < SCEL_HEAP_MIN_BYTES)
    return SCEL_HEAP_MIN_BYTES;
  return sizeof(struct scel_heap)
    + heap_align(inbytes - sizeof(struct scel_heap));
}

scel_heap_t *
scel_heap_init(void *heap, size_t size)
{
  assert(size >= SCEL_HEAP_MIN_BYTES);
  struct scel_heap *h = (struct scel_heap *) heap;
  *h = (struct scel_heap) {
    .heap      = heap + sizeof(struct scel_heap),
    .allocable = size - sizeof(struct scel_heap),
    .allocated = 0,
  };
  return h;
}

size_t
scel_sizeof_heap(scel_heap_t *heap)
{
  return sizeof(*heap) + heap->allocable;
}

size_t
scel_sizeof_allocable_heap(scel_heap_t *heap)
{
  return heap->allocable;
}

size_t
scel_sizeof_allocated_heap(scel_heap_t *heap)
{
  return heap->allocated;
}

static enum scel_error
alloc(scel_heap_t *heap, size_t size, intptr_t *ret)
{
  if (heap->allocable - heap->allocated < size)
    return SCEL_HEAP_EXHAUSTED;
  *ret = (intptr_t) heap->heap;
  heap->heap += size;
  heap->allocated += size;
  return SCEL_NO_ERROR;
}

struct scel_result
scel_alloc_pair(scel_heap_t* heap, struct scel_pair p)
{
  if (!heap) {
    return (struct scel_result)
      { .typ= SCEL_ERROR, .err= SCEL_INVALID_HEAP };
  }
  intptr_t ptr;
  enum scel_error err;
  if (SCEL_NO_ERROR != (err = alloc(heap, sizeof(struct scel_pair), &ptr))) {
    return (struct scel_result)
      { .typ= SCEL_ERROR, .err= err };
  }
  struct scel_pair *result = (struct scel_pair *)ptr;
  *result = p;
  return (struct scel_result)
    { .typ= SCEL_OK, .ok= scel_tag_pair(result) };
}

struct scel_pair
scel_deref_pair(scel_t ptr)
{
  assert(scel_is_pair(ptr));
  return *(struct scel_pair *)scel_untag(ptr);
}
