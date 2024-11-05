#ifndef _ECOM_STREAMINTERFACE_H_ 
#define _ECOM_STREAMINTERFACE_H_

#include "HCEBase.h"

typedef struct tagNET_ESTREAM_ABILITY_CFG
{
    HPR_UINT32 dwBitStreamCount;    //(1)ISUP为预览回放总共支持的路数，默认256。OTAP为预览回放语音对讲总共支持的路数，默认256
    HPR_UINT32 dwVoiceTalkCount;    //ISUP语音对讲支持的路数，默认1
}NET_ESTREAM_ABILITY_CFG, *LPNET_ESTREAM_ABILITY_CFG;

typedef struct tagNET_OTAP_STREAM_RUNTIME_INFO
{
    HPR_UINT32	 dwType;                                   ///< 消息类型\c EZDEVSDK_STREAM_RUNTIME_INFO_E
    void         *pData;                                   ///< 指向具体type类型的数据
    HPR_UINT32   dwLen;                                    ///< data数据长度
    void         *pUserData;
    HPR_UINT8    byRes[32];
} NET_OTAP_STREAM_RUNTIME_INFO, *LPNET_OTAP_STREAM_RUNTIME_INFO;

typedef struct tagNET_OTAP_STREAM_MSG_TO_DEV
{
    HPR_UINT32	 dwType;                                   //[out]消息类型,参考结构体：NET_OTAP_STREAM_MSG_TO_DEV_TYPE_E
    HPR_UINT32   dwLen;                                    //[out]指向消息内容的长度
    void         *pData;                                   //[out]指向消息内容的指针
    void         *pUserData;                               //[out]用户数据
    HPR_INT32    iHandle;                                  //[out]对应NET_EBASE_CreateIOTClient客户端创建接口返回的句柄
    HPR_UINT8    byRes[28];
} NET_OTAP_STREAM_MSG_TO_DEV, *LPNET_OTAP_STREAM_MSG_TO_DEV;

typedef enum tagNET_OTAP_STREAM_DATA_TYPE_ENUM
{
    ENUM_OTAP_STREAM_DATA_TYPE_EXCEPTION = 0, //链路或码流异常，pData为4字节HPR_UINT32，表示错误码
    ENUM_OTAP_STREAM_DATA_TYPE_STREAM = 1, //码流数据,语音对讲支持
}NET_OTAP_STREAM_DATA_TYPE_ENUM;

typedef struct tagNET_OTAP_RECV_STREAM_DATA
{
    HPR_UINT8       byDataType;     //见NET_OTAP_STREAM_DATA_TYPE_ENUM
    HPR_UINT8       byRes1[7];      //保留字节（保证8字节对齐）
    HPR_UINT8*      pData;          //数据指针
    void*           pUserData;
    HPR_UINT32      dwDataLen;      //数据长度
    HPR_UINT32      dwSid;          ///< 码流会话ID，发流时作为句柄传入
    HPR_INT32       iHandle;        //[out]对应NET_EBASE_CreateIOTClient客户端创建接口返回的句柄
    HPR_UINT8       byRes[24];      //保留
} NET_OTAP_RECV_STREAM_DATA;


typedef enum tagNET_OTAP_STREAM_EXCEPTION_ENUM//码流链路异常类型
{
    ENUM_HEARTBEAT_TIMEOUT_EXCEPTION = 0, //心跳超时
}NET_OTAP_STREAM_EXCEPTION_ENUM;

typedef struct tagNET_OTAP_STREAM_EXCEPTION
{
    HPR_UINT8       byExceptionType;//<NET_OTAP_STREAM_EXCEPTION_ENUM
    HPR_UINT8       byRes1[3];      //保留字节（保证8字节对齐）
    HPR_UINT32      dwSid;          ///< 码流会话ID，发流时作为句柄传入
    void*           pUserData;
    HPR_UINT8       byRes2[28];      //保留
}NET_OTAP_STREAM_EXCEPTION;

typedef HPR_BOOL(CALLBACK *StreamDataCallback)(NET_OTAP_RECV_STREAM_DATA *pStreamData);
typedef HPR_INT32(CALLBACK *RecvMsgCB)(NET_OTAP_STREAM_MSG_TO_DEV *pMsg);              ///< 消息回调接口，不可以阻塞，必填，返回不为0表示操作失败
typedef HPR_BOOL(CALLBACK *ExceptionCallback)(NET_OTAP_STREAM_EXCEPTION *pStreamData);

typedef struct tagNET_OTAP_STREAM_CALLBACK
{
    RecvMsgCB fnRecvMsgCB;
    StreamDataCallback fnStreamDataCB;
    ExceptionCallback fnExceptionCB;//异常回调
    void         *pUserData;
    HPR_UINT8    byRes[28];
} NET_OTAP_STREAM_CALLBACK;

