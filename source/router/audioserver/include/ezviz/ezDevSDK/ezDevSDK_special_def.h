/**
 * \file	  ezDevSDK_special_def.h
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     特殊功能领域对外结构体定义、宏定义
 * 
 * \author    liuzhenhao
 * 
 * \date      2017/8/1
 */
#ifndef H_EZDEVSDK_SPECIAL_DEF_H_
#define H_EZDEVSDK_SPECIAL_DEF_H_

#include <time.h>
#include "ezDevSDK_error.h"

/**
 * \addtogroup Special_Module
 * \{
 */

//-------------------------------------错误码------------------------------------------------------

/**
 * \brief 萤石SDK特殊领域的接口错误码，由SDK返回给调用者
 * \note 公用错误码，请参考ezDevSDK_error.h
 */
typedef enum  {
    EZDEVSDK_SPECIAL_SUCESS                     = 0X00000000, ///< 成功
    EZDEVSDK_SPECIAL_CODE_BASE                  = 0XD3999000,   ///< 特殊领域接口错误码起始值，D表示接口错误码，3999，表示特殊领域，最后三位表示具体错误

    ///< 0XD3999000 - 0XD399901F 公共错误码
    EZDEVSDK_SPECIAL_CODE_FAIL                  = EZDEVSDK_SPECIAL_CODE_BASE + EZDEVSDK_ERROR_FAIL,
    EZDEVSDK_SPECIAL_CODE_NOT_INITED            = EZDEVSDK_SPECIAL_CODE_BASE + EZDEVSDK_ERROR_NOT_INITED,
    EZDEVSDK_SPECIAL_CODE_INVALID_PARAM         = EZDEVSDK_SPECIAL_CODE_BASE + EZDEVSDK_ERROR_INVALID_PARAM,
    EZDEVSDK_SPECIAL_CODE_NOT_SUPPORT           = EZDEVSDK_SPECIAL_CODE_BASE + EZDEVSDK_ERROR_NOT_SUPPORT,
    EZDEVSDK_SPECIAL_CODE_NOT_ENOUGH_MEMERY     = EZDEVSDK_SPECIAL_CODE_BASE + EZDEVSDK_ERROR_NOT_ENOUGH_MEMERY,
    EZDEVSDK_SPECIAL_CODE_INIT_FAIL             = EZDEVSDK_SPECIAL_CODE_BASE + EZDEVSDK_ERROR_INIT_FAIL,
    EZDEVSDK_SPECIAL_CODE_THREAD_CREATE_FAIL    = EZDEVSDK_SPECIAL_CODE_BASE + EZDEVSDK_ERROR_THREAD_CREATE_FAIL,
    EZDEVSDK_SPECIAL_CODE_RSP_TIMEOUT           = EZDEVSDK_SPECIAL_CODE_BASE + EZDEVSDK_ERROR_RSP_TIMEOUT,
    EZDEVSDK_SPECIAL_CODE_MSG_DATA_INVAILD      = EZDEVSDK_SPECIAL_CODE_BASE + EZDEVSDK_ERROR_MSG_DATA_INVAILD,

    EZDEVSDK_SPECIAL_CODE_SEND_DATA_ERROR       = 0XD3999020, ///< 发送数据失败
    EZDEVSDK_SPECIAL_CODE_SEND_MSG_ERROR        = 0XD3999021, ///< 向平台发送外部封装的信令失败
    EZDEVSDK_SPECIAL_CODE_VOICE_DOWNLOAD_FAILED = 0XD3999022, ///< 语音文件下载失败
    EZDEVSDK_SPECIAL_CODE_VOICE_NOT_EXIST       = 0XD3999023, ///< 语音文件不存在
    EZDEVSDK_SPECIAL_CODE_VOICE_IS_BUSY         = 0XD3999024, ///< 语音文件正在使用中
    EZDEVSDK_SPECIAL_CODE_VOICE_COUNT_LIMITED   = 0XD3999025, ///< 语音文件个数达到上限

} EZDEVSDK_SPECIAL_CODE_E;

/**
 * \brief 由调用层设置的协议错误码，内部转化后传递到客户端
 */
typedef enum {
    EZDEVSDK_SPECIAL_PROTOCOL_CODE_BASE = 0XF3999000,       ///< 特殊领域协议错误码起始值，F表示协议错误码，3999，表示特殊领域，最后三位表示具体错误
} EZDEVSDK_SPECIAL_PROTOCOL_CODE_E;


