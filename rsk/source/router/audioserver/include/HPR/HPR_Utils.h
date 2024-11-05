#ifndef __HPR_UTILS_H__
#define __HPR_UTILS_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#if defined LLONG_MAX
    #define HPR_INT64_MAX LLONG_MAX
    #define HPR_INT64_MIN LLONG_MIN
#else
    #define HPR_INT64_MAX 0x7fffffffffffffff
    #define HPR_INT64_MIN (0x7fffffffffffffff - 1)
#endif

#define HPR_INT32_MAX INT_MAX
#define HPR_INT32_MIN INT_MIN

#if defined(OS_WINDOWS)
    #define HPR_Vsnprintf _vsnprintf
    #define HPR_Snprintf _snprintf
#elif defined(OS_POSIX)
    #define HPR_Vsnprintf vsnprintf
    #define HPR_Snprintf snprintf
#else
    #error OS Not Implement Yet.
#endif

#if defined (HPR_SUPPORT_NOTHROW)
#define HPR_NEW_RETURN(POINTER,CONSTRUCTURE) \
    do { POINTER = new (::std::nothrow) CONSTRUCTURE;\
    if (POINTER == 0) {return NULL;}\
    return POINTER;\
    } while(0)
#else
#define HPR_NEW_RETURN(POINTER,CONSTRUCTURE) \
    do { try {POINTER = new CONSTRUCTURE;\
            return POINTER;}\
    catch (::std::bad_alloc & a){POINTER=0; return NULL;}\
        } while(0)
#endif


struct HPR_CPU_PERFORMANCE {
    HPR_UINT64 u64IdleTime;
    HPR_UINT64 u64KernelTime;
    HPR_UINT64 u64UserTime;
    HPR_UINT64 u64Reserve1[2];
    HPR_UINT64 u64Reserve2[2];
};

struct HPR_MEMORY_STATUS {
    HPR_UINT64 u64TotalPhys;
    HPR_UINT64 u64AvailPhys;
    HPR_UINT64 u64TotalPageFile;
    HPR_UINT64 u64AvailPageFile;
    HPR_UINT64 u64Reserve[2];
};

struct HPR_NETWORK_FLOWDATA {
    char szAdapterName[260];
    HPR_INT32 iIndex;
    HPR_UINT32 nSpeed;
    HPR_UINT32 nInBytes;
    HPR_UINT32 nInPackets;
    HPR_UINT32 nOutBytes;
    HPR_UINT32 nOutPackets;
    HPR_UINT32 nReserve[4];
};

HPR_DECLARE HPR_VOID CALLBACK HPR_OutputDebug(const char* format, ...);

#if defined _DEBUG
    #define HPR_Debug HPR_OutputDebug
#else
    #define HPR_Debug 
#endif

HPR_DECLARE HPR_VOID CALLBACK HPR_OutputDebugString(const char* lpOutputString);
HPR_DECLARE HPR_VOID CALLBACK HPR_Sleep(HPR_INT32 millisecond);
HPR_DECLARE HPR_VOID CALLBACK HPR_USleep(HPR_INT64 nUSec);
HPR_DECLARE HPR_UINT32 CALLBACK HPR_Rand();
HPR_DECLARE HPR_VOID CALLBACK HPR_ZeroMemory(void* mem, int len);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetCPUNumber();
typedef unsigned char HPR_UUID[16];
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetUUID(HPR_UUID uuid);
HPR_DECLARE HPR_INT32 CALLBACK HPR_StringFromUUID(HPR_UUID uuid, char* uuidString, HPR_INT32 len);

#if defined (CXXCOMPILE)
#include <string>
HPR_DECLARE_CXX std::string CALLBACK HPR_UTF82A(const char* cont);
HPR_DECLARE_CXX std::string CALLBACK HPR_A2UTF8(const char* cont);
#endif

HPR_DECLARE HPR_INT32 CALLBACK HPR_GetCPUPerformance(HPR_CPU_PERFORMANCE *performance, HPR_UINT32 *size);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetMemoryStatus(HPR_MEMORY_STATUS *memory_status);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetNetWorkFlowData(HPR_NETWORK_FLOWDATA *flowdata);

#endif // __HPR_UTILS_H__
