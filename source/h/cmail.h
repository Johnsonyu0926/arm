#ifndef __CMAIL_H__
#define __CMAIL_H__

#include <stdio.h>
#include <string.h>
#include <gmime/gmime.h>
class CMail
{
public:
	CMail(){
		g_mime_init(0);
	}
	~CMail() {
		if(m_szMessage)
		{
			g_free(m_szMessage);
			m_szMessage = NULL;
		}
		if(m_szContent)
		{
			delete []m_szContent;
			m_szContent = NULL;
		}
		//g_mime_shutdown();
	}
	void SetSubject(char* szSubject){ strncpy(m_szSubject,szSubject,sizeof(m_szSubject));}
	void SetFrom(char* szFrom) { strncpy(m_szFrom,szFrom,sizeof(m_szFrom));}
	void SetTo(char* szTo) { strncpy(m_szTo,szTo,sizeof(m_szTo));}
	void SetDate(char* szDate) { strncpy(m_szDate,szDate,sizeof(m_szDate)); }

	void SetContent(char* szContent) { 
		if(szContent==NULL) return;
		m_szContent = new char[strlen(szContent)+1];
		memcpy(m_szContent,szContent,strlen(szContent));
	}

	int BuildMail();
	char* GetMessage() { return m_szMessage; }

private:
	char* m_szMessage;
	char* m_szContent;

	char m_szSubject[1024];
	char m_szFrom[1024];
	char m_szTo[1024];
	char m_szDate[256];
};

#endif

