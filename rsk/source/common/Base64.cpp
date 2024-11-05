#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "Base64.h"

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string CBase64::Encode(const unsigned char* Data, int DataByte) {
    std::string encoded_string;
    int i = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (DataByte--) {
        char_array_3[i++] = *(Data++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                encoded_string += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (int j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (int j = 0; (j < i + 1); j++)
            encoded_string += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            encoded_string += '=';
    }

    return encoded_string;
}

std::string CBase64::Decode(const char* Data, int DataByte, int& OutByte) {
    int i = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string decoded_string;

    while (DataByte-- && (Data[in_] != '=') && is_base64(Data[in_])) {
        char_array_4[i++] = Data[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                decoded_string += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (int j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (int j = 0; j < 4; j++)
            char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (int j = 0; (j < i - 1); j++) decoded_string += char_array_3[j];
    }

    OutByte = static_cast<int>(decoded_string.size());
    return decoded_string;
}

int CBase64::DecodeString(char *szSrc, CString &strDecoded) {
    strDecoded.Empty();
    char *p = szSrc;
    int nLength = strlen(szSrc);
    if (strstr(szSrc, "\r\n")) nLength -= 2;
    if (nLength % 4) return FALSE;

    char sz6Bits[5]{};
    char szTmp[10];
    for (int i = 0; i < nLength;) {
        strncpy(sz6Bits, p, 4);
        sz6Bits[4] = '\0';
        if (!Decode(sz6Bits, szTmp)) return FALSE;
        strDecoded += szTmp;
        p += 4;
        i += 4;
    }
    return TRUE;
}

int CBase64::AsciiTo6Bits(char cAsc) {
    if (cAsc >= 'A' && cAsc <= 'Z') return cAsc - 'A';
    if (cAsc >= 'a' && cAsc <= 'z') return (cAsc - 'a') + ('Z' - 'A' + 1);
    if (cAsc >= '0' && cAsc <= '9') return (cAsc - '0') + ('Z' - 'A' + 1) + ('z' - 'a' + 1);
    if (cAsc == '+') return 62;
    if (cAsc == '/') return 63;
    return -1;
}

int CBase64::Decode(const char *p6Bits, char *szDecoded) {
    char cFirst6Bits = AsciiTo6Bits(p6Bits[0]);
    char cSecond6Bits = AsciiTo6Bits(p6Bits[1]);
    char cTempHigh, cTempLow, cFirstLetter, cSecondLetter, cThirdLetter;

    cTempHigh = (cFirst6Bits << 2);
    cTempLow = cSecond6Bits >> 4;
    cFirstLetter = cTempHigh | cTempLow;

    if (p6Bits[2] == '=') {
        sprintf(szDecoded, "%c", cFirstLetter);
        szDecoded[1] = '\0';
        return TRUE;
    }

    char cThird6Bits = AsciiTo6Bits(p6Bits[2]);
    if (cThird6Bits < 0) return FALSE;

    cTempHigh = cSecond6Bits << 4;
    cTempLow = cThird6Bits >> 2;
    cSecondLetter = cTempHigh | cTempLow;

    if (p6Bits[3] == '=') {
        sprintf(szDecoded, "%c%c", cFirstLetter, cSecondLetter);
        szDecoded[2] = '\0';
        return TRUE;
    }

    char cFourth6Bits = AsciiTo6Bits(p6Bits[3]);
    if (cFourth6Bits < 0) return FALSE;

    cTempHigh = cThird6Bits << 6;
    cThirdLetter = cTempHigh | cFourth6Bits;

    sprintf(szDecoded, "%c%c%c", cFirstLetter, cSecondLetter, cThirdLetter);
    szDecoded[3] = '\0';

    return TRUE;
}

int CBase64::Decode(CString strBody, CString strFileName) {
    FILE* fp1 = fopen("body.txt", "w");
    if (!fp1) return FALSE;
    fputs(LPCTSTR(strBody), fp1);
    fclose(fp1);

    FILE* fp = fopen("body.txt", "r");
    if (!fp) return FALSE;
    char szBuf[1024 + 1];
    CString strBodyDecoded;

    FILE* fp2 = fopen(strFileName, "wb");
    if (!fp2) return FALSE;
    while (fgets(szBuf, 1024, fp)) DecodingString(szBuf, fp2);
    fclose(fp2);
    fclose(fp);

    return TRUE;
}

int CBase64::SetBodyText(CString strBody) {
    m_strBodyText = strBody;
    return TRUE;
}

int CBase64::GetBodyText(CString &strBodyText) {
    strBodyText = m_strBodyText;
    return TRUE;
}

int CBase64::Decode(char *p6Bits, FILE *fp) {
    char cFirst6Bits = AsciiTo6Bits(p6Bits[0]);
    char cSecond6Bits = AsciiTo6Bits(p6Bits[1]);
    char cTempHigh, cTempLow, cFirstLetter, cSecondLetter, cThirdLetter;

    cTempHigh = (cFirst6Bits << 2);
    cTempLow = cSecond6Bits >> 4;
    cFirstLetter = cTempHigh | cTempLow;

    fputc(cFirstLetter, fp);
    if (p6Bits[2] == '=') return TRUE;

    char cThird6Bits = AsciiTo6Bits(p6Bits[2]);
    if (cThird6Bits < 0) return FALSE;

    cTempHigh = cSecond6Bits << 4;
    cTempLow = cThird6Bits >> 2;
    cSecondLetter = cTempHigh | cTempLow;

    fputc(cSecondLetter, fp);
    if (p6Bits[3] == '=') return TRUE;

    char cFourth6Bits = AsciiTo6Bits(p6Bits[3]);
    if (cFourth6Bits < 0) return FALSE;

    cTempHigh = cThird6Bits << 6;
    cThirdLetter = cTempHigh | cFourth6Bits;

    fputc(cThirdLetter, fp);
    return TRUE;
}

int CBase64::DecodingString(char *szSrc, FILE *fp) {
    char *p = szSrc;
    int nLength = strlen(szSrc);
    nLength -= 2;
    if (nLength % 4) return FALSE;
    char sz6Bits[5]{};

    for (int i = 0; i < nLength;) {
        strncpy(sz6Bits, p, 4);
        sz6Bits[4] = '\0';
        Decode(sz6Bits, fp);
        p += 4;
        i += 4;
    }
    return TRUE;
}

int CBase64::DecodeBody(CString strSrc, CString &strDecoded) {
    strDecoded.Empty();
    CMemFile mmfile;
    if (!DecodeBody(strSrc, mmfile)) return FALSE;

    int nRead;
    mmfile.SeekToBegin();
    char szBuf[BUF_MAX + 1];
    while (1) {
        nRead = mmfile.Read(szBuf, BUF_MAX);
        szBuf[nRead] = '\0';
        strDecoded += szBuf;
        if (nRead < BUF_MAX) break;
    }
    return TRUE;
}

int CBase64::DecodeStr(LPCTSTR szDecoding, LPTSTR szOutput) {
    CString sInput(szDecoding);
    if (sInput.GetLength() == 0) return 0;

    int nDecode[256];
    for (int i = 0; i < 256; i++) nDecode[i] = -2;
    for (int i = 0; i < 64; i++) {
        nDecode[m_sBase64Alphabet[i]] = i;
        nDecode[m_sBase64Alphabet[i] | 0x80] = i;
        nDecode['='] = -1;
        nDecode['=' | 0x80] = -1;
    }

    memset(szOutput, 0, sInput.GetLength() + 1);
    int lp = 0;
    for (int i = 0; lp < sInput.GetLength(); lp++) {
        int c = sInput[lp];
        int nDigit = nDecode[c & 0x7F];
        if (nDigit < -1) return 0;
        else if (nDigit >= 0) write_bits(nDigit & 0x3F, 6, szOutput, i);
    }
    return lp;
}

void CBase64::write_bits(UINT nBits, int nNumBits, LPTSTR szOutput, int &i) {
    m_lBitStorage = (m_lBitStorage << nNumBits) | nBits;
    m_nBitsRemaining += nNumBits;
    while (m_nBitsRemaining > 7) {
        UINT nScratch = m_lBitStorage >> (m_nBitsRemaining - 8);
        szOutput[i++] = nScratch & 0xFF;
        m_nBitsRemaining -= 8;
    }
}

int CBase64::Decode(const char* p6Bits, char* szDecoded, int &nBytes) {
    char cFirst6Bits = AsciiTo6Bits(p6Bits[0]);
    char cSecond6Bits = AsciiTo6Bits(p6Bits[1]);
    char cTempHigh, cTempLow, cFirstLetter, cSecondLetter, cThirdLetter;

    cTempHigh = (cFirst6Bits << 2);
    cTempLow = cSecond6Bits >> 4;
    cFirstLetter = cTempHigh | cTempLow;
    szDecoded[0] = cFirstLetter;

    if (p6Bits[2] == '=') {
        szDecoded[1] = '\0';
        nBytes = 1;
        return TRUE;
    }

    char cThird6Bits = AsciiTo6Bits(p6Bits[2]);
    if (cThird6Bits < 0) return FALSE;

    cTempHigh = cSecond6Bits << 4;
    cTempLow = cThird6Bits >> 2;
    cSecondLetter = cTempHigh | cTempLow;
    szDecoded[1] = cSecondLetter;

    if (p6Bits[3] == '=') {
        szDecoded[2] = '\0';
        nBytes = 2;
        return TRUE;
    }

    char cFourth6Bits = AsciiTo6Bits(p6Bits[3]);
    if (cFourth6Bits < 0) return FALSE;

    cTempHigh = cThird6Bits << 6;
    cThirdLetter = cTempHigh | cFourth6Bits;
    szDecoded[2] = cThirdLetter;
    szDecoded[3] = '\0';
    nBytes = 3;
    return TRUE;
}

int CBase64::DecodeString(char* szSrc, char* szDecoded, int &nLineBytes) {
    if (!szSrc || !szDecoded) return FALSE;

    nLineBytes = 0;
    char *p = szSrc;
    int nLength = strlen(szSrc);
    if (strstr(szSrc, "\r\n")) nLength -= 2;
    if (nLength % 4) return FALSE;

    char sz6Bits[5]{};
    for (int i = 0; i < nLength;) {
        strncpy(sz6Bits, p, 4);
        sz6Bits[4] = '\0';
        int nBytes = 0;
        if (!Decode(sz6Bits, szDecoded + nLineBytes, nBytes)) return FALSE;
        nLineBytes += nBytes;
        p += 4;
        i += 4;
    }
    return TRUE;
}

int CBase64::DecodeBody(CString strSrc, CMemFile &mmfile) {
    CString strBuf;
    int nFind = 0;

    while ((nFind = strSrc.Find("\r\n")) > 0) {
        char szBuf[77]{};
        strncpy(szBuf, LPCTSTR(strSrc.Mid(0, nFind)), nFind);
        szBuf[nFind] = '\0';

        int nLineBytes = 0;
        char szDecoded[57]{};
        if (!DecodeString(szBuf, szDecoded, nLineBytes)) return FALSE;
        mmfile.Write(szDecoded, nLineBytes);

        strSrc = strSrc.Mid(nFind + 2);
    }
    return TRUE;
}

CString CBase64::Encode(LPCTSTR szEncoding, int nSize) {
    CString sOutput;
    int nNumBits = 6;
    UINT nDigit;
    int lp = 0;

    m_szInput = szEncoding;
    m_nInputSize = nSize;
    m_nBitsRemaining = 0;

    nDigit = read_bits(nNumBits, &nNumBits, lp);
    while (nNumBits > 0) {
        sOutput += m_sBase64Alphabet[(int) nDigit];
        nDigit = read_bits(nNumBits, &nNumBits, lp);
    }
    while (sOutput.GetLength() % 4 != 0) {
        sOutput += '=';
    }
    return sOutput;
}

UINT CBase64::read_bits(int nNumBits, int *pBitsRead, int &lp) {
    ULONG lScratch;
    while ((m_nBitsRemaining < nNumBits) && (lp < m_nInputSize)) {
        int c = m_szInput[lp++];
        m_lBitStorage <<= 8;
        m_lBitStorage |= (c & 0xff);
        m_nBitsRemaining += 8;
    }
    if (m_nBitsRemaining < nNumBits) {
        lScratch = m_lBitStorage << (nNumBits - m_nBitsRemaining);
        *pBitsRead = m_nBitsRemaining;
        m_nBitsRemaining = 0;
    } else {
        lScratch = m_lBitStorage >> (m_nBitsRemaining - nNumBits);
        *pBitsRead = nNumBits;*********
        m_nBitsRemaining -= nNumBits;
    }
    return (UINT) lScratch & m_nMask[nNumBits];
}
