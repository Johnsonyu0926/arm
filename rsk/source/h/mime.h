// mime.h
#ifndef __MIME_H__
#define __MIME_H__

#include <gmime/gmime.h>
#include <memory>
#include <string>
#include <vector>
#include <string_view>
#include "doorsbase.h"

class CMime {
public:
    CMime() : m_pAttaList(std::make_unique<std::vector<std::string>>()), m_pBody(nullptr) {}

    ~CMime() {
        DS_TRACE("[CMime::~CMime] release mime memory...");
        if (m_pBody) {
            g_free(m_pBody);
            m_pBody = nullptr;
        }
    }

    CMime(const CMime&) = delete;
    CMime& operator=(const CMime&) = delete;
    CMime(CMime&&) noexcept = default;
    CMime& operator=(CMime&&) noexcept = default;

    void SetContent(std::string_view pMail, std::size_t nSize);

    [[nodiscard]] const char* GetBody() const noexcept {
        return m_pBody;
    }

    [[nodiscard]] const std::vector<std::string>& GetAttaList() const noexcept {
        return *m_pAttaList;
    }

    [[nodiscard]] const std::string& GetSubject() const noexcept {
        return m_szSubject;
    }

    [[nodiscard]] const std::string& GetDate() const noexcept {
        return m_szDate;
    }

    [[nodiscard]] const std::string& GetFrom() const noexcept {
        return m_szFrom;
    }

    [[nodiscard]] const std::string& GetTo() const noexcept {
        return m_szTo;
    }

    [[nodiscard]] const std::string& GetCc() const noexcept {
        return m_szCc;
    }

private:
    std::unique_ptr<std::vector<std::string>> m_pAttaList;
    char* m_pBody;
    std::string m_szSubject;
    std::string m_szDate;
    std::string m_szFrom;
    std::string m_szTo;
    std::string m_szCc;

    void print_mime_struct(GMimeObject* part, int depth);
};

#endif // __MIME_H__

//BY GST ARMV8 GCC 13.2