//-------------------------------------初始化信息--------------------------------------------------

/**
 * \brief SDK给应用的消息结构体
 */
typedef struct {
	int     type;       ///< SDK给应用的消息类型, \ref EZDEVSDK_SPECIAL_MSG_TO_DEV_TYPE_E
	void*   data;       ///< 所给消息内容
	int     len;        ///< 所给消息内容的长度
}ezDevSDK_special_msg_to_dev;

/**
 * \brief SDK获取设备信息结构体
 */
typedef struct {
	int     type;       ///< 需获取的设备信息类型, \ref EZDEVSDK_SPECIAL_RUNTIME_INFO_TYPE_E
	void*   data;       ///< 信息内容结构体
	int     len;        ///< 信息内容长度
}ezDevSDK_special_runtime_info;

/**
 * \brief SDK回调给应用层接口, SDK运行时获取信息回调，不能有任何阻塞操作
 * \note  get_runtime_info回调函数中，对于不支持的功能，返回值请填EZDEVSDK_ERROR_NOT_SUPPORT
 */
typedef struct {
	int (*recv_msg)(ezDevSDK_special_msg_to_dev *msg);              ///< 消息回调接口
	int (*get_runtime_info)(ezDevSDK_special_runtime_info *info);	///< 消息回调获取上层状态
} ezDevSDK_special_callback;

/**
 * \brief 特殊领域初始化的参数
 */
typedef struct {
    ezDevSDK_special_callback cb;       ///< 回调函数
    int resetReportAll;
} ezDevSDK_special_init_info;

/**
 * \brief   应用传递SDK的事件类型，对应接口ezDevSDK_special_post_event()
 */
typedef enum
{
    EZDEVSDK_SPECIAL_POST_EVENT_FACE_DET_PUSH_MODEL_STATUS      = 0,
    EZDEVSDK_SPECAIL_POST_EVENT_MAX,
} EZDEVSDK_SPECAIL_POST_EVENT_INFO_E;

/**
 * \brief   设备传递事件给SDK 结构体
 */
typedef struct {
	int    iEvent;  // 参见EZDEVSDK_SPECIAL_POST_EVENT_INFO_E
	char*  pcData;
	int    iLen;
}ezDevSDK_special_post_event_info_S;
//-------------------------------------SDK通知应用层的消息-----------------------------------------

/**
 * \brief SDK通知应用层的消息类型
 */
