#ifndef __CMAILAUDIT_H__
#define __CMAILAUDIT_H__

#include <gmime/gmime.h>
#include <string>
#include <vector>
#include <memory>
#include <array>
#include <string_view>

enum class CheckMode {
    WHITE = 0,
    BLACK = 1
};

enum class AuditResult {
    PASS = 0,
    FORBIDEN_ATTACHMENT = 1,
    FORBIDEN_BODY = 2,
    FORBIDEN_SUBJECT = 3
};

class CQSearch;

class CMailAudit {
public:
    CMailAudit() {
        g_mime_init(0);
        m_szFoundKey.fill('\0');
    }

    ~CMailAudit() {
        // g_mime_shutdown();
    }

    CMailAudit(const CMailAudit&) = delete;
    CMailAudit& operator=(const CMailAudit&) = delete;
    CMailAudit(CMailAudit&&) noexcept = default;
    CMailAudit& operator=(CMailAudit&&) noexcept = default;

    // Getters
    [[nodiscard]] std::string_view GetSubject() const noexcept { return m_szSubject; }
    [[nodiscard]] std::string_view GetFrom() const noexcept { return m_szFrom; }
    [[nodiscard]] std::string_view GetTo() const noexcept { return m_szTo; }
    [[nodiscard]] std::string_view GetCc() const noexcept { return m_szCc; }
    [[nodiscard]] std::string_view GetDate() const noexcept { return m_szDate; }

    // Setters
    void SetAttaList(const std::shared_ptr<std::vector<std::string>>& pAttaList) { m_pAttaList = pAttaList; }
    void SetContList(const std::shared_ptr<std::vector<std::string>>& pContList) { m_pContList = pContList; }
    void SetContCheckMode(CheckMode nCheckMode) { m_nContCheckMode = nCheckMode; }
    void SetAttaCheckMode(CheckMode nCheckMode) { m_nAttaCheckMode = nCheckMode; }
    void SetQSearch(CQSearch* pQSearch) { m_pQSearch = pQSearch; }

    [[nodiscard]] AuditResult Audit(std::string_view pMail);
    [[nodiscard]] bool GetFoundKey(std::span<char> szFoundKey) const;

private:
    std::array<char, 128> m_szFoundKey{};
    std::shared_ptr<std::vector<std::string>> m_pAttaList;
    std::shared_ptr<std::vector<std::string>> m_pContList;
    CheckMode m_nContCheckMode{CheckMode::WHITE};
    CheckMode m_nAttaCheckMode{CheckMode::WHITE};
    CQSearch* m_pQSearch{nullptr};

    std::string m_szSubject;
    std::string m_szFrom;
    std::string m_szTo;
    std::string m_szCc;
    std::string m_szDate;

    [[nodiscard]] std::string DoEncode(std::string_view p) const;
    [[nodiscard]] bool DoDecode(std::string_view p, std::string& pValue) const;
    [[nodiscard]] bool DecodeSubject(std::string_view szEnc, std::string& pValue) const;
};

#endif // __CMAILAUDIT_H__

//BY GST ARMV8 GCC 13.2