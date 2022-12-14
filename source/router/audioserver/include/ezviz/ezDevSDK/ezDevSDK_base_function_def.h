/**
 * \file		ezDevSDK_base_function_def.h
 *
 * \brief		基础功能领域对外结构体定义、宏定义
 *
 * \copyright	HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 *
 * \author		xiangbin
 *
 * \date		2017/4/17
 */

#ifndef H_EZDEVSDK_BASE_FUNCTION_DEF_H_
#define H_EZDEVSDK_BASE_FUNCTION_DEF_H_
#include <time.h>
#include "ezDevSDK_error.h"
#include <arpa/inet.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define UPGRADE_FILE_NAME_LEN	    256			///< 升级包最长文件名
#define EZVIZ_NAME_LEN 			    128			///< 和设备相关的可见字符最大长度，如型号、序列号、操作码
#define EZVIZ_MAX_DISKNUM           16			///< 设备硬盘最大数量
#define EZVIZ_MAX_ALGORITHMCONFIG   10			///< 算法配置信息体的最大数量


/** \addtogroup BasicFunc_Module
 *  \{
 */

//-------------------------------------错误码------------------------------------------------------

/**
 * \brief 萤石SDK基础功能领域的接口错误码，由SDK返回给调用者
 * \note 公用错误码，请参考ezDevSDK_error.h
 */
typedef enum {
    EZDEVSDK_BASE_FUNC_SUCESS                   = 0X00000000, ///< 成功
    EZDEVSDK_BASE_FUNC_CODE_BASE                = 0XD1100000,   ///< 基础功能领域接口错误码起始值，D表示接口错误码，1100，表示基础功能领域，最后三位表示具体错误

    ///< 0XD1100000 - 0XD110001F 公共错误码
    EZDEVSDK_BASE_FUNC_CODE_FAIL                = EZDEVSDK_BASE_FUNC_CODE_BASE + EZDEVSDK_ERROR_FAIL,
    EZDEVSDK_BASE_FUNC_CODE_NOT_INITED          = EZDEVSDK_BASE_FUNC_CODE_BASE + EZDEVSDK_ERROR_NOT_INITED,
    EZDEVSDK_BASE_FUNC_CODE_INVALID_PARAM       = EZDEVSDK_BASE_FUNC_CODE_BASE + EZDEVSDK_ERROR_INVALID_PARAM,
    EZDEVSDK_BASE_FUNC_CODE_NOT_SUPPORT         = EZDEVSDK_BASE_FUNC_CODE_BASE + EZDEVSDK_ERROR_NOT_SUPPORT,
    EZDEVSDK_BASE_FUNC_CODE_NOT_ENOUGH_MEMERY   = EZDEVSDK_BASE_FUNC_CODE_BASE + EZDEVSDK_ERROR_NOT_ENOUGH_MEMERY,
    EZDEVSDK_BASE_FUNC_CODE_INIT_FAIL           = EZDEVSDK_BASE_FUNC_CODE_BASE + EZDEVSDK_ERROR_INIT_FAIL,
    EZDEVSDK_BASE_FUNC_CODE_THREAD_CREATE_FAIL  = EZDEVSDK_BASE_FUNC_CODE_BASE + EZDEVSDK_ERROR_THREAD_CREATE_FAIL,
    EZDEVSDK_BASE_FUNC_CODE_RSP_TIMEOUT         = EZDEVSDK_BASE_FUNC_CODE_BASE + EZDEVSDK_ERROR_RSP_TIMEOUT,
    EZDEVSDK_BASE_FUNC_CODE_MSG_DATA_INVAILD    = EZDEVSDK_BASE_FUNC_CODE_BASE + EZDEVSDK_ERROR_MSG_DATA_INVAILD,

}EZDEVSDK_BASE_FUNC_CODE_E;

/**
 * \brief 由调用层设置的协议错误码，内部转化后传递到客户端
 */
typedef enum {
    EZDEVSDK_BASE_FUNC_PROTOCOL_CODE_BASE = 0XF1100000,   ///< 基础功能领域协议错误码起始值，F表示协议错误码，1100，表示基础功能领域，最后三位表示具体错误
} EZDEVSDK_BASE_FUNC_PROTOCOL_CODE_E;


//-------------------------------------初始化信息--------------------------------------------------

/**
 *  \brief 日志相关参数
 */
typedef struct {
    char path[64];								///< 日志路径
    int log_size;								///< 日志文件大小上限
    int need_buf;								///< 日志是否需要缓存到内存，对于内存文件系统，一般不需要
    int (*log_write)(char *pbuf, int len);  
} ezDevSDK_base_function_log_info;

/**
 *  \brief 基础功能领域和设备通讯的通用消息
 */
typedef struct {
    int  type;									///< 消息类型，\ref EZDEVSDK_BASE_FUNCTION_MSG_TO_DEV_TYPE_E
    void *data;									///< 消息内容，根据type解释为不同具体类型
    int  len;									///< 消息内容缓冲区大小
} ezDevSDK_base_function_msg_to_dev;

/**
 *  \brief 基础功能领域获取设备信息的通用载体
 */
typedef struct {
    int	 type;									///< 信息类型，\ref EZDEVSDK_BASE_FUNCTION_RUNTIME_INFO_E
    void *data;									///< 信息内容，根据type解释为不同具体类型
    int  len;									///< 信息内容缓冲区大小
} ezDevSDK_base_function_runtime_info;

typedef enum
{  
    no_err,
    retry_time_limit,       ///< 重试次数达到上限
}ezDevSDK_down_err_code;

/**
 * \brief   升级包下载的状态
 * \note    根据参数判断接口的运行状态
 */
typedef enum
{  
    OTA_DOWNLOAD_START,   ///< percent != 0,data ; ///< 下载
    OTA_DOWNLOAD_END,     ///< percent = 0, data= NULL,errcode =0;  ///< 下载完成退出
    OTA_DOWNLOAD_ERR,     ///< percent = 0,data = NULL, errcode != 0; ///< 下载出错,退出
}ezDevSDK_down_status;
/**
 * \brief   设备的回调函数定义，用来接收基础功能领域的消息和信息查询
 * \warning 不可以阻塞
 * \note    get_runtime_info回调函数中，对于不支持的功能，返回值请填EZDEVSDK_ERROR_NOT_SUPPORT
 */
typedef struct 
{
    int (*recv_msg)(ezDevSDK_base_function_msg_to_dev *msg);            ///< 消息回调接口，设备通过此接口接收基础功能领域的消息
	int (*get_runtime_info)(ezDevSDK_base_function_runtime_info *info);	///< 基础功能领域获取设备运行时信息的回调接口
    int (*ota_notifier)(int percent, void* data, int data_len, ezDevSDK_down_err_code errcode, ezDevSDK_down_status status);///< 升级包下载接口,内部会进行重试
} ezDevSDK_base_function_callback;

/**
* \brief	基础功能领域的初始化参数
*/
typedef struct 
{
    ezDevSDK_base_function_log_info log_info;							///< 日志参数
    ezDevSDK_base_function_callback cb;									///< 回调函数
    int channel_num_max;                                                ///< 最大通道号
} ezDevSDK_base_function_init_info;


