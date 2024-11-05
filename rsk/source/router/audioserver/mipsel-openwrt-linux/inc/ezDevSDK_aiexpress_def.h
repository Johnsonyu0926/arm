/**
 * \file ezDevSDK_aiexpress_def.h
 * \author xurongjun (xurongjun\hikvision.com)
 * \brief AI高速通道模块对外结构体定义、宏定义
 * \version 0.1
 * \date 2019-06-14
 * 
 * \copyright Copyright (c) 2019
 * 
 */

#ifndef _EZDEVSDK_AIEXPRESS_DEF_H_
#define _EZDEVSDK_AIEXPRESS_DEF_H_

#include <time.h>
#include "ezDevSDK_error.h"

typedef enum
{
    EZDEVSDK_AIEXPRESS_GET_PIPE_CFG = 0, ///< AI高速通道配置信息。信息体: ezDevSDK_aiexpress_pipe_cfgs_t
} EZDEVSDK_AIEXPRESS_RT_INFO_TYPE_E;

typedef enum
{
    EZDEVSDK_AIEXPRESS_SET_PIPE_CFG = 0, ///< AI高速通道配置信息，需要固化到设备上。信息体: ezDevSDK_aiexpress_pipe_cfgs_t
    EZDEVSDK_AIEXPRESS_SEND_ACK,         ///< 异步发送文件消息确认。信息体: ezDevSDK_aiexpress_send_ack_t
} EZDEVSDK_AIEXPRESS_MSG2DEV_TYPE_E;

typedef enum
{
    EZDEVSDK_AIEXPRESS_BUSINESS_ALARM = 1,    ///< 告警业务
    EZDEVSDK_AIEXPRESS_BUSINESS_CALL = 2,     ///< 呼叫业务,此功能未实现
    EZDEVSDK_AIEXPRESS_BUSINESS_LEAVEMSG = 3, ///< 留言业务,此功能未实现
    EZDEVSDK_AIEXPRESS_BUSINESS_ISAPI = 99,   ///< 和ISAPI透传文件关联
} EZDEVSDK_AIEXPRESS_BUSINESS_TYPE_E;

/**
 * @brief 萤石SDK AI高速通道领域的接口错误码，由SDK返回给调用者
 * @note 公用错误码，请参考ezDevSDK_error.h
 */
typedef enum
{
    EZDEVSDK_AIEXPRESS_CODE_SUCESS = 0X00000000, ///< 成功
    EZDEVSDK_AIEXPRESS_CODE_BASE = 0XD8000000,   ///< 领域接口错误码起始值，D表示接口错误码，8000，表示报警领域，最后三位表示具体错误

    ///< 0XD8000000 - 0XD800001F 公共错误码
    EZDEVSDK_AIEXPRESS_CODE_FAIL = EZDEVSDK_AIEXPRESS_CODE_BASE + EZDEVSDK_ERROR_FAIL,
    EZDEVSDK_AIEXPRESS_CODE_NOT_INITED = EZDEVSDK_AIEXPRESS_CODE_BASE + EZDEVSDK_ERROR_NOT_INITED,
    EZDEVSDK_AIEXPRESS_CODE_INVALID_PARAM = EZDEVSDK_AIEXPRESS_CODE_BASE + EZDEVSDK_ERROR_INVALID_PARAM,
    EZDEVSDK_AIEXPRESS_CODE_NOT_SUPPORT = EZDEVSDK_AIEXPRESS_CODE_BASE + EZDEVSDK_ERROR_NOT_SUPPORT,
    EZDEVSDK_AIEXPRESS_CODE_NOT_ENOUGH_MEMERY = EZDEVSDK_AIEXPRESS_CODE_BASE + EZDEVSDK_ERROR_NOT_ENOUGH_MEMERY,
    EZDEVSDK_AIEXPRESS_CODE_INIT_FAIL = EZDEVSDK_AIEXPRESS_CODE_BASE + EZDEVSDK_ERROR_INIT_FAIL,
    EZDEVSDK_AIEXPRESS_CODE_THREAD_CREATE_FAIL = EZDEVSDK_AIEXPRESS_CODE_BASE + EZDEVSDK_ERROR_THREAD_CREATE_FAIL,
    EZDEVSDK_AIEXPRESS_CODE_RSP_TIMEOUT = EZDEVSDK_AIEXPRESS_CODE_BASE + EZDEVSDK_ERROR_RSP_TIMEOUT,
    EZDEVSDK_AIEXPRESS_CODE_MSG_DATA_INVAILD = EZDEVSDK_AIEXPRESS_CODE_BASE + EZDEVSDK_ERROR_MSG_DATA_INVAILD,
    EZDEVSDK_AIEXPRESS_CODE_ALREADY_INITED = EZDEVSDK_AIEXPRESS_CODE_BASE + EZDEVSDK_ERROR_ALREADY_INITED,

    EZDEVSDK_AIEXPRESS_CODE_EXTERN_REGISTR = 0XD8000020,         ///< 领域模块注册失败
    EZDEVSDK_AIEXPRESS_CODE_BUSINESS_NOT_OPEN = 0XD8000021,      ///< 通道状态不正确
    EZDEVSDK_AIEXPRESS_CODE_CONNECT_CLOUD = 0XD8000022,          ///< 连接云服务失败
    EZDEVSDK_AIEXPRESS_CODE_PUSH_PIC = 0XD8000023,               ///< 发送图片失败
    EZDEVSDK_AIEXPRESS_CODE_PUSH_PIC_RSP = 0XD8000024,           ///< 接收上传图片响应失败
    EZDEVSDK_AIEXPRESS_CODE_PLT_EXCEPTION = 0X80000101,          ///< 其他异常
    EZDEVSDK_AIEXPRESS_CODE_PLT_NO_PERMISSION = 0X80000102,      ///< 无权限
    EZDEVSDK_AIEXPRESS_CODE_PLT_DATA_ERROR = 0X80000103,         ///< 数据异常
    EZDEVSDK_AIEXPRESS_CODE_PLT_TOKEN_EXPIRATION = 0X80000104,   ///< 令牌过期
    EZDEVSDK_AIEXPRESS_CODE_PLT_TOKEN_ERROR = 0X80000105,        ///< 令牌异常
    EZDEVSDK_AIEXPRESS_CODE_PLT_PARAMETER_ERROR = 0X80000106,    ///< 参数为空或异常
    EZDEVSDK_AIEXPRESS_CODE_PLT_UNSUPPORT_MSG_TYPE = 0X80000107, ///< 不支持的消息类型
}
EZDEVSDK_AIEXPRESS_CODE_E;

