/**
 * \file      ezDevSDK_preview_def.h  
 * 
 * \filepath  e:\workdir\小项目\ezDevSDK_v2.0.0\userpace\src\domain\base_function\ezDevSDK_base_function_def.h
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     预览领域对外结构体定义、宏定义
 * 
 * \author    xiangbin
 * 
 * \date      2017/5/22
 */
#ifndef H_EZDEVSDK_PREVIEW_DEF_H_
#define H_EZDEVSDK_PREVIEW_DEF_H_
#include "ezDevSDK_error.h"
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \addtogroup Preview_Module
 * \{
 */

//----------------------------------错误码--------------------------------------------------

/**
 *  \brief 萤石SDK错误码
 */
typedef enum  {
    EZDEVSDK_PREVIEW_CODE_SUCCESS                   = 0x00000000,   ///< 成功

    EZDEVSDK_PREVIEW_CODE_BASE                      = 0XD3100000,   ///< 预览领域接口错误码起始值，D表示接口错误码，3100，表示预览控制领域，最后三位表示具体错误
    EZDEVSDK_PREVIEW_CODE_FAIL                      = EZDEVSDK_PREVIEW_CODE_BASE + EZDEVSDK_ERROR_FAIL,
    EZDEVSDK_PREVIEW_CODE_NOT_INITED                = EZDEVSDK_PREVIEW_CODE_BASE + EZDEVSDK_ERROR_NOT_INITED,
    EZDEVSDK_PREVIEW_CODE_INVALID_PARAM             = EZDEVSDK_PREVIEW_CODE_BASE + EZDEVSDK_ERROR_INVALID_PARAM,
    EZDEVSDK_PREVIEW_CODE_NOT_SUPPORT               = EZDEVSDK_PREVIEW_CODE_BASE + EZDEVSDK_ERROR_NOT_SUPPORT,
    EZDEVSDK_PREVIEW_CODE_NO_MEM                    = EZDEVSDK_PREVIEW_CODE_BASE + EZDEVSDK_ERROR_NOT_ENOUGH_MEMERY,
    
    EZDEVSDK_PREVIEW_CODE_BUF_NOT_ENOUGH            = 0XD3100020,   ///缓存长度不够
    EZDEVSDK_PREVIEW_CODE_CHANNEL_NOT_EXIST         = 0XD3100021,   ///通道不存在
    EZDEVSDK_PREVIEW_CODE_SEND_TIMEOUT              = 0XD3100022,   ///发送超时
    EZDEVSDK_PREVIEW_CODE_PREVIEW_NO_RESOURCE       = 0XD3100023,   ///预览无资源

    EZDEVSDK_PREVIEW_CODE_PREVIEW_CHANNEL_NOT_EXIST = 0XD3100024,           ///请求通道号不在NVR的有效范围内
    EZDEVSDK_PREVIEW_CODE_PREVIEW_CHANNEL_NOT_RELATED = 0XD3100025,                ///当前通道未关联前端设备
    EZDEVSDK_PREVIEW_CODE_PREVIEW_CHANNEL_NOT_ONLINE = 0XD3100026,          ///当前通道关联的前端设备不在线
    EZDEVSDK_PREVIEW_CODE_PREVIEW_STREAM_INDEX_NOT_SUPPORT = 0XD3100027,///当前请求码流类型不支持
} EZDEVSDK_PREVIEW_ERROR_CODE_E;

//----------------------------------错误码--------------------------------------------------


//--------------------------------初始化信息---------------------------------------------------------
typedef struct {
    int	 type;                                   ///< 消息类型\c EZDEVSDK_PREVIEW_RUNTIME_INFO_E
    void *data;                                  ///< 指向具体type类型的数据
    int  len;                                    ///< data数据长度
	void *ptx;									 ///< 多中心标记
} ezDevSDK_preview_runtime_info;

typedef struct {
    int  type;                                   ///< 消息类型\c EZDEVSDK_PREVIEW_MSG_TO_DEV_TYPE_E
    void *data;                                  ///< 指向消息内容的指针
    int  len;                                    ///< 指向消息内容的长度
	void *ptx;									 ///< 多中心标记
} ezDevSDK_preview_msg_to_dev;

/**
 * \brief   SDK回调给用户层接口
 */
typedef struct 
{
    int (*recv_msg)(ezDevSDK_preview_msg_to_dev *msg);            ///< 消息回调接口，不可以阻塞，主动接收消息和回调方式二选一，实现了回调接口，消息就走回调函数
    int (*get_runtime_info)(ezDevSDK_preview_runtime_info *info);///< SDK运行时获取信息回调，不能有任何阻塞操作
} ezDevSDK_preview_callback;

typedef struct 
{
    int link_max;                                ///< 链接最大数
    int channel_max_num;                         ///< 通道数
    int stream_max_num;                          ///< 每个通道能产生的码流数
    ezDevSDK_preview_callback cb;                ///< 回调函数
} ezDevSDK_preview_init_info;

//--------------------------------初始化信息---------------------------------------------------------


//--------------------------------SDK通知应用层消息-----------------------------------------
/**
 * \brief   SDK给应用的消息
 */
typedef enum {
    EZDEVSDK_PREVIEW_ON_START_SEND_STREAM,                  ///< 开始取流, 信息体\c ezDevSDK_preview_start_send_stream 
    EZDEVSDK_PREVIEW_ON_STOP_SEND_STREAM,                   ///< 停止取流, 信息体\c ezDevSDK_preview_stop_send_stream 
    EZDEVSDK_PREVIEW_ON_SET_VIDEO_PARAM,                    ///< 设置视频参数, 信息体\c ezDevSDK_preview_video_param 
    EZDEVSDK_PREVIEW_ON_SET_AUDIO_PARAM,                    ///< 设置音频参数, 信息体\c ezDevSDK_preview_audio_param 
    EZDEVSDK_PREVIEW_ON_FORCE_I_FRAME,                      ///< 强制I帧, 信息体\c ezDevSDK_preview_force_i_frame 
    EZDEVSDK_PREVIEW_ON_SET_PACKET_TYPE,                    ///< 设置码流打包封装类型, 信息体\c ezDevSDK_preview_video_pack_info 
    EZDEVSDK_PREVIEW_ON_ADD_MAP_PORT,                       ///< 增加UPNP映射端口, 信息体\c ezDevSDK_preview_port_info 
    EZDEVSDK_PREVIEW_ON_DEL_MAP_PORT,                       ///< 删除UPNP映射端口, 信息体\c ezDevSDK_preview_port_info 
    EZDEVSDK_PREVIEW_ON_SET_MIRROR,                         ///< 开启预览镜像, 信息体\c ezDevSDK_preview_mirror_param 
    EZDEVSDK_PREVIEW_ON_SET_SWITCH_STATUS,                  ///< 设置预览相关开关类, 信息体\c ezDevSDK_preview_switch_status 
    EZDEVSDK_PREVIEW_ON_SET_VIDEO_ENCODE_TYPE,	            ///< 设置视频参数 265->264切换, 信息体\c ezDevSDK_preview_video_param 
    EZDEVSDK_PREVIEW_ON_CHECK_STREAM_RESOURCE,              ///< 检查上层资源是否足够, 信息体\c ezDevSDK_preview_stream_resource_type 
    EZDEVSDK_PREVIEW_ON_SET_AUTO_SWITCH_STREAM_ENABLE,      ///< 平台配置主子码流切换功能, 信息体\c ezDevSDK_preview_auto_switch_stream_enable 
    EZDEVSDK_PREVIEW_ON_CHANGE_STREAM_INDEX,                ///< 预览过程中进行主子码流切换, 信息体\c ezDevSDK_preview_change_stream_index 
	EZDEVSDK_PREVIEW_ON_SET_NIGHT_VISION_MODE,              ///< 设置夜视模式, 信息体\c ezDevSDK_preview_night_vision_mode 
	EZDEVSDK_PREVIEW_ON_SET_ROI_VALUE,                      ///< 设置ROI 参数, 信息体\c ezDevSDK_preview_roi_info 
	EZDEVSDK_PREVIEW_ON_SET_AUTO_NIGHT_VISION,               ///< 自动增强夜视，信息体\c ezDevSDK_preview_auto_night_vision
	EZDEVSDK_PREVIEW_ON_SET_INVERSE_MODE,               	///< 设置逆光模式
    EZDEVSDK_PREVIEW_ON_SET_IMAGE_STYLE_MODE,           	///< 设置图像风格
    EZDEVSDK_PREVIEW_ON_TRANS_CMD_MSG,                      ///< 透传预览领域命令
	EZDEVSDK_PREVIEW_ON_SET_IMAGE_PARA_ADJUST,              ///<设置图像参数调节
	EZDEVSDK_PREVIEW_ON_SET_PREVIEW_PLAN,                     ///< 设置设备计划，信息体\c ezDevSDK_preview_plan
	EZDEVSDK_PREVIEW_ON_CHECK_PREVIEW_PARAM,                ///<检查预览参数。信息体\c ezDevSDK_preview_param
	EZDEVSDK_PREVIEW_ON_SET_PRIVACY_COVER_AREA,              ///< 设置隐私遮蔽区域，信息体\c ezDevSDK_preview_cover_area_info
} EZDEVSDK_PREVIEW_MSG_TO_DEV_TYPE_E;

typedef enum{
    EZDEVSDK_PREVIEW_STREAM_TYPE_VTDU,                  ///< 申请VTDU转发资源
    EZDEVSDK_PREVIEW_STREAM_TYPE_OTHER,                 ///< 申请其他类型取流资源(包括P2P 、直连)
}EZDEVSDK_PREVIEW_STREAM_TYPE_E;                  ///< 检查取流资源类型，目前仅区分VTDU及其他方式取流

/**
 * \brief \c EZDEVSDK_PREVIEW_ON_START_SEND_STREAM
 */
typedef struct {
    int channel;                                        ///< 通道
    int stream_index;                                   ///< 码流索引
    int sid;                                            ///< 码流会话ID，会话发送方式有效，有效值大于0.
    int pre_time;                                       ///< 取预录流长度，单位：秒，0表示不取预录流 
}  ezDevSDK_preview_start_send_stream;

/**
 * \brief \c EZDEVSDK_PREVIEW_ON_STOP_SEND_STREAM
 */
typedef struct {
    int channel;                                        ///< 通道
    int stream_index;                                   ///< 码流索引
    int sid;                                            ///< 码流会话ID，会话发送方式有效，有效值大于0.
}  ezDevSDK_preview_stop_send_stream;
 /**
  * \brief \c EZDEVSDK_PREVIEW_INVERSE_MODE_INFO
  */
 typedef struct {
	 char mode;		///< mode: 0:自动逆光，1：手动逆光
	 char enable;	///< enable： 0:关闭，1:打开
	 char position;	///< position： 1:上，2:下，3:左，4:右，5:中心
} ezDevSDK_preview_inverse_mode_info;

 /**
  * \brief \c EZDEVSDK_PREVIEW_IMAGE_STYLE_INFO
  */
 typedef struct {
	 char mode;		///< mode: 1:标准，2:写实，3:艳丽
} ezDevSDK_preview_image_style_info;
/**
 * \brief \c  EZDEVSDK_PREVIEW_ON_SET_VIDEO_PARAM
 		 \c  EZDEVSDK_PREVIEW_ON_SET_VIDEO_ENCODE_TYPE
 */
typedef struct {
    int channel;                                        ///< 通道号(入参)
    int stream_index;                                   ///< 码流码流索引(入参)
    int stream_type;                                    ///< 码流类型，视频流、音频流和复合流(以下都是出参)
    int encode_type;                                    ///< 视频编码类型
    int resolution;                                     ///< 视频分辨率
    int bit_rate_type;                                  ///< 视频码率类型
    double frame_rate;                                  ///< 视频帧率
    int iframe_interval;                                ///< 视频I帧间隔
    int max_bit_rate;                                   ///< 视频最大码率, 单位bps
    int encode_complexity;                              ///< 视频编码复杂度
    int image_quality;                                  ///< 视频图象质量
} ezDevSDK_preview_video_param;
 /**
  * \brief \c 图像参数调整
  */
 typedef struct {
    int brightness;    ///< 亮度
    int contrast;      ///< 对比度
    int saturation;    ///< 饱和度
    int sharpness;     ///< 锐度
} ezDevSDK_preview_image_para_adjust_info;

/**
 * \brief   \c EZDEVSDK_PREVIEW_ON_SET_AUDIO_PARAM
 */
typedef struct {
    int channel;                                        ///< 通道号(入参)
    int encode_type;                                    ///< 音频编码类型(以下都是出参)
    int sample_rate;                                    ///< 音频采样率
    int bits_per_sample;                                ///< 采样点位率, 单位bit
    int bit_rate;                                       ///< 音频码率, 单位bps
    int channel_num;                                    ///< 声道数
} ezDevSDK_preview_audio_param;

/**
 * \brief \c EZDEVSDK_PREVIEW_ON_FORCE_I_FRAME
 */
typedef struct {
    int channel;                                        ///< 通道
    int stream_index;                                   ///< 码流索引
}  ezDevSDK_preview_force_i_frame;

/**
 * \brief \c EZDEVSDK_PREVIEW_ON_SET_PACKET_TYPE
 */
typedef struct {
    int channel;                                        ///< 通道号(入参)
    int stream_index;                                   ///< 码流码流索引(入参)
    int pack_type;                                      ///< 码流封装格式(设置为入参，获取为出参)
} ezDevSDK_preview_video_pack_info;

/**
 * \brief \c EZDEVSDK_PREVIEW_ON_ADD_MAP_PORT;
            \c EZDEVSDK_PREVIEW_ON_DEL_MAP_PORT
 */
typedef struct {
    int port;                                           ///< 端口(入参)
} ezDevSDK_preview_port_info;

/**
 * \brief \c EZDEVSDK_PREVIEW_ON_SET_MIRROR
 */
typedef struct {
    int channel;                                        ///< 通道(入参)
    char mode[32];                                      ///< 镜像模式(入参) UP_DOWN/LEFT_RIGHT/CENTER/CLOSE
} ezDevSDK_preview_mirror_param;

/**
 * brief \c EZDEVSDK_PREVIEW_ON_SET_SWITCH_STATUS
 */
typedef struct {
    int channel;                                        ///< 通道(入参)  0:代表设备
    int type;                                           ///< 类型(入参) \c EZDEVSDK_PREVIEW_SWITCH_TYPE_E类型 
    int enable;                                         ///< 开关状态 1:打开 0:关闭
} ezDevSDK_preview_switch_status;

/**
 * \brief \c EZDEVSDK_PREVIEW_ON_CHECK_STREAM_RESOURCE
 */
typedef struct {
    int  stream_type;                                   ///< 入参->表明即将要申请的资源类型 \c EZDEVSDK_PREVIEW_STREAM_TYPE_E
} ezDevSDK_preview_stream_resource_type;

/**
 * \brief \c EZDEVSDK_PREVIEW_ON_SET_AUTO_SWITCH_STREAM_ENABLE
 */
typedef struct {
    int enable;                                         ///<0关闭 1打开
} ezDevSDK_preview_auto_switch_stream_enable;

 /**
  * \brief \c EZDEVSDK_PREVIEW_ON_CHANGE_STREAM_INDEX
  */
  typedef struct {
     int sid;
     int channel;
     int stream_index;
 } ezDevSDK_preview_change_stream_index;

 /**
  * \brief \c EZDEVSDK_PREVIEW_ON_SET_NIGHT_VISION_MODE
  */
 typedef struct {
     int graphic_type;   ///<0黑白 1彩色
     int luminance;      ///<亮度标识
     int duration;
 } ezDevSDK_preview_night_vision_mode;

 /**
  * \brief \c EZDEVSDK_PREVIEW_ON_SET_ROI_VALUE
  */
 typedef struct {
    float fAxis_x;
    float fAxis_y;
    float fWidth;
    float fHeight;
} ezDevSDK_preview_roi_info;

 /**
  * \brief \c EZDEVSDK_PREVIEW_ON_SET_AUTO_NIGHT_VISION
  */
 typedef struct {
    int enable;                                         ///< 开关状态 1:打开 0:关闭
} ezDevSDK_preview_auto_night_vision;

/**
  * \brief \c EZDEVSDK_PREVIEW_ON_SET_PRIVACY_COVER_AREA/EZDEVSDK_PREVIEW_RT_INFO_GET_PRIVACY_COVER_AREA
*/
#define ezDevSDK_PREVIEW_COVER_ARE_MAX	4  /* 閬尅鍖哄煙涓暟涓婇檺 */

typedef struct 
{
    int x;
    int y;
}ezDevSDK_preview_cover_point_s;
 
typedef struct 
{
	ezDevSDK_preview_cover_point_s stLeftTop; 
	ezDevSDK_preview_cover_point_s stRightLower;
}ezDevSDK_preview_cover_area_s;
 
typedef struct
{
	int area_num;						 ///< 閬尅鍖哄煙涓暟
	ezDevSDK_preview_cover_area_s area[ezDevSDK_PREVIEW_COVER_ARE_MAX];	///< 鍖哄煙閬尅鍧愭爣
}ezDevSDK_preview_cover_area_info;

/**
 * \brief \c EZDEVSDK_PREVIEW_ON_TRANS_CMD_MSG
 */
typedef struct {
    char* content;          ///< 信令内容
    int content_len;        ///< 信令长度
    unsigned int cmd_id;    ///< 与平台交互的信令ID
    unsigned int msg_seq;   ///< 序号
}ezDevSDK_preview_trans_cmd_msg;

typedef enum {
    EZDEVSDK_PREVIEW_PRIVACY_PROTECT_PLAN = 0x0,     ///< 隐私遮蔽计划
    EZDEVSDK_PREVIEW_PLAN_MAX,
} EZDEVSDK_PREVIEW_PLAN_TYPE_E;

/**
 * @brief   预览计划时间
 */
typedef struct
{
    time_t begin;                               ///< 计划开始时间
    time_t end;                                 ///< 计划结束时间
} ezDevSDK_preview_time_segment;


typedef struct 
{
    int type;                                   ///< 计划类型@ref EZDEVSDK_PREVIEW_PLAN_TYPE_E
    int channel;                                ///< 通道号
    int enable_defence_plan;                    ///< 开关状态
    union
    {
        struct
        {
            ezDevSDK_preview_time_segment time[7][5];
        } preview_privacy_protect_plan;
    } plan_info;
} ezDevSDK_preview_plan;

//--------------------------------SDK获取设备信息-----------------------------------------

/**
 * \brief SDK运行时信息枚举和结构体
 */
typedef enum{
    //以下参数是萤石云下发的配置保存在设备上，当SDK需要时再从设备处获取。
    EZDEVSDK_PREVIEW_RT_INFO_GET_PACKET_TYPE,           ///< 获取当前打包封装类型，<b>必须实现</b>, 信息体\c ezDevSDK_preview_video_pack_info
    EZDEVSDK_PREVIEW_RT_INFO_GET_VIDEO_PARAM,           ///< 获取视频编码参数，<b>必须实现</b> , 信息体\c ezDevSDK_preview_video_param
    EZDEVSDK_PREVIEW_RT_INFO_GET_AUDIO_PARAM,           ///< 获取音频编码参数，<b>必须实现</b>, 信息体\c ezDevSDK_preview_audio_param
    EZDEVSDK_PREVIEW_RT_INFO_GET_SWITCH_STATUS,         ///< 获取开关状态, 信息体\c ezDevSDK_preview_switch_status
    EZDEVSDK_PREVIEW_RT_INFO_GET_DEFAULT_VIDEO_PARAM,  ///< P2P码流自适应功能，获取默认的视频参数(调节上限), 信息体\c ezDevSDK_preview_video_param
	EZDEVSDK_PREVIEW_RT_INFO_GET_NIGHT_VISION_MODE,    ///< 获取当前夜视模式, 信息体\c ezDevSDK_preview_night_vision_mode
	EZDEVSDK_PREVIEW_RT_INFO_GET_ROI_VALUE,             ///< 获取当前ROI 参数, 信息体\c ezDevSDK_preview_roi_info
	EZDEVSDK_PREVIEW_RT_INFO_GET_AUTO_NIGHT_VISION, ///< 获取当前自动增强夜视开关 \c ezDevSDK_preview_auto_night_vision
	EZDEVSDK_PREVIEW_RT_INFO_GET_PRIVACY_COVER_AREA, ///< 获取当前隐私遮蔽区域信息 \c ezDevSDK_preview_cover_area_info
	
    //以下参数是萤石云下发的配置保存在设备上，当SDK需要时再从设备处获取。
    EZDEVSDK_PREVIEW_RT_INFO_GET_MAPPED_PORT = 0x100,  ///< 获取UPNP映射关系，<b>必须实现</b>, 信息体\c ezDevSDK_preview_mapped_port_info
    EZDEVSDK_PREVIEW_RT_INFO_GET_EXTERNAL_IP,           ///< 获取外部地址，<b>必须实现</b>, 信息体\c ezDevSDK_preview_external_ip_info
    EZDEVSDK_PREVIEW_RT_INFO_GET_ENCRYPTKEY,            ///< 获取加密秘钥，<b>必须实现</b>, 信息体\c ezDevSDK_preview_encrypt_key_info
    EZDEVSDK_PREVIEW_RT_INFO_CAPTURE_PICTURE,           ///< 抓图、获取抓图数据, 信息体\c ezDevSDK_preview_capture_picture_info
    EZDEVSDK_PREVIEW_RT_INFO_GET_NETWORK_INTERFACE,    ///< 获取当前活动网卡，<b>必须实现</b>, 信息体\c ezDevSDK_preview_network_interface
	EZDEVSDK_PREVIEW_RT_INFO_GET_INVERSE_MODE,         ///< 获取逆光模式
    EZDEVSDK_PREVIEW_RT_INFO_GET_IMAGE_STYLE,          ///< 获取图像色彩模式
    EZDEVSDK_PREVIEW_RT_INFO_GET_PREVIEW_PLAN,          ///< 获取计划。信息体\c ezDevSDK_preview_plan
	EZDEVSDK_PREVIEW_RT_INFO_GET_IMAGE_PARA_ADJUST,     ///<图像参数调节
} EZDEVSDK_PREVIEW_RUNTIME_INFO_E;


/**
 * \brief \c EZDEVSDK_PREVIEW_RT_INFO_GET_MAPPED_PORT
 */
typedef struct {
    unsigned short port;                                ///< 端口(入参)
    unsigned short map_port;                            ///< 映射后的端口(出参)
} ezDevSDK_preview_mapped_port_info;

/**
 * \brief \c EZDEVSDK_PREVIEW_RT_INFO_GET_EXTERNAL_IP
 */
typedef struct {
    char external_ip[16];                               ///< UPNP外部ip地址(出参)
} ezDevSDK_preview_external_ip_info;

/**
 * brief \c EZDEVSDK_PREVIEW_RT_INFO_GET_ENCRYPTKEY
 */
typedef struct {
    int enable;                                         ///< 密码状态(出参)
    char password[64];                                  ///< 密码(出参)
} ezDevSDK_preview_encrypt_key_info;

/**
 * \brief \c EZDEVSDK_PREVIEW_RT_INFO_CAPTURE_PICTURE
 */
typedef struct {
    int  channel;                                       ///< 通道号(入参)
    int  type;                                          ///< 图片格式(入参)
    int  resolution;                                    ///< 图片分辨率(入参)
    int  quality;                                       ///< 图片质量(入参)
    char *data;                                         ///< 图片地址(入参)
    int  len;                                           ///< 图片长度(入参为data可存储长度地址，出参为实际的图片长度)
} ezDevSDK_preview_capture_picture_info;

/**
 * brief \c EZDEVSDK_PREVIEW_RT_INFO_GET_NETWORK_INTERFACE
 */
typedef struct {
    char interface[16];                                 ///< 网卡名称
    int type;                                           ///< 1有线， 2无线
    int net_status;                                     ///< 网络状态，0表示断网，1表示连网
} ezDevSDK_preview_network_interface;

/**
 * \brief \c EZDEVSDK_PREVIEW_SWITCH_TYPE_E
 */
typedef enum {
    EZDEVSDK_PREVIEW_SWITCH_TYPE_AUTO_ADJUST_STREAM = 1, ///< 自适应码率
    EZDEVSDK_PREVIEW_SWITCH_TYPE_PRIVACY_PROTECT,        ///< 隐私保护 
    EZDEVSDK_PREVIEW_SWITCH_TYPE_AUDIO,                  ///< 视频声音
    EZDEVSDK_PREVIEW_SWITCH_TYPE_INFRARED,				 ///< 红外灯开关
    EZDEVSDK_PREVIEW_SWITCH_TYPE_ROI,                    ///< ROI 开关
    EZDEVSDK_PREVIEW_SWITCH_TYPE_WDR,                    ///< WDR 开关
    EZDEVSDK_PREVIEW_SWITCH_TYPE_CLOSE_UP,               ///< 画面特写开关
    EZDEVSDK_PREVIEW_SWITCH_TYPE_MAX
}EZDEVSDK_PREVIEW_SWITCH_TYPE_E;

typedef struct {
    int channel;                                        ///< 通道
    int stream_index;                                   ///< 码流索引
} ezDevSDK_preview_param;

//--------------------------------应用层设置/获取SDK配置信息-----------------------------------------
typedef enum{
    EZDEVSDK_PREVIEW_OPT_TYPE_GET_MAX_LINK_NUM,                 ///< 该选项暂未支持
    EZDEVSDK_PREVIEW_OPT_TYPE_GET_SEND_STREAM_NUM,
    EZDEVSDK_PREVIEW_OPT_TYPE_GET_QOS_MODE,                     ///< 该选项暂未支持
    EZDEVSDK_PREVIEW_OPT_TYPE_SET_P2P_LINK_NUM,
    EZDEVSDK_PREVIEW_OPT_TYPE_SET_QOS_MODE,                     ///< 该选项暂未支持
    EZDEVSDK_PREVIEW_OPT_TYPE_GET_STREAM_RESOURCE,
    EZDEVSDK_PREVIEW_OPT_TYPE_SET_P2P_QOS_MODE,                 ///<设置P2P QOS模式(int)
    EZDEVSDK_PREVIEW_OPT_TYPE_SET_VTDU_QOS_MODE,                ///<设置VTDU-QOS模式-支持UDP取流0:不支持1:支持默认不支持(int)
    EZDEVSDK_PREVIEW_OPT_TYPE_SET_STUN_DETECT_ENBALE,           ///<STUN检测功能默认打开 ，如无需使用可选择性关闭(int)
    EZDEVSDK_PREVIWE_OPT_TYPE_SET_P2PV3_FLOW_LIMIT,             ///<P2PV3心跳流量限制，默认3链路保活，开启此功能后仅单链路保活或直接不启用V3(int 1:单路保活 2:不启用P2PV3)，各模块初始化前调用
} EZDEVSDK_PREVIEW_OPT_TYPE_E;

/**
 * brief \c EZDEVSDK_PREVIEW_OPT_TYPE_GET_STREAM_RESOURCE
 */
typedef struct {
    int  vtdu_count;
    int  other_count;
}ezDevSDK_preview_stream_resource;

//--------------------------------应用层设置/获取SDK配置信息-----------------------------------------


//--------------------------------API接口相关定义-----------------------------------------
// 帧类型
#define EZDEVSDK_PREVIEW_FRAME_TYPE_I      0x0          ///< I帧
#define EZDEVSDK_PREVIEW_FRAME_TYPE_P      0x1          ///< P帧
#define EZDEVSDK_PREVIEW_FRAME_TYPE_B      0x2          ///< B帧
#define EZDEVSDK_PREVIEW_FRAME_TYPE_AUDIO  0x21         ///< 音频帧

typedef struct {
    char *frame;                                        ///< 帧数据起始地址
    int len;                                            ///< 帧长度
    int frame_type;                                     ///< 帧类型，I/P/B帧/音频帧
    unsigned int time_span;                             ///< 帧绝对时间戳
    unsigned int sequence;                              ///< 帧序号
    int send_mode;                                      ///< 发送模式，流发送还是帧发送
} ezDevSDK_preview_frame_info;

typedef struct{  
    int  channelNo;                                           ///< 通道号
    int  videoLevel;                                          ///< 画面质量
    int  streamTypeIn;                                        ///< 码流类型
    int  streamType;                                          ///< 视频类型
    int  resolution;                                          ///< 分辨率
    int  picQuality;                                          ///< 图像质量
    int  bitRateType;                                         ///< 码率类型
    int  videoBitRate;                                        ///< 码率上限
    int  maxBitRate;                                          ///< 自定义码率
    int  videoFrameRate;                                      ///< 视频帧率
    int  intervalBPFrame;                                     ///< 帧类型
    int  intervalFrameI;                                      ///< I帧间隔      
    int  encodeComplex;                                       ///< 编码复杂度
    char cameraType[64];                                      ///< 设备型号                                                
} ezDevSDK_preview_video_quality_param;

typedef struct{  
    int  num; //视频质量参数个数
    ezDevSDK_preview_video_quality_param*   video_quality;                          
} ezDevSDK_preview_video_quality_array;

typedef struct
{
    int index;
    char *uuid;
    int channel;
    int stream_index;
    unsigned short local_port;
    char *inner_address;
    char *upnp_address;
    char *nat_address;
    unsigned short inner_port;
    unsigned short upnp_port;
    unsigned short nat_port;
    int heart_beat_interval;            //ms
} ezDevSDK_preview_valid_p2p_info;

//--------------------------------API接口相关定义-----------------------------------------

#ifdef __cplusplus
}
#endif

#endif //H_EZDEVSDK_BASE_FUNCTION_DEF_H_