//-------------------------------------SDK通知应用层的消息-----------------------------------------

/**
* \brief  基础功能领域和设备之间通讯的消息类型集合
*/
typedef enum {
	EZDEVSDK_BASE_FUNC_ON_SET_USERID,				///< 设置用户，建立设备-用户绑定关系（添加/删除），消息体: \c ezDevSDK_base_function_user_info
	EZDEVSDK_BASE_FUNC_ON_START_UPGRADE,			///< 请求设备升级，消息体: NULL
	EZDEVSDK_BASE_FUNC_ON_UPGRADE_PACKET_INFO,		///< 设备升级包信息，消息体: \c ezDevSDK_base_function_upgrade_files
	EZDEVSDK_BASE_FUNC_ON_SYS_OPERATION,  			///< 对设备进行系统操作，消息体: \c ezDevSDK_base_function_sys_operation
	EZDEVSDK_BASE_FUNC_ON_FORMAT_DISK,				///< 向设备格式化本地存储，消息体: \c ezDevSDK_base_function_format_disk
	EZDEVSDK_BASE_FUNC_ON_SET_WIFI_PARAM,			///< 向设备配置wifi信息，消息体: \c ezDevSDK_base_function_wifi_param
	EZDEVSDK_BASE_FUNC_ON_SET_TIME_ZONE,			///< 向设备设置时区，消息体: \c ezDevSDK_base_function_time_zone
	EZDEVSDK_BASE_FUNC_ON_SET_DAYLIGHT_SAVING_TIME,	///< 向设备设置夏令时，消息体: \c ezDevSDK_base_function_dadylight_saving_time_period
	EZDEVSDK_BASE_FUNC_ON_SET_TIME_FORMAT,			///< 向设备设置时间格式，消息体: \c ezDevSDK_base_function_time_format
	EZDEVSDK_BASE_FUNC_ON_SET_TIME,					///< 向设备设置时间，消息体: \c ezDevSDK_base_function_server_time
	EZDEVSDK_BASE_FUNC_ON_SET_NTP_INFO,				///< 向设备设置NTP信息，消息体: \c ezDevSDK_base_function_ntp_info
	EZDEVSDK_BASE_FUNC_ON_SET_LANGUAGE,				///< 向设备设置语言，消息体: \c ezDevSDK_base_function_language
	EZDEVSDK_BASE_FUNC_ON_SET_LOG_SERVER_ADDRESS,	///< 向设备设置日志服务器地址，消息体: \c ezDevSDK_base_function_log_server_address
	EZDEVSDK_BASE_FUNC_ON_SET_SWITCH_STATUS,		///< 向设备设置开关状态，消息体: \c ezDevSDK_base_function_switch_status
	EZDEVSDK_BASE_FUNC_ON_SET_ENCRYPT_STATUS,		///< 向设备设置加密密码和状态，消息体: \c ezDevSDK_base_function_encrypt_status
	EZDEVSDK_BASE_FUNC_ON_ADD_PORT_NEED_MAP,		///< \todo 暂未实现
	EZDEVSDK_BASE_FUNC_ON_AUTO_ONLINE,				///< 触发自动上下线条件，基础领域通知设备上下线，消息体: ezDevSDK_base_function_auto_online
	EZDEVSDK_BASE_FUNC_ON_AUTO_ONLINE_CFG,			///< 向设备设置自动上线下配置参数，消息体: \c ezDevSDK_base_function_auto_week_sched
	EZDEVSDK_BASE_FUNC_ON_SET_ALGORITHM_LEVEL,		///< 向设备设置移动侦测，儿歌播放等级别，消息体: \c ezDevSDK_base_function_algorithm_level
	EZDEVSDK_BASE_FUNC_ON_SET_NEW_LBS_DOMAIN,		///< 向设备设置重定向后的LBS地址，消息体: \c ezDevSDK_base_function_lbs_domain
	EZDEVSDK_BASE_FUNC_ON_TRANS_PROTOCOL,			///< APP和设备之间的透明传输，消息体: \c ezDevSDK_base_function_trans_protocol_msg
	EZDEVSDK_BASE_FUNC_ON_SET_LOGO_INFO,	        ///< 设置LOGO信息，消息体: \c ezDevSDK_base_function_osd_info_page
	EZDEVSDK_BASE_FUNC_ON_SET_DCLOG_CFG,        	///< 设置dclog的配置，消息体: \c ezDevSDK_base_function_dclog_Cfg_S
	EZDEVSDK_BASE_FUNC_ON_SET_CHIME_MUSIC,          ///< 设置chime的铃声，消息体: \c ezDevSDK_base_function_ChimeMusic_S
	EZDEVSDK_BASE_FUNC_ON_TRANS_CMD_MSG,			///< 由外部实现的请求所对应的平台回应信息，消息体: \c ezDevSDK_base_function_trans_cmd_msg
    EZDEVSDK_BASE_FUNC_ON_FORCE_OFFLINE,            ///< 告知设备平台要求强制下线，消息体: NULL
    EZDEVSDK_BASE_FUNC_ON_SET_SLEEP_PLAN_CFG,       ///<设置休眠计划
    EZDEVSDK_BASE_FUNC_ON_SET_HOST_PARSE_RESULT,    ///<向设备端设置域名解析结果, 消息体:\c ezDevSDK_base_function_host_parse_result
    EZDEVSDK_BASE_FUNC_ON_DEV_STATIC_DNS,           ///<向设备端设置静态DNS, 消息体:\c ezDevSDK_base_function_msg_to_dev
    EZDEVSDK_BASE_FUNC_ON_DEV_WIFI_MODE,            ///<向设备端设置wifi模式, 消息体:\c ezDevSDK_base_function_msg_to_dev
	EZDEVSDK_BASE_FUNC_ON_SET_INTERRCONNECTION_AUTH_KEY,///<互联互通秘钥
	EZDEVSDK_BASE_FUNC_ON_TOUCH_BIND,               ///< 物理接触绑定，消息体:\c ezDevSDK_base_function_touch_bind
    EZDEVSDK_BASE_FUNC_ON_SET_OTHER_DEVICE_WIFI,    ///<使用当前设备对其他设备配网,消息体:\c ezDevSDK_base_function_target_device_info
	EZDEVSDK_BASE_FUNC_ON_SET_INDICATOR_LIGHT,      ///<设置设备夜间指示灯，消息体: \c ezDevSDK_base_function_IndicatorLight_S
	EZDEVSDK_BASE_FUNC_ON_DEV_AUDITION,             ///<音效试听:\c ezDevSDK_base_function_sound_audition
    EZDEVSDK_BASE_FUNC_ON_PROFILE_INFO,		        ///< 设备升级包信息，消息体: \c ezDevSDK_base_function_profile_info
    EZDEVSDK_BASE_FUNC_ON_QUERY_BIND_TOKEN_RSP,     ///< 设备主动查询用户绑定使用的token响应消息,消息体:\c ezDevSDK_base_query_bind_token_rsp
	EZDEVSDK_BASE_FUNC_ON_UPDATE_DNS_PARSE_RESULT_LIST,      ///<向设备端更新域名解析结果, 消息体:\c ezDevSDK_base_function_dns_list
	EZDEVSDK_BASE_FUNC_ON_SET_AUDIOIN_SWITCH_STATUS,///< 向设备设置音频输入相关功能的开关状态，消息体: \c ezDevSDK_base_function_v3_switch_status
	EZDEVSDK_BASE_FUNC_ON_DETECT_EXPANSION_CARD,	///< 平台向设备触发扩容卡检测，消息体:NULL
} EZDEVSDK_BASE_FUNCTION_MSG_TO_DEV_TYPE_E;

