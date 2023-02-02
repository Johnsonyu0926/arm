/*
 * $Id: doorsbases.h,v 1.31.2.1 2001/01/12 00:51:42 billy Exp $
 */
/*
 * Copyright (C) 2001-2010 Doors Internet Software Co.,Ltd.
 * All rights reserved.
 *
 * Author
 *	 Dongxue Shi <dxshi@yahoo.com>
 *
 * History
 *	2001/10/01
 */
#ifndef __DOORS_BASE_H__
#define __DOORS_BASE_H__

/*
 * For ssl
 */
#include <openssl/rsa.h> /* SSLeay stuff */
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/md5.h>
#include <openssl/err.h>

#ifndef __linux__
#include <iostream>
#endif

#include <pthread.h>

#include <ctype.h> //for toupper
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>	   // for FILE
#include <string.h>	   //for memset
#include <sys/types.h> //for AF_INET
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include "doorsdefs.h"

#define CopyMemory(Dest, Src, Len) memcpy((Dest), (Src), (Len))
#define MoveMemory(Dest, Src, Len) memmove((Dest), (Src), (Len))
#ifdef endl
#undef endl
#define endl "\n"
#endif

#define INET_ADDRSTRLEN 16	/* for IPV4 dotted-decimal */
#define INET6_ADDRSTRLEN 46 /* FOR IPV6 hex string*/

#define INTERVAL_TIME 60
//#define _DEBUG

#ifdef _DEBUG
#include <assert.h>
#ifndef ASSERT
#define ASSERT assert
#endif
#else
#undef ASSERT
#define ASSERT(x) ((void)0)
#endif
#define DS_ASSERT ASSERT

#ifndef WIN32
#undef _stricmp
#define _stricmp strcasecmp
#endif

class CByteStream;
#define CONF_FILE "../etc/proxy.cfg"

#define TIMEOUT -2

class CConfig
{
public:
	int Read(char *szSess, char *szItem, char *szValue, char *szDefault);

public:
	CConfig(char *szIniFile);
	~CConfig();
	BOOL Init(char *szInitFile)
	{
		strcpy(m_szFile, szInitFile);
		return TRUE;
	}
	CConfig() {}

private:
	FILE *m_pFp;
	char m_szFile[512];
};

typedef enum enSHUTDOWN
{
	SHUTDOWN_RECEIVE = 0,
	SHUTDOWN_SEND = 1,
	SHUTDOWN_BOTH = 2
} SHUTDOWN;

typedef enum enSockType
{
	TCP,
	UDP,
	ARP,
	ALL,
	RAW
} SOCKTYPE;

#define BUF_MAX 4096
class CSocket
{
public:
	CSocket();
	CSocket(SOCKET s);

	~CSocket();

public:
	SOCKET m_hSocket;
	BOOL Create(IN SOCKTYPE nSockType);
	// nType=TCP/UDP/RAW

	SOCKET GetHandle();

	// for client:
	BOOL Connect(IN const char *szServAddr, IN int nServPort);
	BOOL Connect(IN DWORD dwIp, IN int nServPort);
	BOOL ConnectTimeOut(const char *szHost, unsigned short nPort, long timeout);

	// for server:

	BOOL Bind(IN int nBindPort);
	int Bind(); // return the port
	BOOL Listen();
	BOOL Accept(CSocket *clnaccept); //
									 //			struct sockaddr_in* pclient); //OK,use this is recomment
	// BOOL Accept(CSocket& clientsock);
	void SetPeerPort(int nPeerPort);
	void SetPeerIp(DWORD dwIp);

	// For tcp:
	int Send(const void *buf, int nBytes);
	// int Send(IN char* szWillSend,IN int nWill);
	int Send(IN char *szWillSend);
	// int Recv(OUT char* szRecvBuf,IN int nBufSize);

	int Recv(OUT void *szRecvBuf, IN int nBufSize);
	int Recv(OUT char *szRecvBuf);

	int Recv(OUT char *szRecvBuf, IN int nBufSize, IN int nTimeOut);
	int Recv(OUT char *szRecvBuf, IN int nBufSize, IN int nTimeOut, BOOL bSure);
	// recv time out in seconds

	// helper
	BOOL InetN2P(void *pSinAddr, char *szPtr, int nLen, int nFamily = AF_INET);

	BOOL InetP2N(char *szIP, void *pSinAddr, int nFamily = AF_INET);
	DWORD InetP2N(char *szIP, int nFamily = AF_INET);

	// For udp:
	int RecvFrom(OUT void *szRecvBuf, IN int nBufSize,
				 OUT struct sockaddr_in *pFromAddr = NULL, OUT int *nAddrLen = NULL);
	int RecvFrom(OUT void *szRecvBuf, IN int nBufSize, long timeout,
				 OUT struct sockaddr_in *pFromAddr = NULL, OUT int *nAddrLen = NULL);
	int RecvFrom(OUT void *szRecvBuf, IN int nBufSize,
				 OUT char *szFromWhere,
				 IN int nFromSize,
				 OUT int &nFromPort);

