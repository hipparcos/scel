#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "testing.h"

define_stack(int_stack, int);

typedef int* pint;
define_stack(pint_stack, pint);

bool panicked = false;

void
panic(const char* fmt, ...)
{
  (void)fmt;
  panicked = true;
}

static void
test_static_stack(void)
{
  int integers[3] = {0};
  struct int_stack stack = int_stack_make_static(3, &integers[0]);
  int_stack_push(&stack, 1);
  int_stack_push(&stack, 2);
  int_stack_push(&stack, 3);
  assert(int_stack_pop(&stack).result == 3);
  assert(int_stack_pop(&stack).result == 2);
  assert(int_stack_pop(&stack).result == 1);
  assert(int_stack_pop(&stack).error == true);
}

static void
test_static_stack_overflow(void)
{
  panicked = false;
  int integers[1] = {0};
  struct int_stack stack = int_stack_make_static(1, &integers[0]);
  int_stack_push(&stack, 1);
  int_stack_push(&stack, 2);
  assert(panicked == true);
  panicked = false;
}

int mallocated = 0;
int reallocated = 0;

int *
int_allocator(int *buffer, size_t cap)
{
  if (!buffer) {
    mallocated++;
    return calloc(cap, sizeof(int));
  }
  reallocated++;
  return realloc(buffer, cap);
}

static void
test_dynamic_stack(void)
{
  panicked = false;
  mallocated = 0;
  reallocated = 0;
  struct int_stack stack = int_stack_make_dynamic(3, int_allocator);
  int_stack_push(&stack, 1);
  int_stack_push(&stack, 2);
  int_stack_push(&stack, 3);
  int_stack_push(&stack, 4);
  assert(panicked == false);
  int_stack_push(&stack, 5);
  int_stack_push(&stack, 6);
  assert(int_stack_pop(&stack).result == 6);
  assert(int_stack_pop(&stack).result == 5);
  assert(int_stack_pop(&stack).result == 4);
  assert(int_stack_pop(&stack).result == 3);
  assert(int_stack_pop(&stack).result == 2);
  assert(int_stack_pop(&stack).result == 1);
  assert(int_stack_pop(&stack).error == true);
  free(stack.buffer);
  assert(mallocated == 1);
  assert(reallocated == 1);
}

static void
test_static_stack_of_pointers(void)
{
  int x = 1, y = 2, z = 3;
  pint integers[3] = {0};
  struct pint_stack stack = pint_stack_make_static(3, integers);
  pint_stack_push(&stack, &x);
  pint_stack_push(&stack, &y);
  pint_stack_push(&stack, &z);
  assert(*pint_stack_pop(&stack).result == 3);
  assert(*pint_stack_pop(&stack).result == 2);
  assert(*pint_stack_pop(&stack).result == 1);
  assert(pint_stack_pop(&stack).error == true);
}

int
main (void)
{
  run_test(static_stack);
  run_test(static_stack_overflow);
  run_test(dynamic_stack);
  run_test(static_stack_of_pointers);

  return EXIT_SUCCESS;
}
