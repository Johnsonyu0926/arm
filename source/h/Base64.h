#ifndef _BASE64_H__
#define _BASE64_H__

#include "doorsbase.h"
#include "memfile.h"
#include "cstring.h"
#include <vector>
#include <string>

class CBase64 {
public:
    CBase64();
    virtual ~CBase64();

    int DecodeBody(const CString& strSrc, CMemFile& mmfile);
    int DecodeString(const char* szSrc, CString& strDecoded);
    CString Encode(const char* szEncoding, int nSize);
    int DecodeBody(const CString& strBody, CString& strDecoded);
    int DecodeEx(const char* szSrc, int nSize, BYTE* pBuf);

private:
    int DecodeStr(const char* szDecoding, char* szOutput);
    int Decode(const char* p6Bits, FILE* fp);
    int GetBodyText(CString& strBodyText);
    int Decode(const char* p6Bits, char* szDecoded);
    int Decode(const char* p6Bits, char* szDecoded, int& nBytes);
    int DecodeString(const char* szSrc, char* szDecoded, int& nLineBytes);
    int AsciiTo6Bits(char cAsc);

    void write_bits(unsigned int nBits, int nNumBits, char* szOutput, int& i);
    int DecodingString(const char* szBuf, FILE* fp);
    int SetBodyText(const CString& strBody);
    int Decode(const CString& strBody, const CString& strFileName);
    unsigned int read_bits(int nNumBits, int* pBitsRead, int& lp);

    int m_nInputSize{0};
    int m_nBitsRemaining{0};
    unsigned long m_lBitStorage{0};
    const char* m_szInput{nullptr};

    static const int m_nMask[];
    static const CString m_sBase64Alphabet;
    CString m_strBodyText;
};

class CQp {
public:
    int Decode(const CString& strBody, CMemFile& mmfile);
    int Decode(const CString& szSrc, CString& szDecoded);
    CQp();
    virtual ~CQp();

private:
    int GetHex(char szHex);
    int GetCharByHex(const char* szHex, char& szChar);
};

class CUtf7 {
public:
    int DecodeEx(const char* szBuf, char* szDecoded);
    int Decode(const char* p, char* pBuf);
    int Encode(const char* p, char* pBuf);
    CUtf7();
    virtual ~CUtf7();

private:
    int Decode(const CString& strSrc, CString& strDecoded);
    int Encode(const char* szSrc, CString& szEncoded);
    int DealWithUtf7(const char* p, char* pBuf);
    int IsPrintable(char c);
    int HandleTheEncodedString(CString& strEncoded);
    int HandleTheDecodeString(CString& strSrc);
    int Encode(const BYTE* bToEncode, int nSize, CString& strEncoded);
    unsigned short Exange(unsigned short dwBye);
};

#endif // _BASE64_H__