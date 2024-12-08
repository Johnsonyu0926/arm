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

#include <pthread.h>
#include <ctype.h> //for toupper
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>    // for FILE
#include <string.h>   //for memset
#include <sys/types.h> //for AF_INET
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>

#define CopyMemory(Dest, Src, Len) memcpy((Dest), (Src), (Len))
#define MoveMemory(Dest, Src, Len) memmove((Dest), (Src), (Len))

#define INET_ADDRSTRLEN 16  /* for IPV4 dotted-decimal */
#define INET6_ADDRSTRLEN 46 /* FOR IPV6 hex string*/

#define INTERVAL_TIME 60

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

#define TIMEOUT -2

typedef enum enSHUTDOWN {
    SHUTDOWN_RECEIVE = 0,
    SHUTDOWN_SEND = 1,
    SHUTDOWN_BOTH = 2
} SHUTDOWN;

typedef enum enSockType {
    TCP,
    UDP,
    RAW
} SOCKTYPE;

#define BUF_MAX 4096
class CSocket {
public:
    CSocket();
    CSocket(SOCKET s);

    ~CSocket();

public:
    SOCKET m_hSocket;
    BOOL Create(IN SOCKTYPE nSockType);

    SOCKET GetHandle();

    BOOL Connect(IN const char *szServAddr, IN int nServPort);
    BOOL Connect(IN DWORD dwIp, IN int nServPort);
    BOOL ConnectTimeOut(const char *szHost, unsigned short nPort, long timeout);

    BOOL Bind(IN int nBindPort);
    int Bind();
    BOOL Listen();
    BOOL Accept(CSocket *clnaccept);

    void SetPeerPort(int nPeerPort);
    void SetPeerIp(DWORD dwIp);

    int Send(const void *buf, int nBytes);
    int Send(IN char *szWillSend);

    int Recv(OUT void *szRecvBuf, IN int nBufSize);
    int Recv(OUT char *szRecvBuf);

    int Recv(OUT char *szRecvBuf, IN int nBufSize, IN int nTimeOut);
    int Recv(OUT char *szRecvBuf, IN int nBufSize, IN int nTimeOut, BOOL bSure);

    BOOL InetN2P(void *pSinAddr, char *szPtr, int nLen, int nFamily = AF_INET);
    BOOL InetP2N(char *szIP, void *pSinAddr, int nFamily = AF_INET);
    DWORD InetP2N(char *szIP, int nFamily = AF_INET);

    int RecvFrom(OUT void *szRecvBuf, IN int nBufSize,
                 OUT struct sockaddr_in *pFromAddr = NULL, OUT int *nAddrLen = NULL);
    int RecvFrom(OUT void *szRecvBuf, IN int nBufSize, long timeout,
                 OUT struct sockaddr_in *pFromAddr = NULL, OUT int *nAddrLen = NULL);
    int RecvFrom(OUT void *szRecvBuf, IN int nBufSize,
                 OUT char *szFromWhere,
                 IN int nFromSize,
                 OUT int &nFromPort);

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
    int ConnectionReset();
    BOOL Shutdown(int nHow);

