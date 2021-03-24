#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "scel.h"
#include "testing.h"

static void
test_fixnum(void)
{
  scel_t s = scel_make_fixnum(42);
  assert(scel_is_immediate(s));
  assert(scel_is_fixnum(s));
  assert(!scel_is_codepoint(s));
  assert(!scel_is_single_float(s));
  assert(!scel_is_pair(s));
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
  assert(!scel_is_pair(s));
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
  assert(!scel_is_pair(s));
  assert(scel_as_codepoint(s) == 97);
}

static void
test_pair(void)
{
  int *dummy = malloc(8);
  scel_t s = scel_tag_pair(dummy);
  assert(scel_is_pair(s));
  assert(!scel_is_immediate(s));
  assert(!scel_is_fixnum(s));
  assert(!scel_is_single_float(s));
  assert((int *)scel_untag(s) == dummy);
  free(dummy);
}

int
main (void)
{
  run_test(fixnum);
  run_test(single_float);
  run_test(codepoint);
  run_test(pair);

  return EXIT_SUCCESS;
}