typedef enum {
    EZDEVSDK_SPECIAL_ON_SET_FLOW_COUNTING_CFG = 0x0000, ///< 设置配置参数，消息体: \c ezDevSDK_special_flow_counting_cfg
    EZDEVSDK_SPECIAL_ON_SET_SWITCH_ENABLE,              ///< 使能开关，消息体: \c ezDevSDK_special_switch_enable
    EZDEVSDK_SPECIAL_ON_DELAY_SLEEP,                    ///< 延迟休眠，消息体: \c ezDevSDK_special_delay_sleep_type
    EZDEVSDK_SPECIAL_ON_OPERATE_CUSTOM_VOICE,           ///< 自定义声音信息，消息体: \c ezDevSDK_special_operate_custom_voice
                                                        //   对应的协议错误码，需要设置为EZDEVSDK_SPECIAL_PROTOCOL_CODE_E
    EZDEVSDK_SPECIAL_ON_SET_LINKAGE_MODE_STATUS,        ///< 关联状态设置，消息体: \c ezDevSDK_linkage_mode_status
    EZDEVSDK_SPECIAL_ON_WAKE,                           ///< 唤醒操作，消息体: \c ezDevSDK_wake
    EZDEVSDK_SPECIAL_ON_DEL_TRUSTEESHIP, 				///< 删除托管关系，消息体: \c ezDevSDK_Special_DelTrusteeship_S，内存由sdk申请和释放
    EZDEVSDK_SPECIAL_ON_TRANS_CMD_MSG,                  ///< 由外部实现的请求所对应的平台回应信息，消息体: \c ezDevSDK_special_trans_cmd_msg
    EZDEVSDK_SPECIAL_ON_UNBINDUSER,                     ///< 解绑用户结果，消息体: \c ezDevSDK_special_unbind_user
    EZDEVSDK_SPECIAL_ON_SET_HIDDNS_MAPPED_PORT,         ///< HIDDNS 解析结果，消息体: \c ezDevSDK_special_hiddns_dev_info
    EZDEVSDK_SPECIAL_ON_SAVE_FACE_DET_INFO,             ///< 保存人脸识别图片URL，消息体: \c ezDevSDK_special_save_face_det_info_s
    EZDEVSDK_SPECIAL_ON_EDIT_FACE_DET_INFO,             ///< 编辑人脸识别图片，消息体: \c ezDevSDK_special_edit_face_det_info_s
    EZDEVSDK_SPECIAL_ON_DELETE_FACE_DET_INFO,           ///< 删除人脸识别图片，消息体: \c 
    EZDEVSDK_SPECIAL_ON_QUERY_FACE_DET_INFO,            ///< 查询人脸识别建模状态，消息体: \c 
    EZDEVSDK_SPECIAL_ON_PROWL_DRIVE_OUT,                ///< 徘徊驱离，消息体:\c ezDevSDK_special_prowl_drive_out
    EZDEVSDK_SPECIAL_ON_DRIVE_OUT_RADIO_UPDATE,         ///< 徘徊驱离音频更新
	EZDEVSDK_SPECIAL_ON_BATTERY_WORK_MODE,              ///< 工作模式，消息体 \c ezDevSDK_special_battery_work_mode
	EZDEVSDK_SPECIAL_ON_RING_SOUND,                     ///< 设置铃声，消息体：\c ezDevSDK_special_ring_type
	EZDEVSDK_SPECIAL_ON_SET_SPECIAL_PLAN,               ///< 设置计划，消息体：\c ezDevSDK_special_plan
    EZDEVSDK_SPECIAL_ON_NET_WORK_MODE,                  ///< 联网模式，消息体：\c ezDevSDK_special_net_work_mode
    EZDEVSDK_SPECIAL_ON_SET_SPECIAL_ONE_CLICK_MUTE,     ///< 一键静音，消息体：\c ezDevSDK_special_one_click_mute
    EZDEVSDK_SPECIAL_ON_SET_SPECIAL_SPEAKER_CONFIG,     ///< chime扬声器功能，消息体：\c ezDevSDK_special_speaker_config
	EZDEVSDK_SPECIAL_MSG_TO_DEV_TYPE_MAX,
} EZDEVSDK_SPECIAL_MSG_TO_DEV_TYPE_E;

/**
 * \brief   客流统计的配置信息
 * \note    客流统计的开关有单独的接口不走配置
 */
typedef struct {
	int interval;										///< 上报间隔，单位:秒

	// 垂直视角，侧向视角
	int angle_vectical; 								///< 视角类型，0:倾斜，1:垂直

	// 划线坐标 (x1,y1) (x2,y2) (0~1)图像范围浮点比例，浮点保留所有小数位精度
	float cross_line_x1;    ///< 划线坐标(x1,y1)的 x 方向数值
	float cross_line_y1;    ///< 划线坐标(x1,y1)的 y 方向数值
	float cross_line_x2;    ///< 划线坐标(x2,y2)的 x 方向数值
	float cross_line_y2;    ///< 划线坐标(x2,y2)的 y 方向数值

	// 方向坐标 (x1,y1) (x2,y2) (0~1)图像范围浮点比例，浮点保留所有小数位精度
	float vector_line_x1;   ///< 方向坐标(x1,y1)的 x 方向数值
	float vector_line_y1;   ///< 方向坐标(x1,y1)的 y 方向数值
	float vector_line_x2;   ///< 方向坐标(x2,y2)的 x 方向数值
	float vector_line_y2;   ///< 方向坐标(x2,y2)的 y 方向数值
}ezDevSDK_special_flow_counting_cfg;

/**
 * \brief 切换使能开关类型
 */
typedef enum {
	EZDEVSDK_SPECIAL_SWITCH_TYPE_FLOW_COUNT = 0x0000,
	EZDEVSDK_SPECIAL_SWITCH_TYPE_LINGER_ALARM,
	EZDEVSDK_SPECIAL_SWITCH_TYPE_FACE_RECOGNITION,
	EZDEVSDK_SPECIAL_SWITCH_TYPE_DISTORTION_CORRECTION,
	EZDEVSDK_SPECIAL_SWITCH_TYPE_CHIME_INDICATOR_LIGHT,
	EZDEVSDK_SPECIAL_SWITCH_TYPE_MAX,
}EZDEVSDK_SPECIAL_SWITCH_ENABLE_TYPE_E;

/**
 * \brief 使能开关信息
 */