	/* Added by shidongxue . for raw socket */
	int SendTo(
		IN void *szWillSend,
		IN int nWant,
		IN char *szInterfaceName);

	int SendTo(
		IN void *szWillSend,
		IN int nWant,
		IN int nToAddrLen,
		IN struct sockaddr_in *pToAddr);

	int SendTo(
		IN const void *szWillSend,
		IN int nWant,
		IN const char *szDotAddr,
		IN int nPort);

	void SetSocket(SOCKET s);
	BOOL Close();
	int RecvLine(char *szBuf, int nBufSize);

	BOOL GetMyIp(char *szIp);
	char *GetRemoteIp();
	DWORD GetPeerIp();
	USHORT GetPeerPort();
	int ConnectionReset(); // reset by peer, disconnect it;
	BOOL Shutdown(int nHow);

	int flags;
	BOOL SetNonBlock()
	{
		flags = fcntl(m_hSocket, F_GETFL, 0);
		fcntl(m_hSocket, F_SETFL, flags | O_NONBLOCK); // Modified by billy. 2004/11/23
		return TRUE;
	}
	BOOL ResetBlock()
	{
		fcntl(m_hSocket, F_SETFL, flags); // Modified by billy. 2004/11/23
		return TRUE;
	}

private:
	BOOL m_bConnected;

protected:
	BOOL ConnectIp(const char *szIp, int nPort);

private:
	DWORD m_dwPeerIp;
	int m_nPeerPort;

public:
	static BOOL QueryDns(char *szAddress, const char *szHost);
	static BOOL IsDotIp(const char *szHost);
};

class CDsList : public CDsObject
{
public:
	CDsList();
	virtual ~CDsList();

	BOOL Add(LPVOID newElement);

	BOOL InsertAt(int nIndex, LPVOID newElement);
	BOOL Remove(LPVOID oldElement);
	BOOL RemoveAt(int nIndex);
	void RemoveAll();

	int GetCount();
	LPVOID LastElement();
	LPVOID FirstElement();
	LPVOID ElementAt(int nIndex);

	BOOL IsEmpty();

	// additional
	BOOL CopyFrom(CDsList &src);
	BOOL Merge(CDsList &src);

protected:
	BOOL SetSize(int nNewSize);

protected:
	LPVOID *m_pData;
	int m_nSize;
};

#define UTIL_EXPORTMODIFIER
//#define CopyMemory	memcpy
//#define MoveMemory	memmove
#define DECREASE_DELTA 512
#define INCREASE_DELTA 16
typedef void *MAPPOS;

class UTIL_EXPORTMODIFIER CInternalVector
{
public:
	CInternalVector();
	CInternalVector(LPVOID *ppHead, int nSize);
	virtual ~CInternalVector();

public:
	BOOL Add(LPVOID lpv);
	BOOL InsertAt(int nIndex, LPVOID lpv);
	BOOL Remove(LPVOID lpv);
	BOOL RemoveAt(int i);
	void RemoveAll();
	// CInternalVector* Clone();

	int GetCount();

	LPVOID operator[](int i);
	BOOL SetAt(int nIndex, LPVOID lpv);
	int GetIndex(LPVOID lpv);
	LPVOID *GetData();

protected:
	BOOL Expand();

protected:
	LPVOID *m_ppHead;
	int m_nAlloc;
	int m_nUsed;
};

/////////////////////////////////////////////////////////////////////////////
// CInternalMap
//
class UTIL_EXPORTMODIFIER CInternalMap
{
protected:
	// Association
	struct CAssoc
	{
		void *key;
		void *value;
	};

public:
	CInternalMap(UINT tablesize = 17);
	virtual ~CInternalMap();

public:
	int GetCount();
	BOOL IsEmpty();
	UINT GetHashTableSize();

	BOOL SetAt(void *key, void *newValue);

	BOOL Lookup(void *key, void *&rValue);

	void *GetValueAt(void *key);
	void *operator[](void *key);

	BOOL RemoveKey(void *key);
	void RemoveAll();

	MAPPOS GetStartPosition();
	void GetNextAssoc(MAPPOS &rNextPosition,
					  void *&rKey, void *&rValue);

	virtual UINT HashKey(void *key);
	virtual BOOL Equals(void *key1, void *key2);

protected:
	CInternalVector *m_pHashTable;
	UINT m_nHashTableSize;
	int m_nCount;

	CAssoc *GetAssocAt(void *, UINT &);
};

#define MAX_PRINTABLE_STRING_LENGTH 5000

class CPrintableString
{
public:
	CPrintableString()
	{
		memset(m_buf, 0, MAX_PRINTABLE_STRING_LENGTH + 1);
		m_len = 0;
		m_bPrintBinary = FALSE;
	}

	~CPrintableString()
	{
	}

public:
	inline CPrintableString &operator<<(char ch)
	{
		DS_ASSERT(!m_bPrintBinary);

		if (m_len + 1 < MAX_PRINTABLE_STRING_LENGTH)
			m_len += sprintf(m_buf + m_len, "%c", ch);

		return *this;
	}

