#ifndef __HPR_NETDB_H__
#define __HPR_NETDB_H__

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Addr.h"

#define HPR_MAX_ADAPTER_NAME_LEN 256
#define HPR_MAX_ADAPTER_ADDRESS_LEN 8
#define HPR_MAX_DESCRIPTION_LEN 128
#define HPR_MAX_IP_ADDRESS_LEN 16
#define HPR_MAX_NIC_NUM 16
#define HPR_DEF_ADAPTER_ADDRESS_LEN 6

struct HPR_IPADDR_STRING {
    char szIPAddr[HPR_MAX_IP_ADDRESS_LEN];
    char szNetMask[HPR_MAX_IP_ADDRESS_LEN];
};

struct HPR_IPADDRESS {
    HPR_IPADDRESS* pNext;
    HPR_IPADDR_STRING struIpAddrString;
};

struct HPR_ADAPTER_INFO {
    HPR_ADAPTER_INFO* pNext;
    HPR_INT32 iIndex;
    char szAdapterName[HPR_MAX_ADAPTER_NAME_LEN + 4];
    char szDescription[HPR_MAX_DESCRIPTION_LEN + 4];
    HPR_UINT32 nMacAddrLen;
    unsigned char szMacAddr[HPR_MAX_ADAPTER_ADDRESS_LEN];
    HPR_UINT32 nType;
    HPR_UINT32 nDhcpEnabled;
    HPR_IPADDRESS* pIpAddrList;
    HPR_IPADDRESS* pGatawayList;
    HPR_IPADDRESS* pDhcpServerList;
    HPR_IPADDRESS* pIpAddrv6List;
};

struct HPR_ADDRESS_INFO {
    HPR_INT32 iFlags;
    HPR_INT32 iFamily;
    HPR_INT32 iSockType;
    HPR_INT32 iProtocol;
    HPR_ADDR_T Address;
    char* sCanonName;
    HPR_ADDRESS_INFO* pNext;
};

HPR_DECLARE HPR_INT32 CALLBACK HPR_GetHostName(char* pName, HPR_INT32 iNameLen);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetMacAddr(const char* pLocalIP, unsigned char* pMacAddr, HPR_INT32* iMacAddrlen);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetAdapterInfo(HPR_ADAPTER_INFO** pAdaterInfo);
HPR_DECLARE HPR_INT32 CALLBACK HPR_FreeAdapterInfo(HPR_ADAPTER_INFO* pAdaterInfo);
HPR_DECLARE hostent* CALLBACK HPR_GetAddressByName(const char* pName);
HPR_DECLARE HPR_UINT16 CALLBACK HPR_Htons(HPR_UINT16 nHostShort);
HPR_DECLARE HPR_UINT16 CALLBACK HPR_Ntohs(HPR_UINT16 nNetShort);
HPR_DECLARE HPR_UINT32 CALLBACK HPR_Htonl(HPR_UINT32 nHostLong);
HPR_DECLARE HPR_UINT32 CALLBACK HPR_Ntohl(HPR_UINT32 nNetLong);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetAddressInfo(const char *sNodeName, const char *sServerName, HPR_ADDRESS_INFO *Hints, HPR_ADDRESS_INFO **pRes);
HPR_DECLARE HPR_INT32 CALLBACK HPR_FreeAddressInfo(HPR_ADDRESS_INFO *pAddressInfo);

#endif // __HPR_NETDB_H__
