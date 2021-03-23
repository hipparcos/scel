#include "scel.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Internal representation of a scel_t:
 *  x payload bits
 *
 * Immediate values:
 *  xx....xx|xxxxxx00 fixnum (62 bits payload)
 *  00....00|00001111 nil
 *  00....xx|00010111 single-float (32 bits payload)
 *  00....xx|00011111 codepoint (21 bits payload)
 */

/* Constants about scel_t. */
const size_t   SCEL_SIZE = sizeof(scel_t);
const unsigned SCEL_BYTE = 8; // bits per byte.
const unsigned SCEL_SIZE_IN_BITS = SCEL_SIZE * SCEL_BYTE;

/* Constants about fixnum.
 * Fixnum have their 2 least significant bits set to 0. This allows
 * arithmetic operations to be carried on directly on them without
 * the need for untagging the operands then tagging the result.
 * On a x86_64 machine, fixnums are 62 bits signed integers.
 * They range from -4e18 to +4e18. */
const unsigned FIXNUM_TAG_MASK  = 0x3; // The 2 lowest bits...
const unsigned FIXNUM_TAG       = 0x0; // ...are zeros (so fixnum operatios.
const unsigned FIXNUM_BIT_SHIFT = 2;
const unsigned FIXNUM_BITS      = SCEL_SIZE_IN_BITS - FIXNUM_BIT_SHIFT;
const int64_t  FIXNUM_MAX       =  (1LL << (FIXNUM_BITS-1))-1; // 2^n-1
const int64_t  FIXNUM_MIN       = -(1LL << (FIXNUM_BITS-1));   // 2^n

const int64_t SCEL_MAX_FIXNUM = FIXNUM_MAX;
const int64_t SCEL_MIN_FIXNUM = FIXNUM_MIN;

/* Constants about other immediate values.
 * Those are tagged so needs to be shifted to the right to be used and
 * Shifted to the left to be encoded.
 * The least significant byte hold the tag, all other bytes are
 * dedicated to the payload. */
const unsigned IMMEDIATE_TAG_MASK  = 0x7; // The 3 lowest bits...
const unsigned IMMEDIATE_TAG       = 0x7; // ...are ones.
const unsigned IMMEDIATE_BIT_SHIFT = 1 * SCEL_BYTE;
const unsigned IMMEDIATE_TYPE_MASK = 0xff | IMMEDIATE_TAG_MASK;

/* The nil type has only a single value.
 * All representation are equivalent. */
const unsigned NIL_TAG = 0x7 | IMMEDIATE_TAG; // 15 = b001111

/* NIL is not defined as 0 because 0 is a fixnum. */
/* NIL and fixnum are disjoint. */
const scel_t SCEL_NIL = (scel_t) NIL_TAG;

/* As double float can't fit a scel_t without a slight reduction in
 * precision I choose to be expicit about the loss in precision by
 * making single float immediate. */
const unsigned SINGLE_FLOAT_TAG  = 0xf | IMMEDIATE_TAG; // 15 = b011111
const unsigned SINGLE_FLOAT_BITS = 32;

/* A unicode codepoint. */
const unsigned CODEPOINT_TAG  = 0x17 | IMMEDIATE_TAG; // 23 = b101111
const unsigned CODEPOINT_BITS = 21; // Max unicode codepoint length.

bool
scel_is_immediate(scel_t val)
{
  return IMMEDIATE_TAG == (val & IMMEDIATE_TAG_MASK) || scel_is_fixnum(val);
}

bool
scel_is_nil(scel_t val)
{
  return NIL_TAG == (val & IMMEDIATE_TYPE_MASK);
}

scel_t
scel_make_fixnum(int64_t num)
{
  assert(num < FIXNUM_MAX);
  assert(num > FIXNUM_MIN);
  return (scel_t) num << FIXNUM_BIT_SHIFT;
}

int64_t
scel_as_fixnum(scel_t val)
{
  return val >> FIXNUM_BIT_SHIFT;
}

bool
scel_is_fixnum(scel_t val)
{
  return FIXNUM_TAG == (val & FIXNUM_TAG_MASK);
}

union scel_single_float
{
  scel_t as_scel_t;
  float as_float;
};

scel_t
scel_make_single_float(float flt)
{
  union scel_single_float f = {.as_float = flt};
  return (f.as_scel_t << IMMEDIATE_BIT_SHIFT) | SINGLE_FLOAT_TAG;
}

float
scel_as_single_float(scel_t val)
{
  scel_t s = val >> IMMEDIATE_BIT_SHIFT;
  union scel_single_float f = {.as_scel_t = s};
  return f.as_float;
}

bool
scel_is_single_float(scel_t val)
{
  return SINGLE_FLOAT_TAG == (val & IMMEDIATE_TYPE_MASK);
}

typedef uint32_t codepoint;

scel_t
scel_make_codepoint(codepoint cp)
{
  return (cp << IMMEDIATE_BIT_SHIFT) | CODEPOINT_TAG;
}

codepoint
scel_as_codepoint(scel_t val)
{
  return (codepoint) val >> IMMEDIATE_BIT_SHIFT;
}

bool
scel_is_codepoint(scel_t val)
{
  return CODEPOINT_TAG == (val & IMMEDIATE_TYPE_MASK);
}
