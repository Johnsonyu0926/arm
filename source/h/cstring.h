#ifndef _CSTRING_H__
#define _CSTRING_H__

#include "Share.h"
#include "doorsbase.h"
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <memory>
#include <algorithm>

constexpr int MAX_PRINTABLE_STRING_LENGTH = 5000;
constexpr char GAP_INITIALIZER = '\0';

class CString {
public:
    CString() : m_szBuf(nullptr), m_bFirstAdd(TRUE), m_nTotal(0) {}

    ~CString() = default;

    CString(const CString& rhs) {
        m_nTotal = rhs.m_nTotal;
        m_bFirstAdd = rhs.m_bFirstAdd;
        m_szBuf = std::make_unique<char[]>(m_nTotal + 1);
        std::strcpy(m_szBuf.get(), rhs.m_szBuf.get());
    }

    CString(const char* szBuf) {
        m_nTotal = std::strlen(szBuf);
        m_bFirstAdd = TRUE;
        m_szBuf = std::make_unique<char[]>(m_nTotal + 1);
        std::strcpy(m_szBuf.get(), szBuf);
    }

    operator const char* () const {
        return m_szBuf.get();
    }

    char operator [](int nIndex) const {
        if (!m_szBuf || nIndex < 0 || nIndex >= m_nTotal || m_nTotal <= 0)
            return GAP_INITIALIZER;
        return m_szBuf[nIndex];
    }

    CString& operator =(const CString& rhs) {
        if (this != &rhs) {
            m_nTotal = rhs.m_nTotal;
            m_bFirstAdd = rhs.m_bFirstAdd;
            m_szBuf = std::make_unique<char[]>(m_nTotal + 1);
            std::strcpy(m_szBuf.get(), rhs.m_szBuf.get());
        }
        return *this;
    }

    CString& operator =(const char* szBuf) {
        m_nTotal = std::strlen(szBuf);
        m_bFirstAdd = TRUE;
        m_szBuf = std::make_unique<char[]>(m_nTotal + 1);
        std::strcpy(m_szBuf.get(), szBuf);
        return *this;
    }

    CString operator +(const CString& rhs) const {
        CString result;
        result.m_nTotal = m_nTotal + rhs.m_nTotal;
        result.m_szBuf = std::make_unique<char[]>(result.m_nTotal + 1);
        std::strcpy(result.m_szBuf.get(), m_szBuf.get());
        std::strcat(result.m_szBuf.get(), rhs.m_szBuf.get());
        return result;
    }

    CString operator +(const char* szBuf) const {
        CString result;
        result.m_nTotal = m_nTotal + std::strlen(szBuf);
        result.m_szBuf = std::make_unique<char[]>(result.m_nTotal + 1);
        std::strcpy(result.m_szBuf.get(), m_szBuf.get());
        std::strcat(result.m_szBuf.get(), szBuf);
        return result;
    }

    CString& operator +=(const CString& rhs) {
        *this = *this + rhs;
        return *this;
    }

    CString& operator +=(const char* szBuf) {
        *this = *this + szBuf;
        return *this;
    }

    CString& operator +=(const char szCh) {
        char buf[2] = { szCh, '\0' };
        *this = *this + buf;
        return *this;
    }

    long GetLength() const {
        return m_nTotal;
    }

    BOOL IsEmpty() const {
        return m_nTotal == 0;
    }

    void Empty() {
        m_szBuf.reset();
        m_nTotal = 0;
        m_bFirstAdd = TRUE;
    }

    char GetAt(int nIndex) const {
        return (*this)[nIndex];
    }

    void SetAt(int nIndex, const char ch) {
        if (nIndex >= 0 && nIndex < m_nTotal) {
            m_szBuf[nIndex] = ch;
        }
    }

    void Replace(const char szSourceCh, const char szDimCh) {
        for (int i = 0; i < m_nTotal; ++i) {
            if (m_szBuf[i] == szSourceCh) {
                m_szBuf[i] = szDimCh;
            }
        }
    }

    int Compare(const char* szBuf) const {
        return std::strcmp(m_szBuf.get(), szBuf);
    }

    int CompareNoCase(const char* szBuf) const {
        return strcasecmp(m_szBuf.get(), szBuf);
    }

    CString Mid(int nFirst) const {
        return Mid(nFirst, m_nTotal - nFirst);
    }

    CString Mid(int nFirst, int nCount) const {
        CString result;
        if (nFirst >= 0 && nFirst < m_nTotal && nCount > 0) {
            result.m_nTotal = nCount;
            result.m_szBuf = std::make_unique<char[]>(nCount + 1);
            std::strncpy(result.m_szBuf.get(), m_szBuf.get() + nFirst, nCount);
            result.m_szBuf[nCount] = '\0';
        }
        return result;
    }

    CString Right(int nCount) const {
        return Mid(m_nTotal - nCount, nCount);
    }

    CString Left(int nCount) const {
        return Mid(0, nCount);
    }

    void MakeUpper() {
        std::transform(m_szBuf.get(), m_szBuf.get() + m_nTotal, m_szBuf.get(), ::toupper);
    }

    void MakeLower() {
        std::transform(m_szBuf.get(), m_szBuf.get() + m_nTotal, m_szBuf.get(), ::tolower);
    }

    void TrimLeft() {
        int i = 0;
        while (i < m_nTotal && std::isspace(m_szBuf[i])) {
            ++i;
        }
        if (i > 0) {
            std::memmove(m_szBuf.get(), m_szBuf.get() + i, m_nTotal - i + 1);
            m_nTotal -= i;
        }
    }

    void TrimRight() {
        int i = m_nTotal - 1;
        while (i >= 0 && std::isspace(m_szBuf[i])) {
            --i;
        }
        m_szBuf[i + 1] = '\0';
        m_nTotal = i + 1;
    }

    int Find(const char* szValue, int nStart = 0) const {
        const char* pos = std::strstr(m_szBuf.get() + nStart, szValue);
        return pos ? pos - m_szBuf.get() : -1;
    }

    int Find(const char szCh, int sStat = 0) const {
        const char* pos = std::strchr(m_szBuf.get() + sStat, szCh);
        return pos ? pos - m_szBuf.get() : -1;
    }

    void Format(const char* pFormat, ...) {
        va_list args;
        va_start(args, pFormat);
        char buffer[MAX_PRINTABLE_STRING_LENGTH];
        vsnprintf(buffer, sizeof(buffer), pFormat, args);
        va_end(args);
        *this = buffer;
    }

private:
    BOOL GetValue(char* szValue) {
        if (!szValue) return FALSE;
        std::strcpy(szValue, m_szBuf.get());
        return TRUE;
    }

    BOOL AddValue(const char* szValue, long nLen) {
        if (!szValue || nLen <= 0) return FALSE;
        auto newBuf = std::make_unique<char[]>(m_nTotal + nLen + 1);
        if (m_szBuf) {
            std::strcpy(newBuf.get(), m_szBuf.get());
            std::strcat(newBuf.get(), szValue);
        } else {
            std::strcpy(newBuf.get(), szValue);
        }
        m_szBuf = std::move(newBuf);
        m_nTotal += nLen;
        return TRUE;
    }

    BOOL AddValue(const char szCh, long nLen) {
        char buf[2] = { szCh, '\0' };
        return AddValue(buf, nLen);
    }

private:
    std::unique_ptr<char[]> m_szBuf;
    BOOL m_bFirstAdd{TRUE};
    long m_nTotal{0};
};

#endif // _CSTRING_H__