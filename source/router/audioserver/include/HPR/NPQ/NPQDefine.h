/**  @file    NPQDefine.h
 *   @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved. 
 *   @brief     集成NPQ库用到的宏，结构体，枚举定义
 *   
 *   @author zhaolin
 *   @modify zhaolin
 *   @date   2016/12/16
 *   
 *   @note:
 */

#ifndef _NPQ_DEFINE_H_
#define _NPQ_DEFINE_H_

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "NPQos.h"

typedef enum
{
    NPQ_MODE_DIRECT_CONNECT = 0, //直连
    NPQ_MODE_BY_SMS = 1          //过流媒体
}PROTO_NPQ_MODE;

//错误码
#define NET_ERR_NPQ_BASE_INDEX      8000                                //NPQ库错误码
#define NET_ERR_NPQ_PARAM           (NET_ERR_NPQ_BASE_INDEX + 1)        //NPQ库参数有误
#define NET_ERR_NPQ_SYSTEM          (NET_ERR_NPQ_BASE_INDEX + 2)        //NPQ库操作系统调用错误(包括资源申请失败或内部错误等）
#define NET_ERR_NPQ_GENRAL          (NET_ERR_NPQ_BASE_INDEX + 3)        //NPQ库内部通用错误
#define NET_ERR_NPQ_PRECONDITION    (NET_ERR_NPQ_BASE_INDEX + 4)        //NPQ库调用顺序错误
#define NET_ERR_NPQ_NOTSUPPORT      (NET_ERR_NPQ_BASE_INDEX + 5)        //NPQ库功能不支持

#define NET_ERR_NPQ_NOTCALLBACK     (NET_ERR_NPQ_BASE_INDEX + 100)      //数据没有回调上来
#define NET_ERR_NPQ_LOADLIB         (NET_ERR_NPQ_BASE_INDEX + 101)      //NPQ库加载失败
#define NET_ERR_NPQ_STEAM_CLOSE (NET_ERR_NPQ_BASE_INDEX + 104) //本路码流NPQ功能未开启
#define NET_ERR_NPQ_MAX_LINK (NET_ERR_NPQ_BASE_INDEX + 110) //NPQ取流路数达到上限
#define NET_ERR_NPQ_STREAM_CFG (NET_ERR_NPQ_BASE_INDEX + 111) //编码参数存在冲突配置

typedef HPR_INT32(CALLBACK *HC_NPQ_Create)(NPQ_QOS_ROLE enType);
typedef HPR_INT32(CALLBACK *HC_NPQ_Destroy)(HPR_INT32 id);
typedef HPR_INT32(CALLBACK *HC_NPQ_Start)(HPR_INT32 id);
typedef HPR_INT32(CALLBACK *HC_NPQ_Stop)(HPR_INT32 id);
typedef HPR_INT32(CALLBACK *HC_NPQ_RegisterDataCallBack)(HPR_INT32 id, HPR_VOID(CALLBACK *fnRegisterDataCallBack)(HPR_INT32 id, HPR_INT32 iDataType, unsigned char* pData, HPR_UINT32 nDataLen, HPR_VOIDPTR pUser), HPR_VOIDPTR pUser);
typedef HPR_INT32(CALLBACK *HC_NPQ_InputData)(HPR_INT32 id, HPR_INT32 iDataType, unsigned char* pData, HPR_UINT32 nDataLen);
typedef HPR_INT32(CALLBACK *HC_NPQ_InputRawData)(HPR_INT32 id, MUX_PARAM* pMuxParam, INPUT_BUF* pstInputData);
typedef HPR_INT32(CALLBACK *HC_NPQ_SetParam)(HPR_INT32 id, HPR_VOIDPTR pParam);
typedef HPR_INT32(CALLBACK *HC_NPQ_GetStat)(HPR_INT32 id, NPQ_MAIN_TYPE enMainType, HPR_VOIDPTR pStat);
typedef HPR_INT32(CALLBACK *HC_NPQ_SetNotifyParam)(HPR_INT32 id, NPQ_SET_NOTIFY_PARAM* pNotifyParam);
typedef HPR_INT32(CALLBACK *HC_NPQ_GetVersion)(char* szVersion, int nLen);


