/**	@NPQos.h
*	@note HangZhou Hikvision Technology Co., Ltd. All Right Reserved.
*	@brief NPQ库接口定义
*
*	@author		wangxinghe
*	@date		2016/11/14
*
*	@note
*
*	@warning 
*/
#ifndef _NPQOS_H_
#define _NPQOS_H_

#if defined(WIN32)
#if defined(NPQOS_EXPORTS)
#define NPQ_API extern "C" __declspec(dllexport)
#else
#define NPQ_API extern "C" __declspec(dllimport)
#endif
#else

#ifndef __stdcall
#define __stdcall
#endif

#ifndef NPQ_API
#define NPQ_API extern "C"
#endif

#endif

#include "IMuxDefine.h"
#include "NPQosAudioDec.h"
#define NPQ_OK              0x00000000  //成功
#define NPQERR_PARA         0x80000001  //参数错误
#define NPQERR_SYSTEM       0x80000002  //操作系统调用错误(包括资源申请失败或内部错误等））
#define NPQERR_GENRAL       0x80000003  //内部通用错误
#define NPQERR_PRECONDITION 0x80000004  //调用顺序错误
#define NPQERR_NOTSUPPORT   0x80000005  //功能不支持
#define NPQERR_DATAFORMAT   0x80000006  //rtp或者rtcp数据错误

/**	@enum	 NPQ_QOS_TYPE 
 *	@brief   Qos策略类型
 *	@note	 
 */
enum NPQ_QOS_TYPE
{
	QOS_TYPE_NACK = (1<<0),          //Nack		
	QOS_TYPE_FEC = (1<<1),			 //FEC
	QOS_TYPE_DEJITTER = (1<<2),		 //去抖动
	QOS_TYPE_BW = (1<<3),			 //拥塞控制
	QOS_TYPE_PLI = (1<<4),			 //PLI
	QOS_TYPE_SYNCHRONOUS = (1<<5),	 //音视频同步
	QOS_TYPE_ALL = 0xff,
};

/**	@enum	 NPQ_QOS_ROLE 
 *	@brief   Qos角色，分接收端和发送端
 *	@note	 
 */
enum NPQ_QOS_ROLE
{
	NPQ_QOS_RECEIVER,	//接收端
	NPQ_QOS_SENDER,		//发送端
};

/**	@enum	 NPQ_MAIN_TYPE 
 *	@brief   Qos主类型
 *	@note	 
 */
enum NPQ_MAIN_TYPE
{
	NPQ_MAIN_VEDIO,		//视频Qos
	NPQ_MAIN_AUDIO,		//音频Qos
	NPQ_MAIN_PRIVATE,	//私有数据Qos
    NPQ_MAIN_ALL = 0xFF,//所有类型
};

/**	@enum	 NPQ_DATA_TYPE 
 *	@brief   数据类型
 *	@note	 
 */
enum NPQ_DATA_TYPE
{
	NPQ_DATA_RTP_VIDEO,			//视频RTP数据
	NPQ_DATA_RTP_AUDIO,			//音频RTP数据
	NPQ_DATA_RTP_PRIVATE,		//私有包RTP数据
	NPQ_DATA_RTCP_VIDEO,		//视频RTCP
	NPQ_DATA_RTCP_AUDIO,		//音频RTCP
	NPQ_DATA_MULITIPLEX,	    //未知类型数据（外部不确定数据类型，由NPQ内部识别,需要结合sdp）
	NPQ_DATA_CMD,				//命令回调，对应NPQ_CMD_TYPE 需要外部做相应处理
    NPQ_DATA_MEDIA_INFO,        //媒体编码信息回调，对应NPQ_MEDIA_INFO 需要外部做相应处理
    NPQ_DATA_STATE,             //状态信息回调，对应NPQ_STAT
    NPQ_DATA_RTCP_PRIVATE,		//私有RTCP
    NPQ_DATA_HIKINFO		    //海康头
};


/**	@enum	 NPQ_MEDIA_VIDEO_ENCODE_TYPE 
 *	@brief   视频编码类型
 *	@note	 
 */