/**
* \brief  透明传输的消息类型
*/
typedef enum {
	EZDEVSDK_BASE_FUNC_TRANS_PROTOCOL_MSG_REQ = 1,	///< 请求
	EZDEVSDK_BASE_FUNC_TRANS_PROTOCOL_MSG_RSP,		///< 响应
}EZDEVSDK_BASE_FUNCTION_TRANS_PROTOCOL_MSG_TYPE_E;

/**
* \brief  算法类型
*/
typedef enum {
	EZDEVSDK_BASE_FUNC_ALGORITHM_TYPE_MOTION_DETECT = 0,	        ///< 移动侦测灵敏度
	EZDEVSDK_BASE_FUNC_ALGORITHM_TYPE_BABY_CRY,    		            ///< 婴儿啼哭哭声时长
	EZDEVSDK_BASE_FUNC_ALGORITHM_TYPE_PIR,    		                ///< PIR灵敏度
	EZDEVSDK_BASE_FUNC_ALGORITHM_TYPE_PIR_EX,                       ///< PIR灵敏度 新(0-100)
	EZDEVSDK_BASE_FUNC_ALGORITHM_TYPE_HUMAN_DETECT,                 ///< 人形检测灵敏度 100档  
    EZDEVSDK_BASE_FUNC_ALGORITHM_TYPE_BABY_DETECT,                  ///< 婴儿啼哭检测灵敏度 100档
	EZDEVSDK_BASE_FUNC_ALGORITHM_TYPE_HUMAN_DETECT_EX,              ///< 人形检测灵敏度 3档
	EZDEVSDK_BASE_FUNC_ALGORITHM_TYPE_NOISY_DETECT,                 ///< 噪声检测灵敏度 100档
}EZDEVSDK_BASE_FUNCTION_ALGORITHM_TYPE_E;

/**
* \brief  升级文件的服务器信息
*/
typedef struct {
	char address[64];								///< 升级服务器ip
	unsigned short port;							///< 升级服务器端口
	char reserve[2];     							///< 节对齐
	char user_name[64];  							///< 升级服务器登入用户名
	char password[64];   							///< 升级服务器登入密码
	int max_try_time;    							///< 最大尝试次数
} ezDevSDK_base_function_upgrade_server;

/**
* \brief   升级文件的信息
*/   
typedef struct  {
	ezDevSDK_base_function_upgrade_server http_server;	///< http服务器
	ezDevSDK_base_function_upgrade_server https_server;	///< https服务器
	ezDevSDK_base_function_upgrade_server ftp_server;	///< ftp服务器
	char file_name[UPGRADE_FILE_NAME_LEN];				///< 软件包文件名
	char check_sum[64];									///< 校验和
	char version[64];									///< 版本
	char describe[64];									///< 描述信息
	int size;											///< 软件包大小，单位Byte
    int ota_type;                                       ///< 升级类型，0完整包、1差分包
} ezDevSDK_base_function_upgrade_file;

/**
* \brief   物模型功能描述文件信息
*/   
typedef struct  {
    char http_server[128];								///< http服务器
    char https_server[128];								///< https服务器
	char check_sum[64];									///< 校验和
    char dev_serial[64];                                ///< 设备序列号
	int expire;											///< 下载地址失效时间
} ezDevSDK_base_function_profile_info;

/**
 * \brief   设备升级包信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_UPGRADE_PACKET_INFO
 */
typedef struct {
	int num;											///< 升级信息数组大小
	ezDevSDK_base_function_upgrade_file upgrade_file[2];///< 升级信息数组
} ezDevSDK_base_function_upgrade_files;


/**
 * \brief   设备上报的模块详细信息
 */
typedef struct {
	int  index;											            ///< 设备模块索引
	char type[64];                                                  ///< 设备独立的模块类型      
	char version[64];                                               ///< 设备上独立模块的版本号
} ezDevSDK_base_function_device_module_info;

/**
 * \brief   设备上报的模块列表,对应消息:EZDEVSDK_BASE_FUNC_RTINFO_GET_DEVICE_MODULE_INFO
 */
typedef struct {
	char dev_serial[64];                                                 ///< 设备序列号
	int  num;											                 ///< 设备模块数量
	ezDevSDK_base_function_device_module_info *pstmodule_info;          ///< 设备模块信息
} ezDevSDK_base_function_device_module_list;

/**
 * \brief   平台通知设备需要升级的模块信息数组，对应消息:EZDEVSDK_BASE_FUNC_RTINFO_GET_DEVICE_MODULE_NUM
 */

typedef struct {
	int num;											///< 设备模块数量,注意该参数必须和ezDevSDK_base_function_device_module_list参数num保持一致
} ezDevSDK_base_function_dev_module_num;

/**
 * \brief   设备视频加密密码和状态，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_ENCRYPT_STATUS, EZDEVSDK_BASE_FUNC_RT_INFO_GET_ENCRYPT_KEY
 */
typedef struct {
	int enable;											///< 0:不启用, 1:启用, 2:修改密码
	char password[64];									///< 加密密码
}  ezDevSDK_base_function_encrypt_status;

/**
 * \brief   wifi配置信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_WIFI_PARAM
 */
typedef struct {
	char ssid[64];										///< WIFI热点名称
	char password[64];									///< 无线AP的认证密码
	int security;										///< 加密方式, 0:无, 1:WEP, 2:WAP-PSK, 3:WPA-PSK/WPA2-PSK, 4:WPA2-PSK, 5:其他
} ezDevSDK_base_function_wifi_param;

/**
 * \brief   时间信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_TIME
 */
typedef struct {
	time_t server_time;									///< 时间
} ezDevSDK_base_function_server_time;

/**
 * \brief   时区信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_TIME_ZONE, EZDEVSDK_BASE_FUNC_RT_INFO_GET_TIME_ZONE
 */
typedef struct {
	int hour;											///< 小时
	int minute;											///< 分钟
	int no;												///< 时区
} ezDevSDK_base_function_time_zone;

/**
 * \brief   时间格式，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_TIME_FORMAT, EZDEVSDK_BASE_FUNC_RT_INFO_GET_TIME_FORMAT
 */
