#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "scel.h"

static void
test_fixnum(void)
{
  scel_t s = scel_make_fixnum(42);
  assert(scel_is_immediate(s));
  assert(scel_is_fixnum(s));
  assert(!scel_is_codepoint(s));
  assert(!scel_is_single_float(s));
  assert(scel_as_fixnum(s) == 42);
  assert(scel_as_fixnum(scel_make_fixnum(42) + scel_make_fixnum(34)
			) == 76L);
  assert(scel_as_fixnum(scel_make_fixnum(42) + scel_make_fixnum(-61)
			) == -19L);
}

static void
test_single_float(void)
{
  scel_t s = scel_make_single_float(3.14);
  assert(scel_is_immediate(s));
  assert(scel_is_single_float(s));
  assert(!scel_is_codepoint(s));
  assert(!scel_is_fixnum(s));
  assert(scel_as_single_float(s) == 3.14f);
}

static void
test_codepoint(void)
{
  scel_t s = scel_make_codepoint(97);
  assert(scel_is_immediate(s));
  assert(scel_is_codepoint(s));
  assert(!scel_is_fixnum(s));
  assert(!scel_is_single_float(s));
  assert(scel_as_codepoint(s) == 97);
}

#define stringify(s) #s
#define run(testname)				\
  do {						\
    printf(stringify(testname)"...");		\
    test_##testname();				\
    printf("PASS\n");				\
  } while (0);

int
main (void)
{
  run(fixnum);
  run(single_float);
  run(codepoint);

  return EXIT_SUCCESS;
}
