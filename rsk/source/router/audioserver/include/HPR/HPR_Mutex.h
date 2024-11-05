#ifndef __HPR_MUTEX_H__
#define __HPR_MUTEX_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#if defined(OS_WINDOWS)
    typedef CRITICAL_SECTION HPR_MUTEX_T;
    #define HPR_MUTEX_FAST 0
    #define HPR_MUTEX_RECURSIVE 0
    #define HPR_MUTEX_ERRORCHECK 0
#elif defined(OS_POSIX)
    #include <pthread.h>
    typedef pthread_mutex_t HPR_MUTEX_T;
    #if defined(OS_APPLE) || defined(OS_S60) || defined(OS_ANDROID)
        #define HPR_MUTEX_FAST PTHREAD_MUTEX_NORMAL
        #define HPR_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE
        #define HPR_MUTEX_ERRORCHECK PTHREAD_MUTEX_ERRORCHECK
    #elif defined(__linux__)
        #define HPR_MUTEX_FAST PTHREAD_MUTEX_FAST_NP
        #define HPR_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE_NP
        #define HPR_MUTEX_ERRORCHECK PTHREAD_MUTEX_ERRORCHECK_NP
    #endif
#else
    #define HPR_MUTEX_FAST PTHREAD_MUTEX_FAST_NP
    #define HPR_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE_NP
    #define HPR_MUTEX_ERRORCHECK PTHREAD_MUTEX_ERRORCHECK_NP
#endif

HPR_DECLARE HPR_INT32 CALLBACK HPR_MutexCreate(HPR_MUTEX_T* pMutex, HPR_INT32 iFlag = -1);
HPR_DECLARE HPR_INT32 CALLBACK HPR_MutexDestroy(HPR_MUTEX_T* pMutex);
HPR_DECLARE HPR_INT32 CALLBACK HPR_MutexLock(HPR_MUTEX_T* pMutex);
HPR_DECLARE HPR_INT32 CALLBACK HPR_MutexTryLock(HPR_MUTEX_T* pMutex);
HPR_DECLARE HPR_INT32 CALLBACK HPR_MutexUnlock(HPR_MUTEX_T* pMutex);

class HPR_DECLARE_CLASS HPR_Mutex {
public:
    HPR_Mutex();
    HPR_Mutex(HPR_UINT32 nFlag);
    ~HPR_Mutex();
    HPR_INT32 Lock();
    HPR_INT32 Unlock();
    HPR_INT32 TryLock();
private:
    HPR_MUTEX_T m_mutex;
};

typedef HPR_MUTEX_T hpr_mutex_t;

namespace hpr {
    class HPR_DECLARE_CLASS hpr_mutex {
    public:
        hpr_mutex();
        hpr_mutex(hpr_int32 attr);
        ~hpr_mutex();
        hpr_int32 lock();
        hpr_int32 trylock();
        hpr_int32 unlock();
    private:
        hpr_mutex_t m_mutex;
    };

    class HPR_DECLARE_CLASS hpr_recursive_mutex {
    public:
        hpr_recursive_mutex();
        ~hpr_recursive_mutex();
        hpr_int32 lock();
        hpr_int32 trylock();
        hpr_int32 unlock();
    private:
        hpr_mutex_t m_mutex;
    };
}

#endif // __HPR_MUTEX_H__
