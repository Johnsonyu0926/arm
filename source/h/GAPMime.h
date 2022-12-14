#ifndef	_GAPMIME_H__
#define	_GAPMIME_H__


////////////////////////////////////////////////////////////////////////////////////
//类的定义：

#include "Base64.h"

class CGAPMime
{
public:
	//public function;
	CGAPMime();
	virtual ~CGAPMime();
	BOOL Init(const BYTE *pBody);
	BOOL Work(long *pnSuccess);
	BOOL SetBodyLen(LONG nBodyLen);
	BOOL GetPartCount(LONG *pnCount);
	BOOL GetPart(int nIndex, int nType, BYTE *pnVal);

	//added by billy.
	char* GetSubject() { return m_szSubject;} //return the subject.
	char* GetFrom() { return m_szFrom;} //return the mail from 
	char* GetTo() { return m_szTo;}
	char* GetCc() { return m_szCc;}
	char* GetDate() { return m_szDate; }
private:
	char m_szCc[1024];
	char m_szTo[1024];
	char m_szFrom[1024];
	char m_szDate[256];
	//because we want to check the subject.
private:
	//added by billy.
	char* GetValueFromKey(char* szLine,char* szKey);
	//private function;
	BOOL GetContentType(int& nContentType, char *pBoundary, 
						int& nEncodingType, char *pFileName);
	BOOL GetContentType(char *szBuf, char *szContentType);
	int GetContentType(char *szContentType, int &nContentType);
	BOOL GetLine(char *szBuf);
	int GetFieldValue(char *szBuf, int nPos,char *szValue);	
	int GetEncodingType(char *szBuf, int &nEncodingType);

	int DealWithTheContentType(int nContentType, char *szBoundary,
								  int nEncodingType, char *szFileName);
	int DealWithMultPart(int nContentType, char *szBoundary,
							int nEncodingType, char *szFileName);
	int DealWithSinglePart(int nContentType, char *szBoundary,
							int nEncodingType, char *szFileName);
	int GotoBoundary(char *szBoundary);
	int DealWithTextPlain(int nContextType,int nEncodingType,char *szBoundary);
	int DealWithOct(int nEncodingType, char *szBoundary, char *szFileName);
	int DealWith7BitsPlainText(int nContextType, char *szBoundary);
	int DealWithBase64PlainText(char *szBoundary);
	int DealWithQP(char *szBoundary);
	int RecvBodyAndDecode(char *szFileName, char *szBoundary);

public:
	//public var;

private:
	//private var;
	const BYTE *m_pBody;
	int m_nOffset;
	//CArray<CMimeInfo,CMimeInfo> m_arrMimeInfo;
	int	m_nBodyLen;
	int m_nCurrentAttachmentCount;
	char m_szAttachmentName[128][1024];

private:
	//链表操作
	LONG m_nPartCount;
	Gap_BodyPart_t m_pBodyPart;

	Gap_BodyPart_t BodyContentCreate();
	Gap_BodyPart_t BodyContentInsert(Gap_BodyPart_t pBodyPart, Gap_BodyPart_t szBodyPart);
	void BodyContentDestroy(Gap_BodyPart_t *pBodyPart);
	void BodyContentDestroyAll(Gap_BodyPart_t *pBodyPart);
	Gap_BodyPart_t BodyContentDelete(Gap_BodyPart_t pBodyPart, int nPart);

	char m_szSubject[1024];
};

extern void ToMakeUpper(char *pStr);

#endif
