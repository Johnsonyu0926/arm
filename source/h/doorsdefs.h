/*
 * $Id: doorsbases.h,v 1.31.2.1 2001/01/12 00:51:42 billy Exp $
 *
 * Author: Billy Shi (dxshi@yahoo.com)
 *
 * Copyright(C) 2001-2010 Doors Internet Software Co.,Ltd. 
 *
 */
#ifndef __DOORS_DEFS_H__
#define __DOORS_DEFS_H__

typedef long long               LONGLONG;
typedef unsigned long           ULONG;
typedef long int                INT32;
typedef int                     INT;
typedef unsigned int		UINT;
typedef unsigned long           DWORD;
typedef int                     BOOL;
typedef unsigned char           BYTE;
typedef unsigned short          WORD;
typedef short          		SHORT;
typedef unsigned short          USHORT;
typedef float                   FLOAT;
typedef long                    LONG;
typedef BOOL*                   LPBOOL;
typedef BYTE*                   LPBYTE;
typedef WORD*                   LPWORD;
typedef DWORD*                  LPDWORD;
typedef INT32*                  LPINT32;
typedef INT*                    LPINT;
typedef void                    VOID;
typedef void*                   LPVOID;
typedef const void*             LPCVOID;
typedef const char*             LPCSTR;
typedef LPCSTR			LPCTSTR;
typedef char*                   LPSTR;
typedef LPSTR			LPTSTR;
typedef const unsigned short*   LPCWSTR;
typedef unsigned short*         LPWSTR;
typedef char			TCHAR;
typedef unsigned short          WCHAR;
typedef int                     SOCKET;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (SOCKET)-1
#endif

class COracleReader;

/*
#include <stdio.h>
#include <string.h>

#define lstrcpyn	strncpy
#define wsprintf	sprintf
*/
class IDsObject
{
public:
	IDsObject(){}
	virtual ~IDsObject(){}
	virtual int AddRef(
				void) = 0;
	virtual int  ReleaseRef(
				void) = 0;
};

class CDsObject : public IDsObject
{
public:
	CDsObject()
	{
		m_dwRef = 0;
	}

	virtual ~CDsObject()
	{
	}

	inline int  AddRef(
				void)
	{
		 m_dwRef++;
		return m_dwRef;
	}

	inline int ReleaseRef(
				void)
	{
		m_dwRef--;
		int iRet = m_dwRef;
		if (m_dwRef == 0)
			delete this;

		return iRet;
	}

protected:

	DWORD		m_dwRef;
};

class IDsNetworkFilter : public CDsObject
{
public:
	//set
	/**
	 * If dwToTime is not 0 or null, it means user want to monitor history 
	 * records.
	 * If dwToTime is 0 or null or later than current time, it means user want 
	 * to monitor lively.
	 */
	virtual void SetTimeRange(
				DWORD			dwFromTime, 
				DWORD			dwToTime) = 0;

	virtual void IncludePort(
				int				port) = 0;
	virtual void ExcludePort(
				int				port) = 0;

	virtual void IncludeAddress(
				LPCSTR			ip_address) = 0;
	virtual void IncludeAddress(
				LPCSTR			ip_address1, 
				LPCSTR			ip_address2) = 0;

	virtual void ExcludeAddress(
				LPCSTR			ip_address) = 0;
	virtual void ExcludeAddress(
				LPCSTR			ip_address1, 
				LPCSTR			ip_address2) = 0;

	//check
	virtual BOOL CheckTime(
				DWORD			dwTime) = 0;

	virtual BOOL CheckPort(
				int				port) = 0;

	virtual BOOL CheckAddress(
				LPCSTR			ip_address1, 
				LPCSTR			ip_address2) = 0;
	virtual	BOOL CheckTimeAfter(DWORD dwTime)=0;
	virtual BOOL CheckTimeBefore(DWORD dwTime)=0;
	virtual DWORD GetFromTime(void)	=0;
	virtual BOOL IsRealTime(void)=0;
	virtual DWORD GetToTime(void)=0;
	
	//added by White 2000/12/20
	virtual void SetFilePath(LPCSTR file_path) = 0;
	virtual LPCSTR GetFilePath() = 0;
	virtual LPCSTR GetAddress() = 0;
	virtual WORD	GetPort() = 0;
	//added by jobs for tp to set/get http port
	//virtual void SetPort(WORD nPort)=0;
	//virtual WORD GetPort()=0; 
	
	//added by Whtie 2001/1/9
	virtual DWORD GetRealToTime()=0;
	virtual void SetRealToTime(DWORD dwTime)=0;
	virtual void SetInterval(DWORD)=0;//sec
	
	//Oliver added 2/19/2001 
	virtual void SetSrcToDst(LPCSTR pSrcIP,LPCSTR pDstIP) = 0;
		//return packet which src addr is pSrcipAddr and dst addr is pDstIpAddr,NULL stand for all.
	virtual LPCSTR GetSrcAddr() = 0;
	virtual LPCSTR GetDstAddr() = 0;
	virtual BOOL  CheckSrcAndDstAddr(LPCSTR lpSrcIPAddr,LPCSTR lpDstIPAddr) = 0;	

	virtual void SetLogTime(int nLogTime) = 0;
	virtual int GetLogTime() = 0;

	virtual int GetCacheCount() = 0;
	virtual void SetCacheCount(int nCount) = 0;
	virtual void SetOracle(COracleReader* oracle) = 0;
	virtual COracleReader* GetOracle() = 0;
};

extern "C"
{
	void DsCreateNetworkFilter(IDsNetworkFilter** ppNetFilter,DWORD* pdwTime);
}
#endif
