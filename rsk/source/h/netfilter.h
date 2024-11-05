// netfilter.h
#ifndef __DS_NETFILTER_H__
#define __DS_NETFILTER_H__

#include <ctime>
#include <string>
#include <array>
#include <string_view>
#include "doorsdefs.h"
#include "ngdb.h"

class CDsNetworkFilter : public IDsNetworkFilter {
public:
    CDsNetworkFilter();
    explicit CDsNetworkFilter(std::uint32_t dwFromTime);
    ~CDsNetworkFilter() override;

    CDsNetworkFilter(const CDsNetworkFilter&) = delete;
    CDsNetworkFilter& operator=(const CDsNetworkFilter&) = delete;
    CDsNetworkFilter(CDsNetworkFilter&&) noexcept = default;
    CDsNetworkFilter& operator=(CDsNetworkFilter&&) noexcept = default;

    [[nodiscard]] COracleReader* GetOracle() const noexcept override { return m_pOracle; }
    void SetOracle(COracleReader* oracle) override { m_pOracle = oracle; }
    [[nodiscard]] int GetCacheCount() const noexcept override { return m_nCacheCount; }
    void SetCacheCount(int nCount) override { m_nCacheCount = nCount; }

    void SetLogTime(int nLogTime) override { m_nLogTime = nLogTime; }
    [[nodiscard]] int GetLogTime() const noexcept override { return m_nLogTime; }
    void SetInterval(std::uint32_t interval) override { m_dwInterval = interval; }

    [[nodiscard]] bool IsRealTime() const noexcept override { return m_iLiveTime; }
    [[nodiscard]] std::uint32_t GetToTime() const noexcept override { return m_dwToTime; }
    [[nodiscard]] std::uint32_t GetFromTime() const noexcept override { return m_dwFromTime; }
    [[nodiscard]] std::uint32_t GetRealToTime() const noexcept override { return static_cast<std::uint32_t>(m_dwRealToTime); }
    void SetRealToTime(std::uint32_t dwTime) override { m_dwRealToTime = static_cast<std::time_t>(dwTime); }
    void SetToTime(std::uint32_t dwTime) override { m_dwToTime = dwTime; }
    void SetFromTime(std::uint32_t dwTime) override { m_dwFromTime = dwTime; }
    void SetTimeRange(std::uint32_t dwFromTime, std::uint32_t dwToTime) override;

    [[nodiscard]] bool CheckTimeAfter(std::uint32_t dwTime) const noexcept override { return dwTime > m_dwFromTime; }
    [[nodiscard]] bool CheckTimeBefore(std::uint32_t dwTime) const noexcept override { return dwTime < m_dwToTime; }
    [[nodiscard]] bool CheckAddress(std::string_view ip_address1, std::string_view ip_address2) const override;
    [[nodiscard]] bool CheckPort(int port) const override;
    [[nodiscard]] bool CheckTime(std::uint32_t dwTime) const noexcept override { return dwTime >= m_dwFromTime && dwTime <= m_dwToTime; }

    void ExcludeAddress(std::string_view ip_address1, std::string_view ip_address2) override;
    void ExcludeAddress(std::string_view ip_address) override;
    void IncludeAddress(std::string_view ip_address1, std::string_view ip_address2) override;
    void IncludeAddress(std::string_view ip_address) override;
    void ExcludePort(int port) override;
    void IncludePort(int port) override;

    void SetFilePath(std::string_view file_path) override { m_FilePath = file_path; }
    [[nodiscard]] std::string_view GetFilePath() const noexcept override { return m_FilePath; }
    [[nodiscard]] std::string_view GetAddress() const noexcept override;
    [[nodiscard]] std::uint16_t GetPort() const noexcept override { return static_cast<std::uint16_t>(m_iIncludePort[0]); }

    void SetSrcToDst(std::string_view pSrcIP, std::string_view pDstIP) override;
    [[nodiscard]] std::string_view GetSrcAddr() const noexcept override { return m_strSrcIP; }
    [[nodiscard]] std::string_view GetDstAddr() const noexcept override { return m_strDstIP; }
    [[nodiscard]] bool CheckSrcAndDstAddr(std::string_view lpSrcIPAddr, std::string_view lpDstIPAddr) const override;

protected:
    void Init();

    std::uint32_t m_dwFromTime{0};
    std::uint32_t m_dwToTime{0};
    std::uint32_t m_dwInterval{0};
    bool m_iLiveTime{false};
    std::time_t m_dwRealToTime{0};

    std::array<int, 256> m_iIncludePort{};
    std::array<int, 256> m_iExcludePort{};
    std::array<std::array<char, 15>, 256> m_szIncludeAddrToAny{};
    std::array<std::array<char, 15>, 256> m_szExcludeAddrToAny{};
    std::array<std::array<char, 32>, 256> m_szIncludeAddrToAddr{};
    std::array<std::array<char, 32>, 256> m_szExcludeAddrToAddr{};

    int m_iInPortCount{0};
    int m_iExPortCount{0};
    int m_iInAddrToAnyCount{0};
    int m_iExAddrToAnyCount{0};
    int m_iInAddrToAddrCount{0};
    int m_iExAddrToAddrCount{0};

    int m_nLogTime{0};
    COracleReader* m_pOracle{nullptr};
    int m_nCacheCount{0};
    std::string m_FilePath;
    std::uint16_t m_wHttpPort{0};
    std::string m_strSrcIP;
    std::string m_strDstIP;
};

#endif // __DS_NETFILTER_H__

//BY GST ARMV8 GCC 13.2