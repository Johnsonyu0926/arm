#include "netfilter.h"
#include <string.h>
#include <stdio.h>

// 初始化CDsNetworkFilter对象
void cds_network_filter_init(CDsNetworkFilter* filter) {
    filter->dwFromTime = 0;
    filter->dwToTime = 0;
    filter->dwInterval = 0;
    filter->iLiveTime = false;
    filter->dwRealToTime = 0;
    filter->nLogTime = 0;
    filter->pOracle = NULL;
    filter->nCacheCount = 0;
    filter->wHttpPort = 0;
    memset(filter->strSrcIP, 0, sizeof(filter->strSrcIP));
    memset(filter->strDstIP, 0, sizeof(filter->strDstIP));
    memset(filter->FilePath, 0, sizeof(filter->FilePath));

    filter->iInPortCount = 0;
    filter->iExPortCount = 0;
    filter->iInAddrToAnyCount = 0;
    filter->iExAddrToAnyCount = 0;
    filter->iInAddrToAddrCount = 0;
    filter->iExAddrToAddrCount = 0;

    memset(filter->iIncludePort, 0, sizeof(filter->iIncludePort));
    memset(filter->iExcludePort, 0, sizeof(filter->iExcludePort));
    memset(filter->szIncludeAddrToAny, 0, sizeof(filter->szIncludeAddrToAny));
    memset(filter->szExcludeAddrToAny, 0, sizeof(filter->szExcludeAddrToAny));
    memset(filter->szIncludeAddrToAddr, 0, sizeof(filter->szIncludeAddrToAddr));
    memset(filter->szExcludeAddrToAddr, 0, sizeof(filter->szExcludeAddrToAddr));
}

// 设置时间范围
void cds_network_filter_set_time_range(CDsNetworkFilter* filter, uint32_t dwFromTime, uint32_t dwToTime) {
    filter->dwFromTime = dwFromTime;
    filter->dwToTime = dwToTime;
}

// 检查地址
bool cds_network_filter_check_address(const CDsNetworkFilter* filter, const char* ip_address1, const char* ip_address2) {
    // 实现地址检查逻辑
    return true; // 占位符
}

// 检查端口
bool cds_network_filter_check_port(const CDsNetworkFilter* filter, int port) {
    // 实现端口检查逻辑
    return true; // 占位符
}

// 排除地址
void cds_network_filter_exclude_address(CDsNetworkFilter* filter, const char* ip_address1, const char* ip_address2) {
    // 实现地址排除逻辑
}

// 排除单个地址
void cds_network_filter_exclude_address_single(CDsNetworkFilter* filter, const char* ip_address) {
    // 实现地址排除逻辑
}

// 包含地址
void cds_network_filter_include_address(CDsNetworkFilter* filter, const char* ip_address1, const char* ip_address2) {
    // 实现地址包含逻辑
}

// 包含单个地址
void cds_network_filter_include_address_single(CDsNetworkFilter* filter, const char* ip_address) {
    // 实现地址包含逻辑
}

// 排除端口
void cds_network_filter_exclude_port(CDsNetworkFilter* filter, int port) {
    // 实现端口排除逻辑
}

// 包含端口
void cds_network_filter_include_port(CDsNetworkFilter* filter, int port) {
    // 实现端口包含逻辑
}

// 获取地址
const char* cds_network_filter_get_address(const CDsNetworkFilter* filter) {
    if (filter->iInAddrToAnyCount == 0) {
        return NULL;
    }
    return filter->szIncludeAddrToAny[0];
}

// 设置源地址和目标地址
void cds_network_filter_set_src_to_dst(CDsNetworkFilter* filter, const char* pSrcIP, const char* pDstIP) {
    strncpy(filter->strSrcIP, pSrcIP, sizeof(filter->strSrcIP) - 1);
    strncpy(filter->strDstIP, pDstIP, sizeof(filter->strDstIP) - 1);
}

// 检查源地址和目标地址
bool cds_network_filter_check_src_and_dst_addr(const CDsNetworkFilter* filter, const char* lpSrcIPAddr, const char* lpDstIPAddr) {
    if (lpSrcIPAddr == NULL || lpDstIPAddr == NULL) {
        return false;
    }

    if (strlen(filter->strSrcIP) == 0 && strlen(filter->strDstIP) == 0) {
        return true;
    }

    if (strlen(filter->strSrcIP) == 0) {
        return (strcmp(filter->strDstIP, lpDstIPAddr) == 0 || strcmp(filter->strDstIP, lpSrcIPAddr) == 0);
    }

    if (strlen(filter->strDstIP) == 0) {
        return (strcmp(filter->strSrcIP, lpSrcIPAddr) == 0 || strcmp(filter->strSrcIP, lpDstIPAddr) == 0);
    }

    return (strcmp(filter->strSrcIP, lpSrcIPAddr) == 0 && strcmp(filter->strDstIP, lpDstIPAddr) == 0) ||
           (strcmp(filter->strSrcIP, lpDstIPAddr) == 0 && strcmp(filter->strDstIP, lpSrcIPAddr) == 0);
}