#ifndef __CMAILAUDIT_H__
#define __CMAILAUDIT_H__

#include <gmime/gmime.h>

#define CHECK_WHITE	0
#define CHECK_BLACK	1

#define AUDIT_PASS		0
#define AUDIT_FORBIDEN_ATTACHMENT	1
#define AUDIT_FORBIDEN_BODY		2
#define AUDIT_FORBIDEN_SUBJECT		3
class CQSearch;
class CMailAudit
{
	public:
		CMailAudit(){
			
			g_mime_init(0);
			memset(m_szFoundKey,0,sizeof(m_szFoundKey));
		}
		~CMailAudit(){
	
			//g_mime_shutdown();
		}

	public:

		char* GetSubject() {
			return m_szSubject;
		}
		char* GetFrom() {
			return m_szFrom;
		}
		char* GetTo(){
			return m_szTo;
		}
		char* GetCc(){
			return m_szCc;
		}
		char* GetDate(){
			return m_szDate;
		}


		void SetAttaList(CDsList* pAttaList) {
			m_pAttaList = pAttaList;
		}

		void SetContList(CDsList* pContList) {
			m_pContList = pContList;
		}

		void SetContCheckMode(int nCheckMode) {
			m_nContCheckMode = nCheckMode;
		}

		void SetAttaCheckMode(int nCheckMode) {
			m_nAttaCheckMode = nCheckMode;
		}

		void SetQSearch(CQSearch* pQSearch) { m_pQSearch = pQSearch; }

	public:
		int Audit(char* pMail, int nMailSize);
		BOOL GetFoundKey(char* szFoundKey,int nSize) 
		{
			strncpy(szFoundKey,m_szFoundKey,nSize);
			return TRUE;
		}
	private:
		char m_szFoundKey[128];
		CDsList* m_pAttaList;
		CDsList* m_pContList;
		int m_nContCheckMode;
		int m_nAttaCheckMode;
		char* DoEncode(char* p);
		BOOL DoDecode(char* p,char* pValue,int size);
		BOOL DecodeSubject(char* szEnc,char* pValue,int size);
		CQSearch* m_pQSearch;
	
	private:
		char m_szSubject[1024];
		char m_szFrom[1024];		
		char m_szTo[1024];		
		char m_szDate[1024];
		char m_szCc[1024];
};
#endif
