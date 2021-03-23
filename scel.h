#ifndef H_SCEL__
#define H_SCEL__

#include <stdbool.h>
#include <stdint.h>

#include "utf8.h"

/** scel_t: a bel value, either immediate or a pointer.
 ** It could be nil, a fixnum, a single float, a unicode codepoint or
 ** a pointer to an object allocated on the heap.
 ** Because fixnums are not tagged and nil is different than 0, the
 ** default value is 0 instead of nil. Thus scel_t should always be
 ** declared as follow:
 **
 **   scel_t val = SCEL_NIL;
 */
typedef int64_t scel_t;

/** scel_nil: the nil value. */
extern const scel_t SCEL_NIL;

/** scel_max_fixnum: the max fixnum storable as immediate. */
extern const int64_t SCEL_MAX_FIXNUM;
/** scel_max_fixnum: the min fixnum storable as immediate. */
extern const int64_t SCEL_MIN_FIXNUM;

/** scel_is_immediate: tell if VAL is an immediate value. */
extern bool scel_is_immediate(scel_t val);
/** scel_is_codepoint: tell if VAL is an unicode codepoint. */
extern bool scel_is_codepoint(scel_t val);
/** scel_is_fixnum: tell if VAL is a fixnum. */
extern bool scel_is_fixnum(scel_t val);
/** scel_is_nil: tell if VAL is nil. */
extern bool scel_is_nil(scel_t val);
/** scel_is_single_float: tell if VAL is a single float. */
extern bool scel_is_single_float(scel_t val);

/** scel_make_codepoint: make scel from codepoint. */
extern scel_t scel_make_codepoint(codepoint cp);
/** scel_make_fixnum: make scel from fixnum. */
extern scel_t scel_make_fixnum(int64_t num);
/** scel_make_single_float: make scel from single float. */
extern scel_t scel_make_single_float(float flt);

/** scel_as_codepoint: return VAL as a codepoint. */
extern codepoint scel_as_codepoint(scel_t val);
/** scel_as_fixnum: return VAL as a fixnum. */
extern int64_t scel_as_fixnum(scel_t val);
/** scel_as_single_float: return VAL as a single float. */
extern float scel_as_single_float(scel_t val);

#endif