typedef struct
{
    int type;           ///< 开关使能类型，\ref EZDEVSDK_SPECIAL_SWITCH_ENABLE_TYPE_E
    int channel;        ///< 通道号
    int enable;         ///< 开关使能状态
} ezDevSDK_special_switch_enable;

/**
 * \brief 延迟休眠信息
 */
typedef struct {
	int channel;    ///< 通道号
	int type;       ///< 平台下发的休眠类型
}ezDevSDK_special_delay_sleep_type;

/**
 * @brief   设置铃声声音类型，对应消息类型: EZDEVSDK_SPECIAL_ON_RING_SOUND
 */
typedef struct
{
    int type;  ///< 声音类型,  1-铃声1,2-铃声2,3-铃声3,4-铃声4,5-铃声5,10-自定义铃声
    int index; ///< 自定义语音的索引号
    int vol;   ///< 音量
} ezDevSDK_special_ring_type;

/**
 * \brief   自定义语音的操作类型
 */
typedef enum {
	CUSTOM_VOICE_OP_ADD	= 0,    						///< 添加
	CUSTOM_VOICE_OP_MODIFY,	    						///< 修改
	CUSTOM_VOICE_OP_DEL,  	    						///< 删除
	CUSTOM_VOICE_OP_MAX
}EZDEVSDK_SPECIAL_CUSTOM_VOICE_OP_TYPE_E;

/**
 * \brief 自定义声音信息
 */
typedef struct {
    int index;                                          ///< 序号
    int type;       									///< 自定义语音操作类型，\ref EZDEVSDK_SPECIAL_CUSTOM_VOICE_OP_TYPE_E
    char name[64];  									///< 语音名称
    char url[512];  									///< 下载URL
    int customVoice; 									//1-告警音,2-铃声(默认1)
}ezDevSDK_special_operate_custom_voice;

/**
 * \brief 关联状态，对应消息类型: EZDEVSDK_SPECIAL_ON_SET_LINKAGE_MODE_STATUS
 */
typedef struct {
    int enable;                         ///< 关联状态，0:关闭，1:开启
    int time;                           ///< 关联模式窗口时间
	char acConnSerial[16];
    char acVerifyCode[16];
} ezDevSDK_linkage_mode_status;

/**
 * \brief 唤醒操作，对应消息类型: EZDEVSDK_SPECIAL_ON_WAKE
 */
typedef struct {
    char subserial[16];                 ///< 设备短序列号
} ezDevSDK_wake;

/**
 * \brief 托管关系信息体
 */
typedef struct
{
    int     iChannel;                   ///< 通道号
    char    acSubserial[16];            ///< 设备短序列号
    char    acBaseserial[16];
    char    cResult;                    ///< 错误码
	char    acNewShipSerial[16];  // 切换关联时有效
    char    acNewShipKey[32];     // 切换关联时有效
} ezDevSDK_Special_DelTrusteeship_Param_S;

/**
 * \brief 托管关系信息数组
 */
typedef struct
{
    unsigned int      uiVersion;
    unsigned short    usNum;
    unsigned char     reserved0[2];

    ezDevSDK_Special_DelTrusteeship_Param_S   astUnit[0];
} ezDevSDK_Special_DelTrusteeship_S;

/**
 * \brief 解绑用户请求响应结果
 */
typedef struct {
    int result;     ///< 解绑用户请求的响应结果码
}ezDevSDK_special_unbind_user;

typedef struct {
    char type[32];
    int local_port;
    int nat_port;
}ezDevSDK_special_hiddns_mapped_info;

typedef struct {
    int dev_status;
    char dev_serial[64];
    char domain[64];
    char nat_ip[16];
    char local_ip[16];
    ezDevSDK_special_hiddns_mapped_info mapped_port_infos[16];
} ezDevSDK_special_hiddns_dev_info;

typedef struct {
    int channel;    
} ezDevSDK_special_prowl_drive_out;

typedef struct {
    int channel;
    char url[512];
} ezDevSDK_special_drive_out_radio_update;

/**
 * \brief 工作模式
 */
typedef struct {
	int mode ;        ///< 0-省电模式 1-最佳监控模式 2-持续工作模式 3-超级省电模式 4-自定义模式 5-用户模式
}ezDevSDK_special_battery_work_mode;

typedef enum 
{
    EZDEVSDK_SPECIAL_WORK_MODE_PLAN = 0x0,   ///< 工作模式计划
    EZDEVSDK_SPECIAL_PLAN_MAX,
}EZDEVSDK_SPECIAL_PLAN_TYPE_E;
/**
 *\brief 工作模式计划时间段
 */