typedef struct {
	int format;											///< 时间格式, 0:YYYY-MM-DD, 1:MM-DD-YYYY, 2:DD-MM-YYYY
} ezDevSDK_base_function_time_format;

 /**
  * \brief  系统操作方式，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SYS_OPERATION
  */
typedef struct {
	int type;											///< 系统操作方式，1:shutdown, 2:reboot, 3:reset, 4:self-test
	int delay;											///< 延迟(s)
} ezDevSDK_base_function_sys_operation;

/**
 * \brief   本地存储格式化，对应消息类型: EZDEVSDK_BASE_FUNC_ON_FORMAT_DISK
 */
typedef struct {
	int index;											///< 硬盘编号，0:全部格式化
	int format_type;                                    ///< 格式化类型0:普通格式化 1:完全格式化
} ezDevSDK_base_function_format_disk;

/**
 * \brief   用户信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_USERID
 */
typedef struct {
	char user_id[64];									///< 绑定设备的用户ID，"0000000000000000"表示解除绑定
} ezDevSDK_base_function_user_info;

/**
 * \brief   日志服务器信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_LOG_SERVER_ADDRESS
 */
typedef struct {
	char address[64];									///< 日志服务器域名
	char path[64];										///< 日志路径
	char session[64];									///< 用来认证合法性
	int port;											///< 日志服务器端口
	int daynum;											///< 需要收集的天数（最近的天数）
	int loglevel;                                       ///< 日志等级,如果为-1表示下发内容不包含该字段
	int module;                                         ///< 主模块ID,如果为-1表示下发内容不包含该字段
	int sub_module;                                     ///< 子模块ID,如果为-1表示下发内容不包含该字段
	char extinfo[256];                                  ///< 扩展字段,如果为NULL表示下发内容不包含该字段                                
} ezDevSDK_base_function_log_server_address;

 /**
  * \brief  夏令时信息
  */
typedef struct {
	int mouth;     										///< 月份(1~12)
	int day;       										///< 天数
	int week_index;										///< 第几个星期，1~5，最后一个星期传99
	int week_day;  										///< 星期几(1~7)，传-1表示按具体日志来
	int hour;      										///< 开始调整的小时
	int minute;    										///< 开始调整的分钟
}ezDevSDK_base_function_dadylight_saving_time;

 /**
  * \brief  夏令时设置信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_DAYLIGHT_SAVING_TIME, EZDEVSDK_BASE_FUNC_RT_INFO_GET_DAYLIGHT_SAVING_TIME
  */
typedef struct {
	int offtime;										///< 偏移时间，单位为分钟
	int enable;											///< 是否开启夏令时，0:不开启，1:开启
	ezDevSDK_base_function_dadylight_saving_time start;	///< 夏令时开启时间
	ezDevSDK_base_function_dadylight_saving_time end;	///< 夏令时结束时间
} ezDevSDK_base_function_dadylight_saving_time_period;

/**
 * \brief   NTP校时服务器信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_NTP_INFO
 */
typedef struct {
	char host[64];										///< 服务器域名
	int port;											///< 服务器端口
	int interval;  										///< 校时(s)
	int hour;      										///< 校时(h)
	int minute;    										///< 校时(m)
}ezDevSDK_base_function_ntp_info;

/**
 * \brief   语言信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_LANGUAGE, EZDEVSDK_BASE_FUNC_RT_INFO_GET_DEV_LANGUAGE
 */
typedef struct {
	char language[32];									///< 语言
} ezDevSDK_base_function_language;

#define EZDEVSDK_BASEFUN_SWITCH_TYPE_DEV_VOICE_PROMPT       1   ///< 设备提示音使能
#define EZDEVSDK_BASEFUN_SWITCH_TYPE_DEV_INDICATOR_LIGHT    2   ///< 设备指示灯使能
#define EZDEVSDK_BASEFUN_SWITCH_TYPE_DEV_SLEEP    			3   ///< 设备休眠使能
#define EZDEVSDK_BASEFUN_SWITCH_TYPE_DEV_AUTO_SLEEP         4   ///< 设备自动休眠使能
#define EZDEVSDK_BASEFUN_SWITCH_TYPE_DEV_UPNP               5   ///< 设备UPNP功能使能
#define EZDEVSDK_BASEFUN_SWITCH_TYPE_DEV_PIR_TEST           6   ///< 设备pir灵敏度调试使能
#define EZDEVSDK_BASEFUN_SWITCH_TYPE_DEV_WIRELESS_TEST_MODE 7   ///< 无线测试模式使能
#define EZDEVSDK_BASEFUN_SWITCH_TYPE_MAX    			    8   ///< 基础领域最大使能开关数

/**
 * \brief   基础领域开关使能信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_SWITCH_STATUS, EZDEVSDK_BASE_FUNC_RT_INFO_GET_SWITCH_STATUS
 */
typedef struct {
    int channel;										///< 通道
	int type;											///< 开关类型，1:语音提示，2:指示灯
	int enable;											///< 是否开启，0:关闭，1:开启
} ezDevSDK_base_function_switch_status;

#define EZDEVSDK_BASEFUN_SWITCH_TYPE_DEV_AUDIOIN_DIR_PICKUP         1     ///< 设备MIC定向拾音使能
#define EZDEVSDK_BASEFUN_SWITCH_TYPE_DEV_AUDIOIN_FUR_PICKUP         2     ///< 设备MIC远程拾音使能
#define EZDEVSDK_BASEFUN_SWITCH_TYPE_DEV_AUDIOIN_WINDNOISE_REDUCE   3     ///< 设备MIC降风噪使能

/**
 * \brief   基础领域开关使能信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_AUDIOIN_SWITCH_STATUS, EZDEVSDK_BASE_FUNC_RT_INFO_GET_AUDIOIN_SWITCH_STATUS
 */
typedef struct {
    int channel;										///< 通道
	int type;											///< 开关类型，eg:EZDEVSDK_BASEFUN_SWITCH_TYPE_DEV_AUDIOIN_DIR_PICKUP
	int enable;											///< 是否开启，0:关闭，1:开启
} ezDevSDK_base_function_v3_switch_status;

/**
 * \brief   映射端口信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_ADD_PORT_NEED_MAP
 */
typedef struct {
    unsigned short local_port;                          ///< 本地端口
    char porto[8];                                      ///< "TCP"or"UDP"
    char des[16];                                       ///< 端口描述信息
}ezDevSDK_base_function_port_need_map;

/**
 * \brief   设备上下线通知，对应消息类型: EZDEVSDK_BASE_FUNC_ON_AUTO_ONLINE
 */
typedef struct {
	int online;											///< 触发自动上下线条件，通知设备上下线 1:上线 0:下线
} ezDevSDK_base_function_auto_online;

/**
 * \brief   每周定时调度计划，对应消息类型: EZDEVSDK_BASE_FUNC_RT_INFO_GET_AUTO_ONLINE_CFG
 * \note    只在获取默认配置参数的时候用
 */
typedef struct {
    time_t begin;										///< 开始时间, 高16位表示小时, 低16位表示分钟
    time_t end;											///< 结束时间, 如0X0017003B, 即"23:59"
} ezDevSDK_base_function_auto_time_period;

