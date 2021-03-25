#ifndef H_STACK__
#define H_STACK__

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "panic.h"

/* Generic stack implementation:
 * Instances can be static or dynamic (growable).
 * Growable stacks must be provided an allocator.
 *
 * Imspired by:
 * https://iafisher.com/blog/2020/06/type-safe-generics-in-c */

#define define_stack(stacktype, elemtype)				\
  const double stacktype##_GROWTH_FACTOR = 2.0; 			\
  									\
  typedef elemtype *(elemtype##_allocator_t)				\
    (elemtype *buffer, size_t cap);					\
  									\
  struct stacktype							\
  {									\
    size_t top; 							\
    size_t cap; 							\
    elemtype *buffer;							\
    elemtype##_allocator_t *allocator;  				\
  };									\
  									\
  struct stacktype##_result						\
  {									\
    bool error; 							\
    elemtype result;							\
  };									\
  									\
  struct stacktype							\
  stacktype##_make_static(size_t cap, elemtype *buffer) 		\
  {									\
    return (struct stacktype)						\
      { 								\
        .top= 0,							\
        .cap= cap,							\
        .buffer= buffer,						\
      };								\
  }									\
  									\
  struct stacktype							\
  stacktype##_make_dynamic(size_t cap, elemtype##_allocator_t allocator)\
  {									\
    return (struct stacktype)						\
      {								\
        .top= 0,							\
        .cap= cap,							\
        .buffer= allocator(NULL, cap), 				\
        .allocator= allocator, 					\
      };								\
  }									\
  									\
  void 								\
  stacktype##_push(struct stacktype *stack, elemtype it)		\
  {									\
    assert(stack);							\
    if (stack->top >= stack->cap) {					\
      if (!stack->allocator) { 					\
        panic("stack is full");					\
        return; /* allow to test panic. */				\
      } else { 							\
        size_t cap = (size_t)( 					\
          (double)stack->cap * stacktype##_GROWTH_FACTOR		\
        );								\
        if (cap == stack->cap) cap++;					\
        stack->buffer = stack->allocator(stack->buffer, cap);		\
        stack->cap = cap;						\
      }								\
    }									\
    stack->buffer[stack->top] = it;					\
    stack->top++;							\
  }									\
  									\
  struct stacktype##_result						\
  stacktype##_pop(struct stacktype *stack)				\
  {									\
    assert(stack);							\
    if (stack->top <= 0) {						\
      return (struct stacktype##_result){.error= true};		\
    }									\
    elemtype top = stack->buffer[stack->top-1];			\
    stack->top--;							\
    return (struct stacktype##_result){.error= false, .result= top};	\
  }

#endif
