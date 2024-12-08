//多SDK公共定义
#ifndef _HC_NET_UTILS_H_
#define _HC_NET_UTILS_H_

#include "NPQ/NPQos.h"
/*******************平台相关的数据类型定义 begin**********************/	

#ifndef _WINDOWS_
    #if (defined(_WIN32) || defined(_WIN64))
        #include <winsock2.h>    
        #include <windows.h>
    #endif
#endif

//NETSDK_UTILS_STATIC ：若进行了定义，表示使用HCNetUtils静态库（目前只有HCNetSDK使用HCNetUtils静态库）

#if (defined(_WIN32)) //windows
    #ifndef NETSDK_UTILS_STATIC
        #ifndef NET_UTILS_API
            #define NET_UTILS_API extern "C" __declspec(dllimport)
        #endif //NET_UTILS_API
    #define NET_UTILS_CLASS __declspec(dllimport)
#else
    #ifndef NET_UTILS_API
        #define NET_UTILS_API extern "C"
    #endif //NET_UTILS_API
    #define NET_UTILS_CLASS
#endif


    #define BOOL int
    typedef  unsigned __int64 UINT64;
#elif defined(__linux__) || defined(__APPLE__) //linux
typedef     unsigned int    DWORD;
typedef     unsigned short  WORD;
typedef     unsigned short  USHORT;
typedef     short           SHORT;
typedef     int             LONG;
typedef  	unsigned char	BYTE;
#define     BOOL int
typedef     unsigned int   	UINT;
typedef 	void* 			LPVOID;
typedef 	void* 			HANDLE;
typedef     unsigned int*  LPDWORD; 
typedef  unsigned long long UINT64;

#ifndef    TRUE
#define    TRUE	1
#endif
#ifndef    FALSE
#define	   FALSE 0
#endif
#ifndef    NULL
#define	   NULL 0
#endif

#define __stdcall 
#define CALLBACK  
#ifdef NETSDK_UTILS_STATIC
    #define NET_UTILS_API
#else
    #define NET_UTILS_API extern "C"
#endif
    #define NET_UTILS_CLASS
#endif

/*******************平台相关的数据类型定义 end**********************/	



/*******************全局错误码 begin**********************/
#ifndef _HC_NET_SDK_H_
//这些错误码已经在SDK中定义过了，因此如果遇到SDK的宏定义，此处则不再定义

#define NET_DVR_NOERROR 					0	//没有错误

#define NET_DVR_NOINIT 						3	//没有初始化

#define NET_DVR_VERSIONNOMATCH				6	//版本不匹配
#define NET_DVR_NETWORK_FAIL_CONNECT		7	//连接服务器失败
#define NET_DVR_NETWORK_SEND_ERROR			8	//向服务器发送失败
#define NET_DVR_NETWORK_RECV_ERROR			9	//从服务器接收数据失败
#define NET_DVR_NETWORK_RECV_TIMEOUT		10	//从服务器接收数据超时
#define NET_DVR_NETWORK_ERRORDATA			11	//传送的数据有误
#define NET_DVR_ORDER_ERROR					12	//调用次序错误

#define NET_DVR_PARAMETER_ERROR 			17  //参数错误

#define NET_DVR_FILEOPENFAIL                35  //打开文件出错

#define NET_DVR_DIR_ERROR					40	//路径错误
#define NET_DVR_ALLOC_RESOURCE_ERROR		41  //资源分配错误

#define NET_DVR_NOENOUGH_BUF				43	//缓冲区太小
#define NET_DVR_CREATESOCKET_ERROR		 	44	//创建SOCKET出错
#define NET_DVR_SETSOCKET_ERROR				45	//设置SOCKET出错
#define NET_DVR_MAX_NUM                     46  //个数达到最大

#define NET_DVR_GETLOCALIPANDMACFAIL		53  //获得本地的IP地址或物理地址失败

#define NET_DVR_CREATEDIR_ERROR				71	//建立日志文件目录失败
#define NET_DVR_BINDSOCKET_ERROR			72	//绑定套接字失败
#define NET_DVR_SOCKETCLOSE_ERROR			73	//socket连接中断，此错误通常是由于连接中断或目的地不可达
#define NET_DVR_USERID_ISUSING			    74	//注销时用户ID正在进行某操作
#define NET_DVR_SOCKETLISTEN_ERROR			75	//监听失败

#define NET_DVR_FUNCTION_NOT_SUPPORT_OS     98  //此功能不支持该操作系统
#define NET_DVR_USE_LOG_SWITCH_FILE			103  //正在使用日志开关文件


#define NET_DVR_LOAD_SSL_LIB_ERROR          148  //加载SSL库失败（可能是版本不匹配，也可能是不存在）

#define NET_ERR_NPQ_BASE_INDEX    8000    //NPQ库错误码
#define NET_ERR_NPQ_PARAM       (NET_ERR_NPQ_BASE_INDEX + 1)        //NPQ库参数有误
#define NET_ERR_NPQ_SYSTEM      (NET_ERR_NPQ_BASE_INDEX + 2)        //NPQ库操作系统调用错误(包括资源申请失败或内部错误等）
#define NET_ERR_NPQ_GENRAL      (NET_ERR_NPQ_BASE_INDEX + 3)        //NPQ库内部通用错误
#define NET_ERR_NPQ_PRECONDITION    (NET_ERR_NPQ_BASE_INDEX + 4)        //NPQ库调用顺序错误
#define NET_ERR_NPQ_NOTSUPPORT        (NET_ERR_NPQ_BASE_INDEX + 5)        //NPQ库功能不支持

#define NET_ERR_NPQ_NOTCALLBACK    (NET_ERR_NPQ_BASE_INDEX + 100)   //数据没有回调上来
#define NET_ERR_NPQ_LOADLIB (NET_ERR_NPQ_BASE_INDEX + 101)   //NPQ库加载失败


#define NET_ERR_SIP_BASE_INDEX  8200    //SIP错误码
#define NET_ERR_SIP_CREATE_OUTGOINGHADLE_FAIL    (NET_ERR_SIP_BASE_INDEX + 1)    //创建呼出句柄失败
#define NET_ERR_SIP_NOT_RINGING (NET_ERR_SIP_BASE_INDEX + 2)    //未振铃或振铃超时
#define NET_ERR_SIP_MAX_CONNECTION_NUM  (NET_ERR_SIP_BASE_INDEX + 3)    //单个终端达到最大呼叫连接数
#define NET_ERR_SIP_LOADLIB (NET_ERR_SIP_BASE_INDEX + 4)    //加载sip库失败

#endif // !_HC_NET_SDK_H_
/*******************全局错误码 end**********************/

//状态值
#define HTTP_CERTIFICATE_ERROR          0x3    //证书错误

#define MACADDR_LEN                  6       //mac地址长度
#define DEV_TYPE_NAME_LEN            24      //设备类型名称长度
#define SERIALNO_LEN                 48      //序列号长度
#define  SDK_MAX_IP_LEN              48
#define DEVICE_IDENTY_CODE           128

#define UPGRADE_SUCC    1   //升级成功
#define UPGRADE_ING     2   //正在升级
#define UPGRADE_TIMEOUT 3   //升级失败，网络超时
#define UPGRADE_FAILED  4   //升级失败，原因未知

#define MAX_DOMAIN_NAME                64        /* 最大域名长度 */

typedef	struct	tagDEVICE_SEEKHELP_INFO
{
    BYTE	 byDeviceMAC[MACADDR_LEN];	//mac地址
    char	 szDevDesc[DEV_TYPE_NAME_LEN];       //设备类型
    char	 szSerialNO[SERIALNO_LEN];	//设备序列号
    char	 szDeviceAddress[SDK_MAX_IP_LEN];	//设备地址
    WORD	 wIDCodeLen;	//识别码长度，上层应用开发的时候做一个判断，如果本字段大于128，请报错，提示用户更新工具/客户端版本
    char	 szIDCode[DEVICE_IDENTY_CODE];
	BYTE     bySupportMultiCast;	//0-不支持多播,1-支持多播
    BYTE	 byRes[123];
}DEVICE_SEEKHELP_INFO,*LPDEVICE_SEEKHELP_INFO;

typedef BOOL (CALLBACK * UBOOT_DEVICEONLINE_CB) (LONG iHandle, LPDEVICE_SEEKHELP_INFO pSeekHelpInfo, void *pUser);

typedef struct tagNET_UTILS_UBOOT_START
{
    UBOOT_DEVICEONLINE_CB   fnCB;
    char            szLocalAddr[SDK_MAX_IP_LEN];
    void *          pUserData;   //用户数据
    BYTE            byRes[32]; 
}NET_UTILS_UBOOT_START,*LPNET_UTILS_UBOOT_START;

typedef struct tagDEVICE_TRYFIX_INFO 
{
    BYTE	byDeviceMAC[MACADDR_LEN];	            //mac地址
    char    szDeviceAddr[SDK_MAX_IP_LEN];  //设备地址
    char    szTFTPAddr[SDK_MAX_IP_LEN];    //TFTP地址
    WORD    wTFTPPort;                     //TFTP端口
	BYTE	bySupportMultiCast;	//0-不支持多播，1-支持多播
    BYTE    byRes[127];
}DEVICE_TRYFIX_INFO,*LPDEVICE_TRYFIX_INFO;

typedef struct tagNET_DVR_CHECK_PAKAGE_COND
{
    DWORD  	dwSize;
    BYTE	byIDCode[128];
    WORD    wPort;
    BYTE    bySvrAddr[MAX_DOMAIN_NAME];
    BYTE	byRes[126];
}NET_DVR_CHECK_PAKAGE_COND,*LPNET_DVR_CHECK_PAKAGE_COND;

typedef struct tagNET_DVR_CHECK_PAKAGE_INFO
{
    DWORD	dwSize;
    BYTE	byIDCode[128];
    BYTE	byVersion[32];
    BYTE	byPakageUrl[512];
    BYTE	byRes[348];
}NET_DVR_CHECK_PAKAGE_INFO,*LPNET_DVR_CHECK_PAKAGE_INFO;

typedef void (CALLBACK *WriteLogStrFunc)(LONG nLevel, char const *pFile, LONG iLine, char const *pBuffer);

typedef struct tagNET_DVR_LOG_INFO
{
    BOOL      bExternalInterface;//是否使用外部接口
    LONG      nLogLevel;
    char      *strLogDir;
    BOOL      bAutoDel;
    WriteLogStrFunc fnWriteLogStr;
}NET_DVR_LOG_INFO, *LPNET_DVR_LOG_INFO;

NET_UTILS_API BOOL    CALLBACK   NET_UTILS_Init();
NET_UTILS_API BOOL    CALLBACK   NET_UTILS_Fini();
NET_UTILS_API LONG    CALLBACK   NET_UTILS_GetLastError();
NET_UTILS_API BOOL    CALLBACK   NET_UTILS_SetLogToFile(LONG nLogLevel, char *strLogDir, BOOL bAutoDel);
NET_UTILS_API BOOL    CALLBACK   NET_UTILS_SetLogToFileEx(LPNET_DVR_LOG_INFO pParam);
NET_UTILS_API DWORD   CALLBACK   NET_UTILS_GetSDKBuildVersion();

#define UTILS_DLL_SSL_PATH  9

NET_UTILS_API BOOL    CALLBACK   NET_UTILS_SetDllLoadPath(DWORD dwDllType, const char *sDllName);

NET_UTILS_API LONG    CALLBACK   NET_UTILS_StartTFTPServer(char *pSvrAddr, WORD wSvrPort, char *pFilePath);
NET_UTILS_API BOOL    CALLBACK   NET_UTILS_StopTFTPServer(LONG   iHandle);
NET_UTILS_API BOOL    CALLBACK   NET_UTILS_GetSendFileProgress(LONG   iHandle, LONG *iProgress, LONG *iStatus);

