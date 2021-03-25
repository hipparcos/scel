#ifndef H_RESULT__
#define H_RESULT__

#include "scel.h"

/** scel_result_type: the type of an allocation result. */
enum scel_result_type
  {
   SCEL_OK = 0,
   SCEL_ERROR = -1,
  };

/** scel_error: allocation error code. */
enum scel_error
  {
   SCEL_NO_ERROR = 0,
   SCEL_INVALID_HEAP = 1,
   SCEL_HEAP_EXHAUSTED,
  };

/** scel_result: the result of an allocation operation. */
struct scel_result
{
  enum scel_result_type typ;
  union
  {
    scel_t ok;
    enum scel_error err;
  };
};

#endif
