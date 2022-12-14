// Base64.h: interface for the CBase64 class.
//
//////////////////////////////////////////////////////////////////////
#ifndef	_BASE64_H__
#define	_BASE64_H__

#include "doorsbase.h"
#include "memfile.h"
#include "cstring.h"

class CBase64  
{
public:
	CBase64();
	virtual ~CBase64();

	
	int DecodeBody(CString strSrc,CMemFile& mmfile);
	
	//对一个可能包含有\r\n的字符串如中文文件名或者是明文TEXT/PLAIN类型的邮件内容
	//进行解码，这样解出的的结果是明文，所以可以直接返回串型值
	/*
		char szDecode[1024];
			strcpy(szDecode,LPCTSTR(strFileName));
			base64.DecodeString(szDecode,strFileName);
	**/

	int DecodeString(char *szSrc, CString& strDecoded);

	//对一个字符串进行编码，编码结果存入CString对象中返回
	//用户：CUtf7
	CString Encode(LPCTSTR szEncoding,int nSize);
	
	int DecodeBody(CString strBody,CString& strDecoded);
	//解码：UTF7
	int DecodeEx(const char* szSrc,int nSize,BYTE* pBuf);
private:
	

	int DecodeStr(LPCTSTR szDecoding, LPTSTR szOutput);

	int Decode(char* p6Bits,FILE* fp);
	int GetBodyText(CString& strBodyText);
	
	//CBase64(CStreamSocket* pSocket);
	//int RecvBodyAndDecode(CString strFileName,
	//	CString strBoundary);

	int Decode(const char* p6Bits,char* szDecoded);
	int Decode(const char* p6Bits,char* szDecoded,int& nBytes/*返回字节数*/);
	int DecodeString(char* szSrc,char* szDecoded,int& nLineBytes);
//	int DecodeBody(CString strSrc,CMemFile& mmfile);
	//分解的结果存入CMemFile& mmfile中

	int AsciiTo6Bits(char cAsc);

private:
	void write_bits(UINT nBits,
						 int nNumBits,
						 LPTSTR szOutput,
						 int& i);

	int DecodingString(char* szBuf,FILE* fp);
	CString m_strBodyText;
	int SetBodyText(CString strBody);
	int Decode(CString strBody,CString strFileName);
	//CStreamSocket* m_pClientSocket;

	UINT read_bits(int nNumBits, int * pBitsRead, int& lp);

	int m_nInputSize;
	int m_nBitsRemaining;
	ULONG m_lBitStorage;
	LPCTSTR m_szInput;

	static int m_nMask[];
	static CString m_sBase64Alphabet;
};

class CQp  
{
public:
	int Decode(CString strBody,CMemFile& mmfile);
	int Decode(CString szSrc,CString& szDecoded);
	CQp();
	virtual ~CQp();

	
private:
	int GetHex(char szHex);
	int GetCharByHex(char* szHex,char& szChar);
};

#define STR_MAX 1024
class CUtf7  
{
public:
	int DecodeEx(const char* szBuf,char* szDecoded);
	//add by sdx 4-1
	int Decode(const char* p,char* pBuf);
	int Encode(const char* p,char* pBuf);
	CUtf7();
	virtual ~CUtf7();
  //end
private:

	int Decode(CString strSrc,CString& strDecoded);

	int Encode(const char* szSrc,CString& szEncoded);




	//add 4-1
	
	int DealWithUtf7(const char* p,char* pBuf);
	int IsPrintable(char c);
	int HandleTheEncodedString(CString& strEncoded);
	int HandleTheDecodeString(CString& strSrc);
	//add end
  
	int Encode(BYTE* bToEncode,int nSize,CString& strEncoded);
	unsigned short Exange(unsigned short dwBye);
};

#endif 