NET_UTILS_API LONG    CALLBACK    NET_UTILS_StartUBootServer(NET_UTILS_UBOOT_START* pUBootStart);
NET_UTILS_API BOOL    CALLBACK    NET_UTILS_StopUBootServer(LONG iHandle);
NET_UTILS_API BOOL    CALLBACK    NET_UTILS_ReplyUBoot(LONG iHandle, DEVICE_TRYFIX_INFO *pFixInfo);

NET_UTILS_API BOOL    CALLBACK    NET_UTILS_GetPakageInfo(LPNET_DVR_CHECK_PAKAGE_COND lpCond, LPNET_DVR_CHECK_PAKAGE_INFO lpInfo);



//SMTP
#define HC_SMTP_MAX_ADDRESS_LEN     256
#define HC_SMTP_MAX_SUBJECT_LEN     256
#define HC_SMTP_MAX_ACCOUNT_LEN     256
#define HC_SMTP_MAX_PASSWORD_LEN    128
#define HC_SMTP_MAX_NAME_LEN        128

enum SMTP_SECURITY_TYPE
{
    DO_NOT_SET,     //不加密
    USE_TLS_OPT,    //如果服务器支持TLS，使用STARTLS加密传输
    USE_SSL         //使用SSL加密
};

enum SMTP_CHARSET_TYPE
{
    US_ASCII,   //西文ASCII码
    UTF_8,      //UTF-8
    GB_2312     //Windows gb2312中文编码
};

typedef	struct tagNET_UTILS_SMTP_PARAM
{
    SMTP_SECURITY_TYPE  enumSecurity;   //加密类型
    SMTP_CHARSET_TYPE   enumCharset;    //字符编码格式
    char			    szSmtpServer[HC_SMTP_MAX_ADDRESS_LEN];  //SMTP邮件服务器地址，可以是域名或ip地址
    WORD		        wPort;  //SMTP邮件服务器端口
    char			    szSender[HC_SMTP_MAX_ADDRESS_LEN];  //发送者邮箱地址
    char			    szSubject[HC_SMTP_MAX_SUBJECT_LEN]; //邮件主题
    char			    szAccount[HC_SMTP_MAX_ACCOUNT_LEN]; //发送者邮箱账号
    char			    szPassword[HC_SMTP_MAX_PASSWORD_LEN];   //发送者邮箱密码
    BOOL		        bReadRecipient;     //邮件读回执
    BOOL                bAuthenticate;      //邮箱账号认证选项 0-不认证 1-认证
    char                szSenderName[HC_SMTP_MAX_NAME_LEN]; //发送者姓名
    DWORD               dwConnectTimeOut;   //连接超时时间，单位毫秒，如果设置为0，默认10秒
    DWORD               dwSendTimeOut;      //发送超时时间，单位毫秒，如果设置为0，默认10秒
    DWORD               dwRecvTimeOut;      //接收超时时间，单位毫秒，如果设置为0，默认10秒
    BYTE                byRes[116];
}NET_UTILS_SMTP_PARAM, *LPNET_UTILS_SMTP_PARAM;

enum SMTP_RECIPIENT_TYPE
{
    COMMON_RECIPIENT,   //普通收件人
    CC_RECIPIENT,       //抄送收件人
    BCC_RECIPIENT       //暗送收件人
};

enum SMTP_ATTACHMENT_TYPE
{
    PATH_TYPE,      //附件以路径的方式
    BUFFER_TYPE     //附件以数据内容的方式
};

enum SMTP_MAIL_BODY_TYPE
{
    PLAIN_TEXT,   //纯文本
    HTML_FORMAT   //HTML格式
};

typedef struct tagSMTP_MAIL_BODY
{
    DWORD  dwBodyType; //邮件正文数据类型  0-文本类型(宏定义PLAIN_TEXT)
    BYTE   *pData;     //数据指针
    DWORD  dwDataLen;  //数据长度
    BYTE   byRes[64];
}SMTP_MAIL_BODY, *LPSMTP_MAIL_BODY;

//以下接口废弃，替换成再紧跟着的几个接口
//NET_UTILS_API LONG CALLBACK NET_UTILS_SMTPCLIENT_Create(const NET_UTILS_SMTP_PARAM *pSmtpParam);
//NET_UTILS_API BOOL CALLBACK NET_UTILS_SMTPCLIENT_SendMail(LONG lHandle);
//NET_UTILS_API BOOL  CALLBACK NET_UTILS_SMTPCLIENT_Destroy(LONG lHandle);
//NET_UTILS_API BOOL  CALLBACK NET_UTILS_SMTPCLIENT_AddAttachment(LONG lHandle, SMTP_ATTACHMENT_TYPE enumType, const char* pFileName, const char* pData, DWORD dwDataLen);
//NET_UTILS_API BOOL  CALLBACK NET_UTILS_SMTPCLIENT_AddRecipient(LONG lHandle, SMTP_RECIPIENT_TYPE enumType, const char* pEmailAddress, const char* pName);
//NET_UTILS_API BOOL  CALLBACK NET_UTILS_SMTPCLIENT_SetMailText(LONG lHandle, const char* pText);

NET_UTILS_API LONG  CALLBACK NET_SMTPCLIENT_Create(const NET_UTILS_SMTP_PARAM *pSmtpParam);
NET_UTILS_API BOOL  CALLBACK NET_SMTPCLIENT_SendMail(LONG lHandle, BOOL (CALLBACK *MAIL_SEND_CB) (LONG lHandle, DWORD dwProgress, void* pUser), void* pUser);
NET_UTILS_API BOOL  CALLBACK NET_SMTPCLIENT_Destroy(LONG lHandle);
NET_UTILS_API BOOL  CALLBACK NET_SMTPCLIENT_AddAttachment(LONG lHandle, SMTP_ATTACHMENT_TYPE enumType, const char* pFileName, const char* pData, DWORD dwDataLen);
NET_UTILS_API BOOL  CALLBACK NET_SMTPCLIENT_AddRecipient(LONG lHandle, SMTP_RECIPIENT_TYPE enumType, const char* pEmailAddress, const char* pName);
NET_UTILS_API BOOL  CALLBACK NET_SMTPCLIENT_SetMailBody(LONG lHandle, const SMTP_MAIL_BODY *pBody);



//SMTP标准错误码
#define SMTP_STD_SERVICE_UNAVAILABLE            421 //服务未就绪, 关闭传输信道
#define SMTP_STD_MAILBOX_IS_BUSY                450 //要求的邮件操作未完成，邮箱不可用（例如，邮箱忙）
#define SMTP_STD_LOCAL_ERROR_IN_PROCESSING      451 //放弃要求的操作；处理过程中出错
#define SMTP_STD_INSUFFICIENT_SYS_STORAGE       452 //系统存储不足，要求的操作未执行（例如接收者过多）
#define SMTP_STD_SYNTAX_ERR                     500 //格式错误，命令不可识别（此错误也包括命令行过长）
#define SMTP_STD_PARAM_ERR                      501 //参数格式错误（比如回复或转发路径过多）
#define SMTP_STD_CMD_NOT_IMPLEMENTED            502 //命令不可实现
#define SMTP_STD_BAD_CMD_SEQ                    503 //错误的命令序列
#define SMTP_STD_CMD_PARAM_NOT_IMPLEMENTED      504 //命令参数不可实现
#define SMTP_STD_AUTH_REQUIRED                  530 //需要身份认证的服务器，未做身份认证
#define SMTP_STD_AUTH_FAILED                    535 //用户验证失败
#define SMTP_STD_MAILBOX_UNAVAILABLE            550 //要求的邮件操作未完成，邮箱不可用（例如，邮箱未找到，或不可访问）
#define SMTP_STD_USER_NOT_LOCAL                 551 //用户非本地，请尝试<forward - path>
#define SMTP_STD_EXCEEDED_STROAGE_ALLOCATION    552 //过量的存储分配，要求的操作未执行（例如，邮件内容过多）
#define SMTP_STD_MAILBOX_NAME_NOT_ALLOWED       553 //邮箱名不可用，要求的操作未执行（例如邮箱格式错误）
#define SMTP_STD_TRANSACTION_FAILED             554 //与服务器连接失败



//SMTP自定义错误码
#define SMTP_ERROR_SERVER_DOMAIN_NOT_EIXST      5001    //服务器域名不存在
#define SMTP_ERROR_ATTACHMENT_IS_TOO_LARGE      5002    //附件超过大小
#define SMTP_ERROR_MAIL_RECIPIENT_IS_NONE       5003    //邮件接收人为空
#define SMTP_ERROR_AUTH_TYPE_NOT_SUPPORT        5004    //认证类型不支持

/********************************HTTP begin****************************************/

typedef	struct tagNET_UTILS_HTTP_SHORT_EXCHANGE
{
    void*      lpReqUrl;		//[in]url，字符串格式，示例如下：
    //http://10.17.133.26:80/ISAPI/PTZCtrl/channels/1/patrols/capabilities
    //https://10.17.130.245:443/ISAPI/PTZCtrl/channels/1/patrols/capabilities
    DWORD      dwReqUrlLen;	//[in]url长度，字符串长度
    void*      lpOutBuf;	//[out]输出数据内存,外部申请；内容不包含HTTP响应头
    DWORD      dwOutBufLen;//[in]输出数据内存长度
    DWORD      dwRetLen;	//[out]输出数据实际长度
    BYTE       byMothod;	//[in]0-GET,1-PUT,2-POST,3-DELETE
    void*      lpRspHead;  //[out]http响应头，外部申请,不需要时置为null
    DWORD      dwRspHeadLen;//[in]http响应头内存大小，不需要时0
    BYTE       byRes[16];
}NET_UTILS_HTTP_SHORT_EXCHANGE,*LPNET_UTILS_HTTP_SHORT_EXCHANGE;

typedef void(CALLBACK *HTTPClientCB)(char *pRecvData, DWORD dwDataLen, DWORD dwStats, void* pUserData);

typedef	struct tagNET_UTILS_HTTP_SHORT_EXCHANGE_EX
{
    void*       lpReqUrl;	   //[in]url，字符串格式，示例如下：
    //http://10.17.133.26:80/ISAPI/PTZCtrl/channels/1/patrols/capabilities
    //https://10.17.130.245:443/ISAPI/PTZCtrl/channels/1/patrols/capabilities
    DWORD        dwReqUrlLen; //[in]url长度，字符串长度
    const void*  lpOutBuf;	   //[out]只读，输出数据内存,外部仅能在接口返回时做拷贝操作；内容不包含HTTP响应头
    DWORD        dwOutBufLen; //[out]只读，输出数据内存长度
    DWORD        dwHttpState; //[out]只读，输出http状态值
    BYTE         byMothod;	  //[in]0-GET,1-PUT,2-POST,3-DELETE
    BYTE         byRes1[3];
    HTTPClientCB fnCB;
    void*        pUserData;
    BYTE         byRes[4];
}NET_UTILS_HTTP_SHORT_EXCHANGE_EX, *LPNET_UTILS_HTTP_SHORT_EXCHANGE_EX;

