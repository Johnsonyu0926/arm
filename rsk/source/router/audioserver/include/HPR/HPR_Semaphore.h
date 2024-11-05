#ifndef __HPR_SEMAPHORE_H__
#define __HPR_SEMAPHORE_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#if defined(OS_WINDOWS)
    typedef HPR_HANDLE HPR_SEM_T;
#elif defined(OS_POSIX)
    #include <semaphore.h>
    #if defined(__APPLE__)
        typedef sem_t* HPR_SEM_T;
    #else
        typedef sem_t HPR_SEM_T;
    #endif
#else
    #error OS Not Implement Yet.
#endif

HPR_DECLARE HPR_INT32 CALLBACK HPR_SemCreate(HPR_SEM_T* pSem, HPR_UINT32 nInitCount);
HPR_DECLARE HPR_INT32 CALLBACK HPR_SemDestroy(HPR_SEM_T* pSem);
HPR_DECLARE HPR_INT32 CALLBACK HPR_SemWait(HPR_SEM_T* pSem);
HPR_DECLARE HPR_INT32 CALLBACK HPR_SemTimedWait(HPR_SEM_T* pSem, HPR_UINT32 nTimeOut);
HPR_DECLARE HPR_INT32 CALLBACK HPR_SemPost(HPR_SEM_T* pSem);

class HPR_DECLARE_CLASS HPR_Sema {
public:
    HPR_Sema(HPR_UINT32 nInitCount, HPR_UINT32 nMaxCount);
    ~HPR_Sema();
    HPR_INT32 Wait();
    HPR_INT32 TimedWait(HPR_UINT32 nTimeOut);
    HPR_INT32 Post();
private:
    HPR_SEM_T m_sem;
    #ifdef OS_POSIX
        HPR_UINT32 m_nMaxCount;
        HPR_Mutex m_csLock;
        #ifdef OS_APPLE
            HPR_UINT32 m_nCurCount;
        #endif
    #endif
};

namespace hpr {
    #if defined(_WIN32) || defined(_WIN64)
        #include <windows.h>
        typedef HANDLE hpr_sem_t;
    #else
        typedef sem_t hpr_sem_t;
    #endif
    
    class HPR_DECLARE_CLASS hpr_sem {
    public:
        hpr_sem();
        hpr_sem(hpr_int32 count);
        virtual ~hpr_sem();
        hpr_int32 post(hpr_int32);
        hpr_int32 post_1();
        hpr_int32 wait(hpr_int32 timeout = -1);
    protected:
        hpr_int32 init(hpr_int32 count);
        hpr_int32 destroy();
    private:
        hpr_sem_t m_sem;
    };
}

#endif // __HPR_SEMAPHORE_H__
