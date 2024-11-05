#ifndef CDNSRECORDSEARCH_H_
#define CDNSRECORDSEARCH_H_

#include <array>
#include <vector>
#include <memory>
#include "doorsbase.h"
#include "cstring.h"

enum class DNSRecordType : unsigned short {
    A = 1,
    NS = 2,
    MD = 3,
    MF = 4,
    CNAME = 5,
    SOA = 6,
    MB = 7,
    MG = 8,
    MR = 9,
    NULL_TYPE = 10,
    WKS = 11,
    PTR = 12,
    HINFO = 13,
    MINFO = 14,
    MX = 15,
    TXT = 16,
    AXFR = 252,
    MAILB = 253,
    MAILA = 254,
    ALL = 255
};

enum class DNSClass : unsigned short {
    IN = 1,
    CS = 2,
    CH = 3,
    HS = 4,
    ALL = 255
};

struct MX_RECORD {
    std::array, 16> szRecord;
    unsigned short nNum;

    MX_RECORD() : nNum(0) {}
};

struct DNSHeadTag {
    unsigned short m_nID;
    std::array m_szCode;
    unsigned short m_nQDCount;
    unsigned short m_nANCount;
    unsigned short m_nNSCount;
    unsigned short m_nARCount;
};

struct ResponseRecord {
    CString m_strName;
    CString m_strAnswer;
};

class CDNSRecordSearch {
public:
    CDNSRecordSearch(const char DNSServerIPAddress, const char ServerName, DNSRecordType Type, DNSClass Class);
    virtual ~CDNSRecordSearch() = default;

    bool Run();
    const ResponseRecord GetARRecord(size_t index) const;
    const ResponseRecord GetNSRecord(size_t index) const;
    const ResponseRecord* GetANRecord(size_t index) const;

private:
    bool DealWithResponse();
    int GetResponse();
    bool SendRequest();
    void AddANRecords();
    void AddNSRecords();
    void AddARRecords();
    void RemoveRecordsFromList();
    int GetRecordData(int StartIndex, CString& Data, bool IsIPAddress);
    void GetNextAnswer(int PassBytes, ResponseRecord* Data);

    bool Init();
    unsigned short m_nANRecordCount;
    unsigned short m_nNSRecordCount;
    unsigned short m_nARRecordCount;
    DNSRecordType m_nType;
    DNSClass m_nClass;
    CSocket m_ssServerSocket;
    std::array m_szBuf;
    int m_nBufPos;
    int m_nReceiveBytes;
    std::vector m_listANRecords;
    std::vector m_listNSRecords;
    std::vector m_listARRecords;
    CString m_strServerName;
    CString m_strDNSServerIPAddress;
};

bool GetMXAddressByName(const char szDNSServerIP, const char szMXServerName, MX_RECORD& MXRecord);

#endif // CDNSRECORDSEARCH_H_

// CDNSRecordSearch.cpp
#include "CDNSRecordSearch.h"
#include <algorithm>
#include <stdexcept>

CDNSRecordSearch::CDNSRecordSearch(const char DNSServerIPAddress, const char ServerName, DNSRecordType Type, DNSClass Class)
    : m_strDNSServerIPAddress(DNSServerIPAddress), m_strServerName(ServerName), m_nType(Type), m_nClass(Class),
      m_nANRecordCount(0), m_nNSRecordCount(0), m_nARRecordCount(0), m_nBufPos(0), m_nReceiveBytes(0) {
    m_szBuf.fill(0);
}

bool CDNSRecordSearch::Run() {
    if (!Init()) {
        return false;
    }
    if (!SendRequest()) {
        return false;
    }
    return DealWithResponse();
}

bool CDNSRecordSearch::Init() {
    // 初始化逻辑
    return true;
}

const ResponseRecord* CDNSRecordSearch::GetARRecord(size_t index) const {
    if (index < m_listARRecords.size()) {
        return m_listARRecords[index].get();
    }
    return nullptr;
}

const ResponseRecord* CDNSRecordSearch::GetNSRecord(size_t index) const {
    if (index < m_listNSRecords.size()) {
        return m_listNSRecords[index].get();
    }
    return nullptr;
}

const ResponseRecord* CDNSRecordSearch::GetANRecord(size_t index) const {
    if (index < m_listANRecords.size()) {
        return m_listANRecords[index].get();
    }
    return nullptr;
}

bool CDNSRecordSearch::DealWithResponse() {
    // 处理应答
    return true;
}

int CDNSRecordSearch::GetResponse() {
    // 获取响应
    return 0;
}

bool CDNSRecordSearch::SendRequest() {
    // 发送请求
    return true;
}

void CDNSRecordSearch::AddANRecords() {
    // 添加应答记录
}

void CDNSRecordSearch::AddNSRecords() {
    // 添加权限记录
}

void CDNSRecordSearch::AddARRecords() {
    // 添加附加记录
}

void CDNSRecordSearch::RemoveRecordsFromList() {
    m_listANRecords.clear();
    m_listNSRecords.clear();
    m_listARRecords.clear();
}

int CDNSRecordSearch::GetRecordData(int StartIndex, CString& Data, bool IsIPAddress) {
    // 获取记录数据
    return 0;
}

void CDNSRecordSearch::GetNextAnswer(int PassBytes, ResponseRecord* Data) {
    // 获取下一个应答
}

bool GetMXAddressByName(const char szDNSServerIP, const char szMXServerName, MX_RECORD& MXRecord) {
    CDNSRecordSearch dnsSearch(szDNSServerIP, szMXServerName, DNSRecordType::MX, DNSClass::IN);
    if (!dnsSearch.Run()) {
        return false;
    }

    MXRecord.nNum = 0;
    for (size_t i = 0; i < 16; ++i) {
        const ResponseRecord* record = dnsSearch.GetANRecord(i);
        if (!record) break;
        strncpy(MXRecord.szRecord[i].data(), record->m_strAnswer.c_str(), 254);
        MXRecord.szRecord[i][254] = '\0';
        ++MXRecord.nNum;
    }

    return MXRecord.nNum > 0;
}

//BY GST ARMV8 GCC142.