/**
 * \brief   每周定时调度计划，对应消息类型: EZDEVSDK_BASE_FUNC_ON_AUTO_ONLINE_CFG, EZDEVSDK_BASE_FUNC_RT_INFO_GET_AUTO_ONLINE_CFG
 */
typedef struct {
    int status;
    ezDevSDK_base_function_auto_time_period plan[7][4];	///< 每周7天, 每天4个时间段
} ezDevSDK_base_function_auto_week_sched;

/**
 * \brief   能力等级信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_ALGORITHM_LEVEL, EZDEVSDK_BASE_FUNC_RTINFO_GET_ALGORITHM_LEVEL
 */
typedef struct {
    int channel;										///< 入参
    int type;											///< EZDEVSDK_BASE_FUNCTION_ALGORITHM_TYPE_E
    int value;											///< 0: 1~6 3:0~100
} ezDevSDK_base_function_algorithm_level;


/**
 * \brief   设备能力级上报,请在设备上线的时候调用一次该接口,请勿多次调用该接口
 */
typedef struct {
    char model[64];										///< 设备型号 例:CS-C6P-7A3WFR
    char version[32];	                                ///< 版本号   例:V5.2.7 build 190119
	char category[64];                                  ///< 类目     例 "C6P"
	char* Capacity;									    ///< 设备能力级,json格式字符串,例"{\"1\":\"1\",\"55\":\"1\",\"2\":\"1\",\"3\":\"1\",\"4\":\"1\"}"
    int   Capacity_len;                                 ///< json字符串的长度
	int   channelNo;								    ///< 通道号
} ezDevSDK_base_function_device_capacity;

/**
 * \brief   设备通道能力级
 */
typedef struct {
    int    channelNo;	                                ///< 通道号
	char*  capacity;                                    ///< 通道能力级，例	"{\"1\":\"1\",\"55\":\"1\",\"2\":\"1\",\"3\":\"1\",\"4\":\"1\"}"				    
} ezDevSDK_base_channel_capacity;

/**
 * \brief   设备通道能力级上报
 */
typedef struct{  
    int  channelNum;	                                          ///< 通道数
	char serial[64];                                              ///< 设备序列号	
    ezDevSDK_base_channel_capacity  *channel_capacity;            ///< 通道能力级参数 ,指针指向内存由外部释放                                                     
} ezDevSDK_base_channel_capacity_array;

/**
 * \brief   重定向后的LBS信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_NEW_LBS_DOMAIN
 */
typedef struct {
    char domain[64];									///< 新的LBS域名
}ezDevSDK_base_function_lbs_domain;

/**
 * \brief   设备应用层和APP间的透传信息体，对应消息类型: EZDEVSDK_BASE_FUNC_ON_TRANS_PROTOCOL
 */
typedef struct {
	EZDEVSDK_BASE_FUNCTION_TRANS_PROTOCOL_MSG_TYPE_E msg_type;	///< 消息类型
	unsigned int iSeq;											///< 消息序号
	char *pProtocolType;										///< 协议类型
	int iProtocolTypeLen;										///< 协议类型长度
	char *pContent;												///< 协议数据
	int iContentLen;											///< 协议数据长度
} ezDevSDK_base_function_trans_protocol_msg;


/**
 * \brief   OSD信息内容
 */
typedef struct {
    int channel;        ///< 通道号
    char serial[64];    ///< 对应的序列号(或者为通道号)
    char name[128];     ///< OSD名称
}ezDevSDK_base_function_osd_info;

/**
 * \brief   批量OSD信息体，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_LOGO_INFO
 * \note    支持批量设置和批量上报
 */
typedef struct {
    int max_count;
    ezDevSDK_base_function_osd_info *pstOsdInfo;     ///< OSD信息体数组
}ezDevSDK_base_function_osd_info_page;

/**
 * @brief   Chime铃声的类型
 */
typedef enum {
    EZDEVSDK_BASE_FUNC_CHIME_MUSIC_DOORBELL = 0,    ///< door bell type
    EZDEVSDK_BASE_FUNC_CHIME_MUSIC_PIR      = 1,    ///< pir type

    EZDEVSDK_BASE_FUNC_CHIME_MUSIC_MAX,
}EZDEVSDK_BASE_FUNCTION_CHIME_MUSIC_TYPE_E;

/**
 * \brief   Chime铃声信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_CHIME_MUSIC, EZDEVSDK_BASE_FUNC_RTINFO_GET_CHIME_MUSIC
 */
typedef struct
{    
    unsigned int uiMusic[EZDEVSDK_BASE_FUNC_CHIME_MUSIC_MAX];   ///< chime铃声状态
    unsigned int uiVolume;                                      ///< chime音量大小
} ezDevSDK_base_function_ChimeMusic_S;

/**
 * \brief   设备指示灯，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_INDICATOR_LIGHT, EZDEVSDK_BASE_FUNC_RTINFO_GET_INDICATOR_LIGHT
 */
typedef struct
{    
    char enable;   ///< 使能开关   0表示关，1表示开
    char mode;     ///< 模式，0表示仅白天开启  1表示始终开启
} ezDevSDK_base_function_IndicatorLight_S;

/**
 * \brief 外部组装的信令信息，或者平台回复的信息，对应消息类型: EZDEVSDK_BASE_FUNC_ON_TRANS_CMD_MSG, EZDEVSDK_BASE_FUNC_TRANS_CMD_MSG
 */
typedef struct {
    char* content;          ///< 组装完毕的信令信息，或者平台回应的信息
    int content_len;        ///< 信息长度
    unsigned int cmd_id;    ///< 与平台交互的信令ID
    unsigned char msg_type; ///< 信令交互类型, 0-请求, 1-响应
    unsigned int msg_seq;   ///< 序号
}ezDevSDK_base_function_trans_cmd_msg;

/**
 * \brief DNS 解析结果以及对应的域名回调至应用层，对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_HOST_PARSE_RESULT
 */
typedef struct {
    int result;         //0：解析成功  -1：解析失败
    char domain[132];   //需要解析的域名，方便后续排查原因
} ezDevSDK_base_function_host_parse_result;

typedef struct {
    char auth_key[64];
} ezDevSDK_base_function_interrconnection_auth_key;

/**
 * \brief 物理接触绑定结构体，对应消息类型: EZDEVSDK_BASE_FUNC_ON_TOUCH_BIND
 */
typedef struct {
    int token;          //随机值，物理接触后需主动上报给平台
    int period;         //窗口周期
} ezDevSDK_base_function_touch_bind;

#define EZSDK_LAST_DNS_BUF_MAX_NUM 20
typedef struct
{
    in_addr_t addr;
    char serverName[100];
}ezviz_dns_s;
typedef struct
{
    ezviz_dns_s dns_value[EZSDK_LAST_DNS_BUF_MAX_NUM];
}ezDevSDK_base_function_dns_list;

typedef struct {
    char target_serial[64];
} ezDevSDK_base_function_target_device_info;

