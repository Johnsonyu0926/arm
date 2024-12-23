#ifndef __HPR_SOCKET_H__
#define __HPR_SOCKET_H__

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Addr.h"

#if defined(OS_WINDOWS)
    #define HPR_SOCK_ERROR SOCKET_ERROR
    #define HPR_INVALID_SOCKET INVALID_SOCKET
    #define HPR_SHUT_RD SD_RECEIVE
    #define HPR_SHUT_WR SD_SEND
    #define HPR_SHUT_RDWR SD_BOTH
    static HPR_BOOL g_bWSAStartup = HPR_FALSE;
#elif defined(OS_POSIX)
    #define HPR_SOCK_ERROR (-1)
    #define HPR_INVALID_SOCKET (-1)
    #define HPR_SHUT_RD SHUT_RD
    #define HPR_SHUT_WR SHUT_WR
    #define HPR_SHUT_RDWR SHUT_RDWR
#else
    #error OS Not Implement Yet.
#endif

HPR_DECLARE HPR_SOCK_T CALLBACK HPR_CreateSocket(HPR_INT32 iAf, HPR_INT32 iType, HPR_INT32 iProto);
HPR_DECLARE HPR_INT32 CALLBACK HPR_CloseSocket(HPR_SOCK_T iSockFd, HPR_BOOL bForce = HPR_FALSE);
HPR_DECLARE HPR_INT32 CALLBACK HPR_ShutDown(HPR_SOCK_T iSockFd, HPR_INT32 iHow);
HPR_DECLARE HPR_INT32 CALLBACK HPR_Bind(HPR_SOCK_T iSockFd, HPR_ADDR_T* pHprAddr);
HPR_DECLARE HPR_INT32 CALLBACK HPR_Listen(HPR_SOCK_T iSockFd, HPR_INT32 iBackLog);
HPR_DECLARE HPR_SOCK_T CALLBACK HPR_Accept(HPR_SOCK_T iSockFd, HPR_ADDR_T* pHprAddr = NULL, HPR_UINT32 nTimeOut = HPR_INFINITE);
HPR_DECLARE HPR_INT32 CALLBACK HPR_ConnectWithTimeOut(HPR_SOCK_T iSockFd, HPR_ADDR_T* pHprAddr, HPR_UINT32 nTimeOut = HPR_INFINITE);
HPR_DECLARE HPR_INT32 CALLBACK HPR_BroadCast(HPR_SOCK_T iSockFd);
HPR_DECLARE HPR_INT32 CALLBACK HPR_JoinMultiCastGroup(HPR_SOCK_T iSockFd, HPR_ADDR_T* pLocal, HPR_ADDR_T* pMcastGroupAddr);
HPR_DECLARE HPR_INT32 CALLBACK HPR_LeaveMultiCastGroup(HPR_SOCK_T iSockFd, HPR_ADDR_T* pLocal, HPR_ADDR_T* pMcastGroupAddr);
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetNonBlock(HPR_SOCK_T iSockFd, HPR_BOOL bYesNo);
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetTTL(HPR_SOCK_T iSockFd, HPR_INT32 iTTL);
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetMultiCastTTL(HPR_SOCK_T iSockFd, HPR_INT32 iTTL);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetTTL(HPR_SOCK_T iSockFd, HPR_INT32* iTTL);
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetTOS(HPR_SOCK_T iSockFd, HPR_INT32 iTOS);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetTOS(HPR_SOCK_T iSockFd, HPR_INT32* iTOS);
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetReuseAddr(HPR_SOCK_T iSockFd, HPR_BOOL bYesNo);
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetTimeOut(HPR_SOCK_T iSockFd, HPR_INT32 iSndTimeO, HPR_INT32 iRcvTimeO);
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetBuffSize(HPR_SOCK_T iSockFd, HPR_INT32 iSndBuffSize, HPR_INT32 iRcvBuffSize);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetBuffSize(HPR_SOCK_T iSockFd, HPR_INT32* iSndBuffSize, HPR_INT32* iRcvBuffSize);
HPR_DECLARE HPR_INT32 CALLBACK HPR_LingerOff(HPR_SOCK_T iSockFd);
HPR_DECLARE HPR_INT32 CALLBACK HPR_LingerOn(HPR_SOCK_T iSockFd, HPR_UINT16 nTimeOutSec);
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetNoDelay(HPR_SOCK_T iSockFd, HPR_BOOL bYesNo);
HPR_DECLARE HPR_INT32 CALLBACK HPR_Send(HPR_SOCK_T iSockFd, HPR_VOIDPTR pBuf, HPR_INT32 iBufLen);
HPR_DECLARE HPR_INT32 CALLBACK HPR_Sendn(HPR_SOCK_T iSockFd, HPR_VOIDPTR pBuf, HPR_INT32 iBufLen, HPR_UINT32 nTimeOut = HPR_INFINITE);
HPR_DECLARE HPR_INT32 CALLBACK HPR_Recv(HPR_SOCK_T iSockFd, HPR_VOIDPTR pBuf, HPR_INT32 iBufCount);
HPR_DECLARE HPR_INT32 CALLBACK HPR_RecvWithTimeOut(HPR_SOCK_T iSockFd, HPR_VOIDPTR pBuf, HPR_INT32 iBufCount, HPR_UINT32 nTimeOut);
HPR_DECLARE HPR_INT32 CALLBACK HPR_Recvn(HPR_SOCK_T iSockFd, HPR_VOIDPTR pBuf, HPR_INT32 iBufCount, HPR_UINT32 nTimeOut = HPR_INFINITE);
HPR_DECLARE HPR_INT32 CALLBACK HPR_SendTo(HPR_SOCK_T iSockFd, HPR_VOIDPTR pBuf, HPR_INT32 iBufLen, HPR_ADDR_T* pHprAddr);
HPR_DECLARE HPR_INT32 CALLBACK HPR_RecvFrom(HPR_SOCK_T iSockFd, HPR_VOIDPTR pBuf, HPR_INT32 iBufCount, HPR_ADDR_T* pHprAddr);
HPR_DECLARE HPR_INT32 CALLBACK HPR_Ioctl(HPR_SOCK_T iSockFd, HPR_INT32 iCmd, HPR_UINT32 *nData);

#endif // __HPR_SOCKET_H__