typedef struct tagNET_OTAP_STREAM_INIT_INFO
{
    NET_OTAP_STREAM_CALLBACK struCallBack;                ///< 回调函数
    HPR_UINT8    byRes[32];
} NET_OTAP_STREAM_INIT_INFO, *LPNET_OTAP_STREAM_INIT_INFO;

typedef struct tagNET_OTAP_STREAM_INIT_PARAM
{
    LPNET_ESTREAM_ABILITY_CFG   pEstreamAbility;    //能力，可为NULL
    LPNET_OTAP_STREAM_INIT_INFO pStreamInitInfo;    //OTAP取流时有效，必填
    HPR_UINT8    byRes[32];
}NET_OTAP_STREAM_INIT_PARAM, *LPNET_OTAP_STREAM_INIT_PARAM;

typedef enum enumNET_STREAM_LOCAL_CFG_TYPE
{
    STREAM_LOCAL_CFG_TYPE_UNKNOW = -1,                   //未知类型
    STREAM_LOCAL_CFG_TYPE_CLIENT_CERT_PATH = 0,          //客户端证书路径，对应pParam为字符串，最大长度128字节，末尾以\0结尾
    STREAM_LOCAL_CFG_TYPE_CLIENT_PRIKEY_PATH = 1,        //客户端私钥路径, 对应pParam为字符串，最大长度128字节，末尾以\0结尾
    STREAM_LOCAL_CFG_TYPE_SERVER_CACERT_PATH = 2         //服务端根证书路径, 对应pParam为字符串，最大长度128字节，末尾以\0结尾
}NET_STREAM_LOCAL_CFG_TYPE;

/************************************************************************/
/*                            本地接口                                  */
/************************************************************************/

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_ESTREAM_Init(LPNET_EBASE_ABILITY_CFG lpStruAbility)
*  @brief  初始化库
*  @param (in)	LPNET_EBASE_ABILITY_CFG lpStruAbility
*  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL   CALLBACK NET_ESTREAM_Init(LPNET_ESTREAM_ABILITY_CFG lpStruAbility);
NET_COM_API HPR_BOOL   CALLBACK NET_OTAP_ESTREAM_Init(LPNET_OTAP_STREAM_INIT_PARAM lpStreamInitParam);
/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_ESTREAM_Fini()
*  @brief 返初始化库
*  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL   CALLBACK NET_ESTREAM_Fini();
/** @fn	NET_COM_API HPR_UINT32 CALLBACK NET_EBASE_GetBuildVersion()
*  @brief 获取版本号
*  @return	HPR_UINT32 版本号
*/
NET_COM_API HPR_UINT32 CALLBACK NET_ESTREAM_GetBuildVersion();
/** @fn	NET_COM_API HPR_UINT32 CALLBACK NET_ESTREAM_SetLocalCfg(HPR_UINT32 dwType, HPR_VOIDPTR pParam)
*  @brief
*  @param (in)	HPR_UINT32 dwType 参见枚举NET_STREAM_LOCAL_CFG_TYPE
*  @param (in)	HPR_VOIDPTR pParam
*  @return	NET_COM_API HPR_UINT32 CALLBACK
*/
NET_COM_API HPR_BOOL CALLBACK NET_ESTREAM_SetLocalCfg(HPR_UINT32 dwType, HPR_VOIDPTR pParam);

/************************************************************************/
/*                            语音对讲接口                              */
/************************************************************************/
typedef enum enumEVoiceTalkDataType
{
    TYPE_UNKNOW = -1,       //未知类型,           ClassType(pData) = NULL
    TYPE_SIGNAL = 0,        //语音对讲信令        ClassType(pData) = LPNET_SIGNAL_VOICE_TALK_DATA
    TYPE_STREAM = 1,        //语音对讲流          ClassType(pData) = LPNET_ESTREAM_VOICE_TALK_DATA
}EVoiceTalkDataType;

typedef enum enumEVoiceTalkSignal
{
    STATUS_UNKNOW = -1,                //未知类型,           
    STATUS_START = 0,                  //语音对讲开始     
    STATUS_STOP  = 1,                  //语音对讲结束 
    STATUS_KEEPLIVE_FAIL = 2,          //语音对讲保活失败
}EVoiceTalkSignal;

typedef struct tagNET_SIGNAL_VOICE_TALK_DATA
{
    HPR_INT32                   iSessionID;                 //会话ID,Base模块开启语音对讲信令中服务端返回
    HPR_UINT32                  dwVoiceTalkSignal;          //语音对讲信令
    HPR_UINT32                  dwStatus;                   //信令交互状态
    HPR_UINT32                  dwMediaInfoLen;             
    HPR_VOIDPTR                 pMediaInfo;                 //在语音对讲开始前，客户端和服务器需要相互告知对方媒体能力,xml格式，暂支持2048个字节，后续可扩充,仅在语音对讲信令TYPE_SIGNAL中STATUS_START有效
    HPR_UINT8                   byRes[128];
}NET_SIGNAL_VOICE_TALK_DATA, *LPNET_SIGNAL_VOICE_TALK_DATA;