	inline CPrintableString &operator<<(unsigned char ch)
	{
		DS_ASSERT(!m_bPrintBinary);

		if (m_len + 1 < MAX_PRINTABLE_STRING_LENGTH)
			m_len += sprintf(m_buf + m_len, "%c", ch);

		return *this;
	}

	inline CPrintableString &operator<<(short s)
	{
		if (m_bPrintBinary)
			PrintBinary(s);
		else
		{
			if (m_len + 6 < MAX_PRINTABLE_STRING_LENGTH)
				m_len += sprintf(m_buf + m_len, "%d", s);
		}

		return *this;
	}

	inline CPrintableString &operator<<(unsigned short s)
	{
		if (m_bPrintBinary)
			PrintBinary(s);
		else
		{
			if (m_len + 5 < MAX_PRINTABLE_STRING_LENGTH)
				m_len += sprintf(m_buf + m_len, "%u", s);
		}

		return *this;
	}

	inline CPrintableString &operator<<(int i)
	{
		if (m_bPrintBinary)
			PrintBinary(i);
		else
		{
			if (m_len + 11 < MAX_PRINTABLE_STRING_LENGTH)
				// m_len += sprintf(m_buf + m_len, "%ld", i);
				m_len += sprintf(m_buf + m_len, "%ld", (long)i);
			// Checkhere
		}

		return *this;
	}

	inline CPrintableString &operator<<(unsigned int i)
	{
		if (m_bPrintBinary)
			PrintBinary(i);
		else
		{
			if (m_len + 10 < MAX_PRINTABLE_STRING_LENGTH)
				// m_len += sprintf(m_buf + m_len, "%lu", i);
				m_len += sprintf(m_buf + m_len, "%lu", (long)i);
			// Checkhere
		}

		return *this;
	}

	inline CPrintableString &operator<<(long l)
	{
		if (m_bPrintBinary)
			PrintBinary(l);
		else
		{
			if (m_len + 11 < MAX_PRINTABLE_STRING_LENGTH)
				m_len += sprintf(m_buf + m_len, "%ld", l);
		}

		return *this;
	}

	inline CPrintableString &operator<<(unsigned long l)
	{
		if (m_bPrintBinary)
			PrintBinary(l);
		else
		{
			if (m_len + 10 < MAX_PRINTABLE_STRING_LENGTH)
				m_len += sprintf(m_buf + m_len, "%lu", l);
		}

		return *this;
	}

	inline CPrintableString &operator<<(float f)
	{
		DS_ASSERT(!m_bPrintBinary);

		if (m_len + 12 < MAX_PRINTABLE_STRING_LENGTH)
			m_len += sprintf(m_buf + m_len, "%ld.", (long)f);

		return *this;
	}

	inline CPrintableString &operator<<(double d)
	{
		DS_ASSERT(!m_bPrintBinary);

		if (m_len + 12 < MAX_PRINTABLE_STRING_LENGTH)
			m_len += sprintf(m_buf + m_len, "%ld.", (long)d);

		return *this;
	}

	inline CPrintableString &operator<<(LPCSTR lpsz)
	{
		DS_ASSERT(!m_bPrintBinary);

		if (lpsz && *lpsz)
		{
			int len = strlen(lpsz);

			if (m_len + len > MAX_PRINTABLE_STRING_LENGTH)
				len = MAX_PRINTABLE_STRING_LENGTH - m_len;

			memcpy(m_buf + m_len, lpsz, len);
			m_len += len;
			m_buf[m_len] = 0;
		}

		return *this;
	}

	// in Hex
	inline CPrintableString &operator<<(LPVOID lpsz)
	{
		DS_ASSERT(!m_bPrintBinary);

		if (m_len + 10 < MAX_PRINTABLE_STRING_LENGTH)
			// m_len += sprintf(m_buf + m_len, "0x%lX", lpsz);
			// Modified by Billy.
			m_len += sprintf(m_buf + m_len, "0x%lX", *(long *)lpsz);

		return *this;
	}

	inline CPrintableString &operator<<(LPBYTE lpsz)
	{
		DS_ASSERT(!m_bPrintBinary);

		m_lpBinary = lpsz;
		m_bPrintBinary = TRUE;

		return *this;
	}

	inline operator LPCSTR()
	{
		return m_buf;
	}

private:
	inline void PrintBinary(DWORD dwLen)
	{
		m_bPrintBinary = FALSE;

		*this << '{';
		if (m_lpBinary)
		{
			for (DWORD i = 0; i < dwLen; i++)
			{
				*this << (int)m_lpBinary[i];
				*this << ',';
			}
		}
		*this << '}';
	}

	TCHAR m_buf[MAX_PRINTABLE_STRING_LENGTH + 1];
	int m_len;
	BOOL m_bPrintBinary;
	LPBYTE m_lpBinary;
};