typedef struct
{
    int mode;  ///<工作模式
    time_t begin;
    time_t end;
}ezDevSDK_special_work_mode_time_segment;

/**
 *\brief 计划
 */
typedef struct{
    int type;         ///< 计划类型 EZDEVSDK_SPECIAL_PLAN_TYPE_E
    int channel;      ///< 通道号
    int enable_plan;  ///< 开关状态
    struct
    {
        ezDevSDK_special_work_mode_time_segment time[7][5];
    }special_work_mode_plan;
}ezDevSDK_special_plan;

/**
 * \brief 联网模式
 */
typedef struct {
	int mode ;        ///< 0-本地直连 1-远程联网
}ezDevSDK_special_net_work_mode;

/**
 *\brief 一键静音功能
 */
typedef struct{
    int enable;      ///< 0-关 1-开
}ezDevSDK_special_one_click_mute;

/**
 *\brief chime 扬声器功能
 */
typedef struct{
    int ring_enable;      ///< 铃声提醒开关 0-关 1-开
    int ring_index;       ///< 铃声索引
    int intel_enable;     ///< 智能检测提醒开关 0-关 1-开
    int intel_index;      ///< 智能检测音索引
}ezDevSDK_special_speaker_config;


//-------------------------------------SDK从应用层获取的消息---------------------------------------

/**
 * \brief   特殊功能领域从从设备获取信息
 */
typedef enum {
	EZDEVSDK_SPECIAL_ON_GET_FLOW_COUNTING_CFG = 0x0000,	///< 获取配置参数，消息体: \c ezDevSDK_special_flow_counting_cfg
	EZDEVSDK_SPECIAL_ON_GET_SWITCH_ENABLE,              ///< 获取开关状态，消息体: \c ezDevSDK_special_switch_enable
	EZDEVSDK_SPECIAL_ON_GET_KEY_VALUE,                  ///< 获取设备键值类状态，消息体: \c ezDevSDK_special_key_value
	EZDEVSDK_SPECIAL_ON_GET_CUSTOM_VOICE_INFOS,         ///< 获取自定义语音信息，消息体: \c ezDevSDK_special_custom_voice_infos
	EZDEVSDK_SPECIAL_ON_GET_LINKAGE_MODE_STATUS, 		///< 获取关联关系状态，消息体：\c ezDevSDK_linkage_mode_status

    //设备运行状态
	EZDEVSDK_SPECIAL_ON_GET_BATTERY_WORK_STATUS,        ///< 获取电池工作状态，消息体：\c ezDevSDK_special_battery_workstatus
	EZDEVSDK_SPECIAL_ON_GET_TRUSTEESHIP, 				///< 获取托管关系，电池设备使用; 消息体：\c ezDevSDK_Trusteeship
	EZDEVSDK_SPECIAL_ON_GET_TRUSTEESHIP_CONNECT, 		///< 获取被托管设备的连通性, Base使用; sdk申请内存并释放，释放结构体首地址。消息体：\c ezDevSDK_Trusteeship_connect
	EZDEVSDK_SPECIAL_ON_GET_POWERSTATUS,                ///< 获取电池电量，消息体：\c ezDevSDK_PowerStatus
	EZDEVSDK_SPECIAL_ON_GET_DELETE_TRUSTEESHIP, 		///< 托管关系删除，消息体：\c ezDevSDK_Special_DelTrusteeship_S 由应用层申请内存，sdk释放，释放结构体首地址
    EZDEVSDK_SPECIAL_ON_GET_HIDDNS_MAPPED_PORT,         ///< HIDDNS映射端口信息，消息体：\c ezDevSDK_special_hiddns_local_mapped_infos
    EZDEVSDK_SPECIAL_ON_GET_ASSOCIATED_DEVICE_CONN_STATUS,  ///<获取关联设备的网络状态(单机方案1对1) 消息体: \c ezDevSDK_special_associated_device_conn_status
	EZDEVSDK_SPECIAL_ON_GET_BATTERY_WORK_MODE,          ///< 获取工作模式,消息体：\c ezDevSDK_special_battery_work_mode
	EZDEVSDK_SPECIAL_ON_GET_RING_SOUND,                 ///< 获取铃声信息,消息体: \c ezDevSDK_special_ring_type
	EZDEVSDK_SPECIAL_ON_GET_SPECIAL_PLAN,               ///< 获取计划
    EZDEVSDK_SPECIAL_ON_GET_NET_WORK_MODE,              ///< 获取联网模式，消息体: \c ezDevSDK_special_net_work_mode
	EZDEVSDK_SPECIAL_ON_GET_4G_INFO,                    ///< 从设备获取4G网卡状态，消息体: \c ezDevSDK_special_4G_Info_S
	EZDEVSDK_SPECIAL_ON_GET_SPECIAL_ONE_CLICK_MUTE,     ///< 一键静音，消息体：\c ezDevSDK_special_one_click_mute
    EZDEVSDK_SPECIAL_ON_GET_SPECIAL_SPEAKER_CONFIG,     ///< chime扬声器功能，消息体：\c ezDevSDK_special_speaker_config
    EZDEVSDK_SPECIAL_RUNTIME_INFO_TYPE_MAX,
} EZDEVSDK_SPECIAL_RUNTIME_INFO_TYPE_E;