typedef struct tagNET_ESTREAM_VOICE_TALK_DATA
{
    HPR_INT32                   iSessionID;               //会话ID,Base模块开启语音对讲信令中服务端返回，每一路对讲sessionid唯一
    HPR_UINT8 *                 pVoiceTalkData;           //语音对讲数据
    HPR_UINT32                  dwDataLen;                //语音对讲数据长度
    HPR_UINT32                  AudioDataType;            //音频数据包类型
    HPR_UINT32                  dwSendLen;                //[out]语音对讲实际发送数据长度
    HPR_UINT8                   byRes[124];
}NET_ESTREAM_VOICE_TALK_DATA, *LPNET_ESTREAM_VOICE_TALK_DATA;

/*当dwDataType为语音对讲信令类型，输出参数pOutBuffer，输入参数pInBuffer对应结构体均为NET_SIGNAL_VOICE_TALK_DATA，当dwDataType为语音对讲流类型，输出参数pOutBuffer对应结构体为NET_ESTREAM_VOICE_TALK_DATA，
  输入参数pInBuffer为NULL，不用返回数据 */
typedef void (CALLBACK *EVoiceTalkFunc)(HPR_INT32 dwHandle, EVoiceTalkDataType dwDataType, HPR_VOIDPTR pOutBuffer, HPR_INT32 dwOutLen, HPR_VOIDPTR pInBuffer, HPR_VOIDPTR pUserData);
typedef struct tagNET_ESTREAM_VOICE_TALK_INFO
{
    HPR_UINT32                  dwSize;                                      //[in]结构体长度
    HPR_INT32                   iSessionID;                                  //会话ID,Base模块开启语音对讲信令中服务端返回
    HPR_UINT8                   byToken[MAX_VOICETALK_TOKEN_LEN];            //Token，Base模块开启语音对讲信令中服务端返回
    HPR_UINT8                   byTTSAdd[MAX_ADD_LEN];                       //[in]流媒体服务端地址，支持IPv4，IPv6，域名，Base模块开启语音对讲信令中服务端返回
    EVoiceTalkFunc              fnCB;                                        //[in]接收到的数据回调
    HPR_VOIDPTR                 pUserData;                                   //[in]用户数据
    HPR_UINT16                  wTTSPort;                                    //[in]流媒体服务端端口，Base模块开启语音对讲信令中服务端返回
    HPR_UINT8                   byEncodeType;                                //语音对讲编码格式类型0- G722_1，1- G711U，2- G711A，3- G726，4- AAC，5- MP2L2，6- PCM, 7-MP3, 8-G723, 9-MP1L2, 99-RAW
    HPR_UINT8                   bySupport;                                   //bit0(bySupport&0x01==0x01):是否链路加密
    HPR_UINT8                   byBroadCast;                               //语音广播标识, 设备接收到本标识为1后不进行音频采集发送给对端
    HPR_UINT8                   byBroadLevel;                              //语音广播优先级标识,0~15优先级从低到高,当存在byBroadcast为1时,0标识最低优先级。当存在byBroadcast为0时，本节点无意义为保留字节
    HPR_UINT8                   byBroadVolume;                             //语音广播音量,0~15音量从低到高,当存在byBroadcast为1时,0标识最低音量。当存在byBroadcast为0时，本节点无意义为保留字节
    HPR_UINT8                   byProtocolType;                            //协议类型  0-ISUP
    HPR_UINT8                   byRes[124];                                //预留字段  

}NET_ESTREAM_VOICE_TALK_INFO, *LPNET_ESTREAM_VOICE_TALK_INFO;

/** @fn	NET_COM_API HPR_INT32 CALLBACK NET_ESTREAM_VOICETALK_CreateClient(LPNET_ESTREAM_VOICE_TALK_INFO lpVoiceTalkInfo)
 *  @brief  创建语音对讲客户端
 *  @param (in)	LPNET_ESTREAM_VOICE_TALK_INFO lpVoiceTalkInfo 创建EHome语音对讲客户端所需参数
 *  @return	HPR_INT32  -1表示失败，其他值表示句柄
 */
NET_COM_API HPR_INT32 CALLBACK NET_ESTREAM_VOICETALK_CreateClient(LPNET_ESTREAM_VOICE_TALK_INFO lpVoiceTalkInfo);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_ESTREAM_VOICETALK_DestroyClient(HPR_INT32 dwHandle)
 *  @brief 销毁语音对讲客户端
 *  @param (in)	HPR_INT32 dwHandle  NET_ESTREAM_VOICETALK_CreateClient返回值
 *  @return HPR_BOOL  HPR_TRUE/HPR_FALSE 成功/失败
 */