typedef struct tagNET_UTILS_NPQ_PACK_PARAM
{
    // 固定的参数
    HPR_UINT32        pack_size;             // PS、TS和RTP设置最大包长，0为默认长度
    HPR_UINT32        track_index;           // 轨道号
    HPR_UINT32        syc_video_track;       // 关联的视频轨道号

    HPR_UINT32        system_format;         // 目标封装格式
    HPR_UINT32        system_format_subtype; // 目标类型的子格式，目前只使用于MP4，其他封装不适用

    HPR_UINT32        video_format;          // 视频类型
    HPR_UINT32        audio_format;          // 音频类型
    HPR_UINT32        privt_format;          // 私有数据类型

    // 参考帧信息，非固定参数
    HPR_UINT32        is_hik_stream;         // 是否符合海康定义
    HPR_UINT32        encrypt_type;          // 加密类型
    HPR_UINT32        frame_type;            // 当前帧类型 I/P/B/audio/privt
    HPR_UINT32        time_stamp;            // 时间戳
    float               duration;              // 帧时长
    HPR_UINT32        frame_num;             // 帧号
    HK_SYSTEMTIME       global_time;           // 全局时间（海康码流才有）

    // 视频参数
    struct
    {
        HPR_UINT16  width_orig;            // 分辨率，原始宽
        HPR_UINT16  height_orig;           // 分辨率，原始高
        HPR_UINT16  width_play;            // 分辨率，裁剪宽
        HPR_UINT16  height_play;           // 分辨率，裁剪高
        float           frame_rate;            // 帧率
        HPR_UINT16  interlace;             // 是否场编码
        HPR_UINT16  b_frame_num;           // 组模式中，B帧的个数
        HPR_UINT32    is_svc_stream;         // 是否SVC码流
    }video;

    // 音频参数
    struct
    {
        HPR_UINT16  channels;              // 声道
        HPR_UINT16  bits_per_sample;       // 样位
        HPR_UINT32    samples_rate;          // 采样率
        HPR_UINT32    bit_rate;              // 比特率
    }audio;

    // 私有参数
    struct
    {
        HPR_UINT32    privt_type;            // 私有类型
        HPR_UINT32    data_type;             // 子类型
    }privt;

}NET_UTILS_NPQ_PACK_PARAM, *LPNET_UTILS_NPQ_PACK_PARAM;



typedef void (CALLBACK *NPQ_TRANS_CB)(HPR_INT32 id, HPR_INT32 iDataType, unsigned char* pData, HPR_UINT32 nDataLen, HPR_VOIDPTR pUser);



typedef struct tagNET_UTILS_NPQ_STATE_SINGLE
{
    HPR_UINT32 dwRttUs;			//rtt，单位us
    HPR_UINT32 dwRealRttUs;		//实时rtt，单位us
    HPR_UINT32 dwBitRate;       //码率
    HPR_UINT8 byLossFraction;	//丢包率，单位1/256
    HPR_UINT8 byLossFraction2;	//经过恢复之后的丢包率，只能在接收端获取，单位1/256
    HPR_UINT8 byRes[126];
}NET_UTILS_NPQ_STATE_SINGLE, *LPNET_UTILS_NPQ_STATE_SINGLE;


typedef struct tagNET_UTILS_NPQ_STATE
{
    HPR_UINT32 dwSize;
    NET_UTILS_NPQ_STATE_SINGLE struAudioState;  //音频传输状态
    NET_UTILS_NPQ_STATE_SINGLE struVideoState;  //视频传输状态
    HPR_UINT8 byRes[256];
}NET_UTILS_NPQ_STATE, *LPNET_UTILS_NPQ_STATE;

// typedef struct tagNET_UTILS_NPQ_CONNECT_PARAM
// {
//     NET_UTILS_NPQ_LINK_COND struLinkRemote;  //对端视音频流连接和RTCP连接地址信息
//     char *sSdp; //对端SDP信息
//     HPR_UINT8 byRes[64];
// }NET_UTILS_NPQ_CONNECT_PARAM, *LPNET_UTILS_NPQ_CONNECT_PARAM;

typedef struct tagNET_UTILS_NPQ_COMPRESSINFO
{
    HPR_UINT32 dwSize;
    HPR_UINT32 dwMaxBitRate; //最大码率
    HPR_UINT8 byRes[64];
}NET_UTILS_NPQ_COMPRESSINFO, *LPNET_UTILS_NPQ_COMPRESSINFO;

typedef struct rtp_hdr//rtp header
{
    unsigned char count : 4;
    unsigned char extension : 1;
    unsigned char padding : 1;
    unsigned char version : 2;
    unsigned char payload : 7;
    unsigned char marker : 1;
    unsigned short sequence;
    unsigned int timestamp;
    unsigned int ssrc;
}RTP_HEADER;

#define COMMON_VIDEO_PT     96      //通用视频payload定义

#endif //_NPQ_DEFINE_H_
