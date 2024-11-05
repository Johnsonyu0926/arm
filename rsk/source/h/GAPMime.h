// GAPMime.h
#ifndef _GAPMIME_H__
#define _GAPMIME_H__

#include "Base64.h"
#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <string_view>

class CGAPMime {
public:
    CGAPMime();
    virtual ~CGAPMime();

    CGAPMime(const CGAPMime&) = delete;
    CGAPMime& operator=(const CGAPMime&) = delete;
    CGAPMime(CGAPMime&&) noexcept = default;
    CGAPMime& operator=(CGAPMime&&) noexcept = default;

    [[nodiscard]] bool Init(std::span<const std::uint8_t> pBody);
    [[nodiscard]] bool Work(long* pnSuccess);
    bool SetBodyLen(long nBodyLen);
    [[nodiscard]] bool GetPartCount(long* pnCount) const;
    [[nodiscard]] bool GetPart(int nIndex, int nType, std::span<std::uint8_t> pnVal) const;

    [[nodiscard]] std::string_view GetSubject() const { return m_szSubject; }
    [[nodiscard]] std::string_view GetFrom() const { return m_szFrom; }
    [[nodiscard]] std::string_view GetTo() const { return m_szTo; }
    [[nodiscard]] std::string_view GetCc() const { return m_szCc; }
    [[nodiscard]] std::string_view GetDate() const { return m_szDate; }

private:
    [[nodiscard]] std::string_view GetValueFromKey(std::string_view szLine, std::string_view szKey) const;
    [[nodiscard]] bool GetContentType(int& nContentType, std::string& pBoundary, int& nEncodingType, std::string& pFileName) const;
    [[nodiscard]] bool GetContentType(std::string_view szBuf, std::string& szContentType) const;
    [[nodiscard]] int GetContentType(std::string_view szContentType, int& nContentType) const;
    [[nodiscard]] bool GetLine(std::string& szBuf);
    [[nodiscard]] int GetFieldValue(std::string_view szBuf, int nPos, std::string& szValue) const;
    [[nodiscard]] int GetEncodingType(std::string_view szBuf, int& nEncodingType) const;

    int DealWithTheContentType(int nContentType, std::string_view szBoundary, int nEncodingType, std::string_view szFileName);
    int DealWithMultPart(int nContentType, std::string_view szBoundary, int nEncodingType, std::string_view szFileName);
    int DealWithSinglePart(int nContentType, std::string_view szBoundary, int nEncodingType, std::string_view szFileName);
    int GotoBoundary(std::string_view szBoundary);
    int DealWithTextPlain(int nContextType, int nEncodingType, std::string_view szBoundary);
    int DealWithOct(int nEncodingType, std::string_view szBoundary, std::string_view szFileName);
    int DealWith7BitsPlainText(int nContextType, std::string_view szBoundary);
    int DealWithBase64PlainText(std::string_view szBoundary);
    int DealWithQP(std::string_view szBoundary);
    int RecvBodyAndDecode(std::string_view szFileName, std::string_view szBoundary);

    std::span<const std::uint8_t> m_pBody;
    int m_nOffset{0};
    int m_nBodyLen{0};
    int m_nCurrentAttachmentCount{0};
    std::array<std::string, 128> m_szAttachmentName;
    long m_nPartCount{0};
    Gap_BodyPart_t m_pBodyPart{};

    [[nodiscard]] Gap_BodyPart_t BodyContentCreate() const;
    Gap_BodyPart_t BodyContentInsert(Gap_BodyPart_t pBodyPart, Gap_BodyPart_t szBodyPart);
    void BodyContentDestroy(Gap_BodyPart_t* pBodyPart);
    void BodyContentDestroyAll(Gap_BodyPart_t* pBodyPart);
    Gap_BodyPart_t BodyContentDelete(Gap_BodyPart_t pBodyPart, int nPart);

    std::string m_szSubject;
    std::string m_szCc;
    std::string m_szTo;
    std::string m_szFrom;
    std::string m_szDate;
};

void ToMakeUpper(std::string& str);

#endif // _GAPMIME_H__

//BY GST ARMV8 GCC 13.2