NET_COM_API HPR_BOOL CALLBACK NET_ESTREAM_VOICETALK_DestroyClient(HPR_INT32 iHandle);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_ESTREAM_SendVoiceTalkData(HPR_INT32 dwHandle, LPNET_ESTREAM_VOICE_TALK_DATA lpVoicTalkData)
 *  @brief 发送语音对讲数据
 *  @param (in)	HPR_INT32 dwHandle  NET_ESTREAM_VOICETALK_CreateClient返回值
 *  @param (in)	LPNET_ESTREAM_VOICE_TALK_DATA lpVoicTalkData 语音对讲数据
 *  @return HPR_BOOL  HPR_TRUE/HPR_FALSE 成功/失败
 */
NET_COM_API HPR_BOOL CALLBACK NET_ESTREAM_SendVoiceTalkData(HPR_INT32 iHandle, LPNET_ESTREAM_VOICE_TALK_DATA lpVoicTalkData);

/************************************************************************/
/*                            码流接口                                  */
/************************************************************************/
#define MAX_STREAM_SESSIONID_LEN 64       //sessionID的长度
#define MAX_STREAM_HEART_ABILITY_LEN 64   //心跳能力结构体的长度
#define MAX_STREAM_HEADER_LEN    40   //码流头长度

typedef struct tagNET_ESTREAM_CLIENT_PARAM
{
    HPR_UINT8   byServerAdd[MAX_ADD_LEN/*256*/];            //[in]服务端地址，支持IPv4，IPv6，域名
    HPR_UINT16  wServerPort;                                //[in]服务端端口
    HPR_UINT8   byStreamType;                               //[in]码流类型 0-预览，1-回放 2-语音对讲
    HPR_UINT8   byLinkMode;                                 //0：TCP，1：UDP，8-NPQ;
    HPR_UINT8   bySessionID[MAX_STREAM_SESSIONID_LEN/*64*/];//[in]服务端发预览请求时，会生成一个sessionID给设备，设备需要将这个值返回给服务端
    HPR_UINT8   byStreamHeader[MAX_STREAM_HEADER_LEN/*40*/];//[in]码流头
    HPR_UINT16  wSDPLength;                                 //SDP信息长度（byLinkMode为8时有效）
    char*       pSDPBuffer;                                 //SDP信息（byLinkMode为8时有效）
    HPR_UINT8   bySupport;                                  //bit0(bySupport&0x01==0x01):是否链路加密,TCP通过TLS传输，UDP(包括NPQ)使用DTLS传输
}NET_ESTREAM_CLIENT_PARAM, *LPNET_ESTREAM_CLIENT_PARAM;

typedef struct tagNET_ESTREAM_STREAM_PARAM
{
    HPR_UINT16  wTimeSpan;                                  //[in]相对时间，时间戳，根据协议预留，目前不用填写
    HPR_UINT8   byFrameType;                                //[in]帧类型,I,B,P帧，根据协议预留，目前不用填写
    char*       pDataBuff;                                  //[in]码流数据，如果是RTP包，要求是一个完整的RTP包
    HPR_UINT32  dwDataLen;                                  //[in]码流数据长度
    HPR_UINT8   byStreamEnd;                                //[in]是否是最后一包数据，如果是最后一包数据，这里会关闭链接
    HPR_UINT8   byType;                                     //[in]码流类型0-码流数据(默认) 15-HLS索引数据
    HPR_UINT8   byPackPos;
    HPR_UINT8   byRes[1];
}NET_ESTREAM_STREAM_PARAM, *LPNET_ESTREAM_STREAM_PARAM;

typedef enum enumEStreamCallBackType
{
    TYPE_CALLBACK_UNKNOW = -1,                 //未知类型,           ClassType(pData) = NULL
    TYPE_EXCEPTION = 0,                        //异常回调            ClassType(pData) = HPR_UINT32,pOutBuffer为错误码,可能心跳超时，连接中断，接收数据错误
    TYPE_NPQ_RTCP = 1,                         //NPQ RTCP数据回调,回调中pOutBuffer为NET_ESTREAM_CALLBACK_DATA，
                                                                    //NET_ESTREAM_CALLBACK_DATA.pBuffer为接收到的NPQ数据中的RTCP包,
                                                                    //NET_ESTREAM_CALLBACK_DATA.dwBufferLen为接收到的NPQ数据中的RTCP包长度
}EStreamCallBackType;

typedef struct tagNET_ESTREAM_CALLBACK_DATA
{
    HPR_UINT32 dwBufferLen;
    char* pBuffer;
}NET_ESTREAM_CALLBACK_DATA, *LPNET_ESTREAM_CALLBACK_DATA;

typedef void (CALLBACK *EStreamCallBack)(HPR_INT32 iHandle, EStreamCallBackType enumDataType, HPR_VOIDPTR pOutBuffer, HPR_VOIDPTR pInBuffer, HPR_VOIDPTR pUserData);
typedef struct tagNET_ESTREAM_CALLBACK_PARAM
{
    EStreamCallBack             fnCB;                                        //[in]接收到的数据回调
    HPR_VOIDPTR                 pUserData;                                   //[in]用户数据
    HPR_UINT8                   byRes[128];
}NET_ESTREAM_CALLBACK_PARAM, *LPNET_ESTREAM_CALLBACK_PARAM;