/**
 * \brief 音效试听
 * 自定义语音: 0-3
 * 内置语音: 101 - 111
 * 提醒模式--滴: 200
 * 警报模式蜂鸣警报声: 201
 * 静音: 202
 */
typedef struct {
	int index ;              ///< 声音文件索引
	int speak_volume;        ///< 扬声器音量
}ezDevSDK_base_function_sound_audition;

typedef struct
{
	unsigned int detect_state;							///< 是否检测中: true-检测中，false-未检测
	unsigned int detect_percent;						///< 检测进度百分比, range:[0,100]
	char detect_result[32]; 							///< 检测结果: "unknown"#未知, "EXPCard"#扩容卡, "NotEXPCard"#非扩容卡 
} ezDevSDK_expansion_card_detection_result;

//-------------------------------------SDK从应用层获取的消息---------------------------------------

/**
 * \brief   基础功能领域从从设备获取信息
 */
typedef enum {
    //以下参数是萤石云下发的配置保存在设备上，当SDK需要时再从设备处获取
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_ENCRYPT_KEY,          		///< 从设备获取视频加密密码，消息体: \c ezDevSDK_base_function_encrypt_status
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_DEV_LANGUAGE,         		///< 从设备获取语言类别，消息体: \c ezDevSDK_base_function_language
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_TIME_ZONE,            		///< 从设备获取时区，消息体: \c ezDevSDK_base_function_time_zone
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_TIME_FORMAT,          		///< 从设备获取时间格式，消息体: \c ezDevSDK_base_function_time_format
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_DAYLIGHT_SAVING_TIME, 		///< 从设备获取夏令时，消息体: \c ezDevSDK_base_function_dadylight_saving_time_period
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_SWITCH_STATUS,        		///< 从设备获取开关状态，消息体: \c ezDevSDK_base_function_switch_status
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_AUTO_ONLINE_CFG,      		///< 从设备获取自动上下线配置，消息体: \c ezDevSDK_base_function_auto_week_sched
    EZDEVSDK_BASE_FUNC_RTINFO_GET_ALGORITHM_LEVEL,       		///< 从设备获取能力等级，消息体: \c ezDevSDK_base_function_algorithm_levels     
    EZDEVSDK_BASE_FUNC_RTINFO_GET_LOGO_INFO,					    ///< 获取当前的OSDlogo信息，消息体: \c ezDevSDK_base_function_osd_info_page
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_SLEEP_PLAN_CFG,              ///<获取休眠计划
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_WIFI_MODE, 			///<获取(猫眼锁WIFI模式，结构体: \c UINT32
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_STATIC_DNS, 		///<获取(猫眼锁静态DNS，结构体: \c UINT32
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_AUDIOIN_SWITCH_STATUS,       ///< 从设备获取音频输入相关功能开关状态，消息体: \c ezDevSDK_base_function_v3_switch_status

    //以下参数是设备运行时信息
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_WIFI_INFO,					///< 从设备获取WIFI信息，消息体: \c ezDevSDK_base_function_ap_infos
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_MAPPED_PORT,					///< 从设备获取端口映射信息，消息体: \c ezDevSDK_base_function_mapped_port
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_DISK_INFO,					///< 从设备获取硬盘配置信息，消息体: \c ezDevSDK_base_function_disk_infos
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_UPGRADE,						///< 从设备获取升级进度，消息体: \c ezDevSDK_base_function_upgrade
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_NETWORK_STATUS,				///< 从设备获取网络状态，消息体: \c ezDevSDK_base_function_network_status
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_NETWORK_INTERFACE,			///< 从设备获取网口信息，消息体: \c ezDevSDK_base_function_network_interface
    EZDEVSDK_BASE_FUNC_RT_INFO_GET_NETWORK_GATEWAY_ADDR,		///< 从设备获取网关IP，消息体: \c ezDevSDK_base_function_gateway_addr

    EZDEVSDK_BASE_FUNC_RTINFO_GET_DCLOG_CFG,                    ///< 获取dclog的配置，消息体: \c ezDevSDK_base_function_dclog_Cfg_S
    EZDEVSDK_BASE_FUNC_RTINFO_GET_CHIME_MUSIC,                  ///< 从设备获取chime的铃声设置，消息体: \c ezDevSDK_base_function_ChimeMusic_S
    EZDEVSDK_BASE_FUNC_RTINFO_GET_USER_VERIFY_STATUS,           ///< 获取用户身份认证的状态，消息体 \c ezDevSDK_base_function_user_verify_status_S
	EZDEVSDK_BASE_FUNC_RTINFO_GET_DEVICE_MODULE_NUM,            ///< 获取当前设备的模块数量，消息体:\c ezDevSDK_base_function_dev_module_num
	EZDEVSDK_BASE_FUNC_RTINFO_GET_DEVICE_MODULE_INFO,           ///< 获取当前设备所有模块信息，消息体:\c ezDevSDK_base_function_device_module_list
	EZDEVSDK_BASE_FUNC_RTINFO_GET_USERID,                       ///<   获取用户，建立设备-用户绑定关系（添加/删除），消息体: \c ezDevSDK_base_function_user_info
	EZDEVSDK_BASE_FUNC_RTINFO_GET_CONFIGURABLE_DEVICE,          ///< 获取当前设备所在网络下可通过IPC配网的设备及SSID，消息体:\c 
	EZDEVSDK_BASE_FUNC_RTINFO_GET_CONFIG_WIFI_RESULT,           ///< 获取设备间配网结果 消息体:\c result
	EZDEVSDK_BASE_FUNC_RTINFO_GET_INDICATOR_LIGHT,              ///< 从设备获取指示灯状态，消息体: \c ezDevSDK_base_function_IndicatorLight_S
	EZDEVSDK_BASE_FUNC_RTINFO_GET_DNS_PARSE_RESULT_LIST,             ///<获取设备端保存的域名解析结果列表, 消息体:\c ezDevSDK_base_function_dns_list
	EZDEVSDK_BASE_FUNC_RTINFO_GET_BAT_EZIVZ_READY,              ///< 获取电池类设备是否准备完成(一般用于启动萤石后暂停萤石云状态初始化线程，避免线程抢占)
	EZDEVSDK_BASE_FUNC_RTINFO_GET_WIFI_SIGNAL,                  ///< 获取wifi信号强度,消息体 ezDevSDK_base_function_wifi_signal
	EZDEVSDK_ALARM_RT_INFO_GET_EXPANSION_CARD_DETECTION_RESULT, ///< 获取硬盘扩容卡检测结果, 消息体: ezDevSDK_expansion_card_detection_result
} EZDEVSDK_BASE_FUNCTION_RUNTIME_INFO_E;

/**
 * \brief   功能能力集信息
 */
typedef struct {
    int num;
    ezDevSDK_base_function_algorithm_level algorithm_level[0];	///< 功能能力等级
} ezDevSDK_base_function_algorithm_levels;

/**
 * \brief   wifi配置信息
 */
typedef struct {
	char ssid[64];  											///< SSID名称
	int signal;     											///< 信号强度
	int security;   											///< 加密方式
} ezDevSDK_base_function_ap_info;

