#ifndef __MIME_H__
#define __MIME_H__

#include <gmime/gmime.h>
#include "doorsbase.h"
class CMime
{
public:
	CMime(){
		m_pAttaList = new CDsList();	
		m_pBody = NULL;
	}
	~CMime(){
		DS_TRACE("[CMime::~CMime] release mime memory...");
		if(m_pBody)
		{
			g_free(m_pBody);
			m_pBody = NULL;
		}
		if(m_pAttaList)
		{
			for(int i=0;i<m_pAttaList->GetCount();i++)
			{
				char* pValue = (char*)m_pAttaList->ElementAt(i);
				delete []pValue;
				pValue =NULL;
			}
			m_pAttaList->RemoveAll();
			m_pAttaList = NULL;
		}
	}
	void SetContent(char* pMail,int nSize);
	char* GetBody(){
		return m_pBody;
	}
	CDsList* GetAttaList() {
		return m_pAttaList;
	}

	char* GetSubject() {
		return m_szSubject;
	}
	char* GetDate() {
		return m_szDate;
	}
	char* GetFrom() {
		return m_szFrom;
	}
	char* GetTo() {
		return m_szTo;
	}
	char* GetCc() {
		return m_szCc;
	}
private:
	CDsList* m_pAttaList;
	char* m_pBody;
	char m_szSubject[1024];
	char m_szDate[1024];
	char m_szFrom[1024];
	char m_szTo[1024];
	char m_szCc[1024];
private:
	void print_mime_struct(GMimeObject* part,int depth);
};
#endif