void DsDebugInfo(char *lpFormat, ...);
void DsLog(char *lpFormat, ...);
#ifdef _DEBUG
#define DS_TRACE(str)                             \
	do                                            \
	{                                             \
		CPrintableString ps;                      \
		DsDebugInfo((char *)(LPCSTR)(ps << str)); \
	} while (0)
#else
#define DS_TRACE(str) (void)0
#endif
#define DS_LOG(str)                         \
	do                                      \
	{                                       \
		CPrintableString ps;                \
		DsLog((char *)(LPCSTR)(ps << str)); \
	} while (0)
//#define DS_TRACE(str) {}
// SSL

typedef enum enSSLTYPE
{
	SSL_SERVER = 1,
	SSL_CLIENT
} SSLTYPE;

class CDsSSL
{
public:
	CDsSSL(SSLTYPE nType);
	~CDsSSL();

public:
	BOOL Init();
	BOOL Create(SOCKET hSocket);
	int Send(char *szBuf, int nLen);
	int Recv(char *szBuf, int nBufSize);

private:
	SSL_CTX *ctx;
	SSL *ssl;

private:
	BOOL InitServer();
	BOOL InitClient();
	BOOL CreateServer(SOCKET hSocket);
	BOOL CreateClient(SOCKET hSocket);

private:
	int m_nType;
};

class CHostNode : public CDsObject
{
public:
	char m_szHost[256];
	char m_szIp[32];
	int m_nCount;

public:
	CHostNode()
	{
		memset(m_szHost, 0, 256);
		memset(m_szIp, 0, 32);
		m_nCount = 0;
	}
	~CHostNode() {}
};
class CDnsCache
{
public:
	CDnsCache();
	virtual ~CDnsCache();

public:
	virtual char *Query(char *szHost);
	virtual BOOL Init();
	virtual BOOL DumpToFile();

private:
	virtual char *SubQuery(char *szHost);
	virtual BOOL Work(char *szHost, char *szIp);

private:
	CDsList m_list;

	char m_szFile[256];
};

typedef enum
{
	DSERR_CONNECT = 0,
	DSERR_DNSQUERY,
	DSERR_PROTO_NOT_SUPPORT,
	DSERR_PARSE,
	DSERR_DISCONNECT_BY_REMOTE
} DSERRTYPE;

class CDsErrTip
{
public:
	CDsErrTip();
	~CDsErrTip();

public:
	// op
	BOOL SetTipType(DSERRTYPE type);
	int GetTipLen();
	BYTE *GetTip();

private:
	BOOL LoadFileToMem(char *szFile);

private:
	BYTE *m_pTip;
	int m_nTipLen;
};

static pthread_mutex_t g_threadcountLock = PTHREAD_MUTEX_INITIALIZER;
static int _threadcount = 0;
class CSThread
{
public:
	CSThread() : m_bAutoDelete(TRUE), m_bJoinable(FALSE)
	{
		 DS_TRACE("[CSThread::CSThread] Constuctor function called.");
	}
	virtual ~CSThread()
	{
		 DS_TRACE("[CSThread::~CSThread] Destroy function called");
	}

public:
	virtual BOOL InitInstance() = 0;
	virtual BOOL ExitInstance() = 0;
	BOOL SetAutoDelete(BOOL b)
	{
		m_bAutoDelete = b;
		return TRUE;
	}
	void SetJoinable(BOOL b) { m_bJoinable = b; }
	pthread_t CreateThread()
	{
		pthread_mutex_lock(&g_threadcountLock);
		_threadcount++;
		pthread_mutex_unlock(&g_threadcountLock);
		DS_TRACE("[CSThread::CreateThread] Creating " << _threadcount << " thread.");
		if (pthread_create(&m_threadId, NULL, CSThread::_threadfunc, this) == 0)
		{
			DS_TRACE("[CSThread::CreateThread] Create thread " << m_threadId << " success!");
			return m_threadId;
		}
		else
		{
			DS_TRACE("[CSThread::CreateThread] Failed create thread  " << _threadcount << "! program exit abnormal!");
			exit(0);
		}
		return FALSE;
	}

protected:
	BOOL Run()
	{
		if (!m_bJoinable)
			pthread_detach(pthread_self()); // This is detach thread, it can not joinable!
		InitInstance();
		pthread_mutex_lock(&g_threadcountLock);
		_threadcount--;
		pthread_mutex_unlock(&g_threadcountLock);
		ExitInstance();
		if (m_bAutoDelete)
		{
            DS_TRACE("[CSThread::ThreadFunc] delete myself! thread count = " << _threadcount << "\n");
			delete this;
		}
		return TRUE;
	}
	static void *_threadfunc(void *pParam)
	{

		CSThread *pObj = (CSThread *)pParam;
		pObj->Run();
		return NULL;
	}

protected:
	BOOL m_bAutoDelete;
	BOOL m_bJoinable;
	pthread_t m_threadId;

public:
	pthread_t GetThreadId()
	{
		DS_TRACE("[CSThread::GetThreadId] the thread id is: " << m_threadId);
		return m_threadId;
	}
};

extern char *DsToUpper(char *szSrc);