typedef enum tagHTTP_CMD_TYPE
{
    ENUM_HTTP_VER = 0,           //仅value生效，类型int *,可不设置，默认都支持,设置0表示仅支持HTTP1.1,设置1表示都支持
    ENUM_HTTP_USER = 1,
    ENUM_HTTP_BODY = 2,
    ENUM_HTTP_PARAM = 3,
    ENUM_HTTP_CALIB_CERT = 4, //保留
    ENUM_HTTP_NOCALIB_CERT = 5,//保留
    ENUM_HTTP_AUTHEN_MODE = 6,
    ENUM_HTTP_SAFE_FIRST = 7,
    ENUM_HTTP_EFFIC_FIRST = 8,
    ENUM_HTTP_CONNECT_TIMEOUT = 9,
    ENUM_HTTP_SEND_TIMEOUT = 10,
    ENUM_HTTP_RECV_TIMEOUT = 11,
    ENUM_HTTP_ALPN_PROTO = 12,
    ENUM_HTTP_CERTIFICATE = 13,
    ENUM_HTTP_MIME_BODY = 14,
    ENUM_HTTP_RECV_BODY_SIZE = 15, //设置HTTP接收缓冲区大小（默认是64M，最大可设置为512M）
    ENUM_HTTP_LOCAL_IP = 16, //本次连接使用的本地ip地址
    ENUM_HTTP_REQUEST_PARAM = 17 //获取请求参数 GET使用
}HTTP_CMD_TYPE;

typedef struct tagNET_UTILS_SSL_CERTIFICATE
{
    BYTE byVerifyMode;    //0-单向认证, 1-双向认证

    BYTE byCertificateFileType; //证书类型0-pem, 1-ANS
    BYTE byPrivateKeyFileType;  //私钥类型0-pem, 1-ANS

    char *pUserCertificateFile; //证书
    char *pUserPrivateKeyFile;  //私钥
    char *pCACertificateFile;   //CA证书

    BYTE byRes[32];
}NET_UTILS_SSL_CERTIFICATE, *LPNET_UTILS_SSL_CERTIFICATE;

NET_UTILS_API LONG    CALLBACK    NET_HTTPCLIENT_Create();
NET_UTILS_API BOOL    CALLBACK    NET_HTTPCLIENT_Destroy(LONG lHandle);
NET_UTILS_API BOOL    CALLBACK    NET_HTTPCLIENT_SetParam(LONG lHandle, DWORD dwCmdType, void* lpName, DWORD dwNameLen, void* lpValue, DWORD dwValueLen);
NET_UTILS_API BOOL    CALLBACK    NET_HTTPCLIENT_GetParam(LONG lHandle, DWORD dwCmdType, void* lpName, DWORD dwNameLen, void* lpValue, DWORD dwValueLen);
NET_UTILS_API BOOL    CALLBACK    NET_HTTPCLIENT_ShortLinkExchange(LONG lHandle, LPNET_UTILS_HTTP_SHORT_EXCHANGE_EX lpExchange);

////以下接口仅做保留用，实际上是已经废弃，后续提供接口给其他人，不再提供
//NET_UTILS_API LONG    CALLBACK    NET_UTILS_CreateHttpClient();
//NET_UTILS_API BOOL    CALLBACK    NET_UTILS_DestroyHttpClient(LONG lHandle);
//NET_UTILS_API BOOL    CALLBACK    NET_UTILS_SetHttpParam(LONG lHandle, DWORD dwCmdType, void* lpName, DWORD dwNameLen, void* lpValue, DWORD dwValueLen);
//NET_UTILS_API BOOL    CALLBACK    NET_UTILS_GetHttpParam(LONG lHandle, DWORD dwCmdType, void* lpName, DWORD dwNameLen, void* lpValue, DWORD dwValueLen);
NET_UTILS_API LONG    CALLBACK    NET_UTILS_ShortLinkExchange(LONG lHandle, LPNET_UTILS_HTTP_SHORT_EXCHANGE lpExchange);

class NET_UTILS_CLASS CHCHttpClient
{
public:
    CHCHttpClient();
    ~CHCHttpClient();
    int   ShortLinkExchange(LPNET_UTILS_HTTP_SHORT_EXCHANGE lpExchange);
    BOOL   SetHttpParam(unsigned int dwCmdType, void* lpName, unsigned int dwNameLen, void* lpValue, unsigned int dwValueLen);
private:
    LONG   m_lHandle;
};


typedef struct tagNET_UTILS_HTTP_SERVER_DATA_CB
{
    DWORD   dwSize;      //本结构体长度
    DWORD   dwStatus;    //状态值,凡大于等于0xff00，均需调用NET_HTTPSERVER_Close关闭接口
                         //       0x0-数据已完整，
                         //       0x1-报文长度过长，分次回调，还有后续数据, 
                         //       0x2-报文类型mime，需要调用NET_UTILS_GetHttpServerRspParam获取mime报文
                         //       0xff00-网络异常 
                         //       0xff01-资源不足
                         //       0xff02-对端关闭
                         //       0xffff-正常关闭
    DWORD   dwDataLen;   //数据长度
    char*   pRecvData;   //接收到的数据
    DWORD   dwStreamID;  //如果是H1协议，该字段始终为0，否则不为0，不过用户不用管具体的数值，获取参数的时候，直接给回来即可
}NET_UTILS_HTTP_SERVER_DATA_CB, *LPNET_UTILS_HTTP_SERVER_DATA_CB;

typedef void (CALLBACK *HTTPSERVERDATACB)(LONG iCallbackHandle, void* pData, void* pUserData);

typedef struct tatNET_UTILS_HTTP_LISTEN_PARAM
{
    DWORD  dwType;          //监听端口类型 0：兼容已知的HTTP协议，含1.1与2.0; 1:仅支持HTTP1.1
    BYTE   szIP[128];       //IP地址
    WORD   wPort;           //端口
    WORD   byRes[2];
    DWORD  dwWaitForAccept; //处于等待accept的链接个数,超过该个数，链接请求将直接被拒绝，默认0x7FFFFFFF，该参数暂时不生效，需修改core的代码
    DWORD  dwRecvBuffSize;  //默认64M(最大支持64M)，接收缓存区最大多大，超过该数值，将分多次回调数据，不再一次性回调完成，避免大数据的传输导致内存过大
    DWORD  dwRecvTimeOut;   //接收超时，单位ms，默认5000,表示该链接多久收不到数据，则认为网络异常
    DWORD  dwSendTimeOut;   //发送超时，单位ms，默认5000
    HTTPSERVERDATACB fnCB;  //接收到数据时的回调
    void*  pUserData;       //用户数据
}NET_UTILS_HTTP_LISTEN_PARAM, *LPNET_UTILS_HTTP_LISTEN_PARAM;


typedef struct tatNET_UTILS_HTTP_LISTEN_PARAM_EX
{
    DWORD  dwType;          //监听端口类型 0：兼容已知的HTTP协议，含1.1与2.0; 1:仅支持HTTP1.1
    BYTE   szIP[128];       //IP地址
    WORD   wPort;           //端口
    WORD   byRes[2];
    DWORD  dwWaitForAccept; //处于等待accept的链接个数,超过该个数，链接请求将直接被拒绝，默认0x7FFFFFFF，该参数暂时不生效，需修改core的代码
    DWORD  dwRecvBuffSize;  //默认64M(最大支持64M)，接收缓存区最大多大，超过该数值，将分多次回调数据，不再一次性回调完成，避免大数据的传输导致内存过大
    DWORD  dwRecvTimeOut;   //接收超时，单位ms，默认5000,表示该链接多久收不到数据，则认为网络异常
    DWORD  dwSendTimeOut;   //发送超时，单位ms，默认5000
    HTTPSERVERDATACB fnCB;  //接收到数据时的回调
    void*  pUserData;       //用户数据

    BYTE byHttps;           //0-不使用HTTPS, 1-使用HTTPS
    BYTE byVerifyMode;      //0-单向认证, 1-双向认证
    BYTE byCertificateFileType; //证书类型0-pem, 1-ANS1
    BYTE byPrivateKeyFileType; //私钥类型0-pem, 1-ANS1

    BYTE *pAlpnProto;
    DWORD dwAlpnProtoLen;

    char *pUserCertificateFile; //公钥证书
    char *pUserPrivateKeyFile;  //私钥
    char *pCACertificateFile;   //CA证书

    DWORD dwSSLVersion; //SSL Method版本
    //0 - SSL23, 1 - SSL2, 2 - SSL3, 3 - TLS1.0, 4 - TLS1.1, 5 - TLS1.2
    //SSL23是兼容模式，会协商客户端和服务端使用的最高版本

    BYTE byRes3[60];

}NET_UTILS_HTTP_LISTEN_PARAM_EX, *LPNET_UTILS_HTTP_LISTEN_PARAM_EX;


//本联合体分两种情况，标准的"Param1:Param2",这种情况不单独处理，直接使用ENUM_HTTP_SERVER_ANY传参数
//非标准的各种情况，具体见注释
typedef enum enumNET_UTILS_HTTP_SERVER_REPLY_PARA
{
    ENUM_HTTP_SERVER_VER = 0,         //仅value生效，类型NET_UTILS_HTTP_VER *,可不设置，默认http/1.1
    ENUM_HTTP_SERVER_STATS = 1,       //仅value生效，可不设置，默认为200 ok
    ENUM_HTTP_SERVER_CONTENTDISPOSITION = 2,    //参数均生效，类型均为char *，可不设置，组成格式"Content-Disposition: %s; filename=%s", lpname, lpvalue,如"Content-Disposition: attachment;filename="test.xls""
    ENUM_HTTP_SERVER_BOUNDARY = 3,    //多表单同时提交的方式暂不支持，客户端与服务端实现起来均比较麻烦
    ENUM_HTTP_SERVER_CONTENT = 4,     //仅value生效，类型char*，若不设置，则响应时仅响应报文头，无报文体
    ENUM_HTTP_SERVER_SENDTIMEOUT = 5, //仅value生效，类型为DWORD *，若不设置，则默认5s
    ENUM_HTTP_SERVER_WITHOUTHEAD = 6, //设置无消息头
    ENUM_HTTP_SERVER_CONTENTWITHOUTHEAD = 7,//仅设置数据内容无数据头

    ENUM_HTTP_SERVER_ANY = 0xff,      //参数均生效，类型均为char *，组成方式 "%s: %s", lpname, lpvalue
                                      //如果参数名称出现重复，则后来的覆盖先有的
}NET_UTILS_HTTP_SERVER_REPLY_PARA;


//本联合体分两种情况，标准协议里一般都会带的,这种单独一个结构体获取
//标准的"Param1:Param2",但是一般不是必带的，直接使用ENUM_HTTP_SERVER_ANY传参数
typedef enum enumNET_UTILS_HTTP_SERVER_REQ_PARA
{
    ENUM_HTTP_SERVER_GENERAL_REQ  = 0,        //仅value生效，类型NET_UTILS_HTTP_SERVER_REQ_PARAM *
    ENUM_HTTP_SERVER_MIME_COUNT   = 1,        //仅value生效，类型DWORD*
    ENUM_HTTP_SERVER_MIME_CONTENT = 2,
    ENUM_HTTP_SERVER_REMOTE_ADDR = 3,        //获取远端IP，仅value生效 *, 类型为char *，长度不能少于48个字符，以兼容IPv6
    ENUM_HTTP_SERVER_MIME_CONTENT_EX = 4,
    ENUM_HTTP_SERVER_ANY_REQ      = 0xff,    //参数均生效，类型均为char *, lpName为参数名称，lpValue为参数数值
}NET_UTILS_HTTP_SERVER_REQ_CMD;


#define MAX_UTILS_REALM_LEN 32                       //最大realm长度
//由于设备安全升级需要，将长度扩大为192
#define MAX_UTILS_NONCE_LEN 192                      //最大nonce长度

#define MAX_UTILS_DEGIST_SRC_LEN	1024*4          //最大摘要信息长度
#define MAX_HA_LEN 33						       //加密串最大长度
#define MAX_UTILS_HTTP_URL   2048

#define  MAX_UTILS_NAME_LEN  480

typedef struct tagNET_UTILS_HTTP_DIGET_PARA
{
    char szUseName[MAX_UTILS_NAME_LEN + 1];
    char szRealm[MAX_UTILS_REALM_LEN];
    char szNonce[MAX_UTILS_NONCE_LEN];
    char szUri[MAX_UTILS_HTTP_URL];
    char szCNonce[MAX_UTILS_NONCE_LEN];
    char szNC[12];
    char szQop[12];  //只有两种选择，auth/auth-int,加上结束符9个字节够了
    char szRespone[MAX_HA_LEN];
}NET_UTILS_HTTP_DIGET_PARA;

