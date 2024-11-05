// GAPAudit.h
#ifndef _GAPAUDIT_H__
#define _GAPAUDIT_H__

#include "GAPMime.h"
#include <string>
#include <array>
#include <memory>
#include <string_view>

inline constexpr int RULECOUNT = 100;

class CGAPAudit {
public:
    CGAPAudit();
    ~CGAPAudit();

    CGAPAudit(const CGAPAudit&) = delete;
    CGAPAudit& operator=(const CGAPAudit&) = delete;
    CGAPAudit(CGAPAudit&&) noexcept = default;
    CGAPAudit& operator=(CGAPAudit&&) noexcept = default;

    [[nodiscard]] bool AddContent(std::span<const std::uint8_t> pBuf);
    [[nodiscard]] bool InitRule();
    [[nodiscard]] bool Audit(DsAuditStatus* pAudited);

    [[nodiscard]] std::string_view GetSubject() const noexcept { return m_szSubject; }
    [[nodiscard]] std::string_view GetFrom() const noexcept { return m_szFrom; }
    [[nodiscard]] std::string_view GetTo() const noexcept { return m_szTo; }
    [[nodiscard]] std::string_view GetCc() const noexcept { return m_szCc; }
    [[nodiscard]] std::string_view GetDate() const noexcept { return m_szDate; }

    [[nodiscard]] bool SetContent(std::string_view szBuf);

private:
    [[nodiscard]] bool Decode(DsEncodingMethod EncodingMethod, std::string_view strBody, std::string& strDecoded) const;
    [[nodiscard]] bool AuditTextPlain(std::span<const std::uint8_t> pBody) const;
    [[nodiscard]] bool AuditHtml(std::span<const std::uint8_t> pBody) const;
    [[nodiscard]] bool AuditAttachment(std::string_view szFileName, int nFileType) const;
    [[nodiscard]] int CheckFileName(std::string_view strFileName) const;
    [[nodiscard]] int AuditSubject() const;

    std::string m_szDate;
    bool m_bReleaseMem{false};

    bool m_nDenyAll{false};
    std::string m_szSubject;
    std::string m_szFrom;
    std::string m_szTo;
    std::string m_szCc;

    std::unique_ptr<char[]> m_szContent;
    std::size_t m_nContentLen{0};
    bool m_bFirstAdd{true};

    std::array<std::unique_ptr<char[]>, RULECOUNT> m_pRuleContent;
    int m_nRuleContentCount{0};
    std::array<std::unique_ptr<char[]>, RULECOUNT> m_pRuleAttachMent;
    int m_nRuleAttachMentCount{0};
    std::array<std::unique_ptr<char[]>, RULECOUNT> m_pRuleScript;
    int m_nRuleScriptCount{0};

    CGAPMime m_Mime;
};

#endif // _GAPAUDIT_H__

//BY GST ARMV8 GCC 13.2