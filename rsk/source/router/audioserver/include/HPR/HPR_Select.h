#ifndef __HPR_SELECT_H__
#define __HPR_SELECT_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#if defined(OS_POSIX)
    #if defined(OS_S60)
        #include <select.h>
    #else
        #include <poll.h>
    #endif
#endif

HPR_DECLARE HPR_INT32 CALLBACK HPR_FdIsSet(HPR_INT32 fd, fd_set *set);
HPR_DECLARE HPR_INT32 CALLBACK HPR_Select(HPR_INT32 iMaxFd, fd_set* fdset_r, fd_set* fdset_w, fd_set* fdset_e, timeval* tv);
HPR_DECLARE HPR_INT32 CALLBACK HPR_SelectEx(HPR_INT32 iMaxFd, fd_set* fdset_r, fd_set* fdset_w, fd_set* fdset_e, timeval* tv);
HPR_DECLARE HPR_INT32 CALLBACK HPR_Poll(struct pollfd* fds, HPR_INT32 iFds, HPR_INT32 iTimeO);
HPR_DECLARE HPR_INT32 CALLBACK HPR_PollEx(struct pollfd* fds, HPR_INT32 iFds, HPR_INT32* iTimeO);

#endif // __HPR_SELECT_H__