typedef enum enumNET_ESTREAM_BSCON_TYPE
{
    ESTREAM_BSCON_TYPE_UNKNOW       = -1,                   //未知类型
    ESTREAM_BSCON_TYPE_SENDTIMEOUT  = 0,                    //发送超时时间，此时对应的lpParam为HPR_UINT32*类型，size = sizeof(HPR_UINT32)
    ESTREAM_BSCON_TYPE_PLAYBACKCALLBACK = 1,                //设置回放相关回调函数，对应结构体为NET_ESTREAM_CALLBACK_PARAM，
    ESTREAM_BSCON_TYPE_PLAYBACKHEARTABILITY = 2,            //设置支持回放心跳能力，此能力与回放暂停功能一起使用，依赖ESTREAM_BSCON_TYPE_PLAYBACKCALLBACK的设置，结构体为NYLL；
    ESTREAM_BSCON_TYPE_NPQ_CALLBACK = 3,                    //设置NPQ数据回调函数，对应结构体为NET_ESTREAM_CALLBACK_PARAM
}NET_ESTREAM_BSCON_TYPE;

typedef enum {
    NET_OTAP_PREVIEW_ON_START_SEND_STREAM = 0,                  //开始预览取流, 参考结构体：NET_OTAP_PREVIEW_START_SEND_STREAM
    NET_OTAP_PREVIEW_ON_STOP_SEND_STREAM,                       //停止预览取流, 参考结构体：NET_OTAP_PREVIEW_STOP_SEND_STREAM
    NET_OTAP_PREVIEW_ON_GET_STREAM_INFO,                        //获取预览流信息，在开始预览取流前获取，参考结构体：NET_OTAP_PREVIEW_GET_STREAM_INFO

    NET_OTAP_VOICE_ON_START_SEND_STREAM = 200,                        //开始语音对讲取流, 参考结构体：NET_OTAP_VOICE_START_SEND_STREAM 
    NET_OTAP_VOICE_ON_STOP_SEND_STREAM,                         //停止语音对讲取流, 参考结构体：NET_OTAP_VOICE_STOP_SEND_STREAM 
    NET_OTAP_VOICE_ON_GET_STREAM_INFO,                    //获取语音对讲流信息，在开始语音对讲取流前获取，参考结构体：NET_OTAP_VOICE_GET_STREAM_INFO


    NET_OTAP_PLAYBACK_ON_START_SEND_STREAM = 400,               //开始回放取流, 参考结构体： NET_OTAP_START_SEND_STREAM 
    NET_OTAP_PLAYBACK_ON_STOP_SEND_STREAM,                      //停止回放取流, 参考结构体： NET_OTAP_STOP_SEND_STREAM 
    NET_OTAP_PLAYBACK_ON_START_DOWNLOAD_STREAM,                 ///<开始录像下载 参考结构体：NET_OTAP_START_SEND_STREAM
    NET_OTAP_PLAYBACK_ON_STOP_DOWNLOAD_STREAM,                  ///<停止录像下载 参考结构体：NET_OTAP_STOP_SEND_STREAM
    NET_OTAP_PLAYBACK_ON_GET_STREAM_INFO                        //获取回放流信息，需要在开始回放取流前获取，参考结构体： NET_OTAP_PLAYBACK_GET_STREAM_INFO

    //NET_OTAP_PLAYBACK_ON_PAUSE_SEND_STREAM,                 ///< 暂停取流, 信息体\c NET_OTAP_PAUSE_SEND_STREAM
    //NET_OTAP_PLAYBACK_ON_RESUME_SEND_STREAM,                 ///< 恢复推送录像码流, NET_OTAP_PAUSE_SEND_STREAM
    //NET_OTAP_PLAYBACK_ON_SEEK_TIME_STREAM,                 ///< 录像回放立即跳转到时间点  NET_OTAP_SEEK_TIME_STREAM
    //NET_OTAP_PLAYBACK_ON_CONTINUE_SEND_STREAM,                 ///< 流媒体录像回放跳转 前一段发送完成后再发送新请求数据 NET_OTAP_CONTINUE_PLAYBACK
    //NET_OTAP_PLAYBACK_ON_MODIFY_SPEED_PLAY_STREAM,                 ///< 流媒体设备录像倍速回放 NET_OTAP_MODIFY_PLAYBACK_SPEED
    //NET_OTAP_PLAYBACK_ON_SEEK_SEGS_STREAM,                     ///<录像回放跳转到拖动录像段 NET_OTAP_SEGS_PLAYBACK


} NET_OTAP_STREAM_MSG_TO_DEV_TYPE_E;