/**
 * \brief   设备键值类型
 */
typedef enum {
	EZDEVSDK_SPECIAL_KEY_ICR_STATUS = 0x0000,			///< ICR
	EZDEVSDK_SPECIAL_KEY_TYPE_MAX,
}EZDEVSDK_SPECIAL_KEY_VALUE_TYPE_E;

/**
 * \brief   设备键值信息体
 */
typedef struct
{
	int key;            ///< 设备键值，\ref EZDEVSDK_SPECIAL_KEY_VALUE_TYPE_E
	int channel;        ///< 通道号
	int value;          ///< 对应的值
} ezDevSDK_special_key_value;

/**
 * \brief   自定义语音信息体
 */
typedef struct {
    int index;              ///< 序号
    int status;             ///< 当前语音文件状态，0:同步完成，1:正在同步，2:同步失败
    char name[64];          ///< 语音名称
    char url[512];          ///< 下载URL
    time_t create_time;     ///< 创建时间
    int customVoice;        ///< 1-告警音,2-铃声(默认1)
}ezDevSDK_special_custom_voice_info;


#define EZDEVSDK_SPECIAL_CUSTOM_VOICE_MAX_NUM        16     ///< 最大自定义语音的个数

/**
 * \brief 自定义语音信息数组，对应消息类型: EZDEVSDK_SPECIAL_ON_OPERATE_CUSTOM_VOICE
 */
typedef struct {
    int num;                							///< 入参，存储空间大小个数，出参，实际获取的自定义声音个数
    ezDevSDK_special_custom_voice_info voice_info[EZDEVSDK_SPECIAL_CUSTOM_VOICE_MAX_NUM];
} ezDevSDK_special_custom_voice_infos;

/**
 * \brief 电池工作状态信息
 */
typedef struct {
    int keep_alive;  									///< 单次保活时长
    int work_time;   									///< 最长电池工作时长
    int special_business;                               ///< 此工作状态影响APP端使用时长，特殊业务可通过此参数设置不受控制。1:回放
}ezDevSDK_special_battery_workstatus;

//EZDEVSDK_SPECIAL_ON_DEL_TRUSTEESHIP, 在删除关联关系时只有subserial有效
typedef struct {
    char subserial[16];
    int connect_status;
    int iChannel;
} ezDevSDK_trusteeship_subserial;

//EZDEVSDK_SPECIAL_ON_GET_TRUSTEESHIP_CONNECT
typedef struct {
    int num;
    ezDevSDK_trusteeship_subserial subserial[0]; //占位符
} ezDevSDK_Trusteeship_connect;

//EZDEVSDK_SPECIAL_ON_GET_TRUSTEESHIP
typedef struct {
    char subserial[16];
} ezDevSDK_Trusteeship;

typedef struct {
    char subserial[16];
    int type;       //0-主电源 1-蓄电池 2-太阳能
    int value;
    char des[64];
    int remaining;
    char baseserial[16];
    int status;     // 1-正在充电 2-充满 3-没有电池 4-电池坏
} ezDevSDK_PowerStatus_subserial;

//EZDEVSDK_SPECIAL_ON_GET_POWERSTATUS
typedef struct {
    int num;
    ezDevSDK_PowerStatus_subserial subserial[0];		///< 占位符
} ezDevSDK_PowerStatus;

/**
 * \brief \c EZDEVSDK_SPECIAL_ON_GET_HIDDNS_MAPPED_PORT
 */
typedef struct {
    char external_ip[16];
    char local_ip[16];
    ezDevSDK_special_hiddns_mapped_info mapped_port_infos[16];
} ezDevSDK_special_hiddns_local_mapped_infos;