/**
 * \brief wifi列表信息，对应消息类型: EZDEVSDK_BASE_FUNC_RT_INFO_GET_WIFI_INFO
 */
typedef struct {
	int num;                                    				///< 入参，存储空间大小个数，出参，实际获取的wifi列表的个数
	ezDevSDK_base_function_ap_info ap_info[0];  				///< 占位符
} ezDevSDK_base_function_ap_infos;

/**
 * \brief   硬盘信息
 */
typedef struct {
    int type;           										///< 0:local, 1:NFS
    int disk_status;    										///< 0状态正常，1介质错误，2未格式化，3正在格式化
    int total_space;    										///< 硬盘大小(MB)
    char first_record_time[64];									///< 首次使用时间
	int healthLevel;											///< 健康状态 0-100
	int index;
	unsigned char cid[16];                                      ///< sd卡CID
	int init_percent;
} ezDevSDK_base_function_disk_info;

/**
 * \brief   硬盘列表信息，对应消息类型: EZDEVSDK_BASE_FUNC_RT_INFO_GET_DISK_INFO
 */
typedef struct {
    int format_percent;											///< 格式化
    int num;													///< 入参，存储空间大小个数，出参，实际获取的磁盘数
    int b_check_first_record_time;								///< 是否需要检查录像时间标记位考虑到频繁检查录像开始时间对资源使用不友好
    ezDevSDK_base_function_disk_info disk_info[0];				///< 占位符
} ezDevSDK_base_function_disk_infos;

/**
 * \brief   升级进度信息，对应消息类型: EZDEVSDK_BASE_FUNC_RT_INFO_GET_UPGRADE
 */
typedef struct {
    int percent;    											///< 升级进度
    int status;     											///< 升级状态，0:upgrading, 1:rebooting, 2:succeed, 3:failed
    int code;       											///< 升级错误时的错误码
} ezDevSDK_base_function_upgrade;

/**
 * \brief   wifi网络状态信息，对应消息类型: EZDEVSDK_BASE_FUNC_RT_INFO_GET_NETWORK_STATUS
 */
typedef struct {
    int signal;
    char ssid[64];
} ezDevSDK_base_function_network_status;

/**
 * \brief   网络状态信息，对应消息类型: EZDEVSDK_BASE_FUNC_RT_INFO_GET_NETWORK_INTERFACE
 */
typedef struct {
    char interface[16];
    int type;													///< 1:有线，2:无线
    int net_status;												///< 网络状态，0:表示断网，1:表示连网
} ezDevSDK_base_function_network_interface;

/**
 * \brief   端口映射信息
 */
typedef struct {
    char type[32];												///< 类型
    unsigned short local_port; 									///< 本地端口
    unsigned short mapped_port;									///< 映射端口
} ezDevSDK_base_function_port;

/**
 * \brief   端口映射列表信息，对应消息类型: EZDEVSDK_BASE_FUNC_RT_INFO_GET_MAPPED_PORT
 */
typedef struct {
    char local_ip[32];											///< 内部IP
    char external_ip[32];										///< 外部IP
    int port_num;												///< 映射的端口数
    ezDevSDK_base_function_port ports[0];
} ezDevSDK_base_function_mapped_port;

/**
 * \brief   网关信息，对应消息类型: EZDEVSDK_BASE_FUNC_RT_INFO_GET_NETWORK_GATEWAY_ADDR
 */
typedef struct {
    char ifname[16];											///< 入参，网络接口名
    char gateway_addr[16];										///< 出参，获取到的网关ip地址
} ezDevSDK_base_function_gateway_addr;


/**
 * \brief   dclog配置参数类型 
 */
typedef enum
{
    EZDEVSDK_BASE_FUNC_DCLOG_CFGTYPE_GLOBAL = 0,    ///< 全局配置
    EZDEVSDK_BASE_FUNC_DCLOG_CFGTYPE_MODULE,        ///< 领域模块配置

    EZDEVSDK_BASE_FUNC_DCLOG_CFGTYPE_MAX,
} EZDEVSDK_BASE_FUNCTION_DCLOG_CFGTYPE_E;

/**
 * \brief   dclog全局配置参数 
 */
typedef struct
{
    char acSrvAddr[64];		    ///< dclog服务器域名
	int	 iPort;			        ///< 端口号
    char reserved[8];           ///< 预留
}ezDevSDK_base_function_dclog_GlobalCfg_S;

/** 
 * \brief   dclog模块配置参数
 */
typedef struct
{
    int           iType;        ///< module的type，参见 \ref EZDEV_DCLOG_REG_TYPE_E(入参)
    unsigned char ucEnable;     ///< 开关 （出参）
    char          reserved0[3]; ///< 预留
    int           iInterval;    ///< 时间间隔（出参）

}ezDevSDK_base_function_dclog_ModuleCfg_S;

/**
 * \brief   dclog配置结构体，用于sdk与应用之间。对应消息类型: EZDEVSDK_BASE_FUNC_ON_SET_DCLOG_CFG, EZDEVSDK_BASE_FUNC_RTINFO_GET_DCLOG_CFG
 */
typedef struct
{
    unsigned char ucCfgType;        ///< dclog配置参数类型，参见 \ref EZDEVSDK_BASE_FUNCTION_DCLOG_CFGTYPE_E
    char          reserved0[3];     ///< 保留

    union
    {
        ezDevSDK_base_function_dclog_GlobalCfg_S  stGlobal; ///< 全局配置
        ezDevSDK_base_function_dclog_ModuleCfg_S  stModule; ///< 模块配置

    }unCfg;
}ezDevSDK_base_function_dclog_Cfg_S;

/**
 * \brief  用户身份认证的当前状??
 */
typedef enum
{
    ezDevSDK_base_function_user_verify_status_success = 0,  ///< 成功
    ezDevSDK_base_function_user_verify_status_not_trigger,  ///< 未触发身份认证
    ezDevSDK_base_function_user_verify_status_processing,   ///< 身份认证中
    ezDevSDK_base_function_user_verify_status_by_other_user,///< 处于被其它用户身份认证中
    ezDevSDK_base_function_user_verify_status_failed,       ///< 身份认证失败
    ezDevSDK_base_function_user_verify_status_no_main_user, ///< 没有主用户

    ezDevSDK_base_function_user_verify_status_max,

} ezDevSDK_base_function_user_verify_status_E;


/**
 * \brief 获取用户身份认证的当前状态，对应消息类型: EZDEVSDK_BASE_FUNC_RTINFO_GET_USER_VERIFY_STATUS
 * \attention  一般设备可不实现，或默认成??目前只应用于智能门锁设备，超时时间内不停查询认证结果
 */
typedef struct
{
    char         acUserId[32];   ///< 入参，用于匹配是否是当前处理的请求用
    int          status;         ///< 回填处理结果,ezDevSDK_base_function_user_verify_status_E
} ezDevSDK_base_function_user_verify_status_S;

