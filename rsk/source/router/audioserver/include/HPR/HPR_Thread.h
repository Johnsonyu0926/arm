#ifndef __HPR_THREAD_H__
#define __HPR_THREAD_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#if defined (OS_WINDOWS)
    #define HPR_INVALID_THREAD (HPR_HANDLE)(NULL)
#elif defined (OS_POSIX)
    #define HPR_INVALID_THREAD (HPR_HANDLE)(pthread_t)(-1)
#endif

HPR_DECLARE HPR_HANDLE CALLBACK HPR_Thread_Create(HPR_VOIDPTR (CALLBACK *StartAddress)(HPR_VOIDPTR), HPR_VOID* Params, HPR_UINT32 StackSize,
                        HPR_BOOL IsSuspend = HPR_FALSE, HPR_INT32 Priority = 0, HPR_INT32 SchedPolicy = 0);

HPR_DECLARE HPR_BOOL CALLBACK HPR_ThreadDetached_Create(HPR_VOIDPTR(CALLBACK *StartAddress)(HPR_VOIDPTR),HPR_VOID* Params, HPR_UINT32 StackSize);

HPR_DECLARE HPR_INT32 CALLBACK HPR_Thread_Wait(HPR_HANDLE ThreadHandle);

HPR_DECLARE HPR_INT32 CALLBACK HPR_Thread_Suspend(HPR_HANDLE ThreadHandle);

HPR_DECLARE HPR_INT32 CALLBACK HPR_Thread_Resume(HPR_HANDLE ThreadHandle);

HPR_DECLARE HPR_INT32 CALLBACK HPR_Thread_SetPriority(HPR_HANDLE ThreadHandle, HPR_INT32 Priority);

HPR_DECLARE HPR_INT32 CALLBACK HPR_Thread_SetSchedPolicy(HPR_HANDLE ThreadHandle, HPR_INT32 SchedPolicy);

HPR_DECLARE HPR_INT32 CALLBACK HPR_Thread_Exit();

HPR_DECLARE HPR_HANDLE CALLBACK HPR_Thread_GetSelfId();

#define HPR_INVALID_TLS (HPR_HANDLE)(-1)

HPR_DECLARE HPR_HANDLE CALLBACK HPR_ThreadTls_Create();
HPR_DECLARE HPR_INT32 CALLBACK HPR_ThreadTls_SetValue(HPR_HANDLE hTls, HPR_VOIDPTR pVal);
HPR_DECLARE HPR_VOIDPTR CALLBACK HPR_ThreadTls_GetValue(HPR_HANDLE hTls);
HPR_DECLARE HPR_INT32 CALLBACK HPR_ThreadTls_Destroy(HPR_HANDLE hTls);

class HPR_Thread {
public:
    HPR_Thread(HPR_VOID);
    ~HPR_Thread(HPR_VOID);

    HPR_INT32 Create(HPR_VOIDPTR(CALLBACK *StartAddress)(HPR_VOIDPTR), HPR_VOIDPTR Params, HPR_UINT32 StackSize,
                        HPR_BOOL IsSuspend, HPR_INT32 Priority, HPR_INT32 SchedPolicy);
    HPR_INT32 Wait(HPR_VOID);
    HPR_INT32 Suspend(HPR_VOID);
    HPR_INT32 Resume(HPR_VOID);
    HPR_INT32 SetPriority(HPR_INT32 Priority);
    HPR_INT32 SetSchedPolicy(HPR_INT32 SchedPolicy);

private:
    HPR_HANDLE m_threadhandle;
};

#endif // __HPR_THREAD_H__