// debug mode
//  wwwproxy 0 = release version. do not write log and output any info
//  wwwproxy 1 = unusefull
//  wwwproxy 5 = only output to the terminal
//  wwwproxy 3 = only write log, don't output to the terminal
//  wwwproxy 7 = write log and output to the terminal
#define __RELEASE__ 0x00
#define __DEBUG__ 0x01
#define __DEBUG_TRACE_LOG__ 0x02
#define __DEBUG_TRACE_TERM__ 0x04

class CDsString : public CDsObject
{
public:
	CDsString();
	CDsString(LPSTR str);
	CDsString(const CDsString &rhs); // Added by Billy for ATString a=b;

	virtual ~CDsString();

	DWORD Read(CByteStream &is);
	DWORD GetLength() const; // Modified by Billy: const
	LPSTR GetData() const;
	// Modified by Billy: const

	BOOL IsEmpty();

	void CopyFrom(LPCSTR str);

	BOOL Equals(LPSTR str);
	BOOL Equals(CDsString &other);

	// Added by Billy 18/Dec 2000
	// friend ostream & operator<< (ostream& theStream,CDsString& theString);
	CDsString &operator=(const CDsString &rhs);
	CDsString &operator=(const char *szNew);

protected:
	LPSTR m_pData;
	DWORD m_len;
};

class CDsBinary : public CDsObject
{
public:
	CDsBinary();
	CDsBinary(LPBYTE lpData, DWORD dwLength);
	virtual ~CDsBinary();

	DWORD Read(CByteStream &is);
	DWORD GetLength();
	LPBYTE GetData();

	BOOL IsEmpty();

	void CopyFrom(LPBYTE lpData, DWORD dwLength);

	BOOL Equals(LPBYTE lpData, DWORD dwLength);
	BOOL Equals(CDsBinary &other);

protected:
	LPBYTE m_pData;
	DWORD m_len;
};

/////////////////////////////////////////////////////////////////////////////
// CByteStream
//
class CByteStream
{
public:
	CByteStream(unsigned char *pBuf)
	{
		DS_ASSERT(pBuf);
		m_pBuf = pBuf;
		m_lCurPos = 0;
	}

public:
	inline long Seek(long lPos)
	{
		long lOld = m_lCurPos;
		m_lCurPos = lPos;
		return lOld;
	}

	inline long Tell()
	{
		return m_lCurPos;
	}

	inline void Skip(long pos)
	{
		m_lCurPos += pos;
	}

	inline unsigned char *GetData()
	{
		return m_pBuf;
	}

	inline void ReadBytes(void *pData, unsigned long lSize)
	{

		if (lSize == 0)
			return;

		DS_ASSERT(pData);

		memcpy(pData, m_pBuf + m_lCurPos, lSize);
		m_lCurPos += lSize;
	}

	inline void WriteBytes(void *pData, unsigned long lSize)
	{
		DS_ASSERT(lSize >= 0);

		if (lSize == 0)
			return;

		DS_ASSERT(pData);

		memcpy(m_pBuf + m_lCurPos, pData, lSize);
		m_lCurPos += lSize;
	}

	inline char *ReadString()
	{
		char *str;
		unsigned long len;
		*this >> len;

		if (len == 0)
			return NULL;

		str = new char[len];
		if (str == NULL)
		{
			m_lCurPos += len;
			return NULL;
		}

#ifdef UNICODE
		for (unsigned long i = 0; i < len; i++)
			*this >> str[i];
#else
		memcpy(str, m_pBuf + m_lCurPos, len);
		m_lCurPos += len;
#endif

		return str;
	}

	inline void WriteString(char *str)
	{
		unsigned long len = 0;
		if (str)
			len = strlen(str) + 1;

		*this << len;

		if (len == 0)
			return;

#ifdef UNICODE
		for (unsigned long i = 0; i < len; i++)
			*this << str[i];
#else
		memcpy(m_pBuf + m_lCurPos, str, len);
		m_lCurPos += len;
#endif
	}
	inline CByteStream &operator<<(unsigned char ch)
	{
		m_pBuf[m_lCurPos++] = ch;
		return *this;
	}
	inline CByteStream &operator<<(char ch)
	{
		m_pBuf[m_lCurPos++] = (unsigned char)ch;
		return *this;
	}
	inline CByteStream &operator<<(short s)
	{
		Swap(&s, sizeof(short));
		WriteBytes(&s, sizeof(short));
		return *this;
	}
	inline CByteStream &operator<<(unsigned short s)
	{
		Swap(&s, sizeof(unsigned short));
		WriteBytes(&s, sizeof(unsigned short));
		return *this;
	}
	inline CByteStream &operator<<(int i)
	{
		Swap(&i, sizeof(int));
		WriteBytes(&i, sizeof(int));
		return *this;
	}
	inline CByteStream &operator<<(unsigned int i)
	{
		Swap(&i, sizeof(unsigned int));
		WriteBytes(&i, sizeof(unsigned int));
		return *this;
	}
	inline CByteStream &operator<<(long l)
	{
		Swap(&l, sizeof(long));
		WriteBytes(&l, sizeof(long));
		return *this;
	}
	inline CByteStream &operator<<(unsigned long l)
	{
		Swap(&l, sizeof(unsigned long));
		WriteBytes(&l, sizeof(unsigned long));
		return *this;
	}
	inline CByteStream &operator<<(float f)
	{
		Swap(&f, sizeof(float));
		WriteBytes(&f, sizeof(float));
		return *this;
	}
	inline CByteStream &operator<<(double d)
	{
		Swap(&d, sizeof(double));
		WriteBytes(&d, sizeof(double));
		return *this;
	}
	inline CByteStream &operator<<(LONGLONG longlong)
	{
		Swap(&longlong, sizeof(LONGLONG));
		WriteBytes(&longlong, sizeof(LONGLONG));
		return *this;
	}