enum NPQ_MEDIA_VIDEO_ENCODE_TYPE
{
	NPQ_VIDEO_H264,
	NPQ_VIDEO_H265,
	NPQ_VIDEO_MPEG4,
    NPQ_VIDEO_SVAC,
	NPQ_VIDEO_UNKNOWN = 0xff,
};




/** @struct  NPQ_VIDEO_INFO
 *  @brief 视频编码信息。
 *  @note
*/
struct NPQ_VIDEO_INFO
{
    NPQ_MEDIA_VIDEO_ENCODE_TYPE enVideoCodeType;      //视频编码类型
    unsigned char               res[252];
};


/** @struct NPQ_AUDIO_INFO
 *  @brief 音频编码信息
 *  @note
*/
struct NPQ_AUDIO_INFO
{
    NPQ_AUDIO_ENCODE_TYPE        enAudioCodeType;     //音频编码类型
    unsigned int                 uSampleRate;         //采样率
    unsigned int                 uBitRate;            //编码码率
    unsigned char                res[244];             

};


/** @struct NPQ_MEDIA_INFO
 *  @brief NPQ媒体信息。
 *  @note
*/
struct NPQ_MEDIA_INFO
{
    int iVersion;                                     //结构体版本，用于以后兼容
    NPQ_MAIN_TYPE enMediaType;                        //媒体类型
    union MEDIA_INFO
    {
        NPQ_VIDEO_INFO struVideoInfo;                 //视频编码信息
        NPQ_AUDIO_INFO struAudioInfo;                 //音频编码信息
        unsigned char  res[256];
    }unMediaInfo;
};

/**	@enum	 NPQ_CMD_TYPE 
 *	@brief   命令类型
 *	@note	 
 */
enum NPQ_CMD_TYPE
{
	NPQ_CMD_ENCODE,			//音视频编码参数
	NPQ_CMD_FORCE_I_FRAME,	//强制I帧
};

/**	@struct	 NPQ_ECODE_PARAM 
 *	@brief   编码参数定义
 *	@note	 
 */
struct NPQ_ECODE_PARAM
{
	int iVersion;				//结构体版本，用于以后兼容
	int iMaxBitRate;			//最大码率   单位 bps，设置给编码器的值
	int iResolution;			//分辨率  按照网络SDK协议定义的索引值
    int iEstimatedBW;           //估计出来的总带宽 单位bps
	unsigned char res[240]; 
};

/**	@struct	 NPQ_CMD 
 *	@brief   Qos命令回调, 只适用于发送端
 *	@note	 
 */
struct NPQ_CMD
{
	int iVersion;					//结构体版本，用于以后兼容
	NPQ_CMD_TYPE enInfoType;		//命令类型
	union INFO_U
	{
		NPQ_ECODE_PARAM struEncode;
		unsigned char res[256]; 
	}unCmd;
};

/**	@struct	 NPQ_STAT 
 *	@brief   Qos状态
 *	@note	 
 */
struct NPQ_STAT
{
	int iVersion;					//结构体版本，用于以后兼容
	unsigned int nRttUs;			//rtt，单位us
	unsigned int nRealRttUs;		//实时rtt，单位us
	unsigned int nBitRate;			//当前所有数据实际码率,单位 bps
	unsigned char cLossFraction;	//丢包率，单位1/256
	unsigned char cLossFraction2;	//经过恢复之后的丢包率，只能在接收端获取，单位1/256
    unsigned char nVideoPicQ;       //视频画质      :0,好；1，良；2，差。
    unsigned char nVideoRTQ;        //视频实时性质量:0,好；1，良；2，差。
    unsigned char nVideoFluQ;       //视频流畅性质量:0,好；1，良；2，差。    
    unsigned char nAudioTonQ;       //音频音质      :0,好；1，良；2，差。
    unsigned char nAudioRTQ;        //音频实时性质量:0,好；1，良；2，差。
    unsigned char nAudioFluQ;       //音频流畅性质量:0,好；1，良；2，差。
    unsigned int nFrameRate;        //获取统计帧率
	unsigned int nBitRateFec;       //当前FEC数据实际码率,单位 bps
	unsigned char res[232];
};

/**	@enum	 NPQ_PARAM_VALUE_TYPE 
 *	@brief   参数类型
 *	@note	 
 */