typedef struct tagNET_UTILS_HTTP_SERVER_REQ_PARAM
{
    char  szUrl[MAX_UTILS_HTTP_URL];
    BYTE  byMethod;  //0 - GET, 1 - PUT, 2 - POST, 3 - DELETE
    BYTE  byRes[3];
    NET_UTILS_HTTP_DIGET_PARA struDiget;
}NET_UTILS_HTTP_SERVER_REQ_PARAM, *LPNET_UTILS_HTTP_SERVER_REQ_PARAM;
 
typedef struct tagNET_UTILS_HTTP_MIME
{
    char szContentType[32];
    char szDisposition[32];
    char szName[256];                   //windows下，路径最多这么长
    char szFilename[256];
    char szEncoding[32];                //7bit，8bit，binary ，quoted-printable，base64和custom
    char szDescription[256];            //描述性语句
    DWORD dwContentLen;                 //Content的长度
    const char* pContent;               //const型，用户需要根据dwContentLen拷贝走，不能进行除拷贝以外的操作
}NET_UTILS_HTTP_MIME, *LPNET_UTILS_HTTP_MIME;

typedef struct tagNET_UTILS_HTTP_MIME_EX
{
    char szContentType[32];
    char szDisposition[32];
    char szName[256];     //windows下，路径最多这么长
    char szFilename[256];
    char szEncoding[32];                //7bit，8bit，binary ，quoted-printable，base64和custom
    char szDescription[256];            //描述性语句
    DWORD dwContentLen;            //Content的长度
    const char* pContent;               //const型，用户需要根据dwContentLen拷贝走，不能进行除拷贝以外的操作
    DWORD dwMimeHeadLen;
    const char* pMimeHead;
    BYTE byRes[128];
} NET_UTILS_HTTP_MIME_EX, *LPNET_UTILS_HTTP_MIME_EX;

typedef struct tagNET_UTILS_MIME_UNIT
{
    //HTTP格式如下
    //Content-Disposition: form-data; name="upload"; filename="C:\Users\test\Desktop\11.txt"
    //Content-Type: text/plain
    char szContentType[32];               //对应Content-Type
    char szName[256];       //对应name字段
    char szFilename[256];   //对应filename字段
    DWORD dwContentLen;              //Content的长度
    char* pContent;                       //数据指针
    BYTE bySelfRead;                 // 0-外界传入文件 1-内部读取数据（通过szFilename传递完整路径）
    BYTE byRes[15];
}NET_UTILS_MIME_UNIT, *LPNET_UTILS_MIME_UNIT;



NET_UTILS_API LONG    CALLBACK    NET_HTTPSERVER_CreateListen(LPNET_UTILS_HTTP_LISTEN_PARAM lpStruParam);
NET_UTILS_API LONG    CALLBACK    NET_HTTPSERVER_CreateListenEx(LPNET_UTILS_HTTP_LISTEN_PARAM_EX lpStruParamEx);

NET_UTILS_API BOOL    CALLBACK    NET_HTTPSERVER_DestroyListen(LONG iListenHandle);

NET_UTILS_API BOOL    CALLBACK    NET_HTTPSERVER_SetParam(LONG iCallbackHandle, DWORD dwCmdType, void* lpName, DWORD dwNameLen, void* lpValue, DWORD dwValueLen);
NET_UTILS_API BOOL    CALLBACK    NET_HTTPSERVER_GetParam(LONG iCallbackHandle, DWORD dwCmdType, void* lpName, DWORD dwNameLen, void* lpValue, DWORD dwValueLen);
NET_UTILS_API LONG    CALLBACK    NET_HTTPSERVER_Reply(LONG iCallbackHandle, BOOL bAutoClose);
NET_UTILS_API BOOL    CALLBACK    NET_HTTPSERVER_Close(LONG iCallbackHandle, BOOL bForceClose);

//V2指H2接口，对H1兼容，因此建议不管H1或者H2均使用本接口
NET_UTILS_API BOOL    CALLBACK    NET_HTTPSERVER_GetParamV2(LONG iCallbackHandle, DWORD dwStreamID, DWORD dwCmdType, void* lpName, DWORD dwNameLen, void* lpValue, DWORD dwValueLen);
NET_UTILS_API BOOL    CALLBACK    NET_HTTPSERVER_SetParamV2(LONG iCallbackHandle, DWORD dwStreamID, DWORD dwCmdType, void* lpName, DWORD dwNameLen, void* lpValue, DWORD dwValueLen);
NET_UTILS_API LONG    CALLBACK    NET_HTTPSERVER_ReplyV2(LONG iCallbackHandle, DWORD dwStreamID, BOOL bAutoClose);

/********************************HTTP end****************************************/

/********************************FTP start***************************************/

#define FTP_UPLOAD_GETSTATUS     1
#define FTP_UPLOAD_PAUSE         2
#define FTP_UPLOAD_RESUME        3
#define FTP_UPLOAD_GETSIZE       4
#define FTP_UPLOAD_MKDIR        5
#define FTP_UPLOAD_CHANGEDIR    6
#define FTP_UPLOAD_UTF8_ON       7
#define FTP_UPLOAD_UTF8_OFF      8


#define FTP_STATUS_SUCC         1
#define FTP_STATUS_UPLOAD       2
#define FTP_STATUS_NETERROR     3
#define FTP_STATUS_NOENOUGHBUF   4
#define FTP_STATUS_FAIL         5

typedef enum tagFTPCharEncodeType
{
    FTP_ENUM_MEM_CHAR_ENCODE_ERR = -1,         //Error   
    FTP_ENUM_MEM_CHAR_ENCODE_NO = 0,          //Don't know.
    FTP_ENUM_MEM_CHAR_ENCODE_CN = 1,          //EUC-CN, GB2312
    FTP_ENUM_MEM_CHAR_ENCODE_GBK = 2,          //EUC-CN, GB2312
    FTP_ENUM_MEM_CHAR_ENCODE_BIG5 = 3,          //BIG5
    FTP_ENUM_MEM_CHAR_ENCODE_JP = 4,          //JISX0208-1, EUC-JP
    FTP_ENUM_MEM_CHAR_ENCODE_KR = 5,          //EUC-KR
    FTP_ENUM_MEM_CHAR_ENCODE_UTF8 = 6,          //UTF-8
    FTP_ENUM_MEM_CHAR_ENCODE_ISO8859_1 = 7,    //ISO-8859-n: ENUM_MEM_CHAR_ENCODE_ISO8859_1 + n -1
}FTP_CHAR_ENCODE_TYPE;


typedef struct tagNET_UTILS_FTP_SERVER_INFO
{
    char    szSvrAddr[128]; //FTP IP地址
    WORD    wSvrPort;       //FTP 端口，如果输入0，SDK默认使用21
    char    szUserName[64];//用户名
    char    szPassword[64];//密码
    BYTE    byRes[254];	 
}NET_UTILS_FTP_SERVER_INFO,*LPNET_UTILS_FTP_SERVER_INFO;

typedef struct tagNET_UTILS_FTP_FILE_INFO
{
    char    szFileName[256];//全路径，包含文件名
    char    szDstDir[256];  //服务器上的目录，格式：xxx/xxx，为0表示上传到ftp主目录
    DWORD   dwOffset;       //文件偏移量(字节数)，断点续传时使用，0表示不偏移
    BYTE    byEncodeType;   //本地编码类型，特指szDstDir字段
    BYTE    szFTPFileName[128];  //ftp服务器上传文件名称
    BYTE    byStream;      //是否为上传码流， 0-根据路径找文件上传，1-根据缓冲器传入数据上传
    DWORD   dwSize;        //文件总长度
    BYTE    byRes[118];
}NET_UTILS_FTP_FILE_INFO,*LPNET_UTILS_FTP_FILE_INFO;

typedef struct tagNET_UTILS_FTP_UPLOAD_STATUS
{
    DWORD  dwStatus;       //1-上传成功，2-正在上传，3-网络异常，4-容量不足，5-上传失败，100以上-ftp原始状态码
    DWORD  dwProgress;	    //0~100
    DWORD  dwSendedSize;   //实际已发送的字节数
    BYTE   byRes[52];
}NET_UTILS_FTP_UPLOAD_STATUS,*LPNET_UTILS_FTP_UPLOAD_STATUS;

typedef struct tagNET_UTILS_FTP_UPLOAD_FILE_INFO
{
    char    szDstFile[256];  //服务器上的目录，格式：xxx/xxx,需要指向具体的文件
    BYTE    byRes[256];
}NET_UTILS_FTP_UPLOAD_FILEINFO, *LPNET_UTILS_FTP_UPLOAD_FILEINFO;

typedef struct tagNET_UTILS_FTP_UPLOAD_FILESIZE
{
    DWORD dwStatus;     //ftp返回的状态码
    DWORD dwSize;    //返回指定文件大小
    BYTE  byRes[256];
}NET_UTILS_FTP_UPLOAD_FILESIZE, *LPNET_UTILS_FTP_UPLOAD_FILESIZE;

NET_UTILS_API LONG    CALLBACK    NET_FTP_CreateClient(LPNET_UTILS_FTP_SERVER_INFO pSvrInfo);
NET_UTILS_API BOOL    CALLBACK    NET_FTP_UploadFile(LONG lUploadHandle, LPNET_UTILS_FTP_FILE_INFO pFileInfo);
NET_UTILS_API BOOL    CALLBACK    NET_FTP_Control(LONG lHandle, DWORD dwControlType, void* lpInBuffer, DWORD dwInLen, void* lpOutBuffer, DWORD dwOutLen);
NET_UTILS_API BOOL    CALLBACK    NET_FTP_DestroyClient(LONG lUploadHandle);

//遇到0xffffffff结束标记结束上传
NET_UTILS_API BOOL    CALLBACK    NET_FTP_InputData(LONG lUploadHandle, void* lpInBuffer, DWORD dwInLen);


/***********************************FTP end******************************************/

/***********************************Websocket begin**********************************/

//webSocket 请求行类型
typedef enum enumNET_UTILS_WEBSOCKET_REPLY_PARA
{
    ENUM_UTILS_WEBSOCKET_CUSTOM = 1,         //webSocket 自定义请求行类型
    ENUM_UTILS_WEBSOCKET_PACK_LEN = 2,       //能支持的最大websocket帧长度，默认64M，最大64M
                                             //一旦超过，则认定数据异常，服务端会断开连接，客户端则是断开连接后重连
                                             //主要用于保护接收端不会因为介绍太长的报文而导致内存不足
                                             //lpName为NULL即可，lpValue为(unsign int *)类型
    ENUM_UTILS_WEBSOCKET_REMOTE_ADDR = 5,    //用于服务端获取客户端的连接IP，仅value生效，目前仅支持服务端，lpValue类型为（char *）
                                             //dwValueLen不能少于48个字节，用于兼容IPv6的情况
    ENUM_UTILS_WEBSOCKET_REQ_URL = 6,        //用于服务端获取客户端请求的URL，仅value生效，目前仅支持服务端，lpValue类型为（char *）
}NET_UTILS_WEBSOCKET_REPLY_PARA;

typedef enum tagWEBSOCKET_MESSAGE_TYPE
{
    NO_TYPE = -1,                   //超时、接收错误数据等，关闭连接时发送
    CONTINUATION_MSG = 0,
    TEXT_MSG = 1,
    BINARY_MSG = 2,
    CLOSE_MSG = 8,
    PING_MSG = 9,
    PONG_MSG = 10,
    ESTABLISH_MSG = 11,             //握手完成
    HANDSHAKE_REQ_MSG = 12,         //握手请求
    RECONN_START = 13,              //重连开始
    RECONN_SUCC = 14                //重连成功
}WEBSOCKET_MESSAGE_TYPE;