/**
 * \brief \c EZDEVSDK_SPECIAL_ON_GET_ASSOCIATED_DEVICE_CONN_STATUS
 */
typedef struct {
    int conn_status;
    int signal;                 ///<信号强度0-100
    char ssid[32];
    char interface_name[16];
    char local_ip[16];
    char mask[16];
    char gateway[16];
    char db_serial[16];
}ezDevSDK_special_associated_device_conn_status;

/**
 * \brief   设备指示灯，对应消息类型: EZDEVSDK_SPECIAL_ON_GET_4G_INFO
 */
typedef struct
{    
    int  status;   ///< 卡状态1:ok 2:未插卡3:pin未解锁4:连接失败
    int  sigal;     ///< 信号强度 status为234时，sigal均为0
    int  type;      ///<1:2G  2:3G 3:4G
    char network[32];  ///<运营商名字， status为234时均为unknown
    char ICCID[32];    ///<IC卡的唯一识别号码
	char IMEI[32];     ///<国际移动设备识别码
} ezDevSDK_special_4G_Info_S;
//-------------------------------------应用层发给SDK（或服务器）的消息------------------------------

typedef struct {
	int enter_num;          							///< 时间段进入次数
	int leave_num;          							///< 时间段出去次数
	time_t	start_time;	    							///< 开始时间
	time_t	stop_time;	    							///< 停止时间
}ezDevSDK_special_flow_counting_seg;

#define EZDEVSDK_SPECIAL_FLOW_COUNTING_MAX_COUNT 10     ///< 单次最多上报10条信息
typedef struct {
	int seg_count;
	ezDevSDK_special_flow_counting_seg seg[EZDEVSDK_SPECIAL_FLOW_COUNTING_MAX_COUNT];
}ezDevSDK_special_flow_counting_seg_count;

/**
 * \brief SDK定义的设备发给服务器的信令集合
 */
typedef enum
{
	EZDEVSDK_SPECIAL_UNBIND_USER = 0,
    EZDEVSDK_SPECIAL_SEND_VERIFICATION_CODE,            ///< 设备发送验证码 \c ezDevSDK_special_send_verification_code
    EZDEVSDK_SPECIAL_QUERY_HIDDNS_MAPPED_PORT,          ///< 设备查询其他设备的端口信息 \c ezDevSDK_special_query_hiddns_mapped_port
    EZDEVSDK_SPECIAL_TRANS_4G_FLOW_INFO,              ///<设备向平台上报4G流向信息，信息体:\c ezDevSDK_special_4G_flow_info_page
    EZDEVSDK_SPECIAL_MSG_TO_SERVER_COUNT,
}EZDEVSDK_SPECIAL_MSG_TO_SERVER_TYPE_E;

typedef struct
{
    char code[128];
} ezDevSDK_special_send_verification_code;

typedef struct
{
    char domain[128];
} ezDevSDK_special_query_hiddns_mapped_port;
/**
 * \brief   设备应用层和服务的透传信息体，对应消息类型: EZDEVSDK_SPECIAL_TRANS_4G_FLOW_INFO
 */
typedef struct { 
  	float incr; 												///< 流量增量(单位MB)
	int  type;      											///<0:上行流量 1:下行流量
    char ICCID[32];    											///<IC卡的唯一识别号码
	char IMEI[32];     											///<国际移动设备识别码 
} ezDevSDK_special_4G_flow_info;

/**
 * \brief   设备应用层和服务的透传信息体，对应消息类型: EZDEVSDK_SPECIAL_TRANS_4G_FLOW_INFO
 */
typedef struct { 
    int max_count;                                    ///< 流量信息体数量
    ezDevSDK_special_4G_flow_info *flow_info;   ///< 4G流量信息体
} ezDevSDK_special_4G_flow_info_page;

/**
* \brief 特殊功能领域接收设备通用消息
*/
typedef struct
{
	EZDEVSDK_SPECIAL_MSG_TO_SERVER_TYPE_E msg_type;		    ///< 消息类型	
	void* msg_struct_ptr;	                                ///< 指向消息内容的指针
}ezDevSDK_special_msg;

/**
 * \brief 外部组装的信令信息，或者平台回复的信息
 */