	inline CByteStream &operator>>(unsigned char &ch)
	{
		ch = m_pBuf[m_lCurPos++];
		return *this;
	}
	inline CByteStream &operator>>(char &ch)
	{
		ch = (char)m_pBuf[m_lCurPos++];
		return *this;
	}
	inline CByteStream &operator>>(short &s)
	{
		ReadBytes(&s, sizeof(short));
		Swap(&s, sizeof(short));
		return *this;
	}
	inline CByteStream &operator>>(unsigned short &s)
	{
		ReadBytes(&s, sizeof(unsigned short));
		Swap(&s, sizeof(unsigned short));
		return *this;
	}
	inline CByteStream &operator>>(int &i)
	{
		ReadBytes(&i, sizeof(int));
		Swap(&i, sizeof(int));
		return *this;
	}
	inline CByteStream &operator>>(unsigned int &i)
	{
		ReadBytes(&i, sizeof(unsigned int));
		Swap(&i, sizeof(unsigned int));
		return *this;
	}
	inline CByteStream &operator>>(long &l)
	{
		ReadBytes(&l, sizeof(long));
		Swap(&l, sizeof(long));
		return *this;
	}
	inline CByteStream &operator>>(unsigned long &l)
	{
		ReadBytes(&l, sizeof(unsigned long));
		Swap(&l, sizeof(unsigned long));
		return *this;
	}
	inline CByteStream &operator>>(float &f)
	{
		ReadBytes(&f, sizeof(float));
		Swap(&f, sizeof(float));
		return *this;
	}
	inline CByteStream &operator>>(double &d)
	{
		ReadBytes(&d, sizeof(double));
		Swap(&d, sizeof(double));
		return *this;
	}
	inline CByteStream &operator>>(LONGLONG &longlong)
	{
		ReadBytes(&longlong, sizeof(LONGLONG));
		Swap(&longlong, sizeof(LONGLONG));
		return *this;
	}

	inline static unsigned char ReadUChar(unsigned char *pBuf, long off)
	{
		return pBuf[off];
	}
	inline static char ReadChar(unsigned char *pBuf, long off)
	{
		return (char)pBuf[off];
	}
	inline static unsigned short ReadUShort(unsigned char *pBuf, long off)
	{
		unsigned short s;
		memcpy(&s, pBuf + off, sizeof(unsigned short));
		Swap(&s, sizeof(unsigned short));
		return s;
	}
	inline static short ReadShort(unsigned char *pBuf, long off)
	{
		short s;
		memcpy(&s, pBuf + off, sizeof(short));
		Swap(&s, sizeof(short));
		return s;
	}
	inline static unsigned int ReadUInt(unsigned char *pBuf, long off)
	{
		unsigned int i;
		memcpy(&i, pBuf + off, sizeof(unsigned int));
		Swap(&i, sizeof(unsigned int));
		return i;
	}
	inline static int ReadInt(unsigned char *pBuf, long off)
	{
		int i;
		memcpy(&i, pBuf + off, sizeof(int));
		Swap(&i, sizeof(int));
		return i;
	}
	inline static unsigned long ReadULong(unsigned char *pBuf, long off)
	{
		unsigned long l;
		memcpy(&l, pBuf + off, sizeof(unsigned long));
		Swap(&l, sizeof(unsigned long));
		return l;
	}
	inline static long ReadLong(unsigned char *pBuf, long off)
	{
		long l;
		memcpy(&l, pBuf + off, sizeof(long));
		Swap(&l, sizeof(long));
		return l;
	}
	inline static float ReadFloat(unsigned char *pBuf, long off)
	{
		float f;
		memcpy(&f, pBuf + off, sizeof(float));
		Swap(&f, sizeof(float));
		return f;
	}
	inline static double ReadDouble(unsigned char *pBuf, long off)
	{
		double d;
		memcpy(&d, pBuf + off, sizeof(double));
		Swap(&d, sizeof(double));
		return d;
	}

