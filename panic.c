#include "panic.h"

#include <stdio.h>
#include <stdlib.h>

void
panic(const char* fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  vfprintf(stderr, fmt, va);
  va_end(va);
  exit(EXIT_FAILURE);
}
