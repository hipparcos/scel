#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "symbol.h"
#include "testing.h"

bool panicked = false;

void
panic(const char* fmt, ...)
{
  (void)fmt;
  panicked = true;
}

static void
test_insert_and_find(void)
{
  panicked = false;
  void *bs = calloc(4, SCEL_SYMBOL_SIZE);
  struct scel_symbol *s1 = scel_symbol_init(bs+SCEL_SYMBOL_SIZE*0, (const byte*)"s1");
  struct scel_symbol *s2 = scel_symbol_init(bs+SCEL_SYMBOL_SIZE*1, (const byte*)"s2");
  struct scel_symbol *s3 = scel_symbol_init(bs+SCEL_SYMBOL_SIZE*2, (const byte*)"s3");
  struct scel_symbol *s4 = scel_symbol_init(bs+SCEL_SYMBOL_SIZE*3, (const byte*)"s4");
  struct scel_symbol *root = NULL;
  root = scel_symbol_tree(root, s1);
  root = scel_symbol_tree(root, s2);
  root = scel_symbol_tree(root, s3);
  root = scel_symbol_tree(root, s4);
  assert(scel_symbol_find(root, (const byte*)"s1") == s1);
  assert(scel_symbol_find(root, (const byte*)"s2") == s2);
  assert(scel_symbol_find(root, (const byte*)"s3") == s3);
  assert(scel_symbol_find(root, (const byte*)"s4") == s4);
  assert(panicked == false);
  free(bs);
}

static void
test_not_found(void)
{
  void *bs = calloc(1, SCEL_SYMBOL_SIZE);
  struct scel_symbol *s1 = scel_symbol_init(bs, (const byte*)"s1");
  struct scel_symbol *root = NULL;
  root = scel_symbol_tree(root, s1);
  assert(scel_symbol_find(root, (const byte*)"not-in-tree") == NULL);
  free(bs);
}

static void
test_insert_duplicate_should_panic()
{
  panicked = false;
  void *bs = calloc(2, SCEL_SYMBOL_SIZE);
  struct scel_symbol *s1 = scel_symbol_init(bs, (const byte*)"s1");
  struct scel_symbol *s1p = scel_symbol_init(bs+SCEL_SYMBOL_SIZE, (const byte*)"s1");
  struct scel_symbol *root = NULL;
  root = scel_symbol_tree(root, s1);
  root = scel_symbol_tree(root, s1p);
  assert(panicked == true);
  free(bs);
}

int
main (void)
{
  run_test(insert_and_find);
  run_test(not_found);
  run_test(insert_duplicate_should_panic);

  return EXIT_SUCCESS;
}
