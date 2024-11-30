#ifndef DS_NETFILTER_H
#define DS_NETFILTER_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define MAX_PORT_COUNT 256
#define MAX_ADDR_COUNT 256
#define ADDR_LEN 32

typedef struct {
    uint32_t dwFromTime;
    uint32_t dwToTime;
    uint32_t dwInterval;
    bool iLiveTime;
    time_t dwRealToTime;

    int iIncludePort[MAX_PORT_COUNT];
    int iExcludePort[MAX_PORT_COUNT];
    char szIncludeAddrToAny[MAX_ADDR_COUNT][ADDR_LEN];
    char szExcludeAddrToAny[MAX_ADDR_COUNT][ADDR_LEN];
    char szIncludeAddrToAddr[MAX_ADDR_COUNT][ADDR_LEN];
    char szExcludeAddrToAddr[MAX_ADDR_COUNT][ADDR_LEN];

    int iInPortCount;
    int iExPortCount;
    int iInAddrToAnyCount;
    int iExAddrToAnyCount;
    int iInAddrToAddrCount;
    int iExAddrToAddrCount;

    int nLogTime;
    void* pOracle;
    int nCacheCount;
    char FilePath[256];
    uint16_t wHttpPort;
    char strSrcIP[ADDR_LEN];
    char strDstIP[ADDR_LEN];
} CDsNetworkFilter;

void cds_network_filter_init(CDsNetworkFilter* filter);
void cds_network_filter_set_time_range(CDsNetworkFilter* filter, uint32_t dwFromTime, uint32_t dwToTime);
bool cds_network_filter_check_address(const CDsNetworkFilter* filter, const char* ip_address1, const char* ip_address2);
bool cds_network_filter_check_port(const CDsNetworkFilter* filter, int port);
void cds_network_filter_exclude_address(CDsNetworkFilter* filter, const char* ip_address1, const char* ip_address2);
void cds_network_filter_exclude_address_single(CDsNetworkFilter* filter, const char* ip_address);
void cds_network_filter_include_address(CDsNetworkFilter* filter, const char* ip_address1, const char* ip_address2);
void cds_network_filter_include_address_single(CDsNetworkFilter* filter, const char* ip_address);
void cds_network_filter_exclude_port(CDsNetworkFilter* filter, int port);
void cds_network_filter_include_port(CDsNetworkFilter* filter, int port);
const char* cds_network_filter_get_address(const CDsNetworkFilter* filter);
void cds_network_filter_set_src_to_dst(CDsNetworkFilter* filter, const char* pSrcIP, const char* pDstIP);
bool cds_network_filter_check_src_and_dst_addr(const CDsNetworkFilter* filter, const char* lpSrcIPAddr, const char* lpDstIPAddr);

#endif // DS_NETFILTER_H