typedef void (CALLBACK *WEBSOCKETDATACB)(LONG iHandle, void* pData, void* pUserData);

typedef struct tagNET_UTILS_WEBSOCKET_DATA_CB
{
    DWORD   dwSize;                     //本结构体长度，出于以后扩展考量，第一个版本可以无视该字段
    DWORD   dwStatus;                   //状态值，与SDK错误码保持一致
    DWORD   dwDataLen;                  //数据长度,不包含websocket帧头,仅只支持最大64M
    WEBSOCKET_MESSAGE_TYPE eType;       //数据类型
    BOOL    bFin;                       //是否为尾帧
    char*   pRecvData;                  //接收到的数据
}NET_UTILS_WEBSOCKET_DATA_CB, *LPNET_UTILS_WEBSOCKET_DATA_CB;

typedef struct tagNET_UTILS_WEBSOCKET_CLIENT_PARAM
{
    LPVOID  lpReqUrl;	          //[in]url，字符串格式，示例如下：
    //http://www.websocket.com/chat
    //http://127.0.0.1:80/chat
    DWORD   dwReqUrlLen;         //[in]url长度，字符串长度
    BYTE    byRelink;            //0:关闭重连功能，1：开启重连功能
    BYTE    byRes[15];           //保留
    DWORD   dwRecvTimeOut;       //单位ms，接收超时时间，默认30000ms，开启重连的情况下，该超时时间满足，则重连，关闭重连的情况下，仅回调网络异常通知
    DWORD   dwRelinkInterval;    //单位ms，重连间隔，即重连失败后，再次尝试重连的间隔时间
    DWORD   dwConnectTimeOut;    //单位ms，连接超时，默认3000ms
    DWORD   dwSendTimeOut;       //单位ms，发送超时，默认5000ms
    WEBSOCKETDATACB fnCB;        //用于回调数据以及网络异常
    LPVOID  pUserData;           //用户数据
}NET_UTILS_WEBSOCKET_CLIENT_PARAM, *LPNET_UTILS_WEBSOCKET_CLIENT_PARAM;

NET_UTILS_API LONG   CALLBACK    NET_WEBSOCKETCLIENT_Create(LPNET_UTILS_WEBSOCKET_CLIENT_PARAM lpParam);
//websocket协议允许用户在握手的时候，自定义字段，因此开放该接口
NET_UTILS_API BOOL   CALLBACK    NET_WEBSOCKETCLIENT_SetParam(LONG lHandle, NET_UTILS_WEBSOCKET_REPLY_PARA byCmdType, LPVOID lpName, DWORD dwNameLen, LPVOID lpValue, DWORD dwValueLen);
//设置客户端发送心跳间隔,特殊情况使用,间隔必须是5000毫秒的倍数,单位ms
NET_UTILS_API BOOL   CALLBACK    NET_WEBSOCKETCLIENT_SetHbInterval(LONG iHandle, DWORD uInterval);
//真正发起握手的函数
NET_UTILS_API BOOL   CALLBACK    NET_WEBSOCKETCLIENT_Start(LONG lHandle);
//用于获取用户自定义的头域
NET_UTILS_API BOOL   CALLBACK    NET_WEBSOCKETCLIENT_GetParam(LONG lHandle, NET_UTILS_WEBSOCKET_REPLY_PARA byCmdType, LPVOID lpName, DWORD dwNameLen, LPVOID lpValue, DWORD dwValueLen);
//发送数据
NET_UTILS_API BOOL   CALLBACK    NET_WEBSOCKETCLIENT_Send(LONG lHandle, WEBSOCKET_MESSAGE_TYPE byMsgType, BOOL bFin, LPVOID pBuff, DWORD dwDataLen);
//关闭websocket连接
NET_UTILS_API BOOL   CALLBACK    NET_WEBSOCKETCLIENT_Destroy(LONG lHandle);


typedef struct tagNET_UTILS_WEBSOCKET_LINK_CB
{
    DWORD   dwSize;      //本结构体长度，出于以后扩展考量，第一个版本可以无视该字段
    DWORD   dwStatus;    //状态值，保留
    BYTE    szSrcIP[128];//IP地址
    WORD   wSrcPort;    //端口
}NET_UTILS_WEBSOCKET_LINK_CB, *LPNET_UTILS_WEBSOCKET_LINK_CB;

//typedef BOOL(CALLBACK *WEBSOCKETLINKCB)(LONG lCallBackHandle, void* pData, void* pUserData);

typedef struct tatNET_UTILS_WEBSOCKET_LISTEN_PARAM
{
    BYTE   szIP[128];       //IP地址
    WORD   wPort;           //端口
    BYTE   byRes[2];
    DWORD  dwSendTimeOut;  //发送超时,默认5000ms
    DWORD  dwRecvTimeOut;  //接收超时,默认30000ms
    DWORD  dwIntervalTime; //心跳间隔时间，默认5000ms
    DWORD  dwWaitForAccept; //处于等待accept的链接个数,超过该个数，链接请求将直接被拒绝，默认0x7FFFFFFF，该参数暂时不生效
    WEBSOCKETDATACB fnCB;   //接收到数据时的回调
    LPVOID pUserData;       //用户数据
}NET_UTILS_WEBSOCKET_LISTEN_PARAM, *LPNET_UTILS_WEBSOCKET_LISTEN_PARAM;

typedef struct tatNET_UTILS_WEBSOCKET_LISTEN_PARAM_EX
{
    BYTE   szIP[128];       //IP地址
    WORD   wPort;           //端口
    BYTE   byRes[2];
    DWORD  dwSendTimeOut;   //发送超时,默认5000ms
    DWORD  dwRecvTimeOut;   //接收超时,默认30000ms
    DWORD  dwIntervalTime;  //心跳间隔时间，默认5000ms
    DWORD  dwWaitForAccept; //处于等待accept的链接个数,超过该个数，链接请求将直接被拒绝，默认0x7FFFFFFF，该参数暂时不生效
    WEBSOCKETDATACB fnCB;   //接收到数据时的回调
    LPVOID pUserData;       //用户数据

    BYTE bySSL;                 //0-不使用SSL, 1-使用SSL

    BYTE byVerifyMode;          //0-单向认证, 1-双向认证(目前不支持双向认证）
    BYTE byCertificateFileType; //证书类型0-pem, 1-ANS1
    BYTE byPrivateKeyFileType;  //私钥类型0-pem, 1-ANS1

    char *pUserCertificateFile; //服务端证书
    char *pUserPrivateKeyFile;  //服务端私钥
    char *pCACertificateFile;   //CA证书（因为不支持双向认证，故该字段目前无效）

    DWORD dwSSLVersion; //SSL Method版本
    //0 - SSL23, 1 - SSL2, 2 - SSL3, 3 - TLS1.0, 4 - TLS1.1, 5 - TLS1.2
    //SSL23是兼容模式，会协商客户端和服务端使用的最高版本

    BYTE byRes3[60];
}NET_UTILS_WEBSOCKET_LISTEN_PARAM_EX, *LPNET_UTILS_WEBSOCKET_LISTEN_PARAM_EX;

//typedef void (CALLBACK *WEBSOCKETSERVERDATACB)(LONG lCallBackHandle, void* pData, void* pUserData);

NET_UTILS_API LONG    CALLBACK    NET_WEBSOCKETSERVER_CreateListen(LPNET_UTILS_WEBSOCKET_LISTEN_PARAM lpParam);
NET_UTILS_API LONG    CALLBACK    NET_WEBSOCKETSERVER_CreateListenEx(LPNET_UTILS_WEBSOCKET_LISTEN_PARAM_EX lpParamEx);

//真正发起握手的函数
NET_UTILS_API BOOL    CALLBACK    NET_WEBSOCKETSERVER_Start(LONG lCallBackHandle);
//设置用户自定义的头域
NET_UTILS_API BOOL    CALLBACK    NET_WEBSOCKETSERVER_SetParam(LONG lCallBackHandle, NET_UTILS_WEBSOCKET_REPLY_PARA byCmdType, LPVOID lpName, DWORD dwNameLen, LPVOID lpValue, DWORD dwValueLen);
//获取用户自定义的头域
NET_UTILS_API BOOL    CALLBACK    NET_WEBSOCKETSERVER_GetParam(LONG lCallBackHandle, NET_UTILS_WEBSOCKET_REPLY_PARA byCmdType, LPVOID lpName, DWORD dwNameLen, LPVOID lpValue, DWORD dwValueLen);
//发送数据
NET_UTILS_API BOOL    CALLBACK    NET_WEBSOCKETSERVER_Send(LONG lCallBackHandle, WEBSOCKET_MESSAGE_TYPE byMsgType, BOOL bFin, LPVOID pBuff, DWORD dwDataLen);
//关闭websocket连接
NET_UTILS_API BOOL    CALLBACK    NET_WEBSOCKETSERVER_Close(LONG lCallBackHandle);
//关闭websocket监听
NET_UTILS_API BOOL    CALLBACK    NET_WEBSOCKETSERVER_DestroyListen(LONG lHandle);

/***********************************Websocket end************************************/

/***********************************NPQ begin****************************************/

#define NET_ERR_NPQ_BASE_INDEX    8000    //NPQ库错误码
#define NET_ERR_NPQ_PARAM       (NET_ERR_NPQ_BASE_INDEX + 1)        //NPQ库参数有误
#define NET_ERR_NPQ_SYSTEM      (NET_ERR_NPQ_BASE_INDEX + 2)        //NPQ库操作系统调用错误(包括资源申请失败或内部错误等）
#define NET_ERR_NPQ_GENRAL      (NET_ERR_NPQ_BASE_INDEX + 3)        //NPQ库内部通用错误
#define NET_ERR_NPQ_PRECONDITION    (NET_ERR_NPQ_BASE_INDEX + 4)        //NPQ库调用顺序错误
#define NET_ERR_NPQ_NOTSUPPORT        (NET_ERR_NPQ_BASE_INDEX + 5)        //NPQ库功能不支持

#define NET_ERR_NPQ_NOTCALLBACK    (NET_ERR_NPQ_BASE_INDEX + 100)   //数据没有回调上来


//enum NPQ_DATA_TYPE
//{
//    NPQ_DATA_RTP_VIDEO,			//视频RTP数据
//    NPQ_DATA_RTP_AUDIO,			//音频RTP数据
//    NPQ_DATA_RTP_PRIVATE,		//私有包RTP数据
//    NPQ_DATA_RTP_MUTI,          //RTP复合流数据
//    NPQ_DATA_RTCP_VIDEO,		//视频RTCP
//    NPQ_DATA_RTCP_AUDIO,		//音频RTCP
//    NPQ_DATA_CMD,				//命令回调，对应NPQ_CMD 需要外部做相应处理
//    NPQ_DATA_MULITIPLEX,        //未知类型数据（外部不确定数据类型，由NPQ内部识别,需要结合sdp）
//};

//enum NPQ_CMD_TYPE
//{
//    NPQ_CMD_ENCODE,		//音视频编码参数
//    NPQ_CMD_FORCE_I_FRAME   //强制I帧消息
//};

struct NET_UTILS_NPQ_ECODE_PARAM
{
    int iVersion;				//结构体版本，用于以后兼容
    int iMaxBitRate;			//最大码率   单位 bps
    int iResolution;			//分辨率  按照网络SDK协议定义的索引值
    unsigned char res[244];
};

