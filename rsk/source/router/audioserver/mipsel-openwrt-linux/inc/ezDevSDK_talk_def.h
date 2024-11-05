/**
 * \file      ezDevSDK_talk_def.h  
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     对讲领域对外结构体定义、宏定义
 * 
 * \author    panlong
 * 
 * \date      2017/4/8
 */
#ifndef H_EZDEVSDK_TALK_DEF_H_
#define H_EZDEVSDK_TALK_DEF_H_

#include <time.h>
#include "ezDevSDK_error.h"
#include "ez_model_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \addtogroup Talk_Module 对讲领域模块
 * \{
 */

/**
 * \brief 萤石SDK错误码
 */
typedef enum  {                 
    EZDEVSDK_TALK_CODE_SUCCESS              = 0x00000000,    ///< 成功
    EZDEVSDK_TALK_CODE_BASE                 = 0xD3300000,    ///< 对讲领域接口错误码起始值，D表示接口错误码，3300，表示云台控制领域，最后三位表示具体错误
    EZDEVSDK_TALK_CODE_FAIL                 = EZDEVSDK_TALK_CODE_BASE + EZDEVSDK_ERROR_FAIL, 
    EZDEVSDK_TALK_CODE_NOT_INITED           = EZDEVSDK_TALK_CODE_BASE + EZDEVSDK_ERROR_NOT_INITED,
    EZDEVSDK_TALK_CODE_INVALID_PARAM        = EZDEVSDK_TALK_CODE_BASE + EZDEVSDK_ERROR_INVALID_PARAM, 
    EZDEVSDK_TALK_CODE_NOT_SUPPORT          = EZDEVSDK_TALK_CODE_BASE + EZDEVSDK_ERROR_NOT_SUPPORT, 
    
    EZDEVSDK_TALK_CODE_NO_RESOURCE          = 0xD3300020,
} EZDEVSDK_TALK_CODE_E;

/**
 * \brief 音频编码类型
 */
typedef enum  {
    EZDEVSDK_AUDIO_G722_1        = 0,           ///< G722编码
    EZDEVSDK_AUDIO_G711_U        = 1,           ///< G711 U率编码
    EZDEVSDK_AUDIO_G711_A        = 2,           ///< G711 A率编码
    EZDEVSDK_AUDIO_G723          = 3,           ///< G723编码
    EZDEVSDK_AUDIO_MP1L2         = 4,           ///< MPEG1L2编码
    EZDEVSDK_AUDIO_MP2L2         = 5,           ///< MPEG2L2编码
    EZDEVSDK_AUDIO_G726_U        = 6,           ///< G726编码
    EZDEVSDK_AUDIO_AAC           = 7,           ///< AAC编码
    EZDEVSDK_AUDIO_G726_A        = 8,
    EZDEVSDK_AUDIO_G726_16       = 9,
    EZDEVSDK_AUDIO_G729          = 10,
    EZDEVSDK_AUDIO_ADPCM         = 11,
    EZDEVSDK_AUDIO_AMR_NB        = 12,
    EZDEVSDK_AUDIO_RAW_DATA8     = 13,          ///< 采样率为8k的原始数据
    EZDEVSDK_AUDIO_RAW_UDATA16   = 14,          ///< 采样率为16k的原始数据，即L16
    EZDEVSDK_AUDIO_MP2L2_32      = 15,
    EZDEVSDK_AUDIO_MP2L2_64      = 16,
    EZDEVSDK_AUDIO_AAC_32        = 17,
    EZDEVSDK_AUDIO_AAC_64        = 18,
	EZDEVSDK_AUDIO_OPUS_8        = 19,          ///< OPUS 8K
	EZDEVSDK_AUDIO_OPUS_16       = 20,          ///< OPUS 16K
	EZDEVSDK_AUDIO_OPUS_48       = 21,          ///< OPUS 48K
	EZDEVSDK_AUDIO_G729_A        = 22,          ///< G729_A
	EZDEVSDK_AUDIO_G729_B        = 23,          ///< G729_B
	EZDEVSDK_AUDIO_PCM           = 24,          ///< PCM
	EZDEVSDK_AUDIO_MP3           = 25,          ///< MP3
	EZDEVSDK_AUDIO_AC3           = 26,          ///< AC3
	EZDEVSDK_AUDIO_RAW           = 99,          ///< RAW
} EZDEVSDK_TALK_ENCODE_TYPE_E;


//--------------------------------应用层通知SDK消息-----------------------------------------


//--------------------------------SDK通知应用层消息-----------------------------------------
/**
 * \brief   SDK给应用的消息类型
 */
typedef enum  {
    EZDEVSDK_TALK_ON_START_TALK,                ///<开启对讲 
    EZDEVSDK_TALK_ON_STOP_TALK,                 ///<停止对讲
    EZDEVSDK_TALK_ON_SET_TALK_MODE,             ///<设置对讲模式
    EZDEVSDK_TALK_ON_SET_CLIENT_TYPE,       	///<设置对讲客户端类型 
	EZDEVSDK_TALK_ON_SET_TALK_VOLUME,           ///< 设置对讲音量
    EZDEVSDK_TALK_ON_RECV_OTAP_MSG,             ///<接收otap信息 消息体：ezDevSDK_talk_otap_msg
    EZDEVSDK_TALK_ON_MAX,
} EZDEVSDK_TALK_MSG_TO_DEV_TYPE_E;

/**
 *  @brief   SDK给应用的消息结构体
 */
typedef struct {
	EZDEVSDK_TALK_MSG_TO_DEV_TYPE_E type;
	void*                           data;
	int                             len;
	void*							ptx;
} ezDevSDK_talk_msg_to_dev;

typedef enum {
    EZDEVSDK_TALK_CLIENT_TYPE_UNKNOW = 0,		///< 默认值
    EZDEVSDK_TALK_CLIENT_TYPE_IOS = 1,
    EZDEVSDK_TALK_CLIENT_TYPE_ANDROID = 3,
    EZDEVSDK_TALK_CLIENT_TYPE_STUDIO = 9,
    EZDEVSDK_TALK_CLIENT_TYPE_OPEN_PLATFORM_IOS = 11,
    EZDEVSDK_TALK_CLIENT_TYPE_OPEN_PLARFORM_ANDROID = 13,
    EZDEVSDK_TALK_CLIENT_TYPE_OPEN_PLARFORM_STUDIO = 19,
} EZDEVSDK_TALK_CLIENT_TYPE_E;

/**
 * \brief \c EZDEVSDK_TALK_ON_START_TALK;
 */
typedef struct {
    int channel;
    int sid;
    int priority;      ///<优先级
    int mode;          ///<音频模式 0-对讲,1-广播,2-监听
    int peer_volume;   ///<播放音量,百分比（0-100）,0为静音，100为最大
    int code_type;     ///音频编码类型,EZDEVSDK_TALK_ENCODE_TYPE_E
    int sample_rate;   ///<音频采样率
} ezDevSDK_talk_start_talk;

/**
 * \brief \c EZDEVSDK_TALK_ON_STOP_TALK;
 */
typedef struct {
    int sid;
} ezDevSDK_talk_stop_talk;

/**
 *  @brief   SDK给应用的消息结构体
 */
typedef struct {
    int type;       ///< 消息体:\c EZDEVSDK_TALK_CLIENT_TYPE_E
} ezDevSDK_talk_client_type;

/**
 * \brief \c EZDEVSDK_TALK_ON_SET_TALK_VOLUME;
 */
typedef struct {
    int speaker_volume;                             ///< 对讲speaker音量
} ezDevSDK_talk_volume;

/**
 *  @brief   otap透传的内容
 */
typedef struct {
    ez_basic_info *basic_info;
    ez_model_msg *buf;
    char *msg_type;         ///< 消息类型,set/operate/query等
    unsigned int msg_seq;  ///< 消息的seq
} ezDevSDK_talk_otap_msg;

//--------------------------------SDK获取设备信息-----------------------------------------

/**
 * \brief   SDK获取设备信息类型
 */
typedef enum {
    EZDEVSDK_TALK_RT_INFO_GET_ENCODE_TYPE,          ///< 获取音频编码类型，<b>必须实现</b>
    EZDEVSDK_TALK_RT_INFO_GET_TALK_MODE,
    EZDEVSDK_TALK_RT_INFO_GET_TALK_VOLUME,          ///< 获取对讲音量
} EZDEVSDK_TALK_RUNTIME_INFO_E;

/**
 * \brief   SDK获取设备信息结构体
 */
typedef struct {
	EZDEVSDK_TALK_RUNTIME_INFO_E    type;
	void*                           data;
	int                             len;
	void*							ptx;
} ezDevSDK_talk_runtime_info;

/**
 * \brief 声音编码类型
 */
typedef struct {
    int channel;    //入参
    int encode;     //出参
} ezDevSDK_talk_encode_type;


/**
 * \brief   双工模式 0半双工模式 1全双工模式
 */
typedef struct {
    int mode;
} ezDevSDK_talk_mode;

/**
 * \brief   SDK回调给用户层接口, SDK运行时获取信息回调，不能有任何阻塞操作
 */
typedef struct {
    int (*recv_msg)(ezDevSDK_talk_msg_to_dev *msg);                 ///< 消息回调接口，不可以阻塞，主动接收消息和回调方式二选一，实现了回调接口，消息就走回调函数
	int (*on_get_runtime_info)(ezDevSDK_talk_runtime_info *info);
    int (*on_recv_talk_data)(int channel, char *data, int len);
    int (*on_check_resource)(int cur_count, int channel);
    int (*on_recv_talk_data_new)(int channel, char *data, int len, int encode_type);
} ezDevSDK_talk_callback;

//--------------------------------初始化参数-----------------------------------------------
typedef struct {
    int max_buf;
    ezDevSDK_talk_callback cb;
} ezDevSDK_talk_init_info;

/*! \} */

#ifdef __cplusplus
}
#endif

#endif //H_EZDEVSDK_TALK_DEF_H_
