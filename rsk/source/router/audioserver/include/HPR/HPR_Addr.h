#ifndef __HPR_ADDR_H__
#define __HPR_ADDR_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#if defined(OS_WINDOWS)
    typedef SOCKET HPR_SOCK_T;
    #if (_MSC_VER < 1310)
        #ifndef s6_addr
            struct in6_addr {
                union {
                    u_char Byte[16];
                    u_short Word[8];
                } u;
            };
            #define in_addr6 in6_addr
            #define _S6_un u
            #define _S6_u8 Byte
            #define s6_addr _S6_un._S6_u8
            #define s6_bytes u.Byte
            #define s6_words u.Word
        #endif
        typedef struct {
            short sin6_family;
            u_short sin6_port;
            u_long sin6_flowinfo;
            struct in6_addr sin6_addr;
            u_long sin6_scope_id;
        } sockaddr_in6_hpr;
    #else
        typedef sockaddr_in6 sockaddr_in6_hpr;
    #endif
#elif defined(OS_POSIX)
    typedef int HPR_SOCK_T;
    typedef sockaddr_in6 sockaddr_in6_hpr;
#endif

typedef struct HPR_ADDR_T {
    union {
        sockaddr_in sin4;
#if defined(OS_SUPPORT_IPV6)
        sockaddr_in6_hpr sin6;
#endif
    } SA;
} HPR_ADDR_T;

typedef struct HPR_ADDR_EXP_T {
    HPR_INT32 iAf;
    union {
        HPR_UINT32 nAddr4;
        HPR_UINT8 nAddr6[16];
    } ADDR;
    HPR_UINT16 nPort;
    HPR_UINT16 nReserved;
} HPR_ADDR_EXP_T;

#define HPR_AF_INET AF_INET
#if defined(OS_SUPPORT_IPV6)
#define HPR_AF_INET6 AF_INET6
#endif

HPR_DECLARE HPR_INT32 CALLBACK HPR_MakeAddrByString(HPR_INT32 iAf, const char* pAddr, HPR_UINT16 nPort, HPR_ADDR_T* pHprAddr);
HPR_DECLARE HPR_INT32 CALLBACK HPR_MakeAddr4ByInt(HPR_UINT32 nAddr, HPR_UINT16 nPort, HPR_ADDR_T* pHprAddr);

#if defined(OS_SUPPORT_IPV6)
HPR_DECLARE HPR_INT32 CALLBACK HPR_MakeAddr6ByInt(HPR_UINT8 nAddr[16], HPR_UINT16 nPort, HPR_ADDR_T* pHprAddr);
#endif

HPR_DECLARE HPR_INT32 CALLBACK HPR_SetAddrPort(HPR_ADDR_T* pHprAddr, HPR_UINT16 nPort);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetAddrType(HPR_ADDR_T* pHprAddr);
HPR_DECLARE HPR_INT32 CALLBACK HPR_MakeAddr(HPR_VOIDPTR pSockAddr, HPR_INT32 iSockAddrLen, HPR_ADDR_T* pHprAddr);
HPR_DECLARE const char* CALLBACK HPR_GetAddrString(HPR_ADDR_T* pHprAddr);
HPR_DECLARE const char* CALLBACK HPR_GetAddrStringEx(HPR_ADDR_T* pHprAddr, char* pAddrBuf, size_t nBuflen);
HPR_DECLARE HPR_UINT16 CALLBACK HPR_GetAddrPort(HPR_ADDR_T* pHprAddr);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetAddr4Int(HPR_ADDR_T* pHprAddr, HPR_UINT32* nAddr);

#if defined(OS_SUPPORT_IPV6)
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetAddr6Int(HPR_ADDR_T* pHprAddr, HPR_UINT8 nAddr[16]);
#endif

HPR_DECLARE HPR_INT32 CALLBACK HPR_GetAddrInfo(HPR_ADDR_T* pHprAddr, HPR_ADDR_EXP_T* pHprExp);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetAddrBySockFd(HPR_SOCK_T iSockFd, HPR_ADDR_T* pHprLocal, HPR_ADDR_T* pRemote);
HPR_DECLARE HPR_INT32 CALLBACK HPR_InetPton(const char* pSrc, HPR_VOIDPTR pDst);
HPR_DECLARE const char* CALLBACK HPR_InetNtop(HPR_INT32 iAf, const unsigned char* pSrc, char* pDst, int iCnt);

#endif // __HPR_ADDR_H__