typedef struct tagNET_UTILS_NPQ_CMD
{
    int iVersion;					//结构体版本，用于以后兼容
    NPQ_CMD_TYPE enInfoType;		//命令类型
    union INFO_U
    {
        NET_UTILS_NPQ_ECODE_PARAM struEncode;
        unsigned char res[256];
    }unCmd;
}NET_UTILS_NPQ_CMD, *LPNET_UTILS_NPQ_CMD;

// 时间
typedef struct _HK_SYSTEMTIME_UTILS
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
}HK_SYSTEMTIME_UTILS;

typedef struct tagNET_UTILS_NPQ_PACK_PARAM
{
    // 固定的参数
    DWORD        pack_size;             // PS、TS和RTP设置最大包长，0为默认长度
    DWORD        track_index;           // 轨道号
    DWORD        syc_video_track;       // 关联的视频轨道号

    DWORD        system_format;         // 目标封装格式
    DWORD        system_format_subtype; // 目标类型的子格式，目前只使用于MP4，其他封装不适用

    DWORD        video_format;          // 视频类型
    DWORD        audio_format;          // 音频类型
    DWORD        privt_format;          // 私有数据类型

    // 参考帧信息，非固定参数
    DWORD        is_hik_stream;         // 是否符合海康定义
    DWORD        encrypt_type;          // 加密类型
    DWORD        frame_type;            // 当前帧类型 I/P/B/audio/privt
    DWORD        time_stamp;            // 时间戳
    float               duration;              // 帧时长
    DWORD        frame_num;             // 帧号
    HK_SYSTEMTIME_UTILS       global_time;           // 全局时间（海康码流才有）

    // 视频参数
    struct
    {
        WORD  width_orig;            // 分辨率，原始宽
        WORD  height_orig;           // 分辨率，原始高
        WORD  width_play;            // 分辨率，裁剪宽
        WORD  height_play;           // 分辨率，裁剪高
        float           frame_rate;            // 帧率
        WORD  interlace;             // 是否场编码
        WORD  b_frame_num;           // 组模式中，B帧的个数
        DWORD    is_svc_stream;         // 是否SVC码流
    }video;

    // 音频参数
    struct
    {
        WORD  channels;              // 声道
        WORD  bits_per_sample;       // 样位
        DWORD    samples_rate;          // 采样率
        DWORD    bit_rate;              // 比特率
    }audio;

    // 私有参数
    struct
    {
        DWORD    privt_type;            // 私有类型
        DWORD    data_type;             // 子类型
    }privt;

}NET_UTILS_NPQ_PACK_PARAM, *LPNET_UTILS_NPQ_PACK_PARAM;

typedef struct tagNET_UTILS_IPADDRESS
{
    char szIP[128];     //IP地址
    WORD  wPort;      //端口号
    BYTE    byRes[2];
}NET_UTILS_IPADDRESS, *LPNET_UTILS_IPADDRESS;

typedef struct tagNET_UTILS_NPQ_LINK_COND
{
    NET_UTILS_IPADDRESS struAudioRtp;       //音频流地址
    NET_UTILS_IPADDRESS struAudioRtcp;     //音频RTCP地址
    NET_UTILS_IPADDRESS struVideoRtp;        //视频流地址
    NET_UTILS_IPADDRESS struVideoRtcp;      //视频RTCP地址
    BYTE   byRes[32];
}NET_UTILS_NPQ_LINK_COND, *LPNET_UTILS_NPQ_LINK_COND;

typedef void (CALLBACK *NPQ_TRANS_CB)(LONG id, LONG iDataType, unsigned char* pData, DWORD nDataLen, void* pUser);

typedef struct tagNET_UTILS_NPQ_SERVICE_COND
{
    NET_UTILS_NPQ_LINK_COND struLinkLocal;      //本地视音频流连接和RTCP连接地址信息
    NPQ_TRANS_CB fnDataCallBack;    //码流数据回调函数
    void* pUser;  //回调函数用户参数
    char *sSdp; //本地SDP信息
    char *sFile;    //文件路径
    BYTE byRes[28];
}NET_UTILS_NPQ_SERVICE_COND, *LPNET_UTILS_NPQ_SERVICE_COND;

//enum NPQ_QOS_TYPE
//{
//    QOS_TYPE_NACK = (1 << 0),          //Nack		
//    QOS_TYPE_FEC = (1 << 1),			 //FEC
//    QOS_TYPE_DEJITTER = (1 << 2),		 //去抖动
//    QOS_TYPE_BW = (1 << 3),			 //拥塞控制
//    QOS_TYPE_PLI = (1 << 4),			 //PLI
//    QOS_TYPE_SYNCHRONOUS = (1 << 5),	 //音视频同步
//    QOS_TYPE_ALL = 0xff,
//};


typedef struct tagNET_UTILS_NPQ_STATE_SINGLE
{
    DWORD dwRttUs;			//rtt，单位us
    DWORD dwRealRttUs;		//实时rtt，单位us
    BYTE byLossFraction;	//丢包率，单位1/256
    BYTE byLossFraction2;	//经过恢复之后的丢包率，只能在接收端获取，单位1/256
    BYTE byRes1[2];
    DWORD dwBitRate;   //当前数据实时码率，单位：bps
    BYTE byRes[120];
}NET_UTILS_NPQ_STATE_SINGLE, *LPNET_UTILS_NPQ_STATE_SINGLE;

typedef struct tagNET_UTILS_NPQ_STATE
{
    DWORD dwSize;
    NET_UTILS_NPQ_STATE_SINGLE struAudioState;  //音频传输状态
    NET_UTILS_NPQ_STATE_SINGLE struVideoState;  //视频传输状态
    BYTE byRes[256];
}NET_UTILS_NPQ_STATE, *LPNET_UTILS_NPQ_STATE;

typedef struct tagNET_UTILS_NPQ_CONNECT_PARAM
{
    NET_UTILS_NPQ_LINK_COND struLinkRemote;  //对端视音频流连接和RTCP连接地址信息
    char *sSdp; //对端SDP信息
    char *sFile;    //文件路径
    BYTE byRes[60];
}NET_UTILS_NPQ_CONNECT_PARAM, *LPNET_UTILS_NPQ_CONNECT_PARAM;

typedef struct tagNET_UTILS_NPQ_COMPRESSINFO
{
    DWORD dwSize;
    DWORD dwMaxBitRate; //最大码率
    char *sFile;    //文件路径
    BYTE byRes[60];
}NET_UTILS_NPQ_COMPRESSINFO, *LPNET_UTILS_NPQ_COMPRESSINFO;

typedef struct tagNET_UTILS_NPQ_PARAM_SINGLE
{
    int iType;  //Qos策略类型，参照NPQ_QOS_TYPE
    BOOL bVideo;    //是否开启视频Qos
    BOOL bAudio;    //是否开启音频Qos
    BYTE byRes[256];
}NET_UTILS_NPQ_PARAM_SINGLE, *LPNET_UTILS_NPQ_PARAM_SINGLE;

typedef struct tagNET_UTILS_NPQ_PARAM
{
    DWORD dwSize;
    NET_UTILS_NPQ_PARAM_SINGLE struSenderParam; //作为发送端的NPQ参数
    NET_UTILS_NPQ_PARAM_SINGLE struReceiverParam; //作为接收端的NPQ参数
    BYTE byRes[128];
}NET_UTILS_NPQ_PARAM, *LPNET_UTILS_NPQ_PARAM;


NET_UTILS_API LONG    CALLBACK    NET_UTILS_NpqTransStart(NET_UTILS_NPQ_SERVICE_COND *lpServiceCond);
NET_UTILS_API BOOL    CALLBACK    NET_UTILS_NpqTransInputData(LONG iHandle, LONG iDataType, unsigned char* pData, DWORD nDataLen);
NET_UTILS_API BOOL    CALLBACK    NET_UTILS_NpqTransInputRawData(LONG iHandle, LPNET_UTILS_NPQ_PACK_PARAM lpPackParam, unsigned char* pData, DWORD nDataLen);
NET_UTILS_API BOOL    CALLBACK    NET_UTILS_NpqTransGetState(LONG iHandle, LPNET_UTILS_NPQ_STATE lpNpqState);
NET_UTILS_API BOOL    CALLBACK    NET_UTILS_NpqTransStop(LONG iHandle);
NET_UTILS_API BOOL    CALLBACK    NET_UTILS_NpqTransGetCompressInfo(LONG iHandle, LPNET_UTILS_NPQ_COMPRESSINFO lpCompressInfo);
NET_UTILS_API BOOL    CALLBACK    NET_UTILS_NpqTransConnect(LONG iHandle, NET_UTILS_NPQ_CONNECT_PARAM *lpConnectParam);
NET_UTILS_API BOOL    CALLBACK    NET_UTILS_NpqSetParams(LONG iHandle, NET_UTILS_NPQ_PARAM *lpParam);


/***********************************NPQ end****************************************/

/***********************************SIP begin****************************************/

//SIP相关接口封装定义
#define SIP_USERNAME_LEN    64  //SIP用户名长度
#define SIP_PASSWORD_LEN    64  //SIP密码长度
#define SIP_CALL_ID_LEN          64  //呼叫ID长度
#define SIP_NAME_LEN       64   //sip终端名称长度
#define SIP_URL_LEN     256 //sip url长度
#define SIP_SDP_LEN     16384   //SDP长度
#define SIP_DEV_TYPE_LEN    24  //SIP设备类型名称长度

typedef enum
{
    SIP_STATUS_100_TRYING = 100,
    SIP_STATUS_180_RINGING = 180,
    SIP_STATUS_181_CALL_IS_BEING_FORWARDED = 181,
    SIP_STATUS_182_QUEUED = 182,
    SIP_STATUS_183_SESSION_PROGRESS = 183,
    SIP_STATUS_200_OK = 200,
    SIP_STATUS_202_ACCEPTED = 202,
    SIP_STATUS_300_MULTIPLE_CHOICES = 300,
    SIP_STATUS_301_MOVED_PERMANENTLY = 301,
    SIP_STATUS_302_MOVED_TEMPORARILY = 302,
    SIP_STATUS_305_USE_PROXY = 305,
    SIP_STATUS_380_ALTERNATIVE_SERVICE = 380,
    SIP_STATUS_400_BAD_REQUEST = 400,
    SIP_STATUS_401_UNAUTHORIZED = 401,
    SIP_STATUS_402_PAYMENT_REQUIRED = 402,
    SIP_STATUS_403_FORBIDDEN = 403,
    SIP_STATUS_404_NOT_FOUND = 404,
    SIP_STATUS_405_METHOD_NOT_ALLOWED = 405,
    SIP_STATUS_406_NOT_ACCEPTABLE = 406,
    SIP_STATUS_407_PROXY_AUTH_REQUIRED = 407,
    SIP_STATUS_408_REQUEST_TIMEOUT = 408,
    SIP_STATUS_409_CONFLICT = 409,
    SIP_STATUS_410_GONE = 410,
    SIP_STATUS_411_LENGTH_REQUIRED = 411,
    SIP_STATUS_412_PRECONDITION_FAILED = 412,
    SIP_STATUS_413_REQUEST_TOO_LARGE = 413,
    SIP_STATUS_414_REQUEST_URI_TOO_LONG = 414,
    SIP_STATUS_415_UNSUPPORTED_MEDIA = 415,
    SIP_STATUS_416_UNSUPPORTED_URI = 416,
    SIP_STATUS_417_RESOURCE_PRIORITY = 417,
    SIP_STATUS_420_BAD_EXTENSION = 420,
    SIP_STATUS_421_EXTENSION_REQUIRED = 421,
    SIP_STATUS_422_SESSION_TIMER_TOO_SMALL = 422,
    SIP_STATUS_423_INTERVAL_TOO_BRIEF = 423,
    SIP_STATUS_423_REGISTRATION_TOO_BRIEF = 423,
    SIP_STATUS_480_TEMPORARILY_UNAVAILABLE = 480,
    SIP_STATUS_481_NO_CALL = 481,
    SIP_STATUS_481_NO_TRANSACTION = 481,
    SIP_STATUS_482_LOOP_DETECTED = 482,
    SIP_STATUS_483_TOO_MANY_HOPS = 483,
    SIP_STATUS_484_ADDRESS_INCOMPLETE = 484,
    SIP_STATUS_485_AMBIGUOUS = 485,
    SIP_STATUS_486_BUSY_HERE = 486,
    SIP_STATUS_487_REQUEST_CANCELLED = 487,
    SIP_STATUS_487_REQUEST_TERMINATED = 487,
    SIP_STATUS_488_NOT_ACCEPTABLE = 488,
    SIP_STATUS_489_BAD_EVENT = 489,
    SIP_STATUS_490_REQUEST_UPDATED = 490,
    SIP_STATUS_491_REQUEST_PENDING = 491,
    SIP_STATUS_493_UNDECIPHERABLE = 493,
    SIP_STATUS_494_SECAGREE_REQUIRED = 494,
    SIP_STATUS_500_INTERNAL_SERVER_ERROR = 500,
    SIP_STATUS_501_NOT_IMPLEMENTED = 501,
    SIP_STATUS_502_BAD_GATEWAY = 502,
    SIP_STATUS_503_SERVICE_UNAVAILABLE = 503,
    SIP_STATUS_504_GATEWAY_TIME_OUT = 504,
    SIP_STATUS_505_VERSION_NOT_SUPPORTED = 505,
    SIP_STATUS_513_MESSAGE_TOO_LARGE = 513,
    SIP_STATUS_580_PRECONDITION = 580,
    SIP_STATUS_590_SELF_DEF_NEEDWAIT = 590,
    SIP_STATUS_600_BUSY_EVERYWHERE = 600,
    SIP_STATUS_603_DECLINE = 603,
    SIP_STATUS_604_DOES_NOT_EXIST_ANYWHERE = 604,
    SIP_STATUS_606_NOT_ACCEPTABLE = 606,
    SIP_STATUS_687_DIALOG_TERMINATED = 687,
}SIP_STATUS_CODE;

