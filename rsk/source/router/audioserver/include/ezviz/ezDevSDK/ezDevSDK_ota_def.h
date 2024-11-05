/**
 * \file ezDevSDK_ota_def.h
 * \author 
 * \brief 
 * \version 0.1
 * \date 2020-05-19
 * 
 * \copyright Copyright (c) 2019
 * 
 */

#ifndef _ezDevSDK_ota_DEF_H_
#define _ezDevSDK_ota_DEF_H_

#include "ezDevSDK_error.h"


typedef enum
{
    ota_down_no_err,     ///< no error
    ota_down_md5_err,    ///< md5 not match, data = NULL
    ota_down_failed,     ///< interface error
} ota_download_error_e;

/**
 * @brief 下载回调接口,cdoe 
 * 
 */

//当回调返回非0时，下载退出不再重试,
typedef int (*ezDevSDK_ota_download_cb)(char* data, int data_len, ota_download_error_e error, int progress);


typedef enum
{
    ota_event_start,        ///< 请求设备升级，消息体: \c ezDevSDK_ota_modules_t
    ota_evnet_upgrade_info, ///< 设备升级包信息，消息体: \c ezDevSDK_ota_upgrade_info_t
} ezDevSDK_ota_event_e;

/**
 * \brief   设备的回调函数定义，用来接收基础功能领域的消息和信息查询
 * \warning 不可以阻塞
 * \note    recv_msg 接收来自平台下发的消息，参见
 */
typedef struct 
{
	int (*ota_event_handler)(ezDevSDK_ota_event_e event, void *data);     ///<ota事件通知接口
} ezDevSDK_ota_callback;


typedef struct
{
    int index;          ///< 模块索引，模块在设备内部的编号;如果设备需要给其他设备升级，上报关联的索引，否则默认0
    char identify[256]; ///< 模块唯一标志（同设备型号）
    char fw_ver[64];    ///< 当前固件版本号
} ezDevSDK_ota_module_info_t;

typedef struct
{
    int num;                                  ///< 升级模块数量，最多支持16组
    ezDevSDK_ota_module_info_t *pmodule_info; ///< 升级模块信息列表
} ezDevSDK_ota_modules_t;

/**
* \brief	基础功能领域的初始化参数
*/
typedef struct 
{
    ezDevSDK_ota_callback  cb;									///< 回调函数
    ezDevSDK_ota_modules_t modules;                             ///< 设备模块信息
} ezDevSDK_ota_init_info;

typedef struct
{
    char identify[256]; ///< 模块唯一标志（同设备型号）
    char fw_ver[64];    ///< 当前固件版本号
    int  ota_type;       ///< 升级类型，0支持整包升级、1支持差分升级
} ezDevSDK_ota_query_info_t;

typedef struct
{
    int num;                                ///< 查询升级升级模块数量，最多支持16组
    ezDevSDK_ota_query_info_t *pquery_info; ///< 升级模块信息列表
} ezDevSDK_ota_query_infos_t;


/**
 * @brief 查询升级包错误码
 * 
 */
typedef enum
{
    ezDevSDK_ota_code_query_succ = 0,                ///< 没有错误
    ezDevSDK_ota_code_query_last_ver = 0xD1000060,  ///< 已经是最新版本，不需要升级
    ezDevSDK_ota_code_query_reject = 0xD1000061,    ///< 升级服务拒绝升级请求
    ezDevSDK_ota_code_query_exception = 0xD1000062, ///< 升级服务内部异常
} ezDevSDK_ota_code_query_e;

/**
* \brief   差分文件的信息
*/
typedef struct
{
    char http_url[270];   ///< 包下载全路径http。
    char https_url[270];  ///< 包下载全路径https。
    char fw_ver_src[64];  ///< 差分包对应的原升级包版本号，只有这个版本的固件才能通过差分包升级至fw_ver版本的固件
    char degist[33];      ///< 升级包的摘要值,包含'\0'
    int size;             ///< 软件包大小，单位Byte
} ezDevSDK_ota_file_diff_t;

/**
* \brief   升级文件的信息
*/
typedef struct
{
    char identify[256];               ///< 模块唯一标志（同设备型号）
    char http_url[270];               ///< 包下载全路径http。
    char https_url[270];              ///< 包下载全路径https。
    char fw_ver[64];                  ///< 升级包版本号
    char degist[33];                  ///< 升级包的摘要值,包含'\0'
    char dsc[256];                    ///< 升级描述信息
    int size;                         ///< 软件包大小，单位Byte
    int diff_num;                     ///< 差分包数量，同一型号一次最多返回两组（统一型号的不同版本同时查询升级包才有可能返回多个差分包的情况）
    ezDevSDK_ota_file_diff_t *pdiffs; ///< 升级至该版本的差分包
} ezDevSDK_ota_file_info_t;

/**
 * \brief   设备升级包信息
 */
typedef struct
{
    ezDevSDK_ota_code_query_e code;       ///< 查询错误码
    int retry_max;                        ///< 下载最大重试次数
    int report_interval;                  ///< 上报进度最小间隔
    int file_num;                         ///< 升级包信息数组大小
    ezDevSDK_ota_file_info_t *pota_files; ///< 升级信息数组
} ezDevSDK_ota_upgrade_info_t;