typedef struct {
    char* content;          ///< 组装完毕的信令信息，或者平台回应的信息
    int content_len;        ///< 信息长度
    unsigned int cmd_id;    ///< 与平台交互的信令ID
    unsigned char msg_type; ///< 信令交互类型, 0-请求, 1-响应
    unsigned int msg_seq;   ///< 序号
}ezDevSDK_special_trans_cmd_msg;

#define EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN                64
#define EZDEVSDK_SPECIAL_FACE_DET_URL_MAX_LEN               100
#define EZDEVSDK_SPECIAL_FACE_DET_MEMBER_IMG_MAX_NUM        5
#define EZDEVSDK_SPECIAL_FACE_DET_MEMBER_MAX_NUM            30

#define EZDEVSDK_SPECIAL_CLEAR_MEMBER           0
#define EZDEVSDK_SPECIAL_DELETE_MEMBER          1

/**
 * \brief 人脸识别图片信息结构体
 */
typedef struct{
    char imgId[EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
    char url[EZDEVSDK_SPECIAL_FACE_DET_URL_MAX_LEN+1];
}ezDevSDK_special_face_det_image_info_s;

/**
 * \brief 人脸信息结构体
 */
typedef struct {
    char memberId[EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
    char labelName[EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
    ezDevSDK_special_face_det_image_info_s imgInfo[EZDEVSDK_SPECIAL_FACE_DET_MEMBER_IMG_MAX_NUM];
    int tagId;
    char alarmFlag;
}ezDevSDK_special_face_det_member_info_s;
/**
 * \brief 人脸管理帧结构体
 */
typedef struct{
    int totalNum;
    char userId[EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
    ezDevSDK_special_face_det_member_info_s *memberInfo;
}ezDevSDK_special_face_det_sync_manage_s;
/**
 * \brief 编辑人脸信息结构体
 */
typedef struct {
    char userId[EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
    char memberId[EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
    char labelName[EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
    char alarmFlag;
    int tagId;
    int deleteImgNum;
    int addImgNum;
    char deleteImgInfo[EZDEVSDK_SPECIAL_FACE_DET_MEMBER_IMG_MAX_NUM][EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
    ezDevSDK_special_face_det_image_info_s addImgInfo[EZDEVSDK_SPECIAL_FACE_DET_MEMBER_IMG_MAX_NUM];
}ezDevSDK_special_edit_face_det_info_s;

/**
 * \brief 删除人脸管理帧结构体
 */
typedef struct{
    char type;//EZDEVSDK_SPECIAL_CLEAR_MEMBER,EZDEVSDK_SPECIAL_DELETE_MEMBER
    char memberId[EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
}ezDevSDK_special_face_det_delete_info_s;
/**
 * \brief 人脸坐标信息
 */
typedef struct{
    double width;
    double x;
    double y;
    double height;
}ezDevSDK_special_face_coordinate_info_s;
/**
 * \brief 人脸建模图片状态结构体
 */
typedef struct{
    ezDevSDK_special_face_coordinate_info_s faceCoor;
    char imgId[EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
    char result;//0表示失败,1表示成功,2表示建模中
}ezDevSDK_special_face_det_model_info_s;
/**
 * \brief 人脸建模成员状态结构体
 */
typedef struct{
    char memberId[EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
    int imgNum;
    ezDevSDK_special_face_det_model_info_s imgResult[EZDEVSDK_SPECIAL_FACE_DET_MEMBER_IMG_MAX_NUM];
}ezDevSDK_special_face_det_model_result_s;

/**
 * \brief 人脸建模上报结果结构体
 */
typedef struct{
    char userId[EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
    int modelResultNum;
    ezDevSDK_special_face_det_model_result_s* modelResult;
}ezDevSDK_special_face_det_report_info_s;

/**
 * \brief 人脸库成员信息结构体
 */
typedef struct{
    char memberId[EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
    int  imgNum; //记录下面数组有效图片数量
    int  imgSrc; //图片来源0-手动新增2-批量导入
    char imgIds[EZDEVSDK_SPECIAL_FACE_DET_MEMBER_IMG_MAX_NUM][EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
}ezDevSDK_special_face_det_member_id_s;

/**
 * \brief 查询人脸库信息结构体
 */
typedef struct{
    int memberNum;
    char userId[EZDEVSDK_SPECIAL_FACE_DET_ID_MAX_LEN+1];
    ezDevSDK_special_face_det_report_info_s *reportInfo;//出参,dev返回给sdk
    ezDevSDK_special_face_det_member_id_s* memberId;
}ezDevSDK_special_face_det_query_info_s;

/*! \} */

#endif
