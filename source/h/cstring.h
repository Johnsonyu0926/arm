#ifndef	_CSTRING_H__
#define _CSTRING_H__

#include "Share.h"
#include "doorsbase.h"

#define	MAX_PRINTABLE_STRING_LENGTH	5000

class CString
{
public:
	CString();
	~CString();
	CString(CString& rhs);
	CString(const char *szBuf);

public:
	operator LPCTSTR ( ) const
	{
		return m_szBuf;
	}

	char operator [](int nIndex) const
	{
		if (!m_szBuf || nIndex <0 || nIndex >=m_nTotal || m_nTotal<=0)
			return GAP_INITIALIZER;

		return m_szBuf[nIndex];
	}

	CString& operator =(CString& rhs);
	CString& operator =(const char *szBuf);
	
	const CString operator +(CString& rhs);
	const CString operator +(const char *szBuf);

	CString& operator +=(CString& rhs);
	CString& operator +=(const char *szBuf);
	CString& operator +=(const char szCh);
	
	long GetLength();

	BOOL IsEmpty();
	void Empty();
		
	const char GetAt(int nIndex);
	void SetAt(int nIndex, const char ch);
	void Replace(const char szSourceCh, const char szDimCh);

	int Compare(const char *szBuf);//0代表两者相等，其他代表两者不等
	int CompareNoCase(const char *szBuf);//0代表两者相等，其他代表两者不等

	const CString Mid(int nFirst);
	const CString Mid(int nFirst, int nCount);
	const CString Right(int nCount);
	const CString Left(int nCount);

	void MakeUpper();
	void MakeLower();

	void TrimLeft();
	void TrimRight();

	int Find(const char *szValue, int nStart=0);//>=0代表找到， <0代表没找到
	int Find(const char szCh, int sStat=0);

	void Format(char *pFormat, ...);

private:
	BOOL GetValue(char *szValue);
	BOOL AddValue(const char *szValue, long nLen);
	BOOL AddValue(const char szCh, long nLen);

private:
	char *m_szBuf;
	BOOL m_bFirstAdd;
	long m_nTotal;

};

#endif
