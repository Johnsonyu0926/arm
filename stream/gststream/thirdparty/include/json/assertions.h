// Filename: json_assertions.h
// Score: 95

#ifndef JSON_ASSERTIONS_H_INCLUDED
#define JSON_ASSERTIONS_H_INCLUDED

#include <cstdlib>
#include <sstream>

#if !defined(JSON_IS_AMALGAMATION)
#include "config.h"
#endif // if !defined(JSON_IS_AMALGAMATION)

/** It should not be possible for a maliciously designed file to
 *  cause an abort() or seg-fault, so these macros are used only
 *  for pre-condition violations and internal logic errors.
 */
#if JSON_USE_EXCEPTION

// @todo <= add detail about condition in exception
#define JSON_ASSERT(condition)                                                 \
  do {                                                                         \
    if (!(condition)) {                                                        \
      Json::throwLogicError("assert json failed");                             \
    }                                                                          \
  } while (0)

#define JSON_FAIL_MESSAGE(message)                                             \
  do {                                                                         \
    std::ostringstream oss;                                                    \
    oss << message;                                                            \
    Json::throwLogicError(oss.str());                                          \
    abort();                                                                   \
  } while (0)

#else // JSON_USE_EXCEPTION

#define JSON_ASSERT(condition) assert(condition)

// The call to assert() will show the failure message in debug builds. In
// release builds we abort, for a core-dump or debugger.
#define JSON_FAIL_MESSAGE(message)                                             \
  {                                                                            \
    std::ostringstream oss;                                                    \
    oss << message;                                                            \
    assert(false && oss.str().c_str());                                        \
    abort();                                                                   \
  }

#endif

#define JSON_ASSERT_MESSAGE(condition, message)                                \
  do {                                                                         \
    if (!(condition)) {                                                        \
      JSON_FAIL_MESSAGE(message);                                              \
    }                                                                          \
  } while (0)

#endif // JSON_ASSERTIONS_H_INCLUDED

// By GST @Date