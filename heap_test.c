#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "heap.h"
#include "testing.h"

static void
test_heap_init(void)
{
  size_t bytes = scel_heap_size(0);
  void *heap = malloc(bytes);
  struct scel_heap *got = scel_heap_init(heap, bytes);
  assert(bytes == SCEL_HEAP_MIN_BYTES);
  assert(scel_sizeof_heap(got) == bytes);
  assert(scel_sizeof_allocable_heap(got) < bytes);
  assert(scel_sizeof_allocable_heap(got) > 0);
  free(heap);
}

static void
test_alloc_pair(void)
{
  size_t bytes = scel_heap_size(0);
  void *heap = malloc(bytes);
  struct scel_heap *h = scel_heap_init(heap, bytes);
  struct scel_pair p = {scel_make_fixnum(42), SCEL_NIL};
  struct scel_result r1 = scel_alloc_pair(h, p);
  assert(r1.typ == SCEL_OK);
  assert(scel_is_pair(r1.ok));
  assert(scel_deref_pair(r1.ok).car == p.car);
  assert(scel_deref_pair(r1.ok).cdr == p.cdr);
  struct scel_result r2 = scel_alloc_pair(h, p);
  assert(r2.typ == SCEL_ERROR);
  assert(r2.err == SCEL_HEAP_EXHAUSTED);
  free(heap);
}

int
main (void)
{
  run_test(heap_init);
  run_test(alloc_pair);

  return EXIT_SUCCESS;
}