typedef enum tagSIP_LINK_TYPE
{
    SIP_UDP = 0,
    SIP_TCP,
    SIP_TLS,
}SIP_LINK_TYPE;

//SIP_I_XXX为对方请求或链接状态改变事件，SIP_R_XXX为对方响应己方的事件
typedef enum tagSIP_EVENT_TYPE
{
    SIP_I_INVITE = 0,           //呼入事件
    SIP_I_ACTIVE,                //呼叫建立
    SIP_I_TERMINATED,    //呼叫中止
    SIP_I_INFO,                     //INFO消息事件
    SIP_R_INVITE,                //INVITE响应事件
    SIP_R_REGISTER,          //REGISTER响应事件
}SIP_EVENT_TYPE;

typedef struct tagNET_SIP_INVITE_INPUT
{
    char sCallID[SIP_CALL_ID_LEN];  //呼叫ID,用于区分一路SIP会话，此参数用于更新一路SIP会话的INVITE请求，对于新建会话，此参数填为空
    char sRequestUrl[SIP_URL_LEN];  //INVITE请求URL信息，e.g. sip:test@10.10.10.10:5060
    char sSdp[SIP_SDP_LEN];     //SDP信息
    BYTE byRes[32];
}NET_SIP_INVITE_INPUT, *LPNET_SIP_INVITE_INPUT;

typedef struct tagNET_SIP_INVITE_OUTPUT
{
    char sCallID[SIP_CALL_ID_LEN];  //呼叫ID,用于区分一路SIP会话
    BYTE byRes[32];
}NET_SIP_INVITE_OUTPUT, *LPNET_SIP_INVITE_OUTPUT;

typedef struct tagNET_SIP_INFO_INPUT
{
    char sCallID[SIP_CALL_ID_LEN];  //呼叫ID,用于区分一路SIP会话
    char *pContent; //INFO消息内容
    DWORD dwContentLength;  //INFO消息内容长度
    BYTE byRes[32];
}NET_SIP_INFO_INPUT, *LPNET_SIP_INFO_INPUT;

typedef struct tagNET_SIP_INFO_OUTPUT
{
    DWORD dwCseq;  //INFO消息的序列号
    BYTE byRes[124];
}NET_SIP_INFO_OUTPUT, *LPNET_SIP_INFO_OUTPUT;

typedef struct tagNET_SIP_REGISTER_INPUT
{
    NET_UTILS_IPADDRESS struSipAddress; //SIP服务器的地址信息
    char sUserName[SIP_USERNAME_LEN];   //SIP用户名，注册服务器用
    char sPassword[SIP_PASSWORD_LEN];    //SIP密码，注册服务器用
    DWORD dwExpires;    //注册有效期，单位：秒，填0默认10min
    DWORD dwKeepAliveInterval;    //保活间隔，单位：秒，填0默认5s
    BYTE byRes[32];
}NET_SIP_REGISTER_INPUT, *LPNET_SIP_REGISTER_INPUT;

typedef struct tagNET_SIP_ANSWER_INVITE_INPUT
{
    char sCallID[SIP_CALL_ID_LEN];  //呼叫ID,用于区分一路SIP会话
    BOOL bAccept;   //是否接听
    char sSdp[SIP_SDP_LEN];     //SDP信息
    BYTE byRes[64];
}NET_SIP_ANSWER_INVITE_INPUT, *LPNET_SIP_ANSWER_INVITE_INPUT;

//事件对应参数

typedef struct tagNET_SIP_I_INVITE_PARAM
{
    char sRemoteUrl[SIP_URL_LEN]; //对方终端标识,e.g. sip:test@10.10.10.10:5060
    char sSdp[SIP_SDP_LEN];     //SDP信息
    char sDevType[SIP_DEV_TYPE_LEN];    //对端设备类型名称
    BYTE byRes[40];
}NET_SIP_I_INVITE_PARAM, *LPNET_SIP_I_INVITE_PARAM;

typedef struct tagNET_SIP_I_INFO_PARAM
{
    char *pContent; //INFO消息内容
    DWORD dwContentLength;  //INFO消息内容长度
    DWORD dwCseq;    //INFO序列号
    BYTE byRes[60];
}NET_SIP_I_INFO_PARAM, *LPNET_SIP_I_INFO_PARAM;

typedef struct tagNET_SIP_R_INVITE_PARAM
{
    char sSdp[SIP_SDP_LEN];     //SDP信息
    char sDevType[SIP_DEV_TYPE_LEN];    //对端设备类型名称
    BYTE byRes[40];
}NET_SIP_R_INVITE_PARAM, *LPNET_SIP_R_INVITE_PARAM;

typedef struct tagNET_SIP_EVENT_PARAM
{
    char sCallID[SIP_CALL_ID_LEN];  //呼叫ID,用于区分一路SIP会话
    union
    {
        BYTE byRes[16704];
        NET_SIP_I_INVITE_PARAM struIInvite; //状态值为200，SIP_I_INVITE事件有效
        NET_SIP_I_INFO_PARAM struIInfo; //状态值为200，SIP_I_INFO事件有效
        NET_SIP_R_INVITE_PARAM struRInvite; //状态值为200，SIP_R_INVITE事件有效
    }uniEventParam;
}NET_SIP_EVENT_PARAM, *LPNET_SIP_EVENT_PARAM;

typedef NET_SIP_INVITE_OUTPUT NET_SIP_CLEAR_INPUT;

/** @fn typedef void (CALLBACK * SIP_EVENT_CALLBACK(CALLBACK *SIP_EVENT_CALLBACK)
*  @brief  SIP事件回调函数
*  @param (in)	LONG lHandle:   SIP服务实例句柄，NET_SIP_Create的返回值
*  @param (in)	SIP_EVENT_TYPE enEventType:   SIP事件类型
*  @param (in)	SIP_STATUS_CODE enStatus:   SIP状态码
*  @param (in)	NET_SIP_EVENT_PARAM* lpParam:   事件对应参数
*  @param (in)	void* pUser:   用户参数
*  @return SIP_STATUS_CODE
*	当收到SIP_I_INVITE事件时，可通过返回值接收或拒绝请求，其中
*	SIP_STATUS_200_OK表示接听，一般在自动接听时使用
*	SIP_STATUS_590_SELF_DEF_NEEDWAIT 表示需要用户接听或拒绝，如果返回此状态，需要
*	调用NET_SIP_AnswerInvite接听或者拒接呼叫
*	当收到SIP_R_INVITE事件时，可通过返回值接收或拒绝对方对自己呼叫的响应
*	其他事件时，返回值无效
*/
typedef SIP_STATUS_CODE(CALLBACK *SIP_EVENT_CALLBACK)(LONG lHandle, SIP_EVENT_TYPE enEventType, SIP_STATUS_CODE enStatus, NET_SIP_EVENT_PARAM* lpParam, void* pUser);

typedef struct tagNET_SIP_CREATE_INPUT
{
    NET_UTILS_IPADDRESS struSipAddress; //本机开启SIP服务的地址信息
    char sLocalName[SIP_NAME_LEN];  //本机名称
    SIP_LINK_TYPE enLinkType;   //使用的传输协议类型，目前只支持UDP
    SIP_EVENT_CALLBACK fnEventCallBack; //SIP事件的回调函数
    void* pUser; //用户参数
    BYTE byRes[32];
}NET_SIP_CREATE_INPUT, *LPNET_SIP_CREATE_INPUT;

NET_UTILS_API LONG    CALLBACK    NET_SIP_Create(NET_SIP_CREATE_INPUT *lpCreateInput);

NET_UTILS_API BOOL    CALLBACK    NET_SIP_Invite(LONG lHandle, NET_SIP_INVITE_INPUT *lpInviteInput, NET_SIP_INVITE_OUTPUT *lpInviteOutput);

NET_UTILS_API BOOL    CALLBACK    NET_SIP_AnswerInvite(LONG lHandle, NET_SIP_ANSWER_INVITE_INPUT *lpAnswerInviteInput);

NET_UTILS_API BOOL    CALLBACK    NET_SIP_ClearCall(LONG lHandle, NET_SIP_CLEAR_INPUT *lpClearInput);

NET_UTILS_API BOOL    CALLBACK    NET_SIP_SendInfo(LONG lHandle, NET_SIP_INFO_INPUT *lpInfoInput);

NET_UTILS_API BOOL    CALLBACK    NET_SIP_SendInfoEx(LONG lHandle, NET_SIP_INFO_INPUT *lpInfoInput, NET_SIP_INFO_OUTPUT *lpInfoOutput);

NET_UTILS_API BOOL    CALLBACK    NET_SIP_Register(LONG lHandle, NET_SIP_REGISTER_INPUT *lpRegisterInput);

NET_UTILS_API BOOL    CALLBACK    NET_SIP_UnRegister(LONG lHandle);

NET_UTILS_API BOOL    CALLBACK    NET_SIP_Destroy(LONG lHandle);

/***********************************SIP end****************************************/

typedef enum tagTRANS_TERMINAL_TYPE
{
    TRANS_TERMINAL_CLIENT = 0,
    TRANS_TERMINAL_SERVER = 1,
    TRANS_TERMINAL_SERVER_LINK = 2
}TRANS_TERMINAL_TYPE;

typedef enum tagTRANS_PROTO_TYPE
{
    TRANS_PROTO_TCP = 0,
    TRANS_PROTO_UDP = 1
}TRANS_PROTO_TYPE;

typedef enum tagTRANS_MODE_TYPE
{
    TRANS_MODE_ASYN = 0,
    TRANS_MODE_SYN = 1
}TRANS_MODE_TYPE;

typedef enum tagTRANS_STATUS_TYPE
{
    TRANS_STATUS_OK = 0,
    TRANS_STATUS_NEW = 1,
    TRANS_STATUS_CLOSE = 2
}TRANS_STATUS_TYPE;