typedef struct
{
    char target_serial[64];     ///<查询的待配网设备序列号
    char ssid[64];              ///<当前此设备的SSID
    int result;                 ///< 0 可配网 1不可配网
} ezDevSDK_base_function_configurable_device_info;

typedef struct
{
    char target_serial[64];     ///<待查询的目标设备
    int result;
} ezDevSDK_base_function_config_wifi_result;

typedef struct
{
    int ready;                  ///<0: 外层还在准备  1:外层已准备好
} ezDevSDK_base_function_bat_ezviz_status;

 /**
 *  @brief   设备wifi信号强度，对应回调信息类型: EZDEVSDK_BASE_FUNC_RTINFO_GET_WIFI_SIGNAL
 */
typedef struct
{
    int signal; ///< 0:无信号 1:差 2:一般 3:良好
} ezDevSDK_base_function_wifi_signal;

typedef struct{
    int force;                      //是否强制更新token，0：非强制，平台如果已经有了则反馈原有token即可，1：强制更新token，平台重新生成
}ezDevSDK_base_function_bind_token_req_info; 

//-------------------------------------应用层发给SDK（或服务器）的消息------------------------------

/**
 * \brief 设备发给服务器的信令集合
 */
typedef enum
{
	EZDEVSDK_BASE_FUNC_UPGRADE = 0,					    ///< 设备升级请求，消息体: \c ezDevSDK_base_function_upgrade_info
	EZDEVSDK_BASE_FUNC_QUERY_UPGRADE_PACKET,			///< 设备查询升级包，消息体: \c ezDevSDK_base_function_upgrade_info
	EZDEVSDK_BASE_FUNC_GET_TIMEZONE,					///< \todo 设备向服务器查询时区(海外设备使用，暂不实现)
	EZDEVSDK_BASE_FUNC_GET_SERVER_TIMESTAMP,			///< 设备获取服务器时间戳，消息体: NULL
	EZDEVSDK_BASE_FUNC_GET_USERLIST,					///< 设备向服务器获取用户列表，消息体: NULL
	EZDEVSDK_BASE_FUNC_PU2CENPLT_TRANS,					///< 设备向APP发送透传信息，消息体: \c  ezDevSDK_base_function_trans_protocol_msg
	EZDEVSDK_BASE_FUNC_OFFLINE,                         ///< 设备向服务发送下线信息
	EZDEVSDK_BASE_FUNC_TRANS_CMD_MSG,					///< 由外部实现的请求所对应的平台回应信息，消息体: \c ezDevSDK_base_function_trans_cmd_msg
	EZDEVSDK_BASE_FUNC_UPGRADE_READY,                   ///< 设备升级准备就绪（固件特征码升级流程)，消息体: \c ezDevSDK_base_function_upgrade_ready
	EZDEVSDK_BASE_FUNC_CAPACITY_NOTIFY,                 ///< 设备上报能力级
    EZDEVSDK_BASE_FUNC_REPORT_TOUCH_BIND_TOKEN,         ///< 设备有线上线，添加前会平台会下发token，设备完成对应物理操作后调用此接口上报，消息体:\c int
    EZDEVSDK_BASE_FUNC_REPORT_BIND_USER_TOKEN,			///< 设备AP配网上线，APP通过本地协议传递给设备token，设备上报token直接完成添加动作，消息体:\c ezDevSDK_base_function_report_bind_user_token
    EZDEVSDK_BASE_FUNC_QUERY_BIND_TOKEN,			    ///< 设备向平台查询用户绑定token,无入参
    EZDEVSDK_BASE_FUNC_QUERY_PROFILE,			        ///< 查询设备，消息体:\c ezDevSDK_base_function_profile_query_info
	EZDEVSDK_BASE_FUNC_COUNT,
}EZDEVSDK_BASE_FUNCTION_MSG_TO_SERVER_TYPE_E;

/**
* \brief 基础功能领域接收设备通用消息
*/
typedef struct
{
	EZDEVSDK_BASE_FUNCTION_MSG_TO_SERVER_TYPE_E msg_type;		///< 消息类型	
	void*										msg_struct_ptr;	///< 指向消息内容的指针
}ezDevSDK_base_function_msg;

/**
* \brief 设备升级消息
*/
typedef struct
{	
	int channel_num;											///< 通道号，0表示本设备，目前只支持本设备升级	
	char dev_mode[EZVIZ_NAME_LEN];								///< 设备型号，如果为NULL，则使用设置的值
	char dev_firmware_id[EZVIZ_NAME_LEN];						///< 硬件特征码，如果为NULL，则使用设置的值
	char dev_serial[EZVIZ_NAME_LEN];							///< 通道设备的序列号
	char dev_version[EZVIZ_NAME_LEN];				            ///< 通道设备的版本号
	int  ota_type;	                                            ///< 升级类型，0整包升级、1差分升级、2两者都支持
}ezDevSDK_base_function_upgrade_info;

/**
* \brief 查询描述文件
*/
typedef struct
{	
	char dev_mode[EZVIZ_NAME_LEN];								///< 设备型号
	char dev_serial[EZVIZ_NAME_LEN];							///< 设备序列号
	char dev_version[EZVIZ_NAME_LEN];				            ///< 设备版本号
	int need_schema;	                                        ///< 描述文件是否携带schema字段，0不带、1带
}ezDevSDK_base_function_profile_query_info;

/**
* \brief 设备可升级状态上报
*/

typedef struct
{
    char check_sum[64]; ///< 校验和
    char version[64];   ///< 版本
    int status;         ///< 0不可用，1可用
} ezDevSDK_base_function_upgrade_ready;

/**
 * \brief   设备向平台上报绑定用户的token值，对应消息类型: EZDEVSDK_BASE_FUNC_REPORT_BIND_USER_TOKEN
 */
typedef struct {
	char *token;
} ezDevSDK_base_function_report_bind_user_token;

/**
 * \brief   设备向平查询绑定用户的token(验证码)响应，对应消息类型: EZDEVSDK_BASE_FUNC_ON_QUERY_BIND_TOKEN_RSP
 */
typedef struct
{
    char errcode[32];                         ///< 响应错误码
	char token[128];                          ///< 用户绑定使用的token
    char registerUrl[128];                    ///< 设备注册平台使用的域名
    char expirationTime[32];                    ///< 设备token过期时间（UTC绝对时间）    
} ezDevSDK_base_query_bind_token_rsp;

//-------------------------------------应用层配置参数----------------------------------------------

/**
* \brief 设备工作模式
*/
typedef struct
{
	int mode;													///< 6:低功耗模式只有唤醒包，8:低功耗模式唤醒头+业务报文，1:正常模式
	int *keepalive;												///< 维持心跳时间(s)，入参:设备要求参数，出参:服务器允许参数
}ezDevSDK_base_function_dev_mode;

/**
* \brief 设备操作码
*/
typedef struct
{
    char dev_operation_code[EZVIZ_NAME_LEN];
}ezDevSDK_base_function_operation_code;

/*! \} */

#ifdef __cplusplus
}
#endif

#endif //H_EZDEVSDK_BASE_FUNCTION_DEF_H_
