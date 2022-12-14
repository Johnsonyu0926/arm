#ifndef _SHARE_H__
#define	_SHARE_H__

#include <malloc.h>

#include "doorsbase.h"

#define GAP_INITIALIZER		0
#define memfile_BUF_SIZE	1024
#define GB_FLAG			"?GB2312?B?"
#define	CP_ACP			0
#define	CP_UTF7			65000
#define CP_UTF8			65001

#define MAX_ATTACH_COUNT	1024//最大的附件数目
#define FIELD_CONTENT_TYPE	"CONTENT-TYPE:"
#define FIELD_BOUNDARY		"BOUNDARY="
//#define FIELD_FILE_NAME		"FILENAME="
#define FIELD_FILE_NAME		"FILENAME="
#define FIELD_ENCODING_TYPE	"CONTENT-TRANSFER-ENCODING:"

#define BOUNDARY_FLAG		"\r\n"

#define	MIME_STR_TEXT_PLAIN					"TEXT/PLAIN"
#define MIME_STR_TEXT_RICHTEXT				"TEXT/RICHTEXT"
#define MIME_STR_TEXT_HTML					"TEXT/HTML"

#define MIME_STR_MULTIPART_MIXED			"MULTIPART/MIXED"
#define MIME_STR_MULTIPART_PARALLEL			"MULTIPART/PARALLEL"
#define MIME_STR_MULTIPART_DIGEST			"MULTIPART/DIGEST"
#define MIME_STR_MULTIPART_ALTERNATIVE		"MULTIPART/ALTERNATIVE"
#define MIME_STR_MULTIPART_REPORT			"MULTIPART/REPORT"
#define MIME_STR_MESSAGE_RFC822				"MESSAGE/RFC822"
#define MIME_STR_MESSAGE_PATIAL				"MESSAGE/PATIAL"
#define MIME_STR_MESSAGE_EXTERNAL_BODY		"MESSAGE/EXTERNAL-BODY"

#define MIME_STR_APPLICATION				"APPLICATION"
#define MIME_STR_APPLICATION_OCT_STREAM		"APPLICATION/OCTET-STREAM"
#define MIME_STR_APPLICATION_POSTSCRIPT		"APPLICATION/POSTSCRIPT"
#define MIME_STR_APPLICATION_X_MSDOWNLOAD	"APPLICATION/X-MSDOWNLOAD"
#define MIME_STR_APPLICATION_MSWORD			"APPLICATION/MSWORD"

#define MIME_STR_IMAGE						"IMAGE"
#define MIME_STR_IMAGE_JPEG					"IMAGE/JPEG"
#define MIME_STR_IMAGE_GIF					"IMAGE/GIF"

#define	MIME_TEXT_PLAIN					10
#define MIME_TEXT_RICHTEXT				11
#define MIME_TEXT_HTML					12

#define MIME_MULTIPART					20
#define MIME_MULTIPART_MIXED			21
#define MIME_MULTIPART_PARALLEL			22
#define MIME_MULTIPART_DIGEST			23
#define MIME_MULTIPART_ALTERNATIVE		24
#define MIME_MULTIPART_REPORT			25


#define MIME_MESSAGE_RFC822				30
#define MIME_MESSAGE_PATIAL				31
#define MIME_MESSAGE_EXTERNAL_BODY		32

#define MIME_APPLICATION				40
#define MIME_APPLICATION_OCT_STREAM		41
#define MIME_APPLICATION_POSTSCRIPT		42
#define MIME_APPLICATION_X_MSDOWNLOAD	43
#define MIME_APPLICATION_MSWORD			44

#define MIME_IMAGE						50
#define MIME_IMAGE_JPEG					51
#define MIME_IMAGE_GIF					52

#define MIME_FORMAT_UNKNOWN				-1

#define ENCODING_UNKNOWN				-1
#define ENCODING_BASE64					0
#define ENCODING_7BIT					1
#define ENCODING_QP						2

//////////////////////////////////////////////////////////////////////////////////////
typedef /* [public][helpstring] */ 
enum __MIDL___MIDL_itf_GAPMime_0000_0001
    {	DsCharsetNone	= 1,
	DsCharsetGB2312	= 2
    }	DsCharsetType;

enum __MIDL___MIDL_itf_GAPMime_0000_0002
    {	TYPE_CONTEXT_TYPE	= 0,
	TYPE_CHARSET	= TYPE_CONTEXT_TYPE + 1,
	TYPE_ENCODING	= TYPE_CHARSET + 1,
	TYPE_DISPOSITION	= TYPE_ENCODING + 1,
	TYPE_FILENAME	= TYPE_DISPOSITION + 1,
	TYPE_CONTENT	= TYPE_FILENAME + 1,
	TYPE_CONTENT_LENGTH	= TYPE_CONTENT + 1
    };

typedef /* [public][helpstring] */ 
enum __MIDL___MIDL_itf_GAPMime_0000_0003
    {	DsEncodingNone	= 1,
	DsEncoding8BIT	= 2,
	DsEncodingBase64	= 3,
	DsEncoding7BIT	= 4,
	DsEncodingQP	= 5
    }	DsEncodingMethod;

typedef /* [public][helpstring] */ 
enum __MIDL___MIDL_itf_GAPMime_0000_0005
    {	DsContextNone	= 1,
	DsContextTextPlain	= 2,
	DsContextTextHtml	= 3,
	DsContextAppOct	= 4,
	DsContextAppMsWord	= 5
    }	DsContextType;

typedef /* [public][helpstring] */ 
enum __MIDL___MIDL_itf_GAPMime_0000_0006
    {	DsDispNone	= 1,
	DsDispAttachment	= 2
    }	DsDisposition;

typedef /* [public][public][helpstring] */ 
enum __MIDL___MIDL_itf_GAPAudit_0000_0001
    {	DsAuditSucceeded	= 1,
	DsAuditFailed	= 2,
	DsAuditFailSubject = 3,
	DsAuditFailBody = 4,
	DsAuditFailAttachment = 5
    }	DsAuditStatus;

////////////////////////////////////////////////////////////////////////////////////
typedef struct BodyPart
{
	DsEncodingMethod m_nEncoding;	//编码方式
	DsContextType m_nContentType;	//上下文类型
	int nPart;
	int m_nBodyLen;	//内容长度（信体）
	char *m_pBody; //信体
	int m_nDisp;		//部署方式，是否为附件
	char *m_pAttachFileName;//附件文件名
	struct BodyPart *next;
	BodyPart()
	{
		m_pBody = NULL;
		nPart=0;
		m_pAttachFileName = NULL;
	}
} *Gap_BodyPart_t;

#endif
