#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utf8.h"
#include "testing.h"

#define IS_SURROGATE(c) ((c) >= 0xd800L && (c) <= 0xdfffL)

static void
test_decode_encode_test_string(void)
{
  const byte *input = (byte*)"😊";
  struct utf8_result decoded = utf8_decode(input, 4);
  assert(decoded.type == UTF8_ACCEPT);
  assert(decoded.cp == 0x1f60a);
  assert(decoded.len == 4);

  byte encoded[5] = {0};
  byte *end = utf8_encode(encoded, 4, decoded.cp);
  assert(strcmp((char*)encoded, (char*)input) == 0);
  assert(end - encoded == 4);
  printf("%s", (char*)encoded);
}

static void
test_decode_all_codepoints(void)
{
  codepoint cp;
  int result = 0;

  for (cp = 0; cp <= 0x1fffff; cp++) {
    byte buf[4];
    utf8_encode(buf, 4, cp);
    struct utf8_result r = utf8_decode(buf, 4);

    if (cp > 0x10ffff) {
      switch (r.type) {
      case UTF8_ACCEPT:
	printf("FAIL: accepted U+%06x (as U+%06x)\n", cp, r.cp);
	result = 1;
	break;
      case UTF8_REJECT:
	break;
      default:
	printf("FAIL: incomplete U+%06x\n", cp);
	result = 1;
	break;
      }
    } else if (IS_SURROGATE(cp)) {
      switch (r.type) {
      case UTF8_ACCEPT:
	printf("FAIL: accepted U+%06x (as U+%06x)\n", cp, r.cp);
	result = 1;
	break;
      case UTF8_REJECT:
	break;
      default:
	printf("FAIL: incomplete U+%06x (surrogate)\n", cp);
	result = 1;
	break;
      }
    } else {
      switch (r.type) {
      case UTF8_ACCEPT:
	if (cp != r.cp) {
	  printf("FAIL: wrong decode U+%06x != U+%06x\n", cp, r.cp);
	  result = 1;
	}
	break;
      case UTF8_REJECT:
	printf("FAIL: rejected U+%06x\n", cp);
	result = 1;
	break;
      default:
	printf("FAIL: incomplete U+%06x\n", cp);
	result = 1;
	break;
      }
    }
  }

  for (cp = 0; cp <= 0x007fL; cp++) {
    byte buf[4];
    struct utf8_result r;

    buf[0] = 0xc0 | (cp >> 6);
    buf[1] = 0x80 | (cp & 0x3f);
    r = utf8_decode(buf, 2);
    switch (r.type) {
    case UTF8_ACCEPT:
      printf("FAIL: accepted overly-long (2) U+%06x\n", cp);
      result = 1;
      break;
    case UTF8_REJECT:
      break;
    default:
      printf("FAIL: incomplete U+%06x\n", cp);
      result = 1;
      break;
    }

    buf[0] = 0xe0;
    buf[1] = 0x80 | (cp >> 6);
    buf[2] = 0x80 | (cp & 0x3f);
    r = utf8_decode(buf, 2);
    switch (r.type) {
    case UTF8_ACCEPT:
      printf("FAIL: accepted overly-long (3) U+%06x\n", cp);
      result = 1;
      break;
    case UTF8_REJECT:
      break;
    default:
      printf("FAIL: incomplete U+%06x\n", cp);
      result = 1;
      break;
    }

    buf[0] = 0xf0;
    buf[1] = 0x80;
    buf[2] = 0x80 | (cp >> 6);
    buf[3] = 0x80 | (cp & 0x3f);
    r = utf8_decode(buf, 4);
    switch (r.type) {
    case UTF8_ACCEPT:
      printf("FAIL: accepted overly-long (4) U+%06x\n", cp);
      result = 1;
      break;
    case UTF8_REJECT:
      break;
    default:
      printf("FAIL: incomplete U+%06x\n", cp);
      result = 1;
      break;
    }
  }

  for (cp = 0x0080L; cp <= 0x07ffL; cp++) {
    byte buf[4];
    struct utf8_result r;

    buf[0] = 0xe0;
    buf[1] = 0x80 | (cp >> 6);
    buf[2] = 0x80 | (cp & 0x3f);
    r = utf8_decode(buf, 3);
    switch (r.type) {
    case UTF8_ACCEPT:
      printf("FAIL: accepted overly-long (3) U+%06x\n", cp);
      result = 1;
      break;
    case UTF8_REJECT:
      break;
    default:
      printf("FAIL: incomplete U+%06x\n", cp);
      result = 1;
      break;
    }

    buf[0] = 0xf0;
    buf[1] = 0x80;
    buf[2] = 0x80 | (cp >> 6);
    buf[3] = 0x80 | (cp & 0x3f);
    r = utf8_decode(buf, 4);
    switch (r.type) {
    case UTF8_ACCEPT:
      printf("FAIL: accepted overly-long (4) U+%06x\n", cp);
      result = 1;
      break;
    case UTF8_REJECT:
      break;
    default:
      printf("FAIL: incomplete U+%06x\n", cp);
      result = 1;
      break;
    }
  }

  for (cp = 0x0800L; cp <= 0xffffL; cp++) {
    byte buf[4];
    struct utf8_result r;

    buf[0] = 0xf0;
    buf[1] = 0x80 |  (cp >> 12);
    buf[2] = 0x80 | ((cp >>  6) & 0x3f);
    buf[3] = 0x80 |  (cp & 0x3f);
    r = utf8_decode(buf, 4);
    switch (r.type) {
    case UTF8_ACCEPT:
      printf("FAIL: accepted overly-long (4) U+%06x\n", cp);
      result = 1;
      break;
    case UTF8_REJECT:
      break;
    default:
      printf("FAIL: incomplete U+%06x\n", cp);
      result = 1;
      break;
    }
  }

  assert(result == 0);
}

int
main (void)
{
  run_test(decode_encode_test_string);
  run_test(decode_all_codepoints);

  return EXIT_SUCCESS;
}
