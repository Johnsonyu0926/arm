#include <stdio.h>
#include "doorsbase.h"
#define HTTP_LOG "../log/httpd.log"
#ifdef LOG_ON_HDC
#define SMTP_SUCCESS_LOG	"/mnt/hdc/configroot/usr/local/pegap/log/smtpsuccess.log"
#define SMTP_FAIL_LOG		"/mnt/hdc/configroot/usr/local/pegap/log/smtpfail.log"

#define POP3_SUCCESS_LOG	"/mnt/hdc/configroot/usr/local/pegap/log/pop3success.log"
#define POP3_FAIL_LOG		"/mnt/hdc/configroot/usr/local/pegap/log/pop3fail.log"
#define POP3_LOGIN_LOG		"/mnt/hdc/configroot/usr/local/pegap/log/pop3login.log"


#define HTTP_SUCCESS_LOG	"/mnt/hdc/configroot/usr/local/pegap/log/httpsuccess.log"
#define HTTP_FAIL_LOG		"/mnt/hdc/configroot/usr/local/pegap/log/httpfail.log"

#else

#define SMTP_SUCCESS_LOG	"../log/smtpsuccess.log"
#define SMTP_FAIL_LOG		"../log/smtpfail.log"

#define POP3_SUCCESS_LOG	"../log/pop3success.log"
#define POP3_FAIL_LOG		"../log/pop3fail.log"
#define POP3_LOGIN_LOG		"../log/pop3login.log"


#define HTTP_SUCCESS_LOG	"../log/httpsuccess.log"
#define HTTP_FAIL_LOG		"../log/httpfail.log"

#endif

class CLog
{
public:
	CLog(char* szSuccessLog)
	{
		DS_TRACE("szSuccessLog:"<<szSuccessLog);
		m_fpSuccess = fopen(szSuccessLog,"a");
	}
	~CLog()
	{
		if(m_fpSuccess)
			fclose(m_fpSuccess);
	}

protected:
	FILE* m_fpSuccess;

	CSTime m_timetool;
	void CheckValid(char* szItem);
};

typedef 
enum _tagHTTPLogType 
    {	DsHTTPSuccess	= 1,	//访问成功
	DsHTTPFailNotAuth	= 2, //失败,原因是: 未认证
	DsHTTPFailPostDeny	= 3, //失败,原因是: POST方法被禁止 
	DsHTTPFailGetDeny	= 4, //失败,原因是: GET方法被禁止 
	DsHTTPFailPostContentDeny	= 5, //失败,原因是: POST内容中有敏感信息 
	DsHTTPFailGetContentDeny	= 6, //失败,原因是: GET内容中有敏感信息 
	DsHTTPFailUrlDeny	= 7, //失败,原因是: 网站被禁止 
	DsHTTPFailUnknown	= 8 //其他未知的失败原因
    }	DsHTTPLogType;
class CHTTPLog:public CLog
{
	public:
		CHTTPLog():CLog(HTTP_LOG)
		{
		}
		~CHTTPLog(){}
	public:
		void Log(
			char* szUser,
			char* szFromIp,
			char* szMethod, //GET or POST 
			char* szHost,	//访问的URL
			char* szPath,   // path.
			int nRespCode, //标准的HTTP应答码
			DsHTTPLogType nLogType) ;	
			
};

#if 0
class CLog
{
public:
	CLog()//char* szSuccessLog,char* szFailLog)
	{
		DS_TRACE("szSuccessLog:"<<szSuccessLog);
		DS_TRACE("szFailLog:"<<szFailLog);
		m_fpSuccess = fopen(szSuccessLog,"a");
		m_fpFail = fopen(szFailLog,"a");
	}
	~CLog()
	{
		if(m_fpSuccess)
			fclose(m_fpSuccess);
		if(m_fpFail)
			fclose(m_fpFail);
	}

protected:
	FILE* m_fpSuccess;
	FILE* m_fpFail;