	inline static int WriteUChar(unsigned char *pBuf, long off, unsigned char ch)
	{
		Swap(&ch, sizeof(unsigned char));
		memcpy(pBuf + off, &ch, sizeof(unsigned char));
		return sizeof(unsigned char);
	}
	inline static int WriteChar(unsigned char *pBuf, long off, char ch)
	{
		Swap(&ch, sizeof(char));
		memcpy(pBuf + off, &ch, sizeof(char));
		return sizeof(char);
	}
	inline static int WriteUShort(unsigned char *pBuf, long off, unsigned short s)
	{
		Swap(&s, sizeof(unsigned short));
		memcpy(pBuf + off, &s, sizeof(unsigned short));
		return sizeof(unsigned short);
	}
	inline static int WriteShort(unsigned char *pBuf, long off, short s)
	{
		Swap(&s, sizeof(short));
		memcpy(pBuf + off, &s, sizeof(short));
		return sizeof(short);
	}
	inline static int WriteUInt(unsigned char *pBuf, long off, unsigned int i)
	{
		Swap(&i, sizeof(unsigned int));
		memcpy(pBuf + off, &i, sizeof(unsigned char));
		return sizeof(unsigned int);
	}
	inline static int WriteInt(unsigned char *pBuf, long off, int i)
	{
		Swap(&i, sizeof(int));
		memcpy(pBuf + off, &i, sizeof(int));
		return sizeof(int);
	}
	inline static int WriteULong(unsigned char *pBuf, long off, unsigned long l)
	{
		Swap(&l, sizeof(unsigned long));
		memcpy(pBuf + off, &l, sizeof(long));
		return sizeof(long);
	}
	inline static int WriteLong(unsigned char *pBuf, long off, long l)
	{
		Swap(&l, sizeof(long));
		memcpy(pBuf + off, &l, sizeof(long));
		return sizeof(long);
	}
	inline static int WriteFloat(unsigned char *pBuf, long off, float f)
	{
		Swap(&f, sizeof(float));
		memcpy(pBuf + off, &f, sizeof(float));
		return sizeof(float);
	}
	inline static int WriteDouble(unsigned char *pBuf, long off, double d)
	{
		Swap(&d, sizeof(double));
		memcpy(pBuf + off, &d, sizeof(double));
		return sizeof(double);
	}

	static inline void Swap(void *pData, int size)
	{
		static int s_nDummy = 1;

		if (*(char *)(&s_nDummy) != 0)
		{
			unsigned char *pch = (unsigned char *)pData;

			DS_ASSERT(size > 0);

			for (int i = 0; i < size / 2; i++)
			{
				unsigned char chTemp;
				chTemp = pch[i];
				pch[i] = pch[size - 1 - i];
				pch[size - 1 - i] = chTemp;
			}
		}
	}

	inline void Free(unsigned char *buf)
	{
		if (buf)
			delete[] buf;
	}

private:
	unsigned char *m_pBuf;
	long m_lCurPos;
};
typedef CDsBinary ATBinary;
typedef CDsString ATString;

class CSTime
{
private:
	struct timeval starttick;
	struct timeval endtick;
	struct timezone tz;

public:
	inline void TickStart()
	{
		gettimeofday(&starttick, &tz);
	}
	inline void TickEnd()
	{

		gettimeofday(&endtick, &tz);
	}
	inline DWORD GetTickCount() // in u seconds
	{
		DWORD t1, t2;
		t1 = starttick.tv_sec * 1000 * 1000 + starttick.tv_usec;
		t2 = endtick.tv_sec * 1000 * 1000 + endtick.tv_usec;
		return t2 - t1;
	}

public:
	char *GetCurrentTime()
	{
		time_t tmpTime;
		time(&tmpTime);
		struct tm tmCur;
		tmCur = *localtime(&tmpTime);
		static char szTime[128];
		sprintf(szTime, "%04d%02d%02d-%02d%02d%02d",
				tmCur.tm_year + 1900,
				tmCur.tm_mon + 1,
				tmCur.tm_mday,
				tmCur.tm_hour,
				tmCur.tm_min,
				tmCur.tm_sec);
		return szTime;
	}
	int GetMinute()
	{
		return m_nMinute;
	}
	int GetHour()
	{
		return m_nHour;
	}
	int GetSecond()
	{
		return m_nSecond;
	}
	int setTime(struct tm &tmCur)
	{
		m_nHour = tmCur.tm_hour;
		m_nMinute = tmCur.tm_min;
		m_nSecond = tmCur.tm_sec;
		m_nYear = tmCur.tm_year + 1900;
		m_nMon = tmCur.tm_mon + 1;
		m_nDay = tmCur.tm_mday;
		m_nwDay = tmCur.tm_wday == 0 ? 7 : tmCur.tm_wday; // 0 =7.

		m_time = mktime(&tmCur);
	}
	DWORD GetCurTime()
	{
		time_t tmpTime;
		time(&tmpTime);
		struct tm tmCur;
		tmCur = *localtime(&tmpTime);
		m_nHour = tmCur.tm_hour;
		m_nMinute = tmCur.tm_min;
		m_nSecond = tmCur.tm_sec;
		m_nYear = tmCur.tm_year + 1900;
		m_nMon = tmCur.tm_mon + 1;
		m_nDay = tmCur.tm_mday;
		m_nwDay = tmCur.tm_wday == 0 ? 7 : tmCur.tm_wday; // 0 =7.
		setTime(tmCur);
		return tmpTime;
	}
	int GetYear()
	{
		return m_nYear;
	}
	int GetMon()
	{
		return m_nMon;
	}
	int GetDay()
	{
		return m_nDay;
	}
	CSTime();
	CSTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);
	CSTime(char *str);
	virtual ~CSTime();
	time_t GetTime();