/**
* \brief \c NET_OTAP_PREVIEW_ON_START_SEND_STREAM
*/
typedef struct tagNET_OTAP_PREVIEW_START_SEND_STREAM
{
    HPR_UINT32 dwChannel;                                        ///< 通道
    HPR_UINT32 dwStreamIndex;                                   ///< 码流索引
    HPR_UINT32 dwSid;                                            ///< 码流会话ID，发流时作为句柄传入
    HPR_UINT32 dwPreTime;                                       ///< 取预录流长度，单位：秒，0表示不取预录流
    HPR_UINT8  bySessionID[MAX_STREAM_SESSIONID_LEN/*64*/];        //[out]NET_OTAP_PREVIEW_GET_STREAM_INFO时用户填入的sessionID
    HPR_UINT8  byRes[32];
}  NET_OTAP_START_SEND_STREAM, *LPNET_OTAP_PREVIEW_START_SEND_STREAM;

/**
* \brief \c NET_OTAP_PREVIEW_ON_STOP_SEND_STREAM
*/
typedef struct tagNET_OTAP_PREVIEW_STOP_SEND_STREAM
{
    HPR_UINT32 dwChannel;                                        ///< 通道
    HPR_UINT32 dwStreamIndex;                                   ///< 码流索引
    HPR_UINT32 dwSid;                                            ///< 码流会话ID，会话发送方式有效，有效值大于0.
    HPR_UINT8  byRes[32];
}NET_OTAP_STOP_SEND_STREAM, *LPNET_OTAP_PREVIEW_STOP_SEND_STREAM;

typedef struct tagNET_OTAP_PAUSE_SEND_STREAM
{
    HPR_UINT32 dwChannel;                                        ///< 通道
    HPR_UINT32 dwStreamIndex;                                   ///< 码流索引
    HPR_UINT32 dwSid;                                            ///< 码流会话ID，会话发送方式有效，有效值大于0.
    HPR_UINT8  byRes[32];
}NET_OTAP_PAUSE_SEND_STREAM, *LPNET_OTAP_PAUSE_SEND_STREAM;

typedef struct tagNET_OTAP_PREVIEW_GET_STREAM_INFO
{
    HPR_UINT32  dwChannel;                                          //[out] 通道
    HPR_UINT8   byStreamType;                                       //[out] 码流类型，1:主码流,2:子码流
    HPR_UINT8   byDataType;                                         //[out] 数据类型，按位表示，0x01-视频，0x02-音频，0x04-结构化数据，默认传输视频和音频数据（0x00）
    HPR_UINT8   byRes1[2];
    HPR_UINT8   bySessionID[MAX_STREAM_SESSIONID_LEN/*64*/];        //[in]设备sessionID
    HPR_UINT8   byStreamHeader[MAX_STREAM_HEADER_LEN/*40*/];        //[in]码流头
    HPR_UINT8   byRes[64];
}NET_OTAP_PREVIEW_GET_STREAM_INFO, *LPNET_OTAP_PREVIEW_GET_STREAM_INFO;

typedef struct tagOTAP_TIME
{
    HPR_UINT16    wYear;      //年
    HPR_UINT8     byMonth;    //月
    HPR_UINT8     byDay;      //日
    HPR_UINT8     byHour;     //时
    HPR_UINT8     byMinute;   //分
    HPR_UINT8     bySecond;   //秒
    HPR_UINT8     byTimeZone;  //1~12表示东1至东12区,13~24表示西1至西12区
    HPR_UINT16    wMSecond;   //毫秒
    HPR_UINT8     byRes[6];
}OTAP_TIME;

typedef struct tagOTAP_PLAYBACK_TIME
{
    OTAP_TIME  struStartTime;  // 按时间录像取流的开始时间
    OTAP_TIME  struStopTime;   // 按时间录像取流的结束时间
}OTAP_PLAYBACK_TIME, *LPOTAP_PLAYBACK_TIME;


typedef struct tagNET_OTAP_PLAYBACK_GET_STREAM_INFO
{
    HPR_UINT32  dwChannel;                                          //[out] 通道
    HPR_UINT8   byStreamType;                                       //[out] 码流类型，1:主码流,2:子码流
    HPR_UINT8   byDataType;                                         //[out] 数据类型，按位表示，0x01-视频，0x02-音频，0x04-结构化数据，默认传输视频和音频数据（0x00）
    HPR_UINT8   byRes1[2];
    HPR_UINT8   bySessionID[MAX_STREAM_SESSIONID_LEN/*64*/];        //[in]设备sessionID
    HPR_UINT8   byStreamHeader[MAX_STREAM_HEADER_LEN/*40*/];        //[in]码流头
    OTAP_PLAYBACK_TIME struTime[20];                                //[out]
    HPR_UINT8   byRes[64];
}NET_OTAP_PLAYBACK_GET_STREAM_INFO, *LPNET_OTAP_PLAYBACK_GET_STREAM_INFO;

typedef struct tagNET_OTAP_SEEK_TIME_STREAM
{
    HPR_UINT32 dwChannel;                                        ///< 通道
    HPR_UINT32 dwStreamIndex;                                   ///< 码流索引
    HPR_UINT32 dwSid;                                            ///< 码流会话ID，会话发送方式有效，有效值大于0.
    OTAP_TIME  struTime;                                  ///<跳转时间点
    HPR_UINT8  byRes[32];
}NET_OTAP_SEEK_TIME_STREAM, *LPNET_OTAP_SEEK_TIME_STREAM;