typedef struct tagNET_TRANS_SEND_PARAM
{
    char* pBuf;       //发送缓冲区
    DWORD dwSendLen;  //发送缓冲区大小
    DWORD dwTimeout;  //超时时间（单位：毫秒）
    BYTE  byRes[36];
} NET_TRANS_SEND_PARAM, *LPNET_TRANS_SEND_PARAM;

typedef struct tagNET_TRANS_RECV_PARAM
{
    char* pBuf;       //接收缓冲区
    DWORD dwBufLen;   //接收缓冲区大小（NET_TRANS_Recv接口使用，异步回调不使用）
    DWORD dwTimeout;  //超时时间（单位：毫秒）（NET_TRANS_Recv接口使用，异步回调不使用）
    DWORD dwWant;     //期望接收的数据长度（目前尚不支持）
    DWORD dwRecvLen;  //实际接收到的数据长度
    DWORD dwStatus;   //状态值，0-表示正常，1-TCP新连接，2-TCP连接断开（异步模式下）
    BYTE  byRes[24];
} NET_TRANS_RECV_PARAM, *LPNET_TRANS_RECV_PARAM;

typedef struct tagNET_TRANS_PEER
{
    char  szPeerIP[128];
    WORD  wPeerPort;
    BYTE  byRes[30];
}NET_TRANS_PEER, *LPNET_TRANS_PEER;


typedef void (CALLBACK *TRANS_RECV_CB)(LONG lHandle, NET_TRANS_RECV_PARAM *pRecvParam, NET_TRANS_PEER *pPeer, void* pUser);

typedef struct tagNET_TRANS_PARAM
{
    BYTE byType;                 //0-Client 1-Server
    BYTE byProto;                //0-TCP 1-UDP
    BYTE byBind;                 //0-不绑定 1-绑定
    BYTE byMode;                 //0-同步 1-异步
    BYTE byRes1[2];
    WORD wPort;           //监听端口
    char szIP[128];       //监听IP
    DWORD dwSendTimeout;  //默认发送数据超时时间（单位：毫秒）
    DWORD dwRecvTimeout;  //默认接收数据超时时间（单位：毫秒）
    DWORD dwConnectTimeout;  //连接超时时间（单位：毫秒）
    TRANS_RECV_CB fnRecvCB;  //数据回调（TCP必须提供，UDP仅在异步模式下提供）
    void *pRecvUser;         //数据回调用户指针

    BYTE  byRes[32];
} NET_TRANS_PARAM, *LPNET_TRANS_PARAM;


NET_UTILS_API  LONG CALLBACK  NET_TRANS_Create(const NET_TRANS_PARAM *pServerParam);
NET_UTILS_API  LONG CALLBACK  NET_TRANS_Send(const LONG lHandle, NET_TRANS_SEND_PARAM *pSendParam, NET_TRANS_PEER *pPeer = NULL);
NET_UTILS_API  LONG CALLBACK  NET_TRANS_Recv(const LONG lHandle, NET_TRANS_RECV_PARAM *pRecvParam, NET_TRANS_PEER *pPeer = NULL);
NET_UTILS_API  int  CALLBACK  NET_TRANS_GetSocket(const LONG lHandle);
NET_UTILS_API  BOOL CALLBACK  NET_TRANS_Destroy(const LONG lHandle);


//--------------------------------RTSP服务端-----------------------------------

typedef BOOL(CALLBACK * RTSPSERVERDATACB)(LONG lSessionHandle, DWORD dwType, void* pData, void* pUser);

typedef struct tagNET_RTSP_SERVER_PARAM
{
    char   szListenIP[128];      //IP地址
    WORD   wListenPort;    //RTSP端口(默认554)
    BYTE   byRes[2];

    RTSPSERVERDATACB fnCB;  //接收到数据时的回调
    void*  pUser;           //用户数据

    BYTE   byRes1[52];

}NET_RTSP_SERVER_PARAM, *LPNET_RTSP_SERVER_PARAM;

typedef struct tagNET_RTSP_SERVER_MCAST_PARAM
{
    char   szMCastIP[128];      //多播IP地址
    WORD   wMCastPort;          //多播端口
    BYTE   byRes[2];
    char*  pConference;
    DWORD  dwConferenceLen;
    LONG   lRtspHandle;
    BYTE   byRes1[48];
}NET_RTSP_SERVER_MCAST_PARAM, *LPNET_RTSP_SERVER_MCAST_PARAM;


#define NET_RTSP_SET_PORT_RANGE  1
#define NET_RTSP_SET_MCAST_ADDR  2

typedef struct tatNET_RTSP_RTP_PORT_RANGE
{
    WORD   wStartPort;    //RTP端口起始地址(TCP/UDP/Mcast公用)（含）
    WORD   wEndPort;      //RTP端口结束地址(TCP/UDP/Mcast公用)（不含）
    BYTE    byRes[12];
}NET_RTSP_RTP_PORT_RANGE, *LPNET_RTSP_RTP_PORT_RANGE;

typedef struct tagNET_RTSP_RTP_MCAST_ADDR
{
    char szIP[128];  //多播IP地址
    BYTE byRes[32];
}NET_RTSP_RTP_MCAST_ADDR, *LPNET_RTSP_RTP_MCAST_ADDR;

NET_UTILS_API  LONG CALLBACK NET_RTSPSERVER_Create(LPNET_RTSP_SERVER_PARAM lpParam);

NET_UTILS_API  BOOL CALLBACK NET_RTSPSERVER_SetParam(LONG lHandle, DWORD dwCommand, void *pData, DWORD dwDataLen);

NET_UTILS_API  BOOL CALLBACK NET_RTSPSERVER_Start(LONG lHandle);

NET_UTILS_API  LONG CALLBACK NET_RTSPSERVER_RtpSend(LONG lSessionHandle, BYTE byRtpTag, char* pSendBuf, DWORD dwSendLen, DWORD dwTimeout);

NET_UTILS_API  BOOL CALLBACK NET_RTSPSERVER_Close(LONG lSessionHandle);

NET_UTILS_API  LONG CALLBACK NET_RTSPSERVER_CreateMCast(LPNET_RTSP_SERVER_MCAST_PARAM lpParam);
NET_UTILS_API  LONG CALLBACK NET_RTSPSERVER_MCastRtpSend(LONG lMCastHandle, BYTE byRtpTag, char* pSendBuf, DWORD dwSendLen, DWORD dwTimeout);
NET_UTILS_API  LONG CALLBACK NET_RTSPSERVER_DestroyMCast(LONG lMCastHandle);

NET_UTILS_API  BOOL CALLBACK NET_RTSPSERVER_Destroy(LONG lHandle);

#define RTSP_MSG_CONNECT       1 //RTSP连接建立消息
#define RTSP_MSG_DISCONNECT    2 //RTSP连接断开消息

#define RTSP_MSG_DESCRIBE      3 //DESCRIBE信息
#define RTSP_MSG_SETUP         4 //SETUP信息
#define RTSP_MSG_PLAY          5 //PLAY信息
#define RTSP_MSG_TEARDOWN      6 //TEARDOWN信息
#define RTSP_MSG_PRIVATETRANS  7 // PRIVATETRANS信息

#define RTSP_MSG_ERROR         256 //错误

//RTSP_MSG_CONNECT
typedef struct tagNET_RTSP_LINK_INFO
{
    char   szClientIP[128]; //RTSP客户端IP
    WORD   wClientPort;     //RTSP客户端端口
    BYTE   byRes[2];
    //char   szLocalIP[128];  //RTSP客户端在本地网卡上的IP(多播使用)
    BYTE   byRes1[28];
} NET_RTSP_LINK_INFO, *LPNET_RTSP_LINK_INFO;

typedef struct tagNET_RTSP_SERVER_SDP_INFO
{
    DWORD dwStartTime; //开始时间戳(单位:毫秒)
    DWORD dwEndTime;   //结束时间戳(单位:毫秒)
    char  szHeader[256];      //头数据
    char  szEncryptDes[64];   //头数据加密方式
    char  szVideoControlUrl[512];    //视频URL
    char  szAudioControlUrl[512];    //音频URL(当前版本尚不支持音频)
    char  szVersion[128];     //版本信息
    BYTE  byRes[256];
}NET_RTSP_SERVER_SDP_INFO, *LPNET_RTSP_SERVER_SDP_INFO;


typedef struct tagNET_RTSP_SERVER_DESCRIBE
{
    char*  pClientIP;        //RTSP客户端IP地址
    WORD   wClientPort;      //RTSP客户端端口
    BYTE   byRes[2];
    char*  pUrl;             //URL信息
    char*  pAuth;            //认证信息

    NET_RTSP_SERVER_SDP_INFO struSDPInfo; //服务端SDP信息

    BYTE   byRes1[12];
}NET_RTSP_SERVER_DESCRIBE, *LPNET_RTSP_SERVER_DESCRIBE;

typedef struct tagNET_RTSP_SERVER_SETUP
{
    char* pControlUrl;   //SETUP传递的URL
    char* pConference;   //Conference信息
    LONG  lMCastHandle;  //多播句柄-1表示多播组未创建，>=0表示将要加入的多播组
    char* pMCastIP;
    WORD  wMCastPort;
    BYTE  byRes[14];
}NET_RTSP_SERVER_SETUP, *LPNET_RTSP_SERVER_SETUP;

typedef struct tagNET_RTSP_SERVER_PLAY
{
    BYTE  byType;   //0-暂停播放，1-恢复播放，2-按时间范围播放，3-按倍率播放
    BYTE  byRtpTag; //0-视频RTP链路 1-音频RTP链路
    BYTE  byRes[2];
    DWORD dwStartTime; // byType=2时有效，播放起始时间(单位：毫秒)
    DWORD dwEndTime;   // byType=2时有效，播放结束时间(单位：毫秒)
    DWORD dwScale;     // byType=3时有效，播放倍率
    BYTE  byRes1[16];
}NET_RTSP_SERVER_PLAY, *LPNET_RTSP_SERVER_PLAY;


typedef struct tagNET_RTSP_SERVER_ERROR_INFO
{
    DWORD dwErrorCode;
    BYTE  byRes[28];
}NET_RTSP_SERVER_ERROR_INFO, *LPNET_RTSP_SERVER_ERROR_INFO;


//-----------------------------推流客户端-----------------------------------

typedef struct tagNET_PSTREAM_PUSH_PARAM
{
    char            szServerIP[128]; //IP地址
    WORD            wServerPort;     //端口
    BYTE            byLinkMode;      //连接类型：0-TCP，1-UDP(尚不支持)，2-HRUDP(尚不支持)
    BYTE            byPackageType;   //封装格式，0-RTP
    char            szDeviceID[32];  //设备ID(客户端定义)
    char           *pHeader;         //媒体头
    DWORD           dwHeaderLen;     //媒体头长度
    BYTE            byRes1[84];
}NET_PSTREAM_PUSH_PARAM, *LPNET_PSTREAM_PUSH_PARAM;

NET_UTILS_API  LONG CALLBACK  NET_PSTREAM_StartPush(LPNET_PSTREAM_PUSH_PARAM pParam);

typedef struct tagNET_PSTREAM_PUSH_DATA
{
    void    *pData;   //要发送的数据缓冲区
    DWORD   dwDataLen;//要发送的数据长度
    BYTE    byRes[24];
}NET_PSTREAM_PUSH_DATA, *LPNET_PSTREAM_PUSH_DATA;

NET_UTILS_API  LONG CALLBACK  NET_PSTREAM_Push(LONG lHandle, LPNET_PSTREAM_PUSH_DATA pData);
NET_UTILS_API  BOOL CALLBACK  NET_PSTREAM_StopPush(LONG lHandle);


#endif