enum NPQ_PARAM_VALUE_TYPE
{
	NPQ_PARAM_NULL,		//空参数，从私有信息中解析
	NPQ_PARAM_SDP,		//sdp信息
};

/**	@struct	 NPQ_SENDER_PARAM 
 *	@brief   发送端Qos配置参数
 *	@note	总大小128字节固定
 */
struct NPQ_SENDER_PARAM
{
	int bVaild;	        //结构体是否有效
	int iMinBitrate;	//最小码率 单位bps
	int iMaxBitrate;	//最大码率 单位bps
    int iStartBitrate;  //初始码率 单位bps,用户可以配置一个初始码率
    char bStopBWProbe;  //关闭带宽探测功能
    char nREDr;         //音频冗余度
    char res[110];		//预留字段
};

/**	@struct	 NPQ_RECEIVER_PARAM 
 *	@brief   接收端Qos配置参数
 *	@note	总大小128字节固定
 */
struct NPQ_RECEIVER_PARAM
{
	int  bVaild;	       //结构体是否有效
    int  bG7221BigEndian;  //配置G722.1大端解码模式
	char res[120];	       //预留字段
};

/**	@struct	 NPQ_PARAM 
 *	@brief   Qos配置参数
 *	@note	 
 */
struct NPQ_PARAM
{
	int iVersion;				//结构体版本，用于以后兼容
	int m_type;					//Qos策略类型 NPQ_QOS_TYPE
	int bVideo;					//是否开启视频Qos
	int bAudio;					//是否开启音频Qos
	NPQ_PARAM_VALUE_TYPE enValueType;	//参数类型
	void*  pValue;						//参数值
	union DIF_PARAM
	{
		NPQ_SENDER_PARAM struSender;	//发送端参数
		NPQ_RECEIVER_PARAM struReceiver;//接收端参数
		unsigned char res[128];	
	}unDifParam;
    unsigned char cVideoQosSet;  //视频：0：常规；1：实时性优先。
    unsigned char cAudioQosSet;  //音频：0：常规；1：实时性优先。

    char          bEnableCpuSet;  //启用cpu指定功能
    unsigned int  uCpuSet;        //指定的CPU集
    unsigned int  uBytesAlign;    //字节对齐定义，格式及内容见下方注释。
    int bPrivate;                //是否开启私有Qos, 自iVersion >=1启用
    unsigned char res[105];	                      
}; 

//字节对齐定义
/* 0                    1                       2                       3	                    高位
   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1 
   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
   |RTP  |RED  |RTCP |FEC  |
   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
   每种封装格式占2bit，封装格式目前定义顺序为：RTP，RED，RTCP，FEC， 后续有其它格式在FEC之后添加即可。
   字段值：00--不用对齐； 01--4字节对齐；  10--8字节对齐；  11--16字节对齐。    目前支持4字节，8字节，16字节三种对齐方式。
*/




/**	@struct	 NPQ_SET_NOTIFY_PARAM 
 *	@brief   配置通知参数，只用于接收端，可随时设置
 *	@note	 
 */
struct NPQ_SET_NOTIFY_PARAM
{
	int iVersion;				//结构体版本，用于以后兼容
	unsigned int nBitRate;		//码率 单位bps
	bool bHaveBitrate;			//码率字段是否有效标志
	bool bHaveForceIframe;		//强制I帧字段是否有效标志
	bool bForceIframe;			//强制I帧
	bool bHaveScale;			//scale字段是否有效标志
	float fScale;				//scale信息  1、2、4、8
	unsigned char res[240];		
};

/**	@enum	 NPQ_STATE 
 *	@brief   NPQ状态
 *	@note	 
 */
enum NPQ_STATE
{
    NPQ_STATE_INIT = 0,            //NPQ初始化状态
    NPQ_STATE_PLAY,                //NPQ 工作状态，NPQ对应类型数据回调输出（包括RTP和RTCP）
    NPQ_STATE_PAUSE,              //NPQ 暂停状态，NPQ对应类型数据停止回调（包括RTP和RTCP）
};

/**	@enum	 NPQ_DATA_INFO_CODE 
 *	@brief   NPQ信息码
 *	@note	 
 */
