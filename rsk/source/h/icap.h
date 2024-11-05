// icap.h
#ifndef ICAP_H
#define ICAP_H

#include <string>
#include <vector>
#include <memory>
#include "doorsbase.h"

struct ContentList {
    std::string Content;
    int id;
};

class CIcap {
public:
    CIcap() : m_nContentLen(0), m_nIcapServerPort(0), count(0) {
        m_pAttaList = std::make_unique<std::vector<std::string>>();
        DS_TRACE("CIcap constructor");
    }

    ~CIcap() {
        DS_TRACE("CIcap destructor");
    }

    CIcap(const CIcap&) = delete;
    CIcap& operator=(const CIcap&) = delete;
    CIcap(CIcap&&) noexcept = default;
    CIcap& operator=(CIcap&&) noexcept = default;

    void AddContent(std::string_view szContent) {
        DS_TRACE("ADD CONTENT " << count);
        m_contentList.push_back({std::string(szContent), ++count});
        m_nContentLen += szContent.length();
    }

    void SetIcapServer(std::string_view szServer, int nServerPort) {
        m_szIcapServer = szServer;
        m_nIcapServerPort = nServerPort;
    }

    [[nodiscard]] bool RespMod(std::string_view pszAddress, int argflag, std::string& szVirus, int nLen);

    void PrintContent() const {
        for (int i = 0; i < count; ++i) {
            DS_TRACE("I IS : " << i);
        }
    }

    bool ReleaseList() {
        m_contentList.clear();
        count = 0;
        m_nContentLen = 0;
        return true;
    }

private:
    std::vector<ContentList> m_contentList;
    std::unique_ptr<std::vector<std::string>> m_pAttaList;
    std::string m_szIcapServer;
    int m_nIcapServerPort{0};
    int m_nContentLen{0};
    int count{0};
};

#endif // ICAP_H

//BY GST ARMV8 GCC 13.2