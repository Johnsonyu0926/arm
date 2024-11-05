// netfilter.cpp
#include "netfilter.h"
#include <cstring>

CDsNetworkFilter::CDsNetworkFilter() {
    Init();
}

CDsNetworkFilter::CDsNetworkFilter(std::uint32_t dwFromTime) {
    Init();
    m_dwFromTime = dwFromTime;
}

CDsNetworkFilter::~CDsNetworkFilter() = default;

void CDsNetworkFilter::SetTimeRange(std::uint32_t dwFromTime, std::uint32_t dwToTime) {
    m_dwFromTime = dwFromTime;
    m_dwToTime = dwToTime;
}

bool CDsNetworkFilter::CheckAddress(std::string_view ip_address1, std::string_view ip_address2) const {
    // Implement address checking logic
    return true; // Placeholder
}

bool CDsNetworkFilter::CheckPort(int port) const {
    // Implement port checking logic
    return true; // Placeholder
}

void CDsNetworkFilter::ExcludeAddress(std::string_view ip_address1, std::string_view ip_address2) {
    // Implement address exclusion logic
}

void CDsNetworkFilter::ExcludeAddress(std::string_view ip_address) {
    // Implement address exclusion logic
}

void CDsNetworkFilter::IncludeAddress(std::string_view ip_address1, std::string_view ip_address2) {
    // Implement address inclusion logic
}

void CDsNetworkFilter::IncludeAddress(std::string_view ip_address) {
    // Implement address inclusion logic
}

void CDsNetworkFilter::ExcludePort(int port) {
    // Implement port exclusion logic
}

void CDsNetworkFilter::IncludePort(int port) {
    // Implement port inclusion logic
}

std::string_view CDsNetworkFilter::GetAddress() const noexcept {
    if (m_iInAddrToAnyCount == 0) {
        return {};
    }
    return m_szIncludeAddrToAny[0].data();
}

void CDsNetworkFilter::SetSrcToDst(std::string_view pSrcIP, std::string_view pDstIP) {
    m_strSrcIP = pSrcIP;
    m_strDstIP = pDstIP;
}

bool CDsNetworkFilter::CheckSrcAndDstAddr(std::string_view lpSrcIPAddr, std::string_view lpDstIPAddr) const {
    if (lpSrcIPAddr.empty() || lpDstIPAddr.empty()) {
        return false;
    }

    if (m_strSrcIP.empty() && m_strDstIP.empty()) {
        return true;
    }

    if (m_strSrcIP.empty()) {
        return (m_strDstIP == lpDstIPAddr || m_strDstIP == lpSrcIPAddr);
    }

    if (m_strDstIP.empty()) {
        return (m_strSrcIP == lpSrcIPAddr || m_strSrcIP == lpDstIPAddr);
    }

    return (m_strSrcIP == lpSrcIPAddr && m_strDstIP == lpDstIPAddr) ||
           (m_strSrcIP == lpDstIPAddr && m_strDstIP == lpSrcIPAddr);
}

void CDsNetworkFilter::Init() {
    m_dwFromTime = 0;
    m_dwToTime = 0;
    m_dwInterval = 0;
    m_iLiveTime = false;
    m_dwRealToTime = 0;
    m_nLogTime = 0;
    m_pOracle = nullptr;
    m_nCacheCount = 0;
    m_wHttpPort = 0;
    m_strSrcIP.clear();
    m_strDstIP.clear();
    m_FilePath.clear();

    m_iInPortCount = 0;
    m_iExPortCount = 0;
    m_iInAddrToAnyCount = 0;
    m_iExAddrToAnyCount = 0;
    m_iInAddrToAddrCount = 0;
    m_iExAddrToAddrCount = 0;

    m_iIncludePort.fill(0);
    m_iExcludePort.fill(0);
    for (auto& addr : m_szIncludeAddrToAny) addr.fill(0);
    for (auto& addr : m_szExcludeAddrToAny) addr.fill(0);
    for (auto& addr : m_szIncludeAddrToAddr) addr.fill(0);
    for (auto& addr : m_szExcludeAddrToAddr) addr.fill(0);
}

//BY GST ARMV8 GCC 13.2