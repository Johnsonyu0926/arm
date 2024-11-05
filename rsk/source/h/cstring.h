// CString.h
#ifndef _CSTRING_H__
#define _CSTRING_H__

#include "Share.h"
#include "doorsbase.h"
#include <string>
#include <algorithm>
#include <cstdarg>

constexpr int MAX_PRINTABLE_STRING_LENGTH = 5000;

class CString {
public:
    CString();
    ~CString();

    CString(const CString& rhs);
    CString(CString&& rhs) noexcept;
    explicit CString(const std::string& szBuf);

    operator std::string() const {
        return m_szBuf ? std::string(m_szBuf) : std::string();
    }

    char operator[](int nIndex) const {
        return (m_szBuf && nIndex >= 0 && nIndex < m_nTotal) ? m_szBuf[nIndex] : GAP_INITIALIZER;
    }

    CString& operator=(const CString& rhs);
    CString& operator=(CString&& rhs) noexcept;
    CString& operator=(const std::string& szBuf);

    CString operator+(const CString& rhs) const;
    CString operator+(const std::string& szBuf) const;

    CString& operator+=(const CString& rhs);
    CString& operator+=(const std::string& szBuf);
    CString& operator+=(char szCh);

    [[nodiscard]] long GetLength() const noexcept { return m_nTotal; }
    [[nodiscard]] bool IsEmpty() const noexcept { return m_nTotal == 0; }
    void Empty();

    [[nodiscard]] char GetAt(int nIndex) const;
    void SetAt(int nIndex, char ch);
    void Replace(char szSourceCh, char szDimCh);

    [[nodiscard]] int Compare(const std::string& szBuf) const;
    [[nodiscard]] int CompareNoCase(const std::string& szBuf) const;

    [[nodiscard]] CString Mid(int nFirst) const;
    [[nodiscard]] CString Mid(int nFirst, int nCount) const;
    [[nodiscard]] CString Right(int nCount) const;
    [[nodiscard]] CString Left(int nCount) const;

    void MakeUpper();
    void MakeLower();

    void TrimLeft();
    void TrimRight();

    [[nodiscard]] int Find(const std::string& szValue, int nStart = 0) const;
    [[nodiscard]] int Find(char szCh, int sStat = 0) const;

    void Format(const char* pFormat, ...);

private:
    bool GetValue(std::string& szValue) const;
    bool AddValue(const std::string& szValue, long nLen);
    bool AddValue(char szCh, long nLen);

    char* m_szBuf;
    long m_nTotal;
};

#endif // _CSTRING_H__

//BY GST ARMV8 GCC 13.2