#ifndef __HPR_TYPES_H__
#define __HPR_TYPES_H__

typedef char HPR_INT8;
typedef unsigned char HPR_UINT8;
typedef short HPR_INT16;
typedef unsigned short HPR_UINT16;
typedef int HPR_INT32;
typedef unsigned int HPR_UINT32;
typedef void* HPR_VOIDPTR;
typedef long HPR_LONG;
typedef unsigned long HPR_ULONG;

typedef float HPR_FLOAT;
#define HPR_VOID void

#if defined(_MSC_VER)
    typedef __int64 HPR_INT64;
    typedef unsigned __int64 HPR_UINT64;
    typedef HANDLE HPR_HANDLE;

    #if (_MSC_VER >= 1310)
        #if defined _WIN32_WCE
            #include <crtdefs.h>
        #else
            #include <stddef.h>
        #endif
        typedef uintptr_t HPR_UINT;
        typedef intptr_t HPR_INT;
    #endif

    #ifndef socklen_t
        typedef int socklen_t;
    #endif

    typedef int (CALLBACK *HPR_PROC)();

#elif defined(__GNUC__) || defined(__SYMBIAN32__)
    #if defined(__LP64__)
        typedef long HPR_INT64;
        typedef unsigned long HPR_UINT64;
    #else
        typedef long long HPR_INT64;
        typedef unsigned long long HPR_UINT64;
    #endif
    typedef void* HPR_HANDLE;

    #include <ctype.h>
    #include <stdint.h>
    #ifdef __APPLE__
        #include <cstdint>
    #endif
    typedef uintptr_t HPR_UINT;
    typedef intptr_t HPR_INT;

    typedef void* HPR_PROC;
#endif

#define HPR_SUPPORT_INT64 1

#ifndef HPR_BOOL
    #define HPR_BOOL HPR_INT32
    #define HPR_TRUE 1
    #define HPR_FALSE 0
#endif

#define HPR_STATUS HPR_INT32

#define HPR_INVALID_HANDLE NULL

typedef HPR_UINT8 hpr_uint8;
typedef HPR_INT8 hpr_int8;
typedef HPR_UINT16 hpr_uint16;
typedef HPR_INT16 hpr_int16;
typedef HPR_UINT32 hpr_uint32;
typedef HPR_INT32 hpr_int32;
typedef HPR_UINT64 hpr_uint64;
typedef HPR_INT64 hpr_int64;
typedef HPR_LONG hpr_long;
typedef HPR_ULONG hpr_ulong;

#if defined(_MSC_VER) &&(_MSC_VER >= 1310)
    typedef HPR_INT hpr_int;
    typedef HPR_UINT hpr_uint;
#elif defined(__GNUC__) || defined(__SYMBIAN32__)
    typedef HPR_INT hpr_int;
    typedef HPR_UINT hpr_uint;
#endif

typedef HPR_HANDLE hpr_handle;
#define hpr_void HPR_VOID
typedef HPR_VOIDPTR hpr_voidptr;
typedef HPR_PROC hpr_proc;

#ifdef __cplusplus
    typedef bool hpr_bool;
#endif

#include <stddef.h>
typedef size_t HPR_SIZE;

#endif // __HPR_TYPES_H__