public: // tools
	time_t m_time;
	int m_nHour;
	int m_nMinute;
	int m_nSecond;
	int m_nYear, m_nMon;
	int m_nDay;
	int m_nwDay; // 1 monday
};

// Doors thread
// API
LPSTR GetTimeStr(time_t tm);
DWORD GetCurTime();
LPSTR GetLocalTimeStr(time_t tm);
LPSTR DsCopyString(LPCSTR lpStr);
BOOL IsInNextIntvl(DWORD &dwTime, DWORD iIntvl_time);
int IpAddrStrToInt(LPCSTR lpszIpAddr);
void IpAddrIntToStr(DWORD dwIp, LPSTR lpszIpAddr);

// IInProcessTimerSink
//////////////////////////////////////////////
/// For Timer
DWORD GetTickCount();

class CInternalCriticalSection
{
public:
	CInternalCriticalSection();
	virtual ~CInternalCriticalSection();

public:
	void Lock();
	BOOL TryLock();
	void Unlock();

private:
	pthread_mutex_t *m_critical;
	pthread_cond_t *m_condition;
	pthread_t m_ownerThreadID;
	int m_nRefCount;
};
/////////////////////////////////////////////////////////////////////////////
// IInProcessTimerSink
//
class IInProcessTimerSink
{
public:
	virtual void OnInProcessTimerTick() = 0;
	virtual void OnInProcessTimeOut(DWORD dwTimeOutType,
									DWORD dwCurCount) = 0;
};

class IInProcessTimer
{
public:
	virtual void Destroy() = 0;

	virtual BOOL SetTimer(UINT nElapse, char *strId) = 0;
	virtual void KillTimer() = 0;

	virtual BOOL SetTimeOut(UINT nTimeOutType, UINT nTimeOutElapse) = 0;
	virtual void KillTimeOut() = 0;
	virtual void KillTimeOut(UINT nTimeOutType) = 0;
};

class CInProcessTimer : public IInProcessTimer
{
public:
	CInProcessTimer(IInProcessTimerSink *pSink);
	virtual ~CInProcessTimer();

public:
	void Destroy();

	BOOL SetTimer(UINT nElapse, char *strId);
	void KillTimer();

	BOOL SetTimeOut(UINT nTimeOutType, UINT nTimeOutElapse);
	void KillTimeOut();
	void KillTimeOut(UINT nTimeOutType);

	// for internal use
public:
	IInProcessTimerSink *GetSink();

protected:
	IInProcessTimerSink *m_pSink;
	CDsString m_strId;
	UINT m_index;
};

typedef struct
{
	UINT m_index;
	BOOL m_bDeleting;
	BOOL m_bTimer;
	UINT m_nElapse;
	UINT m_nReserve; // keep elapse of timer or type of timeout
	CDsString m_strId;
	UINT m_nRunning;
} StruTimerEvent;

class CTimerMgr
{
public:
	CTimerMgr();
	virtual ~CTimerMgr();

public:
	UINT RegisterTimer(IInProcessTimerSink *pSink);
	void UnregisterTimer(UINT idx);

	void SetTimer(UINT idx, UINT nElapse, char *strId);
	void KillTimer(UINT idx);

	void SetTimeOut(UINT idx, UINT nTimeOutType, UINT nTimeOutElapse);
	void KillTimeOut(UINT idx, UINT nTimeOutType);
	void KillTimeOut(UINT idx);

	static CTimerMgr *m_pTimerMgr;
#ifdef __FreeBSD__

	static void WindowProc1(int sig, sig_t *info, void *context);
#else
	static void WindowProc1(int sig, siginfo_t *info, void *context);
#endif
	int WindowProc(WORD uMsg, WORD wParam, LONG lParam);
	static void *worker(void *);
	int GetTimerNumber() { return m_timerList.GetCount(); }

protected:
	CInternalVector m_timerList;
	CInternalVector m_eventList;
	int m_nFirstUnusedSlot;
	DWORD m_dwLastTick;
	UINT m_nTimerEpsilon;
	pthread_t m_timerThreadID;
	CInternalCriticalSection m_runnable;
};
extern "C"
{
	void DsInitializeTimer();
	void DsShutdownTimer();
}
/* add a new line. */

extern unsigned char AscIIToBin(char *szAscII);

#define TRIM_LEFT(p)                                    \
	for (; (p) && (*(p) == ' ' || *(p) == '\t'); (p)++) \
	{                                                   \
	}
/* Error code */
#define DS_OK 0
#define DS_ERR -1

#endif