typedef struct tagNET_OTAP_MODIFY_PLAY_SPEED
{
    HPR_UINT32 dwChannel;                                        ///< 通道
    HPR_UINT32 dwStreamIndex;                                   ///< 码流索引
    HPR_UINT32 dwSid;                                            ///< 码流会话ID，会话发送方式有效，有效值大于0.
    HPR_UINT8 byPlaybackType;                           ///<播放模式, 设备能力决定，1:全帧, 2：抽帧
    HPR_UINT8 bySpeed;                                          ///<播放速度  1: 正常速度, 2：2倍, 3：1/2倍, 4：4倍, 5：1/4倍, 6：8倍, 7：1/8倍, 8：16倍, 9：1/16倍
    HPR_UINT8  byRes[128];
}NET_OTAP_MODIFY_PLAY_SPEED, *LPNET_OTAP_MODIFY_PLAY_SPEED;

typedef struct tagNET_OTAP_SEGS_PLAYBACK
{
    HPR_UINT32 dwChannel;                                        ///< 通道
    HPR_UINT32 dwStreamIndex;                                   ///< 码流索引
    HPR_UINT32 dwSid;                                            ///< 码流会话ID，会话发送方式有效，有效值大于0.
    HPR_UINT32 dwRecordNum;   //请求录像取流的录像个数。参见struRecordStreamingTime
    OTAP_PLAYBACK_TIME struRecordStreamingTime[20];                  ///<录像跳转信息列表
    HPR_UINT8  byRes[128];
}NET_OTAP_SEGS_PLAYBACK, *LPNET_OTAP_SEGS_PLAYBACK;

typedef struct tagNET_OTAP_CONTINUE_PLAYBACK
{
    HPR_UINT32 dwChannel;                                        ///< 通道
    HPR_UINT32 dwStreamIndex;                                   ///< 码流索引
    HPR_UINT32 dwSid;                                            ///< 码流会话ID，会话发送方式有效，有效值大于0.
    HPR_UINT32 dwRecordNum;   //请求录像取流的录像个数。参见struRecordStreamingTime
    OTAP_PLAYBACK_TIME struRecordStreamingTime[20];                  ///<录像跳转信息列表
    HPR_UINT8  byRes[128];
}NET_OTAP_CONTINUE_PLAYBACK, *LPNET_OTAP_CONTINUE_PLAYBACK;

typedef struct tagNET_OTAP_MODIFY_PLAYBACK_SPEED
{
    HPR_UINT32 dwChannel;                                        ///< 通道
    HPR_UINT32 dwStreamIndex;                                   ///< 码流索引
    HPR_UINT32 dwSid;                                            ///< 码流会话ID，会话发送方式有效，有效值大于0.
    HPR_UINT8 byPlaybackType;                           ///<播放模式, 设备能力决定，1:全帧, 2：抽帧
    HPR_UINT8 bySpeed;                                          ///<播放速度  1: 正常速度, 2：2倍, 3：1/2倍, 4：4倍, 5：1/4倍, 6：8倍, 7：1/8倍, 8：16倍, 9：1/16倍
    HPR_UINT8  byRes[128];
}NET_OTAP_MODIFY_PLAYBACK_SPEED, *LPNET_OTAP_MODIFY_PLAYBACK_SPEED;

typedef struct tagNET_OTAP_DOWNLOAD_STREAM
{
    HPR_UINT32 dwChannel;                                        ///< 通道
    HPR_UINT32 dwStreamIndex;                                   ///< 码流索引
    HPR_UINT32 dwSid;                                            ///< 码流会话ID，会话发送方式有效，有效值大于0.
    OTAP_PLAYBACK_TIME strucTimesSpan;          ///下载时间
    HPR_UINT8  byRes[32];
}NET_OTAP_DOWNLOAD_STREAM, *LPNET_OTAP_DOWNLOAD_STREAM;
//-------------------------------------------------------------------------------

typedef struct tagNET_OTAP_VOICE_START_SEND_STREAM
{
    HPR_UINT32 dwChannel;                                       //[out]通道
    HPR_UINT32 dwStreamIndex;                                   //[out]码流索引
    HPR_UINT32 dwSid;                                           //[out]码流会话ID，发流时作为句柄传入
    HPR_UINT8  byRes1[4];
    HPR_UINT8  bySessionID[MAX_STREAM_SESSIONID_LEN/*64*/];     //[out]NET_OTAP_PREVIEW_GET_STREAM_INFO时用户填入的sessionID
    HPR_UINT8  byRes[32];
} NET_OTAP_VOICE_START_SEND_STREAM;