enum NPQ_DATA_INFO_CODE
{
    NPQ_DATA_OK = 0,              //完整帧
    NPQ_FRAME_INCOMPLETE          //非完整帧（当前DATA_TYPE对应的帧）
};

/**	@enum	 NPQ_DATA_FRAME_TYPE 
 *	@brief   NPQ帧信息
 *	@note	 
 */
enum NPQ_DATA_FRAME_TYPE
{
    NPQ_DATA_FRAME_UNKNOWN = 0,    //未知帧类型
    NPQ_DATA_FRAME_VIDEO_I,        //视频I帧
    NPQ_DATA_FRAME_VIDEO_P,        //视频P帧
    NPQ_DATA_FRAME_AUDIO,          //音频帧
    NPQ_DATA_FRAME_PRIVAT,         //私有帧
};

/**	@struct	 NPQ_DATA_INFO 
 *	@brief   NPQ数据信息
 *	@note	 
 */
struct NPQ_DATA_INFO
{
    int             iVersion;	   //结构体版本，用于以后兼容
    int             iDataType;     //见NPQ_DATA_TYPE定义   
    unsigned int    nBuffLen;      //缓存长度
    unsigned char*  pBuff;         //缓存指针
    int             iTimeStamp;    //时间戳
    int             iDataInfoCode; //数据信息，见NPQ_DATA_INFO_CODE
    int             iFrameType;    //帧类型，见NPQ_DATA_FRAME_TYPE
    int             iMainType;     //媒体类型，见NPQ_MAIN_TYPE
    char            res[96];       //保留字段
};

/**	@enum	 NPQ_LOG_LEVEL 
 *	@brief   NPQ日志控制输出等级
 *	@note	 
 */
enum NPQ_LOG_LEVEL
{
    NPQ_LOG_LEVEL_TRACE = 0,
    NPQ_LOG_LEVEL_DEBUG,
    NPQ_LOG_LEVEL_INFO,
    NPQ_LOG_LEVEL_WARN,
    NPQ_LOG_LEVEL_STAT,
    NPQ_LOG_LEVEL_ERROR,
    NPQ_LOG_LEVEL_UNKNOWN
};

/**	@enum	 NPQ_SUB_MOUDLE 
 *	@brief   NPQ子模块，可用于控制日志输出
 *	@note	 
 */
enum NPQ_SUB_MOUDLE
{
    NPQ_SUB_MODULE_ALL = 0,
    NPQ_SUB_MODULE_JITTER,
    NPQ_SUB_MODULE_NACK,
    NPQ_SUB_MODULE_FEC,
    NPQ_SUB_MODULE_BW,
    NPQ_SUB_MODULE_PACING,
    NPQ_SUB_MODULE_PLI,
    NPQ_SUB_MODULE_NETEQ,
    NPQ_SUB_MODULE_SYN
};




/**	@fn NPQCb
*	@brief 回调函数
*	@param  int id  id号
*	@param  int iDataType  数据类型  见NPQ_DATA_TYPE定义
*	@param  unsigned char* pData  数据地址
*	@param  unsigned int uDataLen 数据长度
*	@param  void* pUser 回调用户参数
*	@return void
*/
typedef void  (__stdcall *NPQCb)( int id , int iDataType, unsigned char* pData, unsigned int nDataLen, void* pUser);

/**	@fn NPQDataInfoCb
*	@brief 数据信息回调函数
*	@param  int id  id号
*	@param  unsigned char* pData  数据地址，使用NPQ_DATA_INFO的数据结构
*	@param  unsigned int uDataLen 数据长度，NPQ_DATA_INFO的大小
*	@param  void* pUser 回调用户参数
*	@return void
*/
typedef void  (__stdcall *NPQDataInfoCb)( int id , void* pDataInfo, unsigned int nDataInfoLen, void* pUser);

/**	@fn NPQLogCb
*	@brief NPQ日志输出回调函数
*	@param  iLogLevel           NPQ日志输出等级,见NPQ_LOG_LEVEL
*   @param  iNPQSubModule       NPQ子模块，见NPQ_SUB_MOUDLE
*	@param  const char* format  NPQ日志输出格式
*	@param  void* varlist       NPQ日志输出参数，varlist指针为va_list*，用户不用再调用va_start和va_end
*	@param  void* pUser 回调用户参数
*   @param  int id 用于区分不同句柄生成的日志
*	@return void
*/
typedef void  (__stdcall *NPQLogCb)( int iLogLevel, int iNPQSubModule, const char* format, void* varlist, void* pUser, int id);

