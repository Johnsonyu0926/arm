#include <fcntl.h>
#include <stdarg.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
//#include <stdio.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <utmp.h>

#include <sys/stat.h>
//#ifndef __linux__

#include <stdarg.h>
#include <stdio.h>	//for printf()
#include <string.h> //for memset()
#include <stdlib.h>

/*
 * For socket(),recv(),select(),gethostbyname(),errno
 */

#include <sys/time.h>
//#ifdef __FreeBSD__
#include <sys/types.h>
//#endif
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>		//for gethostbyname
#include <netinet/in.h> //for inet_ntoa()
#include <arpa/inet.h>	//for inet_ntoa()
#include <errno.h>		//for errno

/*
 * For ssl
 */
#include <openssl/rsa.h> /* SSLeay stuff */
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
/*
 * For class CConfig,CSocket
 */

//#endif

#include "doorsdefs.h"

#include "doorsbase.h"
//#include "global.h"

#define CHK_NULL(x)  \
	if ((x) == NULL) \
	exit(1)
#define CHK_ERR(err, s) \
	if ((err) == -1)    \
	{                   \
		perror(s);      \
		exit(1);        \
	}
#define CHK_SSL(err)                 \
	if ((err) == -1)                 \
	{                                \
		ERR_print_errors_fp(stderr); \
		exit(2);                     \
	}
#define HOME "./"
#define CERTF HOME "foo-cert.pem"
#define KEYF HOME "foo-cert.pem"

CConfig::CConfig(char *szFile)
{
	m_pFp = NULL;
	if (szFile)
	{
		strcpy(m_szFile, szFile);
	}
}
CConfig::~CConfig()
{
}

int CConfig::Read(char *szSess, char *szItem, char *szValue, char *szDefault)
{
	// DS_TRACE("config file : " << m_szFile);
	m_pFp = fopen(m_szFile, "r");

	if (m_pFp == NULL)
	{
		DS_TRACE("[CConfig::Read] Your config file " << m_szFile << " can not be found, use your default setting.\n");

		strcpy(szValue, szDefault);
		return 0;
	}

	char szSess1[128];
	sprintf(szSess1, "[%s]", szSess);

	int bEnterSess = 0;
	char szLine[256];
	while (fgets(szLine, 255, m_pFp))
	{
		// DS_TRACE( szLine);
		if (!bEnterSess)
		{
			if (strncasecmp(szLine, szSess1, strlen(szSess1)) == 0)
				bEnterSess = 1;
		}
		else
		{
			if (szLine[0] == '[')
			{
				DS_TRACE("Out section!");
				break;
			}
			if (strncasecmp(szLine, szItem, strlen(szItem)) == 0)
			{
				// find it!
				char *p = strchr(szLine, '=');
				if (!p)
					return 0;
				p++;
				while (*p == ' ')
					p++; // skip ' '
				char *p2 = strchr(p, ';');
				if (!p2)
				{
					DS_TRACE("Your config file format is error! must end with ';'!\n");
					return 0;
				}
				strncpy(szValue, p, p2 - p);
				szValue[p2 - p] = '\0';

				fclose(m_pFp);
				m_pFp = NULL;
				return 1;
			}
		}
	}

	DS_TRACE("[CConfig::Read] The config item " << szItem << " on file " << m_szFile << " had not been set or the format is invalid, use your default setting.\n");
	strcpy(szValue, szDefault);
	fclose(m_pFp);
	m_pFp = NULL;
	return 0;
}

BOOL CSocket::GetMyIp(char *szBuf)
{
	struct sockaddr_in sockaddr;
	bzero(&sockaddr, sizeof(sockaddr));
	unsigned int len;
	if (getpeername(m_hSocket, (struct sockaddr *)&sockaddr, &len) < 0)
	{
		DS_TRACE("getpeername failed.\n");
		return FALSE;
	}
	strcpy(szBuf, inet_ntoa(sockaddr.sin_addr));
	return TRUE;
}
CSocket::CSocket()
{
	m_hSocket = INVALID_SOCKET;
	m_bConnected = FALSE;
}
CSocket::CSocket(SOCKET s)
{
	m_hSocket = s;
}

CSocket::~CSocket()
{
	Close();
}