	CSTime m_timetool;
	void CheckValid(char* szItem);
};

typedef 
enum _tagSMTPLogType 
    {	DsSMTPSuccess	= 1,	//发送成功
	DsSMTPFailNotAuth	= 2, //发送失败,原因是: 未认证
	DsSMTPFailAuditSubject	= 3, //邮件主题中有非法内容
	DsSMTPFailAuditBody	= 4, //邮件内容中有非法关键字
	DsSMTPFailAuditAttachment	= 5, //邮件内容中有非法关键字
	DsSMTPFailUnknown	= 6 //其他未知的发信失败原因
    }	DsSMTPLogType;
class CSMTPLog:public CLog
{
	public:
		 CSMTPLog():CLog(SMTP_SUCCESS_LOG,SMTP_FAIL_LOG)
		{
			//m_fpAuth = fopen(SMTP_AUTH_LOG,"a");
		}
		~CSMTPLog()
		{
		}

	public:

		void Log(
			char* szUser,	//szUser is different to szMailFrom. szUser is the UAC's account, szMailFrom is the sender's mail address.
			char* szFromIp,	//from where.
			char* szMailFrom,
			char* szMailTo,
			char* szCc,
			char* szSubject,
			DsSMTPLogType nCode);

	private:
		//FILE* m_fpAuth;

};

typedef 
enum _tagPOP3LogType 
    {	DsPOP3Success	= 1,	//
	DsPOP3FailNotAuth	= 2, //收信失败:未认证 
	DsPOP3FailUnknown	= 3, //其他未知的收信失败原因
	DsPOP3LoginSuccess		= 4, //POP3登录成功
	DsPOP3LoginFail			= 5 //POP3登录失败
    }	DsPOP3LogType;

class CPOP3Log
{
	private:
		FILE* m_pFp;
		int m_nShouldLog;
		char m_szLog[128];
	public:
		CPOP3Log(int nShouldLog,char* szLog)
		{
			m_pFp = NULL;
			m_nShouldLog = nShouldLog;
			if(!m_nShouldLog)
				return ;
			strncpy(m_szLog,szLog,sizeof(szLog));
			m_pFp = fopen(m_szLog,"a");
		}
		~CPOP3Log(){
			if(m_pFp)
			{
				fclose(m_pFp);
				m_pFp = NULL;
			}
		}

	public:
		void Log(
			char* szUser, //收信者
			char* szFromIp, //receiver's ip.
			char* szMailFrom,	
			char* szMailTo,
			char* szCc,
			char* szSubject,
			DsPOP3LogType nLogType) ;
			
};

typedef 
enum _tagHTTPLogType 
    {	DsHTTPSuccess	= 1,	//访问成功
	DsHTTPFailNotAuth	= 2, //失败,原因是: 未认证
	DsHTTPFailPostDeny	= 3, //失败,原因是: POST方法被禁止 
	DsHTTPFailGetDeny	= 4, //失败,原因是: GET方法被禁止 
	DsHTTPFailPostContentDeny	= 5, //失败,原因是: POST内容中有敏感信息 
	DsHTTPFailGetContentDeny	= 6, //失败,原因是: GET内容中有敏感信息 
	DsHTTPFailUrlDeny	= 7, //失败,原因是: 网站被禁止 
	DsHTTPFailUnknown	= 8 //其他未知的失败原因
    }	DsHTTPLogType;
class CHTTPLog:public CLog
{
	public:
		CHTTPLog():CLog(HTTP_SUCCESS_LOG,HTTP_FAIL_LOG)
		{
		}
		~CHTTPLog(){}
	public:
		void Log(
			char* szUser,
			char* szFromIp,
			char* szMethod, //GET or POST 
			char* szHost,	//访问的URL
			char* szPath,   // path.
			int nRespCode, //标准的HTTP应答码
			DsHTTPLogType nLogType) ;	
			
};
#endif
