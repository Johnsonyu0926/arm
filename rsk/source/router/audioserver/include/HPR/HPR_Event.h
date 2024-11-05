#ifndef __HPR_EVENT_H__
#define __HPR_EVENT_H__

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Addr.h"

#define HPR_FD_BASE      1
#define HPR_FD_READ      (HPR_FD_BASE << 0)
#define HPR_FD_WRITE     (HPR_FD_BASE << 1)
#define HPR_FD_OOB       (HPR_FD_BASE << 2)
#define HPR_FD_ACCEPT    (HPR_FD_BASE << 3)
#define HPR_FD_CONNECT   (HPR_FD_BASE << 4)
#define HPR_FD_CLOSE     (HPR_FD_BASE << 5)
#define HPR_FD_QOS       (HPR_FD_BASE << 6)
#define HPR_GROUP_QOS    (HPR_FD_BASE << 7)

#define HPR_FD_MAX_EVENT 8

#if defined(OS_WINDOWS)
    #define HPR_WAIT_OBJECT_0 WAIT_OBJECT_0
    #define HPR_WAIT_TIMEOUT WAIT_TIMEOUT
    #define HPR_WAIT_INVALID_PARAM ((HPR_UINT32)0x7ffffffe)
    #define HPR_WAIT_INVALID_EVENT ((HPR_UINT32)0x7ffffffd)
    #define HPR_WAIT_FAILED ((HPR_UINT32)0x7ffffffc)
    typedef WSANETWORKEVENTS HPR_NETWORKEVENTS_T;
#elif defined(OS_POSIX)
    #define HPR_WAIT_OBJECT_0 ((HPR_UINT32)0x0)
    #define HPR_WAIT_TIMEOUT ((HPR_UINT32)0x7fffffff)
    #define HPR_WAIT_INVALID_PARAM ((HPR_UINT32)0x7ffffffe)
    #define HPR_WAIT_INVALID_EVENT ((HPR_UINT32)0x7ffffffd)
    #define HPR_WAIT_FAILED ((HPR_UINT32)0x7ffffffc)
    typedef struct HPR_NETWORKEVENTS_S {
        HPR_UINT32 lNetworkEvents;
        HPR_INT32 iErrorCode[HPR_FD_MAX_EVENT];
    } HPR_NETWORKEVENTS_T;
#endif

HPR_DECLARE HPR_HANDLE CALLBACK HPR_CreateEvent(HPR_BOOL bInitState);
HPR_DECLARE HPR_BOOL CALLBACK HPR_CloseEvent(HPR_HANDLE hEvent);
HPR_DECLARE HPR_UINT32 CALLBACK HPR_WaitForSingleObject(HPR_HANDLE hEvent, HPR_UINT32 nTimeOut);
HPR_DECLARE HPR_UINT32 CALLBACK HPR_WaitForMultipleObjects(HPR_UINT32 nEventCount, HPR_HANDLE* hEvents, HPR_BOOL bWaitAll, HPR_UINT32 nTimeOut);
HPR_DECLARE HPR_BOOL CALLBACK HPR_ResetEvent(HPR_HANDLE hEvent);
HPR_DECLARE HPR_BOOL CALLBACK HPR_SetEvent(HPR_HANDLE hEvent);
HPR_DECLARE HPR_INT32 CALLBACK HPR_EventSelect(HPR_SOCK_T nSockFd, HPR_HANDLE hEvent, HPR_UINT32 nFlag);
HPR_DECLARE HPR_INT32 CALLBACK HPR_EnumNetworkEvents(HPR_SOCK_T nSockFd, HPR_HANDLE hEvent, HPR_NETWORKEVENTS_T* pNetworkEvents);

#endif // __HPR_EVENT_H__