/**	@fn NPQ_Create
*	@brief 创建一路Qos服务
*	@param  NPQ_QOS_ROLE enType  类型
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_Create(NPQ_QOS_ROLE enType);

/**	@fn NPQ_Destroy
*	@brief 销毁一路Qos服务
*	@param  int id  id号
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_Destroy(int id);

/**	@fn NPQ_Start
*	@brief 启动一路Qos服务
*	@param  int id  id号
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_Start(int id);

/**	@fn NPQ_Stop
*	@brief 停止一路Qos服务
*	@param  int id  id号
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_Stop(int id);

/**	@fn NPQ_RegisterDataCallBack
*	@brief 注册数据回调
*	@param  int id  id号
*	@param  NPQCb pCallback 回调函数指针
*	@param  void* pUser 用户参数
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_RegisterDataCallBack(int id, NPQCb pCallback, void* pUser);

/**	@fn NPQ_RegisterDataInfoCallBack
*	@brief 注册数据消息回调,覆盖NPQ_RegisterDataCallBack回调范围，回调信息更加详细
*   @brief 相较于NPQ_RegisterDataCallBack，优先使用该函数注册的回调
*	@param  int id  id号
*	@param  NPQDataInfoCb pCallback 回调函数指针
*	@param  void* pUser 用户参数
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_RegisterDataInfoCallBack(int id, NPQDataInfoCb pCallback, void* pUser);

/**	@fn NPQ_RegisterRecoveriedDataCallBack
*	@brief 注册抗丢包恢复后数据回调
*	@param  int id  id号
*	@param  NPQCb pCallback 回调函数指针
*	@param  void* pUser 用户参数
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_RegisterRecoveriedDataCallBack(int id, NPQCb pCallback, void* pUser);

/**	@fn NPQ_RegisterAudioDecFun
*	@brief 注册音频解码相关函数
*	@param  int id  id号
*	@param  NPQ_AUDIO_DECODE_FUN* pDecFuns 音频解码相关函数
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_RegisterAudioDecFun(int id, NPQ_AUDIO_DECODE_FUN *pRegisterFun);

/**	@fn NPQ_InputData
*	@brief 输入RTP和RTCP数据
*	@param  int id  id号
*	@param  int iDataType  数据类型  同NPQ_DATA_TYPE
*	@param  unsigned char* pData  数据地址
*	@param  unsigned int uDataLen 数据长度
*	@param  void* pUser 回调用户参数
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_InputData(int id, int iDataType, unsigned char* pData, unsigned int nDataLen);

/**	@fn NPQ_InputRawData
*	@brief 输入裸数据，内部进行RTP打包处理，按帧输入
*	@param  int id  id号
*	@param  MUX_PARAM* pMuxParam  打包参数
*	@param  INPUT_BUF* pstInputData 输入数据地址和长度
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_InputRawData(int id, MUX_PARAM* pMuxParam, INPUT_BUF* pstInputData);

/**	@fn NPQ_SetParam
*	@brief 设置参数
*	@param  int id  id号
*	@param  void* pParam   参数，见NPQ_PARAM
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_SetParam(int id, void* pParam);

/**	@fn NPQ_SetMediaSrcID
*	@brief  设置码流源ID，支持调用NPQ_Start之前及之后调用该接口。
*   @       倘若不设置码流源ID，则相同NPQ_MAIN_TYPE的数据源默认码流源不一样。
*	@param  int id  id号
*	@param  NPQ_MAIN_TYPE enMainType  Qos主类型
*	@param  unsigned int nSrcID       码流源ID，相同的码流源ID值一样，否则不一样。
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_SetMediaSrcID(int id, NPQ_MAIN_TYPE enMainType, unsigned int nSrcID);

/**	@fn NPQ_SetMinMaxBitrate
*	@brief  设置最小最大码率，支持调用NPQ_Start之前及之后调用该接口。
*   @       NPQ回调出来的NPQ_CMD_ENCODE的码率值不会超过该最大码率。
*   @       发送端输入NPQ的平均码率不能超过通过NPQ_CMD_ENCODE回调出来的码率。
*	@param  int id  id号
*	@param  unsigned int nMinBitRate  最小码率
*	@param  unsigned int nMaxBitRate  最大码率
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_SetMinMaxBitrate(int id, NPQ_MAIN_TYPE enMainType, unsigned int nMinBitRate, unsigned int nMaxBitRate);

/**	@fn NPQ_SetMediaDelay
*	@brief  设置媒体延时时间，支持调用NPQ_Start之前及之后调用该接口。
*   @       延时时间为从NPQ回调输出那一刻到渲染那一刻的延时时间。
*	@param  int id  id号
*	@param  NPQ_MAIN_TYPE enMainType  Qos主类型
*	@param  unsigned int nDelayMs     从NPQ输出到渲染的延时时间，单位为ms
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_SetMediaDelay(int id, NPQ_MAIN_TYPE enMainType, unsigned int nDelayMs);

/**	@fn NPQ_SetMediaNTPTimeAndTimeStamp
*	@brief  设置媒体采集时刻的时间和时间戳参数。通过RTCP从发送端发给接收端，用于音视频同步。
*	@param  int id  id号
*	@param  NPQ_MAIN_TYPE enMainType  Qos主类型
*	@param  long long nCaptureTime    从1970年1月1号至媒体数据采集时刻的us数，单位为us。
*   @param  unsigned int nTimeStamp   媒体数据采集时刻的时间戳，单位与封装格式中的保持一致
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_SetMediaNTPTimeAndTimeStamp(int id, NPQ_MAIN_TYPE enMainType, long long nCaptureTime, unsigned int nTimeStamp);

/**	@fn NPQ_SetNotifyParam
*	@brief 设置通知参数
*	@param  int id  id号
*	@param  void* pNotifyParam   参数，见NPQ_SET_NOTIFY_PARAM
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_SetNotifyParam(int id,  NPQ_SET_NOTIFY_PARAM* pNotifyParam);

/**	@fn NPQ_SetState
*	@brief 设置NPQ的状态
*	@param  int id  id号
*	@param  NPQ_MAIN_TYPE enMainType  Qos主类型
*	@param  NPQ_STATE enState         将NPQ状态配置为该状态
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_SetState(int id, NPQ_MAIN_TYPE enMainType, NPQ_STATE enState);

/**	@fn NPQ_GetStat
*	@brief 获取状态
*	@param  int id  id号
*	@param  NPQ_MAIN_TYPE enMainType  Qos主类型
*	@param  void* pStat  状态信息，见NPQ_STAT
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_GetStat(int id,NPQ_MAIN_TYPE enMainType, void* pStat);

/**	@fn NPQ_GetData
*	@brief 获取音频数据接口
*	@param  int iDataType  数据类型  见NPQ_DATA_TYPE定义
*	@param  unsigned char* pData  数据地址
*	@param  unsigned int* pDataLen 数据长度,输入时为pData长度，输出时为获取的数据长度
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_GetData(int id, int iDataType, unsigned char* pData, unsigned int* pDataLen);

/**	@fn NPQ_SetLogFile
*	@brief 设置日志文件路径（不支持hlog的平台）
*	@param  char* szAbsFileName  日志文件路径（包含文件名）
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_SetLogFile(char* szAbsFileName);

/**	@fn NPQ_RegisterLogCallBack
*	@brief 注册日志输出回调,若注册了该函数，优先使用该回调，
*   @brief 相较于NPQ_SetLogFile，优先使用该函数注册的回调
*	@param  NPQLogCb pCallback 回调函数指针
*	@param  void* pUser 用户参数
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_RegisterLogCallBack(NPQLogCb pCallback, void* pUser);

/**	@fn NPQ_GetVersion
*	@brief 获取版本号
*	@param  char* szVersion 版本信息 格式：Vx.x.x build20xxxxxx
*	@return NPQ错误码
*/
NPQ_API int __stdcall NPQ_GetVersion(char* szVersion,int nLen);







#endif