    int flags;
    BOOL SetNonBlock() {
        flags = fcntl(m_hSocket, F_GETFL, 0);
        fcntl(m_hSocket, F_SETFL, flags | O_NONBLOCK);
        return TRUE;
    }
    BOOL ResetBlock() {
        fcntl(m_hSocket, F_SETFL, flags);
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

class CDsSSL {
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

class CSThread {
public:
    CSThread() : m_bAutoDelete(TRUE), m_bJoinable(FALSE) {
        DS_TRACE("[CSThread::CSThread] Constructor function called.");
    }
    virtual ~CSThread() {
        DS_TRACE("[CSThread::~CSThread] Destroy function called");
    }

public:
    virtual BOOL InitInstance() = 0;
    virtual BOOL ExitInstance() = 0;
    BOOL SetAutoDelete(BOOL b) {
        m_bAutoDelete = b;
        return TRUE;
    }
    void SetJoinable(BOOL b) { m_bJoinable = b; }
    pthread_t CreateThread() {
        pthread_mutex_lock(&g_threadcountLock);
        _threadcount++;
        pthread_mutex_unlock(&g_threadcountLock);
        DS_TRACE("[CSThread::CreateThread] Creating " << _threadcount << " thread.");
        if (pthread_create(&m_threadId, NULL, CSThread::_threadfunc, this) == 0) {
            DS_TRACE("[CSThread::CreateThread] Create thread " << m_threadId << " success!");
            return m_threadId;
        } else {
            DS_TRACE("[CSThread::CreateThread] Failed create thread  " << _threadcount << "! program exit abnormal!");
            exit(0);
        }
        return FALSE;
    }

protected:
    BOOL Run() {
        if (!m_bJoinable)
            pthread_detach(pthread_self());
        InitInstance();
        pthread_mutex_lock(&g_threadcountLock);
        _threadcount--;
        pthread_mutex_unlock(&g_threadcountLock);
        ExitInstance();
        if (m_bAutoDelete) {
            DS_TRACE("[CSThread::ThreadFunc] delete myself! thread count = " << _threadcount << "\n");
            delete this;
        }
        return TRUE;
    }
    static void *_threadfunc(void *pParam) {
        CSThread *pObj = (CSThread *)pParam;
        pObj->Run();
        return NULL;
    }

protected:
    BOOL m_bAutoDelete;
    BOOL m_bJoinable;
    pthread_t m_threadId;

public:
    pthread_t GetThreadId() {
        DS_TRACE("[CSThread::GetThreadId] the thread id is: " << m_threadId);
        return m_threadId;
    }
};

extern char *DsToUpper(char *szSrc);

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

class CPrintableString {
public:
    CPrintableString() {
        memset(m_buf, 0, MAX_PRINTABLE_STRING_LENGTH + 1);
        m_len = 0;
        m_bPrintBinary = FALSE;
    }

    ~CPrintableString() {}

public:
    inline CPrintableString &operator<<(char ch) {
        DS_ASSERT(!m_bPrintBinary);

        if (m_len + 1 < MAX_PRINTABLE_STRING_LENGTH)
            m_len += sprintf(m_buf + m_len, "%c", ch);

        return *this;
    }

    inline CPrintableString &operator<<(unsigned char ch) {
        DS_ASSERT(!m_bPrintBinary);

        if (m_len + 1 < MAX_PRINTABLE_STRING_LENGTH)
            m_len += sprintf(m_buf + m_len, "%c", ch);

        return *this;
    }

    inline CPrintableString &operator<<(short s) {
        if (m_bPrintBinary)
            PrintBinary(s);
        else {
            if (m_len + 6 < MAX_PRINTABLE_STRING_LENGTH)
                m_len += sprintf(m_buf + m_len, "%d", s);
        }

        return *this;
    }

    inline CPrintableString &operator<<(unsigned short s) {
        if (m_bPrintBinary)
            PrintBinary(s);
        else {
            if (m_len + 5 < MAX_PRINTABLE_STRING_LENGTH)
                m_len += sprintf(m_buf + m_len, "%u", s);
        }

        return *this;
    }

    inline CPrintableString &operator<<(int i) {
        if (m_bPrintBinary)
            PrintBinary(i);
        else {
            if (m_len + 11 < MAX_PRINTABLE_STRING_LENGTH)
                m_len += sprintf(m_buf + m_len, "%ld", (long)i);
        }

        return *this;
    }

    inline CPrintableString &operator<<(unsigned int i) {
        if (m_bPrintBinary)
            PrintBinary(i);
        else {
            if (m_len + 10 < MAX_PRINTABLE_STRING_LENGTH)
                m_len += sprintf(m_buf + m_len, "%lu", (long)i);
        }

        return *this;
    }

    inline CPrintableString &operator<<(long l) {
        if (m_bPrintBinary)
            PrintBinary(l);
        else {
            if (m_len + 11 < MAX_PRINTABLE_STRING_LENGTH)
                m_len += sprintf(m_buf + m_len, "%ld", l);
        }

        return *this;
    }

    inline CPrintableString &operator<<(unsigned long l) {
        if (m_bPrintBinary)
            PrintBinary(l);
        else {
            if (m_len + 10 < MAX_PRINTABLE_STRING_LENGTH)
                m_len += sprintf(m_buf + m_len, "%lu", l);
        }

        return *this;
    }

    inline CPrintableString &operator<<(float f) {
        DS_ASSERT(!m_bPrintBinary);

        if (m_len + 12 < MAX_PRINTABLE_STRING_LENGTH)
            m_len += sprintf(m_buf + m_len, "%ld.", (long)f);

        return *this;
    }

    inline CPrintableString &operator<<(double d) {
        DS_ASSERT(!m_bPrintBinary);

        if (m_len + 12 < MAX_PRINTABLE_STRING_LENGTH)
            m_len += sprintf(m_buf + m_len, "%ld.", (long)d);

        return *this;
    }

    inline CPrintableString &operator<<(LPCSTR lpsz) {
        DS_ASSERT(!m_bPrintBinary);

        if (lpsz && *lpsz) {
            int len = strlen(lpsz);

            if (m_len + len > MAX_PRINTABLE_STRING_LENGTH)
                len = MAX_PRINTABLE_STRING_LENGTH - m_len;

            memcpy(m_buf + m_len, lpsz, len);
            m_len += len;
            m_buf[m_len] = 0;
        }

        return *this;
    }

    inline CPrintableString &operator<<(LPVOID lpsz) {
        DS_ASSERT(!m_bPrintBinary);

        if (m_len + 10 < MAX PRINTABLE_STRING_LENGTH)
            m_len += sprintf(m_buf + m_len, "0x%lX", *(long *)lpsz);

        return *this;
    }

    inline CPrintableString &operator<<(LPBYTE lpsz) {
        DS_ASSERT(!m_bPrintBinary);

        m_lpBinary = lpsz;
        m_bPrintBinary = TRUE;

        return *this;
    }

    inline operator LPCSTR() {
        return m_buf;
    }

private:
    inline void PrintBinary(DWORD dwLen) {
        m_bPrintBinary = FALSE;

        *this << '{';
        if (m_lpBinary) {
            for (DWORD i = 0; i < dwLen; i++) {
                *this << (int)m_lpBinary[i];
                *this << ',';
            }
        }
        *this << '}';
    }

    TCHAR m_buf[MAX PRINTABLE_STRING_LENGTH + 1];
    int m_len;
    BOOL m_bPrintBinary;
    LPBYTE m_lpBinary;
};

void DsDebugInfo(char *lpFormat, ...);
void DsLog(char *lpFormat, ...);

#endif // __DOORS_BASE_H__