typedef struct tagNET_OTAP_VOICE_STOP_SEND_STREAM
{
    HPR_UINT32 dwChannel;                                        ///< 通道
    HPR_UINT32 dwStreamIndex;                                   ///< 码流索引
    HPR_UINT32 dwSid;                                            ///< 码流会话ID，会话发送方式有效，有效值大于0.
    HPR_UINT8  byRes[32];
}NET_OTAP_VOICE_STOP_SEND_STREAM;

typedef struct tagNET_OTAP_VOICE_GET_STREAM_INFO
{
    HPR_UINT32  dwChannel;                                          //[out]通道
    HPR_UINT16  wPriority;                                          //[out]对讲优先级取值范围为[1,16],优先级1最高，2次之，以此类推，16最低
    HPR_UINT16  wVolume;                                            //[out]音量以百分比的方式呈现,范围[0,100],若wVolume不在[0,100]范围，表示按设备默认音量
    HPR_UINT16  wTransmissionMode;                                   //[out]0-双向对讲 1-单向广播
    HPR_UINT16  wAudioType;                                         //[outin]音频类型0-G722_1，1-G711_MU，2-G711_A，3-G723，4-MP1L2，5-MP2L2，
                                                                               // 6-G726，7-AAC，8-G726A，9-G72616，10-G729，11-ADPCM，12-AMRNB，13-RAW8，14-RAW16，
                                                                               // 15 - MP2L232，16 - MP2L264，17 - AAC32，18 - AAC64，19 - OPUS8，20 - OPUS16，21 - OPUS48，
                                                                                // 22 - G729A，23 - G729B，24 - PCM，25 - MP3，26 - AC3，99 - RAW
    HPR_UINT32  dwAudioSamplingRate;                                //[outin]0-按设备默认采样率
    HPR_UINT8   bySessionID[MAX_STREAM_SESSIONID_LEN];              //[in]设备sessionID
    HPR_UINT8   byRes[64];
}NET_OTAP_VOICE_GET_STREAM_INFO;
/** @fn	NET_COM_API HPR_INT32 CALLBACK NET_ESTREAM_BITSTREAM_CreateClient(LPNET_ESTREAM_CLIENT_PARAM lpStruParam)
*  @brief 创建一路码流客户端
*  @param (in)	LPNET_ESTREAM_CLIENT_PARAM lpStruParam
*  @return	NET_COM_API HPR_INT32 CALLBACK 返回该路码流的句柄
*/
NET_COM_API HPR_INT32 CALLBACK NET_ESTREAM_BITSTREAM_CreateClient(LPNET_ESTREAM_CLIENT_PARAM lpStruParam);
/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_ESTREAM_BITSTREAM_DestroyClient(HPR_INT32 iHandle)
*  @brief  释放一路码流客户端
*  @param (in)	HPR_INT32 iHandle
*  @return	NET_COM_API HPR_BOOL CALLBACK HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL  CALLBACK NET_ESTREAM_BITSTREAM_DestroyClient(HPR_INT32 iHandle);
/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_ESTREAM_BITSTREAM_SendData(HPR_INT32 iHandle, const char *pDataBuff, HPR_UINT32 dwDataLen)
*  @brief  发送码流函数，lDwSendLen为0时表示发送已被阻塞，需要等待
*  @param (in)	HPR_INT32 iHandle 码流句柄，NET_ESTREAM_BITSTREAM_CreateClient返回值
*  @param (in)	const char * pDataBuff
*  @param (in)	HPR_UINT32 dwDataLen
*  @param (out) HPR_INT32* lDwSendLen  当返回HPR_TRUE时，lDwSendLen表示pDataBuff已发送的长度值，下次传入的lpStruParam.pDataBuff向后偏移lDwSendLen
*  @return	NET_COM_API HPR_BOOL CALLBACK
*/
NET_COM_API HPR_BOOL  CALLBACK NET_ESTREAM_BITSTREAM_SendData(HPR_INT32 iHandle, LPNET_ESTREAM_STREAM_PARAM lpStruParam, HPR_INT32* lDwSendLen);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_ESTREAM_BITSTREAM_SetParam(HPR_INT32 iHandle, HPR_UINT32 dwType, HPR_VOIDPTR lpParam, HPR_UINT32 dwSize)
 *  @brief
 *  @param (in)	HPR_INT32 iHandle    
 *  @param (in)	HPR_UINT32 dwType    待设置的参数类型，见NET_ESTREAM_BSCON_TYPE
 *  @param (in)	HPR_VOIDPTR lpParam  待设置的参数，见NET_ESTREAM_BSCON_TYPE
 *  @param (in)	HPR_UINT32 dwSize    lpParam所占的内存大小
 *  @return	NET_COM_API HPR_BOOL CALLBACK
 */
NET_COM_API HPR_BOOL  CALLBACK NET_ESTREAM_BITSTREAM_SetParam(HPR_INT32 iHandle, NET_ESTREAM_BSCON_TYPE enumType, HPR_VOIDPTR lpParam, HPR_UINT32 dwSize);

#endif //_ECOM_STREAMINTERFACE_H_
