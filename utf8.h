#ifndef H_UTF8__
#define H_UTF8__

#include <stdint.h>
#include <stddef.h>

/** codepoint: a unicode codepoint. */
typedef uint32_t codepoint;

/** byte: a 8-bits byte used in utf-8 streams. */
typedef uint8_t byte;

/** utf8_result_type: either a valid or an invalid codepoint seen. */
enum utf8_result_type { UTF8_ACCEPT = 0, UTF8_REJECT = -1 };

/** utf8_result: the result of a decode opration,
 ** When type is UTF8_ACCEPT, the decoded codepoint is returned along
 ** the consumed length of the input string.
 ** When type is UTF8_REJECT, the decoded codepoint is set to 0 and
 ** the length of the rejected string is returned. */
struct utf8_result
{
  enum utf8_result_type type;
  codepoint cp;
  size_t len;
};

/** utf8_decode: decode a single codepoint from the stream. */
struct utf8_result utf8_decode(const byte *stream, size_t maxlen);
/** utf8_encode: encode a single codepoint C in STREAM.
 ** The validity of C is not checked.
 ** Return NULL when the resulting encoding is longer than MAXLEN. */
byte* utf8_encode(byte *stream, size_t maxlen, codepoint c);

#endif
