#ifndef H_HEAP__
#define H_HEAP__

#include <stddef.h>

#include "scel.h"
#include "pair.h"
#include "result.h"

/** scel_heap_t: an opaque type pointing to the heap.
 ** The heap is a memory allocator responsible for tracking what is
 ** allocated. It allows the heap memory to be garbage collected by
 ** hiding the allocation implementation behind an API.
 ** The garbage collection logic is not implemented yet and the heap
 ** is not growable. Allocation is done until the heap is exhausted. */
typedef struct scel_heap scel_heap_t;

/** SCEL_HEAP_MIN_BYTES: the min size of the heap in bytes. */
extern const unsigned SCEL_HEAP_MIN_BYTES;
/** SCEL_HEAP_ALIGNMENT: number of bytes at which the heap is aligned to, */
extern const unsigned SCEL_HEAP_ALIGNMENT;

/** scel_heap_size: return a properly aligned heap size in byte. */
size_t scel_heap_size(size_t inbytes);
/** scel_heap_init: init HEAP of size SIZE to be used as a heap.
 ** The caller is responsible for freeing HEAP.
 ** The returned opaque structure must not be freed.
 ** It must not be used after HEAP has been freed. */
scel_heap_t* scel_heap_init(void *heap, size_t size);

/** scel_alloc_pair: allocate a pair on the heap, set it to P. */
struct scel_result scel_alloc_pair(scel_heap_t* heap, struct scel_pair p);

/** scel_deref_pair: dereference PTR, returning the associated pair. */
struct scel_pair scel_deref_pair(scel_t ptr);

/* Accessors */
/** scel_sizeof_heap: get the full size of HEAP and its header. */
size_t scel_sizeof_heap(scel_heap_t* heap);
/** scel_sizeof_allocable_heap: get the allocable memory. */
size_t scel_sizeof_allocable_heap(scel_heap_t* heap);
/** scel_sizeof_allocated_heap: get the allocated memory. */
size_t scel_sizeof_allocated_heap(scel_heap_t* heap);

#endif
