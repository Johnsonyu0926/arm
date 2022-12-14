#ifndef __ICAP_H__
#define __ICAP_H__
#include "doorsbase.h"
typedef struct ContentList{
	char* Content;
	int ContentLen;
	int id;
	struct ContentList *Next;
}CList;

class CIcap
{
public:
	CIcap(){ 
		MyList =new CList;
		pList = MyList; 
		MyList->Content = NULL;
		MyList->ContentLen = 0;
		MyList->Next = NULL;
		m_pContent = NULL;
	        m_nContentLen = 0;
		count = 0;
		}
	~CIcap() { 
		if(m_pContent) {
			delete m_pContent;
			m_pContent =NULL;
		}
	}
		
	void AddContent(char* szContent,int nLen)
	{
		DS_TRACE("ADD CONTENT \n"<<count);
		count++;
		CList *NewNode = new CList;
		NewNode->Content = szContent;
		NewNode->ContentLen = nLen;
		NewNode->id = count;
		NewNode->Next = NULL;
		pList->Next =NewNode;
		pList = pList->Next;
		m_nContentLen = m_nContentLen + nLen;
	}

	void SetIcapServer(char* szServer,int nServerPort)
	{
		strncpy(m_szIcapServer,szServer,sizeof(m_szIcapServer));
		m_nIcapServerPort = nServerPort;
	}
		

	// use respmod to send to icap server and check the response.  
	BOOL RespMod(char * pszAddress, int argflag,char* szVirus,int nLen);
	void PrintContent()
	{	
		for (int i =0 ; i<count ; i++)
		{
			DS_TRACE("I IS : "<<i);
		}
	}

	BOOL ReleaseList();
	
private:
	char* m_pContent;
	int m_nContentLen;
	char m_szIcapServer[32];
	int m_nIcapServerPort;
	CList *MyList;
	CList *pList;
	BOOL IsFirst;
	int count;
};
#endif
