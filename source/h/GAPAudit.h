#ifndef	_GAPAUDIT_H__
#define	_GAPAUDIT_H__

#define		RULECOUNT	100

#include 	"GAPMime.h"

class CGAPAudit
{
public:
	CGAPAudit();
	~CGAPAudit();
	BOOL AddContent(BYTE *pBuf, int nLen);
	//BOOL AddContent(const char *pBuf, int nLen);
	BOOL InitRule();
	BOOL Audit(DsAuditStatus *pAudited);
	
	char* GetSubject()
	{
		return m_szSubject;
	}
	char* GetFrom()
	{
		return m_szFrom;
	}
	char* GetTo()
	{
		return m_szTo;
	}
	char* GetCc()
	{
		return m_szCc;
	
	}
	char* GetDate()
	{
		return m_szDate;
	}
public:
	BOOL SetContent(char* szBuf,int nLen);

private:
	char m_szDate[256];
	int m_bReleaseMem;

	int m_nDenyAll;
	char m_szSubject[1024];
	char m_szFrom[1024];
	char m_szTo[1024];
	char m_szCc[1024];

	char *m_szContent;
	long m_nContentLen;	
	BOOL m_bFirstAdd;

	char *m_pRuleContent[RULECOUNT];		//邮件规则的内容
	int m_nRuleContentCount;
	char *m_pRuleAttachMent[RULECOUNT];   //附件规则的内容
	int m_nRuleAttachMentCount;
	char *m_pRuleScript[RULECOUNT];       //脚本规则的内容
	int m_nRuleScriptCount;

	CGAPMime m_Mime;

private:
	BOOL Decode(DsEncodingMethod EncodingMethod,CString strBody, CString& strDecoded);
	BOOL AuditTextPlain(BYTE* pBody);
	BOOL AuditHtml(BYTE* pBody);
	BOOL AuditAttachment(char* szFileName,int nFileType);
	int CheckFileName(CString &strFileName);


	int AuditSubject();
};

#endif