/**
 * @brief 萤石SDK OTA领域的接口错误码，由SDK返回给调用者
 * @note 公用错误码，请参考ezDevSDK_error.h
 */
typedef enum
{
    ezDevSDK_ota_code_succ = 0X00000000,                                                   ///< 成功
    ezDevSDK_ota_code_fail = 0XD1000000 + EZDEVSDK_ERROR_FAIL,                             ///< 通用错误码
    ezDevSDK_ota_code_not_inited = 0XD1000000 + EZDEVSDK_ERROR_NOT_INITED,                 ///< 模块未初始化
    ezDevSDK_ota_code_invalid_param = 0XD1000000 + EZDEVSDK_ERROR_INVALID_PARAM,           ///< 非法参数
    ezDevSDK_ota_code_not_enough_memory = 0XD1000000 + EZDEVSDK_ERROR_NOT_ENOUGH_MEMERY,   ///< 内存不足
    ezDevSDK_ota_code_init_fail = 0XD1000000 + EZDEVSDK_ERROR_INIT_FAIL,                   ///< 领域初始化失败
    ezDevSDK_ota_code_thread_create_fial = 0XD1000000 + EZDEVSDK_ERROR_THREAD_CREATE_FAIL, ///< 线程创建失败
    ezDevSDK_ota_code_already_inited = 0XD1000000 + EZDEVSDK_ERROR_ALREADY_INITED,         ///< 领域已初始化
    ezDevSDK_ota_code_json_err= 0XD1000000 + EZDEVSDK_ERROR_JSON_ERROR,                    ///< JSON err
    ezDevSDK_ota_code_msg_send_err= 0XD1000000 + EZDEVSDK_ERROR_MSG_SEND_ERROR,                ///< 消息发送出错

    ezDevSDK_ota_code_device_upgrading = 0XD1000040, ///< 正在升级中
    ezDevSDK_ota_code_device_on_busy = 0XD1000041, ///< 设备忙，拒绝升级
} ezDevSDK_ota_code_e;

/**
 * @brief 升级上报错误码
 * 
 */
typedef enum
{
    ota_report_no_err = 0,                    ///< 没有错误
    ota_report_query_timeout = 0xD1000020, ///< 查询升级包超时
    ota_report_query_err = 0xD1000021,     ///< 查询升级包出错\服务拒绝升级请求
    ota_report_size_mismatch = 0xD1000022, ///< 文件大小和查询的信息不匹配
    ota_report_size_range = 0xD1000023,    ///< 文件大小超过分区大小
    ota_report_connect = 0xD1000024,       ///< 域名解析失败或无法连上升级服务器
    ota_report_download = 0xD1000025,      ///< 下载出错（网络不稳定，下载超时，超过最大重试次数）
    ota_report_digest = 0xD1000026,        ///< 摘要值不匹配
    ota_report_verify = 0xD1000027,        ///< 验证固件签名失败（RSA\ECC验签）
    ota_report_damaged = 0xD1000028,       ///< 升级包损坏（内部格式不对）
    ota_report_memory = 0xD1000029,        ///< 内存不足
    ota_report_burn = 0xD100002a,          ///< 烧录固件出错
    ota_report_genneral = 0xD100002b,      ///< 未知错误
    ota_report_cancel = 0xD100002c,        ///< 用户主动停止/下载过程被取消/安装过程掉电
    ota_report_recovery = 0xD100002d,      ///< 升级失败，通过备份系统或最小系统恢复
} ezDevSDK_ota_report_err_e;


/**
 * @brief 升级过程状态变化，设备只有在ota_state_upgradeable、ota_state_succ、ota_state_fail三种状态下,
 *        云端才能判断设备是否可升级。
 */
typedef enum
{
    ota_state_upgradeable = 0,        ///< 可升级状态（非升级重启报这个状态）
    ota_state_starting = 1,           ///< 开始升级
    ota_state_downloading = 2,        ///< 正在下载
    ota_state_download_completed = 3, ///< 下载完成
    ota_state_burning = 4,            ///< 正在烧录
    ota_state_burning_completed = 5,  ///< 烧录完成
    ota_state_rebooting = 6,          ///< 正在重启
    ota_state_succ = 7,               ///< 升级成功
    ota_state_fail = 8,               ///< 升级失败
} ezDevSDK_ota_state_e;

typedef enum
{
    ota_progress_min = 0,

    ota_progress_max = 100
} ezDevSDK_ota_progress_e;


//下载的方式,目前仅支持http方式
typedef enum
{
    ota_http = 0,
    ota_https,
} ezDevSDK_ota_http_type;

/**
 * \brief   下载升级包信息
 */
typedef struct
{
    char url[270];                         ///< 升级包的url信息 http://开头
    char check_sum[33];                    ///< 升级包的md5 值(包含'\0')
    int http_type;                         ///< 下载支持的http类型,目前仅支持http方式下载
    int total_size;                        ///< 升级包的总大小
    int recv_buf_size;                     ///< 接收回调内容的大小
    int timeout_s;                         ///< 超时时间
} ezDevSDK_ota_download_info_t;



#endif //_ezDevSDK_ota_DEF_H_