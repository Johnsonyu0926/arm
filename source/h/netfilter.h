// netfilter.h: interface for the CDsNetworkFilter class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __DS_NETFILTER_H__
#define __DS_NETFILTER_H__

#include <time.h>//for time_t
#include "doorsdefs.h"
//#include "tpreader.h"	//for TP_CONNECTION_TYPE_TCPIP ...
//#include "tcpreader.h"
//#include "httpreader.h"
#include "ngdb.h"

class CDsNetworkFilter : public IDsNetworkFilter
{
public:

	//added by billy.
	//2003/11/13
	COracleReader* GetOracle()
	{
		return m_pOracle;
	}
	int GetCacheCount()
	{
		return m_nCacheCount;
	}

	void SetOracle(COracleReader* oracle)
	{
		m_pOracle = oracle;
	}
	void SetCacheCount(int nCount)
	{
		m_nCacheCount = nCount;
	}

	void SetLogTime(int nLogTime) { m_nLogTime = nLogTime; }
	int GetLogTime() { return m_nLogTime; } 
	void SetInterval(DWORD);//sec
	BOOL IsRealTime(void);
	DWORD GetToTime(void);
	DWORD GetFromTime(void);
	DWORD GetRealToTime(void);
	
	void SetRealToTime(DWORD dwTime);
	void SetToTime(DWORD dwTime);
	void SetFromTime(DWORD dwTime);
	void SetTimeRange(DWORD dwFromTime, DWORD dwToTime);
	BOOL CheckTimeAfter(DWORD dwTime);
	BOOL CheckTimeBefore(DWORD dwTime);
	BOOL CheckAddress(LPCSTR ip_address1, LPCSTR ip_address2);
	BOOL CheckPort(int port);
	BOOL CheckTime(DWORD dwTime);
	void ExcludeAddress(LPCSTR ip_address1, LPCSTR ip_address2);
	void ExcludeAddress(LPCSTR ip_address);
	void IncludeAddress(LPCSTR ip_address1, LPCSTR ip_address2);
	void IncludeAddress(LPCSTR ip_address);
	void ExcludePort(int port);
	void IncludePort(int port);
	CDsNetworkFilter();
	CDsNetworkFilter(DWORD dwFromTime);
	virtual ~CDsNetworkFilter();

	//added by White 2000/12/20
	void SetFilePath(LPCSTR file_path);
	LPCSTR GetFilePath();
    	LPCSTR GetAddress()
    	{
    		if(m_iInAddrToAnyCount==0)
    			return NULL;
    		return m_szIncludeAddrToAny[0];
    	}
    	WORD	GetPort() {return (WORD)m_iIncludePort[0];}
	//virtual void SetPort(WORD nPort){m_wHttpPort=nPort;}
	//virtual WORD GetPort(){return m_wHttpPort;} 
	
	//oliver added 
	 void SetSrcToDst(LPCSTR pSrcIP,LPCSTR pDstIP)
	 {
		m_strSrcIP.CopyFrom(pSrcIP);
		m_strDstIP.CopyFrom(pDstIP); 	
	 }	
	 	
		//return packet which src addr is pSrcipAddr and dst addr is pDstIpAddr,NULL stand for all.
	LPCSTR GetSrcAddr(){ return m_strSrcIP.GetData();}
	LPCSTR GetDstAddr(){ return m_strDstIP.GetData();}
	BOOL  CheckSrcAndDstAddr(LPCSTR lpSrcIPAddr,LPCSTR lpDstIPAddr)
	{
		if(!lpSrcIPAddr || !lpDstIPAddr)
			return FALSE;
			
		if(m_strSrcIP.GetData() == NULL && m_strDstIP.GetData() == NULL)
			return TRUE;
			
		if(m_strSrcIP.GetData() == NULL)
		{
			if((strcmp(m_strDstIP.GetData(),lpDstIPAddr) == 0)||
				(strcmp(m_strDstIP.GetData(),lpSrcIPAddr)==0))
				return TRUE;
			return FALSE;	
		}	

		if(m_strDstIP.GetData() == NULL)
		{
			if((strcmp(m_strSrcIP.GetData(),lpSrcIPAddr) == 0)||
				(strcmp(m_strSrcIP.GetData(),lpDstIPAddr) == 0))
				return TRUE;
			return FALSE;	
		}	
		
		if(strcmp(m_strSrcIP.GetData(),lpSrcIPAddr) == 0 && strcmp(m_strDstIP.GetData(),lpDstIPAddr) == 0 )
			return TRUE;
		
		if(strcmp(m_strSrcIP.GetData(),lpDstIPAddr) == 0 && strcmp(m_strDstIP.GetData(),lpSrcIPAddr) == 0 )
			return TRUE;
		
		
		return FALSE;	
	}		

//private:		
protected:
	//modified by billy
	//2000 12/17

	int m_nLogTime;
	void Init(void);
    DWORD		m_dwFromTime;
    DWORD		m_dwToTime;
	
    int		m_iIncludePort[256];
    int		m_iExcludePort[256];	
    char	m_szIncludeAddrToAny[256][15];
    char	m_szExcludeAddrToAny[256][15];
    char	m_szIncludeAddrToAddr[256][32];
    char	m_szExcludeAddrToAddr[256][32];
	
	/*
    CDsList*	m_iIncludePort;
    CDsList*	m_iExcludePort;	
    CDsList*	m_szIncludeAddrToAny;
    CDsList*	m_szExcludeAddrToAny;
    CDsList*	m_szIncludeAddrToAddr;
    CDsList*	m_szExcludeAddrToAddr;
	*/
	int		m_iInPortCount;
	int		m_iExPortCount;
	int		m_iInAddrToAnyCount;
	int		m_iExAddrToAnyCount;
	int		m_iInAddrToAddrCount;
	int		m_iExAddrToAddrCount;

    	BOOL		m_iLiveTime;
    	time_t		m_dwRealToTime;
    	//DWORD		m_dwRealFromTime;
	DWORD		m_dwInterval;

	ATString	m_FilePath;
	WORD 		m_wHttpPort;
	
	ATString        m_strSrcIP;
	ATString        m_strDstIP;

private:
	COracleReader* m_pOracle;
	int m_nCacheCount;
};

/*
//add by billy

#define TP_DEFAULT_PORT_TCPIP	1270
#define TP_DEFAULT_PORT_HTTP	80

class CDsTpServerParam;
class CDsTpNetworkFilter  : public CDsNetworkFilter
{
public:
	int AddTpServerToMonitor(CDsTpServerParam* pTpParam);
	CDsTpNetworkFilter();
	CDsTpNetworkFilter(DWORD dwTime);
	virtual ~CDsTpNetworkFilter();
	int CheckConnectionType(IDsTcpConnection* pTcpConn,
		ATString& szServerAddress,
		int& nServerPort,
		ATString& szClientAddress,
		int& nClientPort);
	//Add by Billy 26/Dec
	int CheckConnectionType(IDsHttpPacket* pHttpPacket);
	int GetHttpPort();
protected:
	CDsList* m_pTpServerList;

};
*/
#endif // !defined(__WSZ_NETFILTER_H__)
