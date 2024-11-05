#ifndef BASE64_H_
#define BASE64_H_

#include <cstdio>
#include "doorsbase.h"
#include "memfile.h"
#include "cstring.h"

class CBase64 {
public:
    CBase64();
    virtual ~CBase64() = default;

    int DecodeBody(const CString& strSrc, CMemFile& mmfile);
    int DecodeString(const char szSrc, CString& strDecoded);
    CString Encode(const char szEncoding, int nSize);
    int DecodeBody(const CString& strBody, CString& strDecoded);
    int DecodeEx(const char szSrc, int nSize, BYTE pBuf);

private:
    int DecodeStr(const char szDecoding, char szOutput);
    int Decode(const char p6Bits, FILE fp);
    int GetBodyText(CString& strBodyText);
    int Decode(const char p6Bits, char szDecoded);
    int Decode(const char p6Bits, char szDecoded, int& nBytes);
    int DecodeString(char szSrc, char szDecoded, int& nLineBytes);
    int AsciiTo6Bits(char cAsc);
    void write_bits(UINT nBits, int nNumBits, char szOutput, int& i);
    int DecodingString(const char szBuf, FILE fp);
    int SetBodyText(const CString& strBody);
    int Decode(const CString& strBody, const CString& strFileName);
    UINT read_bits(int nNumBits, int pBitsRead, int& lp);

    CString m_strBodyText;
    int m_nInputSize;
    int m_nBitsRemaining;
    ULONG m_lBitStorage;
    const char* m_szInput;

    static const int m_nMask[];
    static const CString m_sBase64Alphabet;
};

class CQp {
public:
    CQp() = default;
    virtual ~CQp() = default;

    int Decode(const CString& strBody, CMemFile& mmfile);
    int Decode(const CString& szSrc, CString& szDecoded);

private:
    int GetHex(const char szHex);
    int GetCharByHex(const char szHex, char& szChar);
};

constexpr int STR_MAX = 1024;

class CUtf7 {
public:
    CUtf7() = default;
    virtual ~CUtf7() = default;

    int DecodeEx(const char szBuf, char szDecoded);
    int Decode(const char p, char pBuf);
    int Encode(const char p, char pBuf);

private:
    int Decode(const CString& strSrc, CString& strDecoded);
    int Encode(const char szSrc, CString& szEncoded);
    int DealWithUtf7(const char p, char pBuf);
    bool IsPrintable(char c);
    int HandleTheEncodedString(CString& strEncoded);
    int HandleTheDecodeString(CString& strSrc);
    int Encode(const BYTE bToEncode, int nSize, CString& strEncoded);
    unsigned short Exchange(unsigned short dwBye);
};

#endif // BASE64_H_

//BY GST ARMV8 GCC142.