#ifndef _HIK_DATATYPE_DEFINITION_
#define _HIK_DATATYPE_DEFINITION_

#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
    #include "stdint.h"
    typedef uint64_t U64;
    typedef int64_t S64;
    typedef uint32_t U32;
    typedef int32_t S32;
#elif defined(_WIN32) || defined(WIN32)
    typedef unsigned __int64 U64;
    typedef signed __int64 S64;
    typedef unsigned int U32;
    typedef signed int S32;
#elif defined(_WIN64) || defined(WIN64)
    typedef unsigned long U64;
    typedef signed long S64;
    typedef unsigned int U32;
    typedef signed int S32;
#else
    typedef unsigned long long U64;
    typedef signed long long S64;
    typedef unsigned int U32;
    typedef signed int S32;
#endif

typedef unsigned short U16;
typedef signed short S16;
typedef unsigned char U08;
typedef signed char S08;

#endif // _HIK_DATATYPE_DEFINITION_ok