void CSocket::SetSocket(SOCKET s)
{
	m_hSocket = s;
	m_bConnected = TRUE;
}
int CSocket::ConnectionReset()
{
	m_bConnected = FALSE;
	// return Close();
	return 1;
}
BOOL CSocket::Create(IN SOCKTYPE nSockType)
{

	switch (nSockType)
	{
	case TCP:
		m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
		break;
	case UDP:
		m_hSocket = socket(AF_INET, SOCK_DGRAM, 0);
		break;
	case ARP:
		m_hSocket = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ARP));
		if (m_hSocket > 0)
		{
			int optval = 1;
			if (setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) == -1)
			{
				printf("Could not setsocketopt on raw socket\n");
				close(m_hSocket);
				return FALSE;
			}
		}
		break;
	case ALL:
		m_hSocket = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ALL));
		if (m_hSocket > 0)
		{
			int optval = 1;
			if (setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) == -1)
			{
				printf("Could not setsocketopt on raw socket\n");
				close(m_hSocket);
				return FALSE;
			}
		}
		break;
	default:
		break;
	}

	if (m_hSocket < 0)
	{
		int nError = errno;
		DS_TRACE("[ERROR/CSocket::Create] Create socket failed. errno=" << nError << ",strerror()=" << strerror(nError));
		switch (nError)
		{
		case EMFILE:
			DS_TRACE("[ERROR/CSocket::Create] EMFILE: The pre-process descriptor table is full.");
			break;
		case EACCES:
			DS_TRACE("[ERROR/CSocket::Create] EACCES: Permission to create a socket of the specified type and/or protocol is denied.");
			break;
		case ENOMEM:
			DS_TRACE("[ERROR/CSocket::Create] ENOMEM: Insufficient memory is avaliable.");
			break;
#ifdef SOLARIS
		case ENOSR:
			DS_TRACE("[ERROR/CSocket::Create] ENOSR: There are sufficient STREAMS resource avaliable to complete the operation.");
			break;
#endif
		case EPROTONOSUPPORT:
			DS_TRACE("[ERROR/CSocket::Create] EPROTONOSUPPORT: The protocol type or the specified protocol is not supported within this domain.");
			break;
		default:
			break;
		}
		return FALSE;
	}

	DS_TRACE("[INFO/CSocket::Create] create socket ok! fd = " << GetHandle());
	return TRUE;
}
BOOL CSocket::Connect(DWORD dwIp, int nPort)
{
	// in network order!
	struct sockaddr_in Addr;
	memset(&Addr, 0, sizeof(Addr));

	Addr.sin_family = AF_INET;
	Addr.sin_port = nPort; // socket port
	Addr.sin_addr.s_addr = dwIp;

	int nErrorCode;
	nErrorCode = connect(m_hSocket, (struct sockaddr *)&Addr, sizeof(Addr));
	if (nErrorCode < 0)
	{
		m_bConnected = FALSE;
		int nError = errno;
		DS_TRACE("[ERROR/CSocket::Create] Connect failed. errno=" << nError << ",strerror()=" << strerror(nError));
		return FALSE;
	}
	DS_TRACE("[INFO/CSocket::Connect] Connect success! fd = " << GetHandle());
	m_bConnected = TRUE;
	return TRUE;
}
BOOL CSocket::ConnectTimeOut(const char *szIp, unsigned short nPort, long timeout)
{
	char szAddress[32];
	if (IsDotIp(szIp))
	{
		strncpy(szAddress, szIp, sizeof(szAddress));
	}
	else
	{
		if (!QueryDns(szAddress, szIp))
		{
			DS_TRACE("[CSocket::ConnectTimeOut] host error!");
			return FALSE;
		}
	}
	m_bConnected = FALSE;
	struct timeval tval;
	fd_set rset, wset;
	socklen_t len;

	int flags = fcntl(m_hSocket, F_GETFL, 0);
	fcntl(m_hSocket, F_SETFL, flags | O_NONBLOCK);
	errno = 0;

	int n;
	struct sockaddr_in server;
	//连接
	server.sin_family = AF_INET;
	server.sin_port = htons(nPort);
	server.sin_addr.s_addr = inet_addr(szAddress);
	if (server.sin_addr.s_addr == INADDR_NONE)
	{
		return 0;
	}
	if ((n = connect(m_hSocket, (const struct sockaddr *)&server, sizeof(server))) < 0)
	{
		if (errno != EINPROGRESS)
		{
			DS_TRACE("[ERROR/CSocket::ConnectTimeOut] Error connect!");
			int nError = errno;
			DS_TRACE("[ERROR/CSocket::ConnectTimeOut] connect failed. errno=" << nError << ",strerror()=" << strerror(nError));
			return 0;
		}
	}
	// if( (n = Connect(szIp,nPort) ) <0)
	//	if(errno!=EINPROGRESS)
	//		return 0;

	if (n == 0)
	{
		DS_TRACE("[INFO/CSocket::ConnectTimeOut] connect success! fd = " << GetHandle());
		goto done;
	}

	FD_ZERO(&rset);
	FD_SET(m_hSocket, &rset);
	wset = rset;
	tval.tv_sec = timeout;
	tval.tv_usec = 0;

	if ((n = select(m_hSocket + 1, &rset, &wset, NULL, &tval)) == 0)
	{
		DS_TRACE("[ERROR/CSocket::ConnectTimeOut] TIME OUT!");
		return 0;
	}
	int error;
	if (FD_ISSET(m_hSocket, &rset) || FD_ISSET(m_hSocket, &wset))
	{
		len = sizeof(error);
		if (getsockopt(m_hSocket, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
		{
			DS_TRACE("[ERROR/CSocket::ConnectTimeOut] getsockopt failed.");
			return 0;
		}
		else
		{
			DS_TRACE("Connect!");
		}
	}
	else
	{
		DS_TRACE("[ERROR/CSocket::ConnectTimeOut] connect failed socket is not read or writeable.");
		return 0;
	}
done:
	fcntl(m_hSocket, F_SETFL, flags);
	if (error)
	{
		errno = error;
		return 0;
	}
	m_bConnected = TRUE;
	DS_TRACE("[CSocket::ConnectTimeOut] connect success! fd = " << GetHandle());
	return 1;
}

BOOL CSocket::IsDotIp(const char *szHost)
{
	// Modified by billy. 2002/9/29
	// If it is a dot. ip, do not query dns
	int a1, a2, a3, a4;
	a1 = -1;
	a2 = -1;
	a3 = -1;
	a4 = -1;
	sscanf(szHost, "%d.%d.%d.%d", &a1, &a2, &a3, &a4);
	if (a1 <= 255 && a1 >= 0 &&
		a2 <= 255 && a2 >= 0 &&
		a3 <= 255 && a3 >= 0 &&
		a4 <= 255 && a4 >= 0)
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CSocket::Connect(const char *szHost, int nPort)
{

	m_bConnected = FALSE;

	// Modified by billy. 2002/9/29
	// If it is a dot. ip, do not query dns
	if (IsDotIp(szHost))
	{
		// DS_TRACE("[CSocket::Connect] Connecting to ip "<<szHost<<" ...(don't need DNS query)");
		m_bConnected = ConnectIp(szHost, nPort);
		return m_bConnected;
	}

	char szAddress[32];
	memset(szAddress, 0, 32);
	// else query dns
	if (QueryDns(szAddress, szHost))
	{
		m_bConnected = ConnectIp(szAddress, nPort);
		return m_bConnected;
	}

	// query dns failed!
	return m_bConnected;
}
BOOL CSocket::QueryDns(char *szAddress, const char *szHost)
{
	struct hostent *host_ent;

	// DS_TRACE("[CSocket::Connect] Query dns for host:"<<szHost<<"...\n");
	host_ent = gethostbyname(szHost);
	if (host_ent == 0)
	{
		DS_TRACE("[ERROR/CSocket::Connect] Query DNS : " << szHost << " failed!\n");
		return 0;
	}
	char *pIp = inet_ntoa(*(struct in_addr *)*(host_ent->h_addr_list));
	if (pIp)
	{
		strncpy(szAddress,
				pIp,
				31);
	}
	else
	{
		DS_TRACE("[ERROR/CSocket::Connect] Query DNS : " << szHost << " failed!\n");
		return 0;
	}

	return TRUE;
}

// private:
BOOL CSocket::ConnectIp(const char *szAddress, int nPort)
{
	struct sockaddr_in Addr;
	memset(&Addr, 0, sizeof(Addr));

	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(nPort); // socket port
	Addr.sin_addr.s_addr = inet_addr(szAddress);

	int nErrorCode;
	nErrorCode = connect(m_hSocket, (struct sockaddr *)&Addr, sizeof(Addr));
	if (nErrorCode < 0)
	{
		DS_TRACE("[ERROR/CSocket::Connect]: connect " << szAddress << ":" << nPort << " failed - " << errno << "," << strerror(errno));
		return FALSE;
	}
	DS_TRACE("[CSocket::Connect] Connect to server(" << szAddress << ":" << nPort << ") success!\n");
	return TRUE;
}

BOOL CSocket::Bind(IN int nBindPort)
{
	char szOptVal[] = "SO_REUSEADDR";
	int nOptLen = strlen(szOptVal);
	setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, szOptVal, nOptLen);

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(nBindPort);
	if (bind(m_hSocket, (struct sockaddr *)&server, sizeof(server)) == 0)
	{
		DS_TRACE("[CSocket::Bind] Bind port " << nBindPort < " successfully!\n");
		return TRUE;
	}
	m_bConnected = TRUE;
	return FALSE;
}

int CSocket::Bind()
{
	// bind a round port
	// For the TCP ftp data server,this restricts the socket to receive
	// incoming client connections destined only to that IP address!
	// struct hostent* ent;
	// ent=gethostbyname(szIP);

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(0);
	if (m_hSocket == -1)
	{
		printf("#Bind: Bind failed - Not sock!\n");
		return FALSE;
	}
	if (bind(m_hSocket, (struct sockaddr *)&server, sizeof(server)) == 0)
	{
		/*if (!Listen())
		{
			printf("Bind: listen failed - %s!\n", strerror(errno));
			return FALSE;
		}*/
		// notes:
		// just only after the listen is called,the kernel will
		// chooses an ephemeral port for the socker!!!!

		struct sockaddr_in server1;
		unsigned int nSize = sizeof(server1);
		if (getsockname(m_hSocket, (struct sockaddr *)&server1,
						&nSize) == 0)
		{
			int nPort = ntohs(server1.sin_port);
			printf("[CSocket::Bind] Bind %d port successfully.\n", nPort);
			return nPort;
		}
	}
	return FALSE;
}

BOOL CSocket::Listen()
{
	if (listen(m_hSocket, 3) == 0)
	{
		return TRUE;
	}
	printf("#Listen: listen failed - %s!\n", strerror(errno));
	return FALSE;
}

BOOL CSocket::Accept(CSocket *pClientSocket)
{
	SOCKET clnsock;

	struct sockaddr_in client;
	unsigned int namelen = sizeof(client);

	if (m_hSocket < 0)
	{
		printf("FATAL/CSocket::Accept] The m_hSocket is invalid!\n");
		return 0;
	}
	try
	{
		while (1)
		{
			clnsock = accept(m_hSocket, (struct sockaddr *)&client, &namelen);
			if (clnsock < 0 && (errno == EINTR || errno == 130 || errno == 53))
			{
				// ERROR INTERUPT:
				// 130 - Software aboad the connection.

				DS_TRACE("[ERROR/CSocket::Accept] accept failed - " << strerror(errno) << ",error=" << errno << ",Ign,Continue...");
				continue;
			}
			else
				break;
		}
	}
	catch (...)
	{
		DS_TRACE("[FATAL/CSocket::Accept] Accept failed! - " << strerror(errno) << ",errno=" << errno);
		return 0;
	}
	if (clnsock == -1)
	{
		DS_TRACE("[ERROR/CSocket::Accept] accept failed - " << strerror(errno) << ",error=" << errno);
		return FALSE;
	}

	pClientSocket->SetPeerIp(*(DWORD *)&client.sin_addr);
	pClientSocket->SetPeerPort(client.sin_port);
	pClientSocket->SetSocket(clnsock);
	DS_TRACE("[CSocket::Accept] Accept success! SOCKET = " << clnsock);
	return TRUE;
}
/*
int CSocket::Send(IN char* szWillSend, IN int nWant)
{
	return send(m_hSocket, szWillSend, nWant, 0);
}
*/

int CSocket::Send(const void *buf, int nBytes)
{
	return send(m_hSocket, buf, nBytes, 0);
}

int CSocket::Send(IN char *szWillSend)
{
	return send(m_hSocket, szWillSend, strlen(szWillSend), 0);
}
/*
int CSocket::Recv(OUT char* szRecvBuf, IN int nBufSize)
{
	return recv(m_hSocket, szRecvBuf, nBufSize, 0);
}
*/
int CSocket::Recv(OUT void *buf, int nBufSize)
{
	int nRecv = recv(m_hSocket, buf, nBufSize, 0);
	if (nRecv < 0)
	{
		ConnectionReset();
	}
	return nRecv;
}

int CSocket::Recv(OUT char *szRecvBuf)
{
	char szRecv[BUF_MAX];
	char *szBuf; // unknown size...
	szBuf = NULL;
	int nRecv;
	int nCur, nMax;
	nCur = nMax = 0;
	do
	{
		nRecv = Recv(szRecv, BUF_MAX);
		if (nRecv < 0) // only can be -1 :SOCK_ERROR
		{
			// LOG THE EVENT
			// LogToFile(SOCKERR);
			printf("#Recv: recv failed - %s!\n", strerror(errno));
			return FALSE;
		}
		if (nRecv == 0)
		{
			// client close
			break;
		}
		// OK...
		if (nCur + nRecv + 1 > nMax)
		{
			nMax = BUF_MAX * ((nCur + nRecv + 1) / BUF_MAX + 1);
			if ((szBuf = (char *)realloc(szBuf, nMax)) == NULL)
			{
				printf("#Recv: realloc failed - %s!\n", strerror(errno));
				return FALSE;
			}
		}
		strcpy(szBuf + nCur, szRecv);
		nCur += nRecv;
	} while (nRecv == BUF_MAX);
	// Recv success!
	// DealWithTheRequest(szBuf);

	strcpy(szRecvBuf, szBuf);
	printf("#Recv: All succeeded.\n");
	return strlen(szRecvBuf);
}

BOOL CSocket::Shutdown(int nHow)
{
	if (shutdown(m_hSocket, nHow) == 0)
	{
		DS_TRACE("Shutdown m_hSocket= " << m_hSocket << " Success!");
		return TRUE;
	}

	return FALSE;
}
BOOL CSocket::Close()
{
	if (m_hSocket == INVALID_SOCKET)
	{

		return TRUE;
	}

	if (m_bConnected)
	{
		// if (shutdown(m_hSocket, 2) != 0)
		/*	if (shutdown(m_hSocket, SHUTDOWN_SEND) != 0)
			{
				DS_TRACE("[ERROR/CSocket::Close]  shutdown " << m_hSocket<<" failed - "<<strerror(errno)<<"errno="<<errno);
				//return FALSE;
			}
			else
			{
				//DS_TRACE("[CSocket::Close] shutdown "<< m_hSocket<<" OK!");
			}*/
	}
	int nSocket = m_hSocket;
	if (close(m_hSocket) != 0)
	{
		DS_TRACE("[ERROR/CSocket::Close] close " << nSocket << " failed - " << strerror(errno) << ",errno=" << errno);
		return FALSE;
	}
	else
	{
		// DS_TRACE("[CSocket::Close] Close socket success! the SOCKET="<< nSocket);
	}
	m_hSocket = INVALID_SOCKET;
	return TRUE;
}
// For raw socket:
int CSocket::SendTo(
	IN void *szWillSend,
	IN int nWant,
	IN char *szInterfaceName)
{
	struct sockaddr ToAddr;
	/*ToAddr.sin_family = AF_INET;
	ToAddr.sin_port = htons(nPort);
	ToAddr.sin_addr.s_addr = inet_addr(szDotAddr);
	*/
	strcpy(ToAddr.sa_data, szInterfaceName);
	DS_TRACE("[CSocket::SendTo]: ");
	int nToLen = sizeof(ToAddr);
	int nSend;
	nSend = sendto(m_hSocket, szWillSend, nWant, 0, (sockaddr *)&ToAddr, nToLen);
	if (nSend < 0)
	{
		DS_TRACE("[CSocket::SendTo]: failed to sendto the client, errno:" << errno << ", string:" << strerror(errno));
		return nSend;
	}
	DS_TRACE("[Socket::SendTo] SendTo success, bytes = " << nSend);
	return nSend;
}
// For UDP:
int CSocket::SendTo(
	IN const void *szWillSend,
	IN int nWant,
	IN const char *szDotAddr,
	IN int nPort)
{
	struct sockaddr_in ToAddr;
	ToAddr.sin_family = AF_INET;
	ToAddr.sin_port = htons(nPort);
	ToAddr.sin_addr.s_addr = inet_addr(szDotAddr);

	DS_TRACE("[CSocket::SendTo]: ");
	int nToLen = sizeof(ToAddr);
	int nSend;
	nSend = sendto(m_hSocket, szWillSend, nWant, 0, (sockaddr *)&ToAddr, nToLen);
	if (nSend < 0)
	{
		DS_TRACE("[CSocket::SendTo]: failed to sendto the client.");
		return nSend;
	}
	DS_TRACE("[Socket::SendTo] SendTo success, bytes = " << nSend);
	return nSend;
}
int CSocket::SendTo(
	IN void *szWillSend,
	IN int nWant,
	IN int nToAddrLen,
	IN struct sockaddr_in *pToAddr)
{
	DS_ASSERT(pToAddr != NULL);
	char szTo[32];
	InetN2P(&pToAddr->sin_addr, szTo, 32);
	int nToPort = ntohs(pToAddr->sin_port);
	DS_TRACE("[CSocket::SendTo] SendTo:" << szTo << ":" << nToPort);

	int nSend;
	nSend = sendto(m_hSocket, szWillSend, nWant, 0, (struct sockaddr *)pToAddr, nToAddrLen);
	if (nSend < 0)
	{
		DS_TRACE("[CSocket::SendTo]: failed to sendto the client.");
		return nSend;
	}
	DS_TRACE("[Socket::SendTo] SendTo success, bytes = " << nSend);
	return nSend;
}

int CSocket::RecvFrom(OUT void *szRecvBuf, IN int nBufSize,
					  OUT struct sockaddr_in *pFromAddr, OUT int *pnAddrLen)
{
	DS_TRACE("[CSocket::RecvFrom] RecvFrom...");
	int nRecv;

	if (!pFromAddr && !pnAddrLen)
	{
		struct sockaddr_in FromAddr;
		memset(&FromAddr, 0, sizeof(FromAddr));
		int nAddrLen;
		nRecv = recvfrom(m_hSocket, szRecvBuf, nBufSize, 0, (struct sockaddr *)&FromAddr, (socklen_t *)&nAddrLen);
	}
	else
	{
		nRecv = recvfrom(m_hSocket, szRecvBuf, nBufSize, 0, (struct sockaddr *)pFromAddr, (socklen_t *)pnAddrLen);
		DS_TRACE("[Socket::RecvFrom] recvfrom success, bytes = " << nRecv << "From:" << pFromAddr->sin_addr.s_addr << ":" << pFromAddr->sin_port);
	}

	if (nRecv < 0)
	{
		DS_TRACE("[CSocket::RecvFrom] failed recvfrom!\n");
		return nRecv;
	}
	DS_TRACE("[Socket::RecvFrom] recvfrom success, bytes = " << nRecv << "From:");

	return nRecv;
}

int CSocket::RecvFrom(OUT void *szRecvBuf, IN int nBufSize, long timeout,
					  OUT struct sockaddr_in *pFromAddr, OUT int *pnAddrLen)
{
	DS_TRACE("[CSocket::RecvFrom] RecvFrom...");
	int nRecv;
	struct timeval tvWaitTime;

	tvWaitTime.tv_sec = 120;
	tvWaitTime.tv_usec = 0;
	fd_set ReadFDs;
	FD_ZERO(&ReadFDs);
	FD_SET(m_hSocket, &ReadFDs);
	int nResult = select(
		m_hSocket + 1,
		&ReadFDs, NULL, NULL,
		&tvWaitTime);
	if (nResult > 0 && FD_ISSET(m_hSocket, &ReadFDs))
	{
		// if (nResult > 0)
		{
			if (!pFromAddr && !pnAddrLen)
			{
				struct sockaddr_in FromAddr;
				memset(&FromAddr, 0, sizeof(FromAddr));
				int nAddrLen;
				nRecv = recvfrom(m_hSocket, szRecvBuf, nBufSize, 0, (struct sockaddr *)&FromAddr, (socklen_t *)&nAddrLen);
			}
			else
			{
				nRecv = recvfrom(m_hSocket, szRecvBuf, nBufSize, 0, (struct sockaddr *)pFromAddr, (socklen_t *)pnAddrLen);
				DS_TRACE("[Socket::RecvFrom] recvfrom success, bytes = " << nRecv << "From:" << pFromAddr->sin_addr.s_addr << ":" << pFromAddr->sin_port);
			}

			if (nRecv < 0)
			{
				DS_TRACE("[CSocket::RecvFrom] failed recvfrom!\n");
				return nRecv;
			}
			DS_TRACE("[Socket::RecvFrom] recvfrom success, bytes = " << nRecv << "From:");

			return nRecv;
		}
	}

	return -1;
}

int CSocket::RecvFrom(OUT void *szRecvBuf, IN int nBufSize,
					  OUT char *szFromWhere,
					  IN int nFromSize,
					  OUT int &nFromPort)
{
	struct sockaddr_in FromAddr;
	memset(&FromAddr, 0, sizeof(FromAddr));
	int nAddrLen;
	int nRecv = recvfrom(m_hSocket, szRecvBuf, nBufSize, 0, (struct sockaddr *)&FromAddr, (socklen_t *)&nAddrLen);
	if (nRecv < 0)
	{
		DS_TRACE("[CSocket::RecvFrom] failed recvfrom!\n");
		return nRecv;
	}
	DS_TRACE("[Socket::RecvFrom] recvfrom success, bytes = " << nRecv << ",FROM:" << FromAddr.sin_addr.s_addr << ":" << ntohs(FromAddr.sin_port));

	InetN2P(&FromAddr.sin_addr, szFromWhere, nFromSize);
	nFromPort = ntohs(FromAddr.sin_port);
	return nRecv;
}
BOOL CSocket::InetN2P(void *pSinAddr, char *szPtr, int nLen, int nFamily)
{
	BYTE *p = (BYTE *)pSinAddr;
	if (nFamily == AF_INET)
	{
		char temp[INET_ADDRSTRLEN];
		snprintf(temp, sizeof(temp), "%d.%d.%d.%d",
				 p[0], p[1], p[2], p[3]);
		if ((int)strlen(temp) >= nLen)
		{
			return FALSE;
		}
		strcpy(szPtr, temp);
	}
	return FALSE;
}
BOOL CSocket::InetP2N(char *szIP, void *pSinAddr, int nFamily)
{
	if (nFamily == AF_INET)
	{
		struct in_addr in_val;
		if (inet_aton(szIP, &in_val))
		{
			memcpy(pSinAddr, &in_val, sizeof(struct in_addr));
			return TRUE;
		}
	}
	return FALSE;
}
DWORD CSocket::InetP2N(char *szIP, int nFamily)
{
	if (nFamily == AF_INET)
	{
		return inet_addr(szIP);
	}
	return FALSE;
}

int CSocket::RecvLine(char *szBuf, int nBufSize)
{
	char szTmp[2];
	int nTotal = 0;
	for (int i = 0; i < nBufSize; i++)
	{
		if (Recv(szTmp, 1) != 1)
			return FALSE;
		szTmp[1] = '\0';
		nTotal++;
		strcat(szBuf, szTmp);

		if (szTmp[0] == '\n')
			break;
	}
	szBuf[nTotal] = '\0';
	szBuf[nTotal - 1] = '\0';
	return strlen(szBuf);
}

char *CSocket::GetRemoteIp()
{
	static char szBuf[32];
	memset(szBuf, 0, 32);

	if (!m_bConnected)
	{
		strcpy(szBuf, "UNKNOWN");
		return szBuf;
	}

	struct sockaddr_in sockaddr;
	memset(&sockaddr, 0, sizeof(sockaddr));
	unsigned int len = sizeof(sockaddr);
	if (getpeername(m_hSocket, (struct sockaddr *)&sockaddr, &len) < 0)
	{
		DS_TRACE("[CSocket::GetRemoteIp] getpeername failed - " << strerror(errno) << ",errno=" << errno);
		strcpy(szBuf, "INVALID_ADDRESS");
		return szBuf;
	}
	// sprintf(szBuf,"%s:%d", inet_ntoa(sockaddr.sin_addr),sockaddr.sin_port);
	sprintf(szBuf, "%s", inet_ntoa(sockaddr.sin_addr));
	return szBuf;
}
void CSocket::SetPeerIp(DWORD ip)
{
	m_dwPeerIp = ip;
}
void CSocket::SetPeerPort(int nPort)
{
	m_nPeerPort = nPort;
}
DWORD CSocket::GetPeerIp()
{
	/*
		if(!m_bConnected || m_hSocket== INVALID_SOCKET)
		{
			return 0;
		}
		//in network order
		struct sockaddr_in sockaddr;
		memset(&sockaddr,0, sizeof(sockaddr));
		unsigned int len = sizeof(sockaddr);
		if (getpeername(m_hSocket, (struct sockaddr*)&sockaddr, &len) < 0)
		{
			DS_TRACE("[CSocket::GetPeerIp] failed get peer address.");
			return 0;
		}
	*/
	return m_dwPeerIp;
}
USHORT CSocket::GetPeerPort()
{
	/*	if(!m_bConnected || m_hSocket == INVALID_SOCKET)
		{
			return 0;
		}
		//in network order
		struct sockaddr_in sockaddr;
		memset(&sockaddr,0, sizeof(sockaddr));
		unsigned int len = sizeof(sockaddr);
		if (getpeername(m_hSocket, (struct sockaddr*)&sockaddr, &len) < 0)
		{
			DS_TRACE("[CSocket::GetPeerPort] Failed get peer port");
			return 0;
		}
	*/
	return m_nPeerPort;
}
int CSocket::Recv(OUT char *szRecvBuf, IN int nBufSize, IN int nTimeOut, BOOL bSure)
{
	struct timeval tvWaitTime;
	tvWaitTime.tv_sec = nTimeOut;
	tvWaitTime.tv_usec = 0;
	fd_set ReadFDs;

	int nTotalRecv = 0;
	while (1)
	{
		FD_ZERO(&ReadFDs);
		FD_SET(m_hSocket, &ReadFDs);
		int nResult = select(
			m_hSocket + 1,
			&ReadFDs, NULL, NULL,
			&tvWaitTime);
		if (nResult > 0 &&
			FD_ISSET(m_hSocket, &ReadFDs))
		{
			int nRecved = recv(m_hSocket, (char *)szRecvBuf + nTotalRecv, nBufSize - nTotalRecv,
							   0);
			if (nRecved <= 0)
			{
				if (bSure)
				{
					DS_TRACE("[CSocket::Recv] receive error or it disconnect. nRecved = " << nRecved);
				}
				return nRecved;
			}
			if (bSure)
			{
				nTotalRecv += nRecved;
				if (nTotalRecv == nBufSize)
					return nTotalRecv;
			}
			else
			{
				return nRecved;
			}
		}
		else
		{
			return -1;
		}
	}

	// unreachable statement
	return TIMEOUT;
}
int CSocket::Recv(OUT char *szRecvBuf, IN int nBufSize, IN int nTimeOut)
{
	struct timeval tvWaitTime;
	tvWaitTime.tv_sec = nTimeOut;
	tvWaitTime.tv_usec = 0;
	fd_set ReadFDs;
	FD_ZERO(&ReadFDs);
	FD_SET(m_hSocket, &ReadFDs);
	int nResult = select(
		m_hSocket + 1,
		&ReadFDs, NULL, NULL,
		&tvWaitTime);
	if (nResult > 0 &&
		FD_ISSET(m_hSocket, &ReadFDs))
	{
		return recv(m_hSocket, (char *)szRecvBuf, nBufSize,
					0);
	}

	return TIMEOUT;
}
SOCKET CSocket::GetHandle()
{
	return m_hSocket;
}

//////////////////////////////////////////////////////
// class CDsList
CDsList::CDsList()
{
	// DS_TRACE("[CDsList::CDsList] begin...m_nSize = "<<m_nSize);
	m_pData = NULL;
	m_nSize = 0;
}

CDsList::~CDsList()
{
	if (m_pData)
		free(m_pData);

	//		delete []m_pData;
}

BOOL CDsList::SetSize(int nNewSize)
{
	// DS_TRACE("[CDsList::CDsList] SetSize begin... nNewSize = "<<nNewSize);
	DS_ASSERT(nNewSize >= 0);

	if (nNewSize == 0)
	{
		//表示全清空
		// DS_TRACE("[CDsList::CDsList] SetSize begin... nNewSize ==0 delete all node. ");
		if (m_pData)
			free(m_pData);
		// delete[] m_pData;
		m_pData = NULL;
		m_nSize = 0;
		return TRUE;
	}
	else if (m_pData == NULL)
	{
		// DS_TRACE("[CDsList::CDsList] SetSize begin... m_pData == NULL, Begin to new "<<nNewSize<<" node.");
		// m_pData = new LPVOID[nNewSize];
		m_pData = (LPVOID *)malloc(sizeof(LPVOID) * nNewSize);
		if (m_pData == NULL)
		{
			// DS_TRACE("[CDsList::SetSize] alloc memory failed!");
			return FALSE;
		}
		memset(m_pData, 0, nNewSize * sizeof(LPVOID));
		m_nSize = nNewSize;
		// DS_TRACE("[CDsList::SetSize] ok! now the size is, m_nSize = "<<m_nSize);
		return TRUE;
	}
	else
	{
#if 0
		LPVOID* pNewData = new LPVOID[nNewSize];
		
		//DS_TRACE("[CDsList::SetSize] the old size is, m_nSize = "<<m_nSize<<" , new size  nNewSize = "<<nNewSize);
		if (pNewData == NULL)
		{	
			//DS_TRACE("[CDsList::SetSize] alloc memory failed.");
	
			return FALSE;
		}
		DS_ASSERT(nNewSize > m_nSize);

		// modified by billy 2004/11/1.
		//memcpy(pNewData, m_pData, m_nSize * sizeof(LPVOID));
		//memset(&pNewData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(LPVOID));
		// end here.

		memset(pNewData,0,nNewSize*sizeof(LPVOID));
		memcpy(pNewData,m_pData,m_nSize*sizeof(LPVOID));			

		delete []m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;

#endif
		m_nSize = nNewSize;
		m_pData = (LPVOID *)realloc(m_pData, sizeof(LPVOID) * m_nSize);
		// DS_TRACE("[CDsList::SetSize] ok! copy the old node to the new list.");
		return TRUE;
	}
	return FALSE;
}

BOOL CDsList::Add(LPVOID newElement)
{
	// DS_TRACE("[CDsList::Add] begin...");
	DS_ASSERT(newElement);

	int nIndex = m_nSize;

	// DS_TRACE("[CDsList::Add] seting size to "<<nIndex+1);
	if (SetSize(nIndex + 1))
	{
		m_pData[nIndex] = newElement;
		// DS_TRACE("[CDsList::Add] add node success!");
		return TRUE;
	}
	// DS_TRACE("[CDsList::Add] failed add node.");
	return FALSE;
}

BOOL CDsList::InsertAt(int nIndex, LPVOID newElement)
{
	DS_ASSERT(nIndex >= 0 && nIndex <= m_nSize);
	if (nIndex >= 0 && nIndex <= m_nSize)
	{
		if (nIndex == m_nSize)
			return Add(newElement);

		else if (nIndex < m_nSize)
		{
			int nOldSize = m_nSize;
			if (SetSize(m_nSize + 1))
			{
				memmove(&m_pData[nIndex + 1], &m_pData[nIndex], (nOldSize - nIndex) * sizeof(LPVOID));
				memset(&m_pData[nIndex], 0, sizeof(LPVOID));
			}
			m_pData[nIndex] = newElement;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDsList::Remove(LPVOID oldElement)
{
	DS_ASSERT(oldElement);
	int i;
	for (i = 0; i < m_nSize; i++)
		if (m_pData[i] == oldElement)
			break;

	return RemoveAt(i);
}

BOOL CDsList::RemoveAt(int nIndex)
{
	DS_ASSERT(nIndex >= 0 && nIndex < m_nSize);
	if (nIndex >= 0 && nIndex < m_nSize)
	{
		int nMoveCount = m_nSize - (nIndex + 1);

		if (nMoveCount)
			memmove(&m_pData[nIndex], &m_pData[nIndex + 1], nMoveCount * sizeof(LPVOID));

		m_nSize--;
		if (m_nSize == 0)
		{
			RemoveAll();
		}
		return TRUE;
	}
	return FALSE;
}

void CDsList::RemoveAll()
{
	SetSize(0);
}

int CDsList::GetCount()
{
	return m_nSize;
}

LPVOID CDsList::ElementAt(int nIndex)
{
	//	DS_TRACE("nIndex= "<<nIndex<<"m_nSize= "<<m_nSize<<"\n");
	DS_ASSERT(nIndex >= 0 && nIndex < m_nSize);

	return m_pData[nIndex];
}

BOOL CDsList::IsEmpty()
{
	return m_nSize == 0;
}

// additional
BOOL CDsList::CopyFrom(CDsList &src)
{
	if (SetSize(src.m_nSize))
	{
		memcpy(m_pData, src.m_pData, src.m_nSize * sizeof(void *));
		return TRUE;
	}
	return FALSE;
}

BOOL CDsList::Merge(CDsList &src)
{
	int nOldSize = m_nSize;
	if (SetSize(m_nSize + src.m_nSize))
	{
		// just append
		memcpy(m_pData + nOldSize, src.m_pData, src.m_nSize * sizeof(LPVOID));
		return TRUE;
	}
	return FALSE;
}

LPVOID CDsList::LastElement()
{
	DS_ASSERT(m_nSize);

	return ElementAt(GetCount() - 1);
}

LPVOID CDsList::FirstElement()
{
	DS_ASSERT(m_nSize);

	return ElementAt(0);
}

#include <pthread.h>
pthread_mutex_t g_ThreadsLogLock = PTHREAD_MUTEX_INITIALIZER;

void DsLog(char *lpFormat, ...)
{
	/*
		if(lpFormat==NULL)
			return ;
	pthread_mutex_lock(&g_ThreadsLogLock);
	{
		static BOOL bPrint=TRUE;

		char tchMsg[MAX_PRINTABLE_STRING_LENGTH+1];
		memset(tchMsg,0,MAX_PRINTABLE_STRING_LENGTH+1);

		va_list vl;
		va_start(vl, lpFormat);
		vsnprintf(tchMsg,MAX_PRINTABLE_STRING_LENGTH,lpFormat, vl);
		va_end(vl);

		char tchBuf[MAX_PRINTABLE_STRING_LENGTH+100];
		memset(tchBuf,0,MAX_PRINTABLE_STRING_LENGTH+100);

		time_t tmpTime;
		time(&tmpTime);

		struct tm	tmCur;
		tmCur = *localtime(&tmpTime);
		char szBuf[128];
		memset(szBuf,0,128);
		snprintf(szBuf,128,"%d.%d.%d %d:%d:%d",
				tmCur.tm_year+1900,
				tmCur.tm_mon+1,
				tmCur.tm_mday,
				tmCur.tm_hour,
				tmCur.tm_min,
				tmCur.tm_sec);

		strcat(tchBuf,szBuf);
		strcat(tchBuf,"\t");
		strcat(tchBuf, tchMsg);
		strcat(tchBuf, "\n");

		static char fileName[128]={0};
		FILE* f;
		if(bPrint)
		{
			printf("Pid=%d\n",(int)getpid());
			sprintf(fileName,"../log/log.%d",(int)getpid());
			bPrint=FALSE;
		}

		f = fopen(fileName, "at");
		if( f )
		{
			fprintf(f, tchBuf);
			fclose(f);
		}
		else
		{
			printf("Access deny! %s\n",fileName);
		}
		printf(tchBuf);
	}
	pthread_mutex_unlock(&g_ThreadsLogLock);
	*/
}
/*
#define __RELEASE__			0x00
#define __DEBUG__                   	0x01
#define __DEBUG_TRACE_LOG__ 		0x02
#define __DEBUG_TRACE_TERM__ 		0x04
*/
int g_nDebugMode = __DEBUG__ | __DEBUG_TRACE_LOG__ | __DEBUG_TRACE_TERM__;

pthread_mutex_t g_ThreadsTraceLock = PTHREAD_MUTEX_INITIALIZER;

// Donot call this func directly.
//!!!!!!!!!
void DsDebugInfo(char *lpFormat, ...)
{
	//#ifdef __DEBUG__
	if (g_nDebugMode & __DEBUG__)
	{
		if (lpFormat == NULL)
			return;
		char *p1;
		p1 = strstr(lpFormat, "%s");

		if (p1)
		{
			printf("[Ds_Debug] has error value in string.\n");
			return;
			*p1++ = '-';
			*p1 = '-';
			// strcat(p1,"[ERROR_FORMAT]");
			// return;
		}
		p1 = strstr(lpFormat, "%d");

		if (p1)
		{
			printf("[Ds_Debug] has error value in string.\n");
			return;
			*p1++ = '-';
			*p1 = '-';
			// strcat(p1,"[ERROR_FORMAT]");
			// return;
		}
		p1 = strstr(lpFormat, "%f");

		if (p1)
		{
			printf("[Ds_Debug] has error value in string.\n");
			return;
			*p1++ = '-';
			*p1 = '-';
			// strcat(p1,"[ERROR_FORMAT]");
			// return;
		}

		p1 = strstr(lpFormat, "%u");

		if (p1)
		{
			printf("has error value in string.");
			return;
			*p1 = '\0';
			// strcat(p1,"[ERROR_FORMAT]");
			// return;
		}
		pthread_mutex_lock(&g_ThreadsTraceLock);
		{
			static BOOL bPrint = TRUE;
			/*
			char tchMsg[MAX_PRINTABLE_STRING_LENGTH+1];
			memset(tchMsg,0,MAX_PRINTABLE_STRING_LENGTH+1);

			va_list vl;
			va_start(vl, lpFormat);
			vsnprintf(tchMsg,MAX_PRINTABLE_STRING_LENGTH,lpFormat, vl);
			va_end(vl);
			*/

			char tchBuf[MAX_PRINTABLE_STRING_LENGTH + 100];
			memset(tchBuf, 0, MAX_PRINTABLE_STRING_LENGTH + 100);
			CSTime t;
			strcat(tchBuf, t.GetCurrentTime());
			strcat(tchBuf, " SOHO-Router/");
			strcat(tchBuf, "-->");
			// printf("lpFormat len=%d\n",strlen(lpFormat));
			strcat(tchBuf, lpFormat); //,MAX_PRINTABLE_STRING_LENGTH);
			// strncat(tchBuf, lpFormat,MAX_PRINTABLE_STRING_LENGTH);
			strcat(tchBuf, "\n");

			//#ifdef __DEBUG_TRACE_LOG__
			if (g_nDebugMode & __DEBUG_TRACE_LOG__)
			{
				static char fileName[128] = {0};
				FILE *f;
				if (bPrint)
				{
					printf("Pid=%d\n", (int)getpid());
					sprintf(fileName, "../log/proxy.%d", (int)getpid());
					bPrint = FALSE;
				}

				f = fopen(fileName, "at");
				if (f)
				{
					fprintf(f, tchBuf);
					fclose(f);
				}
				else
				{
					printf("Access deny! %s\n", fileName);
				}
			}
			//#endif //__DEBUG_TRACE_LOG__

			//#ifdef __DEBUG_TRACE_TERM__
			if (g_nDebugMode & __DEBUG_TRACE_TERM__)
			{
				printf(tchBuf);
			}
			//#endif //__DEBUG_TRACE_TERM__
		}
		pthread_mutex_unlock(&g_ThreadsTraceLock);
	}
	//#endif //__DEBUG__
}

int CDsSSL::InitServer()
{

	/* SSL preliminaries. We keep the certificate and key with the context. */

	const SSL_METHOD *meth;

	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();
	meth = SSLv23_server_method();
	ctx = SSL_CTX_new(meth);
	if (!ctx)
	{
		ERR_print_errors_fp(stderr);
		exit(2);
	}

	if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM) <= 0)
	{
		ERR_print_errors_fp(stderr);
		exit(3);
	}
	if (SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM) <= 0)
	{
		ERR_print_errors_fp(stderr);
		exit(4);
	}

	if (!SSL_CTX_check_private_key(ctx))
	{
		fprintf(stderr, "Private key does not match the certificate public key\n");
		exit(5);
	}
	return TRUE;
}

BOOL CDsSSL::CreateServer(SOCKET sd)
{
	/* ----------------------------------------------- */
	/* TCP connection is ready. Do server side SSL. */

	ssl = SSL_new(ctx);
	CHK_NULL(ssl);
	SSL_set_fd(ssl, sd);
	int err = SSL_accept(ssl);
	CHK_SSL(err);

	/* Get the cipher - opt */

	printf("SSL connection using %s\n", SSL_get_cipher(ssl));

	/* Get client's certificate (note: beware of dynamic allocation) - opt */

	X509 *client_cert;
	client_cert = SSL_get_peer_certificate(ssl);
	if (client_cert != NULL)
	{
		printf("Client certificate:\n");

		char *str = X509_NAME_oneline(X509_get_subject_name(client_cert), 0, 0);
		CHK_NULL(str);
		printf("\t subject: %s\n", str);
		free(str);

		str = X509_NAME_oneline(X509_get_issuer_name(client_cert), 0, 0);
		CHK_NULL(str);
		printf("\t issuer: %s\n", str);
		free(str);

		/* We could do all sorts of certificate verification stuff here before
		deallocating the certificate. */

		X509_free(client_cert);
	}
	else
		printf("Client does not have certificate.\n");

	/* DATA EXCHANGE - Receive message and send reply. */

	return TRUE;
}

int CDsSSL::Send(char *szBuf, int nLen)
{
	return SSL_write(ssl, szBuf, nLen);
}
int CDsSSL::Recv(char *szBuf, int nBufSize)
{
	return SSL_read(ssl, szBuf, nBufSize);
}
BOOL CDsSSL::InitClient()
{
	//	Init ssl stuff

	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();

	ctx = SSL_CTX_new(SSLv23_client_method());
	if (ctx == NULL)
	{
		DS_TRACE("Failed create SSL_CTX_new!");
		return FALSE;
	}

	//	Create an SSL thingey and try to negotiate the connection

	ssl = SSL_new(ctx);

	if (ssl == NULL)
	{
		printf("Can not create an ssl thingey!\n");
		return 0;
	}

	return TRUE;
}
BOOL CDsSSL::CreateClient(SOCKET theSocket)
{
	SSL_set_fd(ssl, theSocket);

	int errCode = SSL_connect(ssl);

	if (errCode == -1)
	{
		ERR_print_errors_fp(stderr);

		return FALSE;
	}
	return TRUE;
}
CDsSSL::~CDsSSL()
{

	if (ssl != NULL)
	{
		SSL_free(ssl);
	}

	if (ctx != NULL)
	{
		SSL_CTX_free(ctx);
	}
}
CDsSSL::CDsSSL(SSLTYPE nType)
{
	m_nType = nType;
}
BOOL CDsSSL::Init()
{
	if (m_nType == SSL_SERVER) // Server
	{
		return InitServer();
	}

	return InitClient();
}
BOOL CDsSSL::Create(SOCKET hSocket)
{
	if (m_nType == SSL_SERVER)
	{
		// Server
		return CreateServer(hSocket);
	}
	else if (m_nType == SSL_CLIENT)
	{
		return CreateClient(hSocket);
	}

	return FALSE;
}

/*
 * CDnsCache
 */

CDnsCache::CDnsCache()
{

	memset(m_szFile, 0, 256);
	CConfig cfg(CONF_FILE);
	cfg.Read("dnscache", "cachefile", m_szFile, "../etc/dnscache.txt");
}

CDnsCache::~CDnsCache()
{
	for (int i = 0; i < m_list.GetCount(); i++)
	{
		CHostNode *pNode = (CHostNode *)m_list.ElementAt(i);
		if (pNode)
		{
			delete pNode;
			pNode = NULL;
		}
	}
	m_list.RemoveAll();
}
BOOL CDnsCache::Init()
{

	char szBuf[256];
	FILE *fp;

	fp = fopen(m_szFile, "r");
	if (!fp)
	{
		return FALSE;
	}

	memset(szBuf, 0, 256);
	while (fgets(szBuf, 255, fp))
	{
		CHostNode *pNode = new CHostNode();
		sscanf(szBuf, "%s %s %d", pNode->m_szHost, pNode->m_szIp, &pNode->m_nCount);
		memset(szBuf, 0, 256);

		m_list.Add(pNode);
	}
	fclose(fp);

	return TRUE;
}

char *CDnsCache::Query(char *szHost)
{
	char *szIp;
	for (int i = 0; i < m_list.GetCount(); i++)
	{
		CHostNode *pNode = (CHostNode *)m_list.ElementAt(i);
		if (strcmp(szHost, pNode->m_szHost) == 0)
		{
			pNode->m_nCount++;
			return pNode->m_szIp;
		}
	}

	if (!(szIp = SubQuery(szHost)))
	{
		printf("the query is error !\n");
		return NULL;
	}

	CHostNode *pNode = new CHostNode();
	if (!strcpy(pNode->m_szHost, szHost))
		printf(" The Add Node->m_szHost is error!");
	if (!strcpy(pNode->m_szIp, szIp))
		printf(" The Add Node->m_szIp is error!");
	if (m_list.GetCount() >= 149)
	{
		Work(szHost, szIp);
	}
	pNode->m_nCount++;
	m_list.Add(pNode);
	return szIp;
}

BOOL CDnsCache::Work(char *szHost, char *szIp)
{
	for (int i = 0; i <= m_list.GetCount() && i < 150; i++)
	{
		CHostNode *pTempNode = (CHostNode *)m_list.ElementAt(i);
		pTempNode->m_nCount--;
		if (pTempNode->m_nCount == 0)
		{
			strcpy(pTempNode->m_szHost, szHost);
			strcpy(pTempNode->m_szIp, szIp);
			pTempNode->m_nCount++;
			return TRUE;
		}
	}
	return TRUE;
}

BOOL CDnsCache::DumpToFile()
{
	int i;
	FILE *fp;
	fp = fopen(m_szFile, "w");
	if (!fp)
	{
		return FALSE;
	}
	for (i = 0; i < m_list.GetCount(); i++)
	{
		CHostNode *pNode = (CHostNode *)m_list.ElementAt(i);
		fprintf(fp, "%s %s  %d", pNode->m_szHost, pNode->m_szIp, pNode->m_nCount);
		fprintf(fp, "\n");
	}
	fclose(fp);

	return TRUE;
}

char *CDnsCache::SubQuery(char *szHost)
{
	static char szIp[32];
	memset(szIp, 0, 32);
	struct hostent *Host_ent;

	printf("[CDnsCache::Query]Query dns for Host :%s...\n", szHost);
	Host_ent = gethostbyname(szHost);

	if (Host_ent == 0)
	{
		return NULL;
	}

	strncpy(szIp, inet_ntoa(*(struct in_addr *)*(Host_ent->h_addr_list)), 32);
	printf("[CDnsCache::SuBQuery] SubQuery is done!%s = %s .", szHost, szIp);

	return szIp;
}

/*
 * CDsErrTip
 * 2001/11/22
 * Billy.
 */

CDsErrTip::CDsErrTip()
{
}
CDsErrTip::~CDsErrTip()
{
	if (m_pTip)
	{
		delete[] m_pTip;
		m_nTipLen = 0;
	}
}

int CDsErrTip::GetTipLen()
{
	return m_nTipLen;
}
BYTE *CDsErrTip::GetTip()
{
	return m_pTip;
}

#define DSERR_CONNECT_TIP "../etc/dserr_connect.htm"
#define DSERR_DNSQUERY_TIP "../etc/dserr_dnsquery.htm"
#define DSERR_DISCONNECT_BY_REMOTE_TIP "../etc/dserr_disconnect_by_remote.htm"
#define DSERR_PARSE_TIP "../etc/dserr_parse.htm"
#define DSERR_PROTO_NOT_SUPPORT_TIP "../etc/dserr_proto_not_support.htm"

BOOL CDsErrTip::SetTipType(DSERRTYPE type)
{
	CConfig cfg;

	char szFileName[256];

	switch (type)
	{
	case DSERR_CONNECT:
		cfg.Read("tip", "connect", szFileName, DSERR_CONNECT_TIP);
		return LoadFileToMem(szFileName);
		break;
	case DSERR_DNSQUERY:
		cfg.Read("tip", "dnsquery", szFileName, DSERR_CONNECT_TIP);
		return LoadFileToMem(szFileName);
		break;
	case DSERR_PARSE:
		cfg.Read("tip", "parse", szFileName, DSERR_CONNECT_TIP);
		return LoadFileToMem(szFileName);
		break;
	case DSERR_DISCONNECT_BY_REMOTE:
		cfg.Read("tip", "disconnect_by_remote", szFileName, DSERR_CONNECT_TIP);
		return LoadFileToMem(szFileName);
		break;
	case DSERR_PROTO_NOT_SUPPORT:
		cfg.Read("tip", "proto_not_suport", szFileName, DSERR_CONNECT_TIP);
		return LoadFileToMem(szFileName);
		break;
	default:
		break;
	}
	return FALSE;
}

// Private:

BOOL CDsErrTip::LoadFileToMem(char *szFile)
{
	struct stat st;

	stat(szFile, &st);

	if (m_pTip)
	{
		delete[] m_pTip;
		m_pTip = NULL;
		m_nTipLen = 0;
	}

	m_pTip = new BYTE[st.st_size + 1];

	FILE *fp;
	fp = fopen(szFile, "rb");
	if (!fp)
		return FALSE;

	while (fread(m_pTip + m_nTipLen, 1, 1, fp) == 1)
	{
		m_nTipLen += 1;
	}
	fclose(fp);

	return TRUE;
}

char *DsToUpper(char *szSrc)
{
	//	static char szBuf[1024];
	//	memset(szBuf,0,1024);
	//	if(strlen(szSrc)>1024)
	//		return NULL;

	for (int i = 0; i < (int)strlen(szSrc); i++)
	{
		szSrc[i] = toupper(szSrc[i]);
	}
	return szSrc;
}
//////////////////////////////////////////////////////
// class CDsString

CDsString::CDsString()
{
	m_pData = NULL;
	m_len = 0;
}

CDsString::CDsString(LPSTR str)
{
	m_pData = NULL;
	m_len = 0;

	CopyFrom(str);
}

CDsString::~CDsString()
{
	if (m_pData)
		delete[] m_pData;
	m_pData = NULL;
}

DWORD CDsString::Read(CByteStream &is)
{

	is >> m_len;

	if (m_len > 0 && m_len < 65535)
	{
		m_pData = new char[m_len];
		if (!m_pData)
		{
			m_len = 0;

			return 0;
		}

		memset(m_pData, 0, m_len);
		is.ReadBytes(m_pData, m_len);
	}

	return m_len;
}

DWORD CDsString::GetLength() const
{
	return m_len;
}

LPSTR CDsString::GetData() const
{
	return m_pData;
}

BOOL CDsString::IsEmpty()
{
	return m_len == 0;
}

void CDsString::CopyFrom(LPCSTR str)
{
	if (m_pData)
		delete[] m_pData;

	if (str == NULL)
	{
		m_pData = NULL;
		m_len = 0;
		return;
	}
	m_len = strlen(str) + 1;
	m_pData = new char[m_len];
	if (m_pData == NULL)
	{
		m_len = 0;
		return;
	}

	memcpy(m_pData, str, m_len);
}

BOOL CDsString::Equals(LPSTR str)
{
	DWORD len = 0;
	if (str)
		len = strlen(str);

	if (m_len != len + 1)
		return FALSE;

	for (DWORD i = 0; i < len; i++)
		if (str[i] != m_pData[i])
			return FALSE;

	return TRUE;
}

BOOL CDsString::Equals(CDsString &wstr)
{
	return Equals(wstr.GetData());
}

// add by billy ===
CDsString::CDsString(const CDsString &rhs)
{
	// copy construct
	m_len = rhs.GetLength();
	m_pData = new char[m_len];
	memset(m_pData, 0, m_len);
	memcpy(m_pData, rhs.GetData(), m_len);
}

/*ostream& operator<<(ostream& theStream,CDsString& theString)
{
	theStream<<theString.GetData();
	return theStream;
}*/

CDsString &CDsString::operator=(const CDsString &rhs)
{
	if (this == &rhs)
		return *this;

	if (m_pData)
		delete[] m_pData;

	m_len = rhs.GetLength();

	char *p = rhs.GetData();

	CopyFrom(p);
	return *this;
}
CDsString &CDsString::operator=(const char *str)
{

	if (m_pData)
		delete[] m_pData;

	if (str == NULL)
	{
		m_pData = NULL;
		m_len = 0;
		return *this;
	}
	m_len = strlen(str) + 1;
	m_pData = new char[m_len];
	if (m_pData == NULL)
	{
		m_len = 0;
		return *this;
	}

	memcpy(m_pData, str, m_len);

	return *this;
}

// end of add ============

////////////////////////////////////////////////////
// class CDsBinary
CDsBinary::CDsBinary()
{
	m_pData = NULL;
	m_len = 0;
}

CDsBinary::CDsBinary(LPBYTE lpData, DWORD dwLength)
{
	m_pData = NULL;
	m_len = 0;

	CopyFrom(lpData, dwLength);
}

CDsBinary::~CDsBinary()
{
	if (m_pData)
		delete m_pData;
}

DWORD CDsBinary::Read(CByteStream &is)
{
	is >> m_len;

	if (m_len > 0 && m_len < 65535)
	{
		m_pData = new BYTE[m_len];

		if (m_pData == NULL)
		{
			m_len = 0;
			return 0;
		}

		is.ReadBytes(m_pData, m_len);
	}

	return m_len;
}

DWORD CDsBinary::GetLength()
{
	return m_len;
}

LPBYTE CDsBinary::GetData()
{
	return m_pData;
}

BOOL CDsBinary::IsEmpty()
{
	return m_len == 0;
}

void CDsBinary::CopyFrom(LPBYTE lpData, DWORD dwLength)
{
	if (dwLength == 0)
		return;

	if (m_pData)
		delete m_pData;

	m_pData = NULL;
	m_len = 0;

	m_len = dwLength;
	m_pData = new BYTE[m_len];
	if (m_pData == NULL)
	{
		m_len = 0;
		return;
	}

	memcpy(m_pData, lpData, m_len);
}

BOOL CDsBinary::Equals(LPBYTE lpData, DWORD dwLength)
{
	if (m_len == 0 || dwLength == 0)
		return m_len == dwLength;

	if (m_len != dwLength)
		return FALSE;

	for (DWORD i = 0; i < m_len; i++)
		if (lpData[i] != m_pData[i])
			return FALSE;

	return TRUE;
}

BOOL CDsBinary::Equals(CDsBinary &wszBin)
{
	return Equals(wszBin.GetData(), wszBin.GetLength());
}

LPSTR DsCopyString(LPCSTR lpStr)
{
	if (!lpStr)
		return NULL;
	int len = strlen(lpStr);
	if (!len)
		return NULL;
	LPSTR str = new char[len + 1];
	memset(str, 0, len + 1);
	strcpy(str, lpStr);
	return str;
}

LPSTR GetTimeStr(time_t tm)
{
	char *buf = new char[20];
	memset(buf, 0, 20);
	strftime(buf, 20, "%m/%d/%Y %H:%M:%S", gmtime(&tm));
	return buf;
}

LPSTR GetLocalTimeStr(time_t tm)
{
	char *buf = new char[20];
	memset(buf, 0, 20);
	strftime(buf, 20, "%m/%d/%Y %H:%M:%S", localtime(&tm));
	return buf;
}

INT IpAddrStrToInt(LPCSTR lpszIpAddr)
{
	int ip1 = 0, ip2 = 0, ip3 = 0, ip4 = 0;

	sscanf((const char *)lpszIpAddr, "%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4);

	return (ip1 << 24) + (ip2 << 16) + (ip3 << 8) + ip4;
}
void IpAddrIntToStr(DWORD dwIp, LPSTR lpszIpAddr)
{
	sprintf((char *)lpszIpAddr, "%ld.%ld.%ld.%ld", (dwIp >> 24) & (0x000000ff), (dwIp >> 16) & (0x000000ff), (dwIp >> 8) & 0x000000ff, dwIp & 0x000000ff);
}
/////////////////////////////
//////GetTickCount

DWORD GetTickCount()
{
	DWORD ret;
	struct timeval timeVal;

	gettimeofday(&timeVal, NULL);
	ret = timeVal.tv_sec * 1000 + timeVal.tv_usec / 1000;

	return ret;
}
CInternalVector::CInternalVector()
{
	m_ppHead = NULL;
	m_nAlloc = 0;
	m_nUsed = 0;
}

CInternalVector::CInternalVector(LPVOID *ppHead, int nSize)
{
	m_ppHead = NULL;
	m_nAlloc = 0;
	m_nUsed = 0;

	if (ppHead != NULL)
	{
		m_ppHead = new LPVOID[nSize];
		if (m_ppHead)
		{
			CopyMemory(m_ppHead, ppHead, sizeof(LPVOID) * nSize);
			m_nAlloc = nSize;
			m_nUsed = nSize;
		}
	}
}

CInternalVector::~CInternalVector()
{
	if (m_ppHead)
		delete[] m_ppHead;
}

BOOL CInternalVector::Add(LPVOID lpv)
{
	DS_ASSERT(lpv != NULL);
	if (Expand())
	{
		m_ppHead[m_nUsed++] = lpv;
		return TRUE;
	}
	return FALSE;
}

BOOL CInternalVector::InsertAt(int nIndex, LPVOID lpv)
{
	DS_ASSERT(nIndex >= 0 && nIndex <= m_nUsed);

	if (nIndex >= 0 && nIndex <= m_nUsed)
	{
		if (!Expand())
			return FALSE;

		if (nIndex < m_nUsed)
			// Changed by Stone Aug 15, 1997.
			// change CopyMemory to memmove to ensure overlap security.
			MoveMemory(&m_ppHead[nIndex + 1], &m_ppHead[nIndex],
					   sizeof(LPVOID) * (m_nUsed - nIndex));

		m_ppHead[nIndex] = lpv;
		m_nUsed++;

		return TRUE;
	}
	return FALSE;
}

BOOL CInternalVector::SetAt(int nIndex, LPVOID lpv)
{
	ASSERT(nIndex >= 0 && nIndex < m_nUsed);

	if (nIndex >= 0 && nIndex < m_nUsed)
	{
		m_ppHead[nIndex] = lpv;

		return TRUE;
	}

	return FALSE;
}

BOOL CInternalVector::Remove(LPVOID lpv)
{
	ASSERT(lpv);
	int i;
	for (i = 0; i < m_nUsed; i++)
		if (m_ppHead[i] == lpv)
			break;

	return RemoveAt(i);
}

BOOL CInternalVector::RemoveAt(int i)
{
	if (i < m_nUsed)
	{
		if (i < m_nUsed - 1)
			CopyMemory(&m_ppHead[i], &m_ppHead[i + 1],
					   sizeof(LPVOID) * (m_nUsed - i - 1));
		m_nUsed--;

		if (m_nUsed + DECREASE_DELTA < m_nAlloc)
		{
			LPVOID *ppNew = new LPVOID[m_nAlloc - DECREASE_DELTA];
			if (ppNew)
			{
				if (m_nUsed > 0)
					CopyMemory(ppNew, m_ppHead, sizeof(LPVOID) * m_nUsed);

				if (m_ppHead)
					delete[] m_ppHead;

				m_ppHead = ppNew;
				m_nAlloc -= DECREASE_DELTA;
			}
		}

		return TRUE;
	}
	return FALSE;
}

void CInternalVector::RemoveAll()
{
	if (m_ppHead)
		delete[] m_ppHead;
	m_ppHead = NULL;
	m_nAlloc = 0;
	m_nUsed = 0;
}

/*CInternalVector* CInternalVector::Clone()
{
	return new CInternalVector(m_ppHead, m_nUsed);
}*/

LPVOID *CInternalVector::GetData()
{
	return m_ppHead;
}

int CInternalVector::GetCount()
{
	return m_nUsed;
}

LPVOID CInternalVector::operator[](int i)
{
	if (i >= 0 && i < m_nUsed)
		return m_ppHead[i];

	ASSERT(FALSE);
	return NULL;
}

int CInternalVector::GetIndex(LPVOID lpv)
{
	for (int i = 0; i < m_nUsed; i++)
		if (m_ppHead[i] == lpv)
			return i;

	return -1;
}

BOOL CInternalVector::Expand()
{
	if (m_nUsed >= m_nAlloc)
	{
		LPVOID *ppNew = new LPVOID[m_nAlloc + INCREASE_DELTA];
		if (ppNew)
		{
			if (m_ppHead)
			{
				CopyMemory(ppNew, m_ppHead, sizeof(LPVOID) * m_nAlloc);
				delete[] m_ppHead;
			}
			m_ppHead = ppNew;
			m_nAlloc += INCREASE_DELTA;
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CInternalMap
//
CInternalMap::CInternalMap(UINT tablesize)
{
	DS_ASSERT(tablesize > 0);
	m_nHashTableSize = tablesize;
	m_pHashTable = new CInternalVector[tablesize];
	m_nCount = 0;
}

CInternalMap::~CInternalMap()
{
	RemoveAll();
	ASSERT(m_nCount == 0);

	if (m_pHashTable)
		delete[] m_pHashTable;
}

int CInternalMap::GetCount()
{
	return m_nCount;
}

BOOL CInternalMap::IsEmpty()
{
	return (m_nCount == 0);
}

UINT CInternalMap::GetHashTableSize()
{
	return m_nHashTableSize;
}

BOOL CInternalMap::SetAt(void *key, void *newValue)
{
	DS_TRACE("CInternalMap::SetAt...");
	if (m_pHashTable == NULL)
		return FALSE;

	UINT nHash;
	CAssoc *pAssoc;
	DS_TRACE("Get assoc at the key");
	if ((pAssoc = GetAssocAt(key, nHash)) == NULL)
	{
		// it doesn't exist, add a new Association
		CAssoc *pAssoc = new CAssoc;
		if (pAssoc == NULL)
			return FALSE;

		pAssoc->key = key;
		pAssoc->value = newValue;

		// put into hash table
		if (!m_pHashTable[nHash % m_nHashTableSize].Add(pAssoc))
		{
			delete pAssoc;
			return FALSE;
		}

		m_nCount++;
		ASSERT(m_nCount > 0); // make sure we don't overflow
	}
	else
		pAssoc->value = newValue;

	return TRUE;
}

BOOL CInternalMap::Lookup(void *key, void *&rValue)
{
	UINT nHash;
	CAssoc *pAssoc = GetAssocAt(key, nHash);
	if (pAssoc == NULL)
		return FALSE; // not in map

	rValue = pAssoc->value;
	return TRUE;
}

// remove key - return TRUE if removed
BOOL CInternalMap::RemoveKey(void *key)
{
	if (m_pHashTable == NULL)
		return FALSE;

	CInternalVector &vector = m_pHashTable[HashKey(key) % m_nHashTableSize];

	for (int i = 0; i < vector.GetCount(); i++)
	{
		CAssoc *pAssoc = (CAssoc *)(vector[i]);
		if (Equals(pAssoc->key, key))
		{
			vector.RemoveAt(i);
			delete pAssoc;
			m_nCount--;
			return TRUE;
		}
	}

	return FALSE; // not found
}

void CInternalMap::RemoveAll()
{
	if (m_pHashTable != NULL)
	{
		for (UINT i = 0; i < m_nHashTableSize; i++)
		{
			CInternalVector &vector = m_pHashTable[i];
			for (int j = 0; j < vector.GetCount(); j++)
			{
				CAssoc *pAssoc = (CAssoc *)(vector[j]);
				delete pAssoc;
			}
			vector.RemoveAll();
		}
	}

	m_nCount = 0;
}

MAPPOS CInternalMap::GetStartPosition()
{
	if (m_pHashTable == NULL || m_nCount == 0)
		return NULL;

	return (MAPPOS)1;
}

// Notice: Donot call me when rNextPosition==NULL
void CInternalMap::GetNextAssoc(MAPPOS &rNextPosition,
								void *&rKey, void *&rValue)
{
	int count = *(int *)rNextPosition - 1;
	if (m_pHashTable == NULL || rNextPosition == NULL || count >= m_nCount)
	{
		DS_ASSERT(FALSE);
		rKey = NULL;
		rValue = NULL;
		return;
	}

	int i, n = 0;
	for (i = 0; count - n >= m_pHashTable[i].GetCount(); i++)
		n += m_pHashTable[i].GetCount();

	CAssoc *pAssoc = (CAssoc *)(m_pHashTable[i][count - n]);
	rKey = pAssoc->key;
	rValue = pAssoc->value;

	count++;
	if (count >= m_nCount)
		rNextPosition = NULL;
	else
		rNextPosition = (MAPPOS)(count + 1);
}

// find association (or return NULL)
CInternalMap::CAssoc *
CInternalMap::GetAssocAt(void *key, UINT &nHash)
{
	ASSERT(key);

	if (m_pHashTable == NULL)
		return NULL;

	nHash = HashKey(key) % m_nHashTableSize;

	CInternalVector &vector = m_pHashTable[nHash % m_nHashTableSize];
	for (int i = 0; i < vector.GetCount(); i++)
	{
		CAssoc *pAssoc = (CAssoc *)(vector[i]);
		if (Equals(pAssoc->key, key))
			return pAssoc;
	}
	return NULL;
}

// find value (or return NULL -- NULL values not different as a result)
void *CInternalMap::GetValueAt(void *key)
{
	UINT nHash;
	CAssoc *pAssoc = GetAssocAt(key, nHash);
	if (pAssoc)
		return pAssoc->value;

	return NULL;
}

void *CInternalMap::operator[](void *key)
{
	return GetValueAt(key);
}

UINT CInternalMap::HashKey(void *key)
{
	return *(int *)key;
	// default identity hash - works for most primitive values
}

BOOL CInternalMap::Equals(void *key1, void *key2)
{
	return (key1 == key2);
}
////////////////////////////////////////////////////////////////////////
// class CInternalCriticalSection
//

CTimerMgr *g_pTimerMgr = NULL;
CTimerMgr *CTimerMgr::m_pTimerMgr = 0;

CInternalCriticalSection::CInternalCriticalSection()
{
	m_critical = new (pthread_mutex_t);
	int ret = pthread_mutex_init(m_critical, NULL);
	ASSERT(ret == 0);
	m_condition = new (pthread_cond_t);
	ret = pthread_cond_init(m_condition, NULL);
	ASSERT(ret == 0);
	m_ownerThreadID = 0;
	m_nRefCount = 0;
}

CInternalCriticalSection::~CInternalCriticalSection()
{
	pthread_mutex_destroy(m_critical);
	delete m_critical;
	pthread_cond_destroy(m_condition);
	delete m_condition;
}

BOOL CInternalCriticalSection::TryLock()
{
	BOOL bResult = FALSE;

	pthread_mutex_lock(m_critical);

	if (m_ownerThreadID == 0)
	{
		ASSERT(m_nRefCount == 0);

		m_ownerThreadID = pthread_self();
		m_nRefCount = 1;
		bResult = TRUE;
	}
	else if (m_ownerThreadID == pthread_self())
	{
		ASSERT(m_nRefCount >= 1);
		m_nRefCount++;
		bResult = TRUE;
	}

	pthread_mutex_unlock(m_critical);
	return bResult;
}

void CInternalCriticalSection::Lock()
{
	int nReturn = pthread_mutex_lock(m_critical);
	ASSERT(nReturn == 0);

	while (m_ownerThreadID != pthread_self() && m_ownerThreadID != 0)
	{
		nReturn = pthread_cond_wait(m_condition, m_critical);

		switch (nReturn)
		{
		case EINTR:
			break;

		default:
			ASSERT(FALSE);
			break;
		}
	}

	if (m_ownerThreadID == 0)
	{
		ASSERT(m_nRefCount == 0);

		m_ownerThreadID = pthread_self();
		m_nRefCount = 1;
	}
	else
	{
		ASSERT(m_nRefCount >= 1);
		m_nRefCount++;
	}
	pthread_mutex_unlock(m_critical);
}

void CInternalCriticalSection::Unlock()
{
	pthread_mutex_lock(m_critical);

	ASSERT(m_ownerThreadID == pthread_self());
	m_nRefCount--;
	if (m_nRefCount == 0)
	{
		m_ownerThreadID = 0;
		pthread_cond_signal(m_condition);
	}

	pthread_mutex_unlock(m_critical);
}

/////////////////////////////////////////////////////////////////////////////
// CTimerMgr
//
CTimerMgr::CTimerMgr()
{
	m_pTimerMgr = this;
	pthread_attr_t thrattr;
	pthread_attr_init(&thrattr);
	pthread_attr_setdetachstate(&thrattr, PTHREAD_CREATE_JOINABLE);
	int err;
	err = pthread_create(&m_timerThreadID, &thrattr, worker, (void *)this);
	ASSERT(err == 0);
	ASSERT(m_timerThreadID != 0);
	struct sched_param param;
	param.sched_priority = 80;
	pthread_setschedparam(m_timerThreadID, SCHED_FIFO, &param);
	pthread_attr_destroy(&thrattr);
	m_nFirstUnusedSlot = -1;
	m_dwLastTick = GetTickCount();
}
CTimerMgr::~CTimerMgr()
{
	// TRCFUNC("CTimerMgr::~CTimerMgr");
	m_runnable.Lock();
	if (m_timerThreadID != 0)
	{
		pthread_cancel(m_timerThreadID);
		pthread_join(m_timerThreadID, NULL);
		m_timerThreadID = 0;
		m_timerList.RemoveAll();
		for (int i = 0; i < m_eventList.GetCount(); i++)
		{
			StruTimerEvent *pEvent = (StruTimerEvent *)m_eventList[i];
			delete pEvent;
		}
		m_eventList.RemoveAll();
	}
	m_runnable.Unlock();
}

UINT CTimerMgr::RegisterTimer(IInProcessTimerSink *pSink)
{
	DS_TRACE("CTimerMgr::RegisterTimer");
	UINT idx;
	m_runnable.Lock();
	if (m_nFirstUnusedSlot < 0)
	{
		m_timerList.Add(pSink);
		idx = m_timerList.GetCount() - 1;
	}
	else
	{
		idx = m_nFirstUnusedSlot;
		m_nFirstUnusedSlot = *(int *)m_timerList[m_nFirstUnusedSlot];
		// m_nFirstUnusedSlot = (UINT)m_timerList[m_nFirstUnusedSlot];
		m_timerList.SetAt(idx, pSink);
	}
	m_runnable.Unlock();
	return idx;
}
void CTimerMgr::UnregisterTimer(UINT idx)
{
	m_runnable.Lock();
	m_timerList.SetAt(idx, (void *)m_nFirstUnusedSlot);
	m_nFirstUnusedSlot = idx;
	KillTimer(idx);
	KillTimeOut(idx);
	m_runnable.Unlock();
}

void CTimerMgr::SetTimer(UINT idx, UINT nElapse, char *strId)
{
	DS_TRACE("CTimerMgr::SetTimer, idx=" << idx << ", strId=" << strId);
	StruTimerEvent *pEvent = new StruTimerEvent;
	if (pEvent)
	{
		pEvent->m_index = idx;
		pEvent->m_bDeleting = FALSE;
		pEvent->m_bTimer = TRUE;
		pEvent->m_nElapse = nElapse;
		pEvent->m_nReserve = nElapse;
		pEvent->m_nRunning = 0;
		if (strId)
			pEvent->m_strId.CopyFrom(strId);
		else
			pEvent->m_strId.CopyFrom("Undefined");
		m_eventList.Add(pEvent);
	}
}

void CTimerMgr::KillTimer(UINT idx)
{
	DS_TRACE("CTimerMgr::KillTimer, idx=" << idx);
	m_runnable.Lock();
	for (int i = 0; i < m_eventList.GetCount(); i++)
	{
		StruTimerEvent *pEvent = (StruTimerEvent *)m_eventList[i];
		if (pEvent->m_index == idx)
		{
			pEvent->m_bDeleting = TRUE;
			break;
		}
	}
	m_runnable.Unlock();
}

void CTimerMgr::SetTimeOut(UINT idx, UINT nTimeOutType, UINT nTimeOutElapse)
{
	DS_TRACE("CTimerMgr::SetTimeOut");
	StruTimerEvent *pEvent = new StruTimerEvent;
	if (pEvent)
	{
		pEvent->m_index = idx;
		pEvent->m_bDeleting = FALSE;
		pEvent->m_bTimer = FALSE;
		pEvent->m_nElapse = nTimeOutElapse;
		pEvent->m_nReserve = nTimeOutType;
		pEvent->m_nRunning = 0;
		m_eventList.Add(pEvent);
	}
}

void CTimerMgr::KillTimeOut(UINT idx, UINT nTimeOutType)
{
	DS_TRACE("CTimerMgr::KillTimeOut,IDX=" << idx << "TimeOutType=" << nTimeOutType);

	m_runnable.Lock();
	for (int i = 0; i < m_eventList.GetCount(); i++)
	{
		StruTimerEvent *pEvent = (StruTimerEvent *)m_eventList[i];
		if (pEvent->m_index == idx && pEvent->m_nReserve == nTimeOutType)
		{
			pEvent->m_bDeleting = TRUE;
			break;
		}
	}
	m_runnable.Unlock();
}

void CTimerMgr::KillTimeOut(UINT idx)
{
	DS_TRACE("CTimerMgr::KillTimeOut,IDX=" << idx);
	m_runnable.Lock();
	for (int i = 0; i < m_eventList.GetCount(); i++)
	{
		StruTimerEvent *pEvent = (StruTimerEvent *)m_eventList[i];
		if (pEvent->m_index == idx)
			pEvent->m_bDeleting = TRUE;
	}
	m_runnable.Unlock();
}

DWORD g_check_living_tick = 0;
DWORD g_check_interval_tick = 0;

void *CTimerMgr::worker(void *p)
{
	DS_TRACE("CTimerMgr::worker");
	while (1)
	{
#ifdef __FreeBSD__
		WindowProc1(SIGALRM, (sig_t *)NULL, (void *)NULL);
#else
		WindowProc1(SIGALRM, (siginfo_t *)NULL, (void *)NULL);
#endif
		pthread_testcancel();
		usleep(100);
	}
	return 0;
}

int CTimerMgr::WindowProc(WORD uMsg, WORD wParam, LONG lParam)
{
	return 0;
}
#ifdef __FreeBSD__
void CTimerMgr::WindowProc1(int sig, sig_t *info, void *context)
#else
void CTimerMgr::WindowProc1(int sig, siginfo_t *info, void *context)
#endif

{
	// char traceBuf[200];
	// struct timespec ts;
	switch (sig)
	{
	case SIGALRM:
		if (m_pTimerMgr->m_runnable.TryLock())
		{
			if (g_check_living_tick == 0)
				g_check_living_tick = GetTickCount();
			else
			{
				if (GetTickCount() - g_check_living_tick > 600000)
					g_check_living_tick = GetTickCount();
			}
			DWORD dwStep = GetTickCount() - m_pTimerMgr->m_dwLastTick;
			DWORD dwProcess;
			dwProcess = GetTickCount();
			for (int i = 0; i < m_pTimerMgr->m_eventList.GetCount(); i++)
			{
				StruTimerEvent *pEvent = (StruTimerEvent *)m_pTimerMgr->m_eventList[i];
				if (pEvent->m_bDeleting)
				{
					m_pTimerMgr->m_eventList.RemoveAt(i);
					delete pEvent;
					i--;
					continue;
				}
				if (pEvent->m_bTimer)
				{
					if (pEvent->m_nElapse < dwStep)
					{
						// DS_TRACE("CTimerMgr::WindowProc1,4");
						DWORD dwTemp;
						dwTemp = GetTickCount();
						((IInProcessTimerSink *)m_pTimerMgr->m_timerList[pEvent->m_index])->OnInProcessTimerTick();
						pEvent->m_nElapse = pEvent->m_nReserve;
					}
					else
						pEvent->m_nElapse -= dwStep;
				}
				else
				{
					if (pEvent->m_nElapse < dwStep)
					{
						((IInProcessTimerSink *)m_pTimerMgr->m_timerList[pEvent->m_index])->OnInProcessTimeOut(pEvent->m_nReserve, 1);
						pEvent->m_bDeleting = TRUE;
					}
					else
						pEvent->m_nElapse -= dwStep;
				}
			}
			g_check_interval_tick = GetTickCount();
			m_pTimerMgr->m_dwLastTick = GetTickCount();
			m_pTimerMgr->m_runnable.Unlock();
		}
		return;
	case SIGABRT:
		break;
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CInProcessTimer
//
CInProcessTimer::CInProcessTimer(IInProcessTimerSink *pSink)
{
	DS_TRACE("CInProcessTimer::CInProcessTimer");
	m_pSink = pSink;
	if (g_pTimerMgr)
		m_index = g_pTimerMgr->RegisterTimer(pSink);
	else
		DS_ASSERT(FALSE);
}

CInProcessTimer::~CInProcessTimer()
{
	DS_TRACE("CInProcessTimer::~CInProcessTimer");
	if (g_pTimerMgr)
		g_pTimerMgr->UnregisterTimer(m_index);
	else
		DS_ASSERT(FALSE);
}

void CInProcessTimer::Destroy()
{
	delete this;
}

BOOL CInProcessTimer::SetTimer(UINT nElapse, char *strId)
{
	DS_TRACE("CInProcessTimer::SetTimer");
	if (g_pTimerMgr)
		g_pTimerMgr->SetTimer(m_index, nElapse, strId);
	else
		DS_ASSERT(FALSE);
	return TRUE;
}

void CInProcessTimer::KillTimer()
{
	DS_TRACE("CInProcessTimer::KillTimer");
	if (g_pTimerMgr)
		g_pTimerMgr->KillTimer(m_index);
	else
		DS_ASSERT(FALSE);
}

BOOL CInProcessTimer::SetTimeOut(UINT nTimeOutType, UINT nTimeOutElapse)
{
	DS_TRACE("CInProcessTimer::SetTimeOut");
	if (g_pTimerMgr)
		g_pTimerMgr->SetTimeOut(m_index, nTimeOutType, nTimeOutElapse);
	else
		DS_ASSERT(FALSE);
	return TRUE;
}

void CInProcessTimer::KillTimeOut()
{
	if (g_pTimerMgr)
		g_pTimerMgr->KillTimeOut(m_index);
	else
		DS_ASSERT(FALSE);
}

void CInProcessTimer::KillTimeOut(UINT nTimeOutType)
{
	DS_TRACE("CInProcessTimer::KillTimeOut");
	if (g_pTimerMgr)
		g_pTimerMgr->KillTimeOut(m_index, nTimeOutType);
	else
		DS_ASSERT(FALSE);
}

void DsInitializeTimer()
{
	DS_TRACE("WSZINITIALIZETIMER");
	printf("enter WSZINITIALIZETIMER\n");
	if (!g_pTimerMgr)
		g_pTimerMgr = new CTimerMgr();
	printf("leave WSZINITIALIZETIMER\n");
}

void DsShutdownTimer()
{
	if (g_pTimerMgr->GetTimerNumber() == 0)
		delete g_pTimerMgr;
}
//////////////////////////////////////////////////////////////////////
// CSTime Class
//////////////////////////////////////////////////////////////////////

CSTime::CSTime()
{
}

CSTime::~CSTime()
{
}

time_t CSTime::GetTime()
{
	return m_time;
}
CSTime::CSTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)
{

	struct tm tmTime;

	tmTime.tm_year = nYear - 1900;
	tmTime.tm_mon = nMonth - 1;
	tmTime.tm_mday = nDay;
	tmTime.tm_hour = nHour;
	tmTime.tm_min = nMin;
	tmTime.tm_sec = nSec;
	tmTime.tm_isdst = -1;

	m_time = mktime(&tmTime);
	setTime(tmTime);
}

CSTime::CSTime(char *str)
{
	if (strlen(str) < 10)
		return;
	struct tm tmTime;

	char y[5], t[3];
	memset(y, 0, 5);
	memset(t, 0, 3);
	strncpy(y, str, 4);
	tmTime.tm_year = atoi(y) - 1900;
	strncpy(t, str + 4, 2);
	tmTime.tm_mon = atoi(t) - 1;
	strncpy(t, str + 6, 2);
	tmTime.tm_mday = atoi(t);
	strncpy(t, str + 8, 2);
	tmTime.tm_hour = atoi(t);

	if (strlen(str) >= 12)
	{
		strncpy(t, str + 10, 2);
		tmTime.tm_min = atoi(t);
	}
	else
	{
		tmTime.tm_min = 0;
	}
	if (strlen(str) == 14)
	{
		strncpy(t, str + 12, 2);
		tmTime.tm_sec = atoi(t);
	}
	else
	{
		tmTime.tm_sec = 0;
	}
	tmTime.tm_isdst = -1;
	m_time = mktime(&tmTime);
}

/*
ATString CSTime::GetCurrentTime()
{

	time_t	tmpTime;
	time(&tmpTime); //Get current time.
	ATString strTmp;
	strTmp=ConvertTimeToStr(tmpTime);
	return strTmp;
}*/
DWORD g_dwCurrentTime = 0;

DWORD GetCurTime()
{
	/*
	time_t t;
	time(&t);

	DWORD dwTm = t;
	return dwTm;
	*/

	return g_dwCurrentTime;
}
BOOL IsInNextIntvl(DWORD &dwTime, DWORD iIntvl_time)
{

	int iHourtime = GetCurTime() / 3600 * 3600;
	int iIntvlCnt = (GetCurTime() - iHourtime) / INTERVAL_TIME;
	DWORD dwCurIntvlTm = iHourtime + iIntvlCnt * INTERVAL_TIME;
	if (dwCurIntvlTm == dwTime)
		return FALSE;
	dwTime = dwCurIntvlTm;
	return TRUE;
}

unsigned char HexToDec(char szHex)
{
	switch (szHex)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'a':
	case 'A':
		return 10;
	case 'b':
	case 'B':
		return 11;
	case 'c':
	case 'C':
		return 12;
	case 'd':
	case 'D':
		return 13;
	case 'e':
	case 'E':
		return 14;
	case 'f':
	case 'F':
		return 15;
	}
	return -1;
}

/*
** 函数：AscIIToBin()
** 作用：
** 将ascii形式的字符串1f转成二进制bin格式：0x1f
*/

unsigned char AscIIToBin(char *szAscII)
{
	/*assert(strlen(szAscII)==2);*/ /*确保为两个字节的字符串*/

	return HexToDec(szAscII[0]) * 16 + HexToDec(szAscII[1]);
}