typedef struct
{
    unsigned int channel; ///< 开通的通道号
    char serviceId[64];   ///< 业务方ID
    unsigned int type;    ///< 业务类型
    unsigned char status; ///< 开关状态
} ezDevSDK_aiexpress_pipe_cfg_t;

typedef struct
{
    int cfg_count;                       ///< 通道配置的数量
    ezDevSDK_aiexpress_pipe_cfg_t *pcfg; ///< 通道配置
} ezDevSDK_aiexpress_pipe_cfgs_t;

typedef struct
{
    unsigned int last_error; ///< 错误码
    unsigned int seq;        ///< 文件发送seq
} ezDevSDK_aiexpress_send_ack_t;

typedef struct
{
    EZDEVSDK_AIEXPRESS_MSG2DEV_TYPE_E type; ///< SDK给应用的消息类型
    void *data;                             ///< 信息内容结构体
    unsigned int len;                       ///< 信息内容长度
} ezDevSDK_aiexpress_msg2dev_t;

/**
 *  \brief SDK获取设备信息结构体
 */
typedef struct
{
    EZDEVSDK_AIEXPRESS_RT_INFO_TYPE_E type; ///< 运行时信息类型
    void *data;                             ///< 信息内容结构体
    unsigned int len;                       ///< 信息内容长度
} ezDevSDK_aiexpress_runtime_info_t;

/** 
 * \brief  和设备进行数据交互的回调接口
 * \warning 不能有任何阻塞操作
 */
typedef struct
{
    int (*recv_msg)(ezDevSDK_aiexpress_msg2dev_t *msg);                  ///< 消息回调接口，不可以阻塞
    int (*on_get_runtime_info)(ezDevSDK_aiexpress_runtime_info_t *info); ///< 获取设备运行时信息回调接口
} ezDevSDK_aiexpress_callback_t;

/**
 * \brief领域初始化的参数
 */
typedef struct
{
    int channel_num_max;              ///< 最大通道号，默认为1。(如果最大通道号是2，表示0-2通道可用)
    int thread_num;                   ///< 异步发送时线程池最大线程数量
    int task_num;                     ///<异步发送时最大任务数量，不传该值默认最大100
    ezDevSDK_aiexpress_callback_t cb; ///< 回调函数
} ezDevSDK_aiexpress_init_info_t;

typedef struct
{
    char *pdata;  ///< 发送的数据
    int data_len; ///< 发送数据大小
} ezDevSDK_aiexpress_data_t;

/**
 * @brief   文件发送相关属性值
 */
typedef struct
{
    int is_asyn_send;          ///< 0同步发送，1:异步发送(现在只支持同步发送)
    int timeout_ms;            ///< 发送超时时间，-1表示无限等待
    unsigned int seq;          ///< 发送序号，接口异步调用会在回调中带回
    unsigned int encrypt_mode; ///< 0不加密，1加密，2用户秘钥加密(暂时不支持)
} ezDevSDK_aiexpress_attrs_t;

typedef struct
{
    char alarm_type[64];  ///< 报警类型
    char customType[64];  ///< 自定义信息类型
    char customInfo[128]; ///< 自定义信息数据
} ezDevSDK_aiexpress_alarm_extinfo_t;

typedef struct
{
    char *purl_key;  ///< 需要通过isapi发送图片的urlkey
    int url_key_len; ///< 发送数据大小
} ezDevSDK_aiexpress_isapi_extinfo_t;

/**
 *  @brief 高速通道上传相关信息
 */
typedef struct
{
    unsigned char business_type; ///< 告警业务类型，@ref EZDEVSDK_AIEXPRESS_BUSINESS_TYPE_E
    unsigned int channel;        ///< 通道号，0代表设备本身
    time_t timestamp;            ///< 报警时间戳

    /* 和业务相关的拓展信息 */
    union {
        ezDevSDK_aiexpress_alarm_extinfo_t alarm_extinfo;
        ezDevSDK_aiexpress_isapi_extinfo_t isapi_extinfo;
    } extinfo_u;
} ezDevSDK_aiexpress_info_t;

#endif //_EZDEVSDK_AIEXPRESS_DEF_H_