/**
 * \file		ezDevSDK_thirdparty_def.h
 *
 * \brief		开放平台对外结构体定义、宏定义
 *
 * \copyright	HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 *
 * \author		liuzhenhao
 *
 * \date		2017/3/10
 */

#ifndef H_EZDEVSDK_THIRDPARTY_DEF_H_
#define H_EZDEVSDK_THIRDPARTY_DEF_H_
#include "ezDevSDK_error.h"
/**
 * @addtogroup Thirdparty_Module 第三方领域模块
 * \{
 */

/** 
 *  \brief 萤石SDK第三方领域的接口错误码，由SDK返回给调用者
 *  \note 公用错误码，请参考ezDevSDK_error.h
 */
typedef enum
{
    EZDEVSDK_THIRDPARTY_SUCESS = 0X00000000,    ///< 成功
    EZDEVSDK_THIRDPARTY_CODE_BASE = 0XD5000000, ///< 第三方领域接口错误码起始值，D表示接口错误码，5000，表示第三方领域，最后三位表示具体错误

    ///< 0XD5000000 - 0XD500001F 公共错误码
    EZDEVSDK_THIRDPARTY_CODE_FAIL = EZDEVSDK_THIRDPARTY_CODE_BASE + EZDEVSDK_ERROR_FAIL,
    EZDEVSDK_THIRDPARTY_CODE_NOT_INITED = EZDEVSDK_THIRDPARTY_CODE_BASE + EZDEVSDK_ERROR_NOT_INITED,
    EZDEVSDK_THIRDPARTY_CODE_INVALID_PARAM = EZDEVSDK_THIRDPARTY_CODE_BASE + EZDEVSDK_ERROR_INVALID_PARAM,
    EZDEVSDK_THIRDPARTY_CODE_NOT_SUPPORT = EZDEVSDK_THIRDPARTY_CODE_BASE + EZDEVSDK_ERROR_NOT_SUPPORT,
    EZDEVSDK_THIRDPARTY_CODE_NOT_ENOUGH_MEMERY = EZDEVSDK_THIRDPARTY_CODE_BASE + EZDEVSDK_ERROR_NOT_ENOUGH_MEMERY,
    EZDEVSDK_THIRDPARTY_CODE_INIT_FAIL = EZDEVSDK_THIRDPARTY_CODE_BASE + EZDEVSDK_ERROR_INIT_FAIL,
    EZDEVSDK_THIRDPARTY_CODE_THREAD_CREATE_FAIL = EZDEVSDK_THIRDPARTY_CODE_BASE + EZDEVSDK_ERROR_THREAD_CREATE_FAIL,
    EZDEVSDK_THIRDPARTY_CODE_RSP_TIMEOUT = EZDEVSDK_THIRDPARTY_CODE_BASE + EZDEVSDK_ERROR_RSP_TIMEOUT,
    EZDEVSDK_THIRDPARTY_CODE_MSG_DATA_INVAILD = EZDEVSDK_THIRDPARTY_CODE_BASE + EZDEVSDK_ERROR_MSG_DATA_INVAILD,

    EZDEVSDK_THIRDPARTY_CODE_NO_PERMISSION = 0XD5000020,         ///< 权限不足
    EZDEVSDK_THIRDPARTY_CODE_NO_ENOUGH_BUF = 0XD5000021,         ///< 开放平台返回消息长度大于外部预分配长度
    EZDEVSDK_THIRDPARTY_CODE_CONNECT_ERROR = 0XD5000022,         ///< 连接平台服务失败
    EZDEVSDK_THIRDPARTY_CODE_SEND_DATA_ERROR = 0XD5000023,       ///< 向开放平台发送数据失败
    EZDEVSDK_THIRDPARTY_CODE_RECEIVE_DATA_ERROR = 0XD5000024,    ///< 接收开放平台返回信息失败
    EZDEVSDK_THIRDPARTY_CODE_PROTOL_FALID = 0XD5000025,          ///< 开放平台返回信息协议不符
    EZDEVSDK_THIRDPARTY_CODE_UNVALIABLE_CB = 0XD5000026,         ///< 回调函数未设置
    EZDEVSDK_THIRDPARTY_CODE_UPLOADFILE_INCOMPLETE = 0XD5000027, ///< 文件上传不完整
    EZDEVSDK_THIRDPARTY_CODE_NO_CRC_HEADER = 0XD5000028,         ///< 开放平台返回消息头不含crc字段
    EZDEVSDK_THIRDPARTY_CODE_TIMEOUT = 0XD5000029,               ///< 超过预设等待时间
} EZDEVSDK_THIRDPARTY_CODE_E;

/**
 * @brief 信令发送可靠性等级
 */
typedef enum
{
    EZDEVSDK_ISAPI_QOS0, ///< 把消息发出去，不管是否送达
    EZDEVSDK_ISAPI_QOS1, ///< 等待服务器ack，确认消息送达
    EZDEVSDK_ISAPI_QOS2, ///< 保留字段，暂未实现
} EZDEVSDK_ISAPI_QOS_TYPE_E;

/**
 * \brief   SDK给应用层的基础业务消息结构体
 */
typedef struct
{
    int type;   ///< 业务类型， \ref EZDEVSDK_THIRDPARTY_BUSINESS_MSG_TYPE_E
    void *data; ///< 所给消息内容
    int len;    ///< 所给消息内容的长度
} ezDevSDK_thirdparty_business_msg;

/**
 * \brief   ISAPI消息类型
 */
typedef enum
{
    EZDEVSDK_THIRDPARTY_ISAPI_UP,   ///< 上行请求或下行响应
    EZDEVSDK_THIRDPARTY_ISAPI_DOWN, ///< 下行请求或上行响应
} EZDEVSDK_THIRDPARTY_ISAPI_TYPE_E;

/** 
 *  \brief APP和设备ISAPI透传的消息结构体
 */
typedef struct
{
    EZDEVSDK_THIRDPARTY_ISAPI_TYPE_E type; ///< 协议消息类型
    int seq;                               ///< sequence number
    char *content;                         ///< 透传消息内容
    int len;                               ///< 透传消息大小
	EZDEVSDK_ISAPI_QOS_TYPE_E qos;		   ///< 信令可靠性等级
} ezDevSDK_thirdparty_isapi_msg;

typedef struct
{
    unsigned int last_error;       ///< 错误码
    unsigned int seq;              ///< 消息发送seq
    EZDEVSDK_ISAPI_QOS_TYPE_E qos; ///< 消息QOS等级
} ezDevSDK_isapi_msg_ack;

typedef struct
{
    unsigned int last_error; ///< 错误码
    unsigned int seq;        ///< 文件发送seq
} ezDevSDK_isapi_file_ack;

/** 
 * \brief   第三方领域回调设备的接口
 * \warning 不能有任何阻塞操作
 */
typedef struct
{
    int (*on_recv_msg)(ezDevSDK_thirdparty_business_msg *msg);    ///< 第三方领域基础业务回调
    int (*on_recv_isapi_msg)(ezDevSDK_thirdparty_isapi_msg *msg); ///< ISAPI 回调
} ezDevSDK_thirdparty_callback;

/**
 * \brief 第三方领域初始化的参数
 */
typedef struct
{
    int max_channel;                    ///< 最大通道数
    int cache_urlkey;                   ///< 是否需要缓存urlkey，0不需要，1需要
    int cache_urlkey_num;               ///< 需要缓存urlkey的数量最大 50,不传该值默认缓存5个,超过20时微内核需要在编译是选择支持256k消息的发送和接收
    int send_thread_num;                ///< 异步发送时的最大发送线程数
    int task_num;                       ///< 异步发送时的最大任务数
    long timeval_tv_sec;                ///< 设置底层setsockopt函数的timeval.tv_sec(单位秒 seconds,默认值10,最大值100)
    ezDevSDK_thirdparty_callback cb;    ///< 回调函数
} ezDevSDK_thirdparty_init_info;

//----------------------------------SDK通知应用层的消息--------------------------------------------

/**
 * @brief   SDK通知应用层的业务消息类型
 */
typedef enum
{
    EZDEVSDK_THIRDPARTY_TOKEN_INFO,    ///< token信息，消息体: ezDevSDK_thirdparty_token
    EZDEVSDK_THIRDPARTY_TRANSFER_MSG,  ///< 透传消息，消息体: ezDevSDK_thirdparty_transfer_data_msg
    EZDEVSDK_THIRDPARTY_RESPONSE_MSG,  ///< 响应消息，消息体: ezDevSDK_thirdparty_response_msg
    EZDEVSDK_THIRDPARTY_MSG_ACK_INFO,  ///< 消息发送回执，消息体: ezDevSDK_isapi_msg_ack
    EZDEVSDK_THIRDPARTY_FILE_ACK_INFO, ///< 消息发送回执，消息体: ezDevSDK_isapi_file_ack
    EZDEVSDK_THIRDPARTY_MSG_TYPE_MAX,
} EZDEVSDK_THIRDPARTY_BUSINESS_MSG_TYPE_E;

/**
 * \brief   SDK给应用层的token消息
 */
typedef struct
{
    int code;    ///< 错误码
    char *msg;   ///< 返回消息
    char *token; ///< 平台返回的token
} ezDevSDK_thirdparty_token;

/**
 * \brief   SDK给应用层的透传消息,平台向设备发送请求以及设备的响应(除msg外和请求的一致)
 */
typedef struct
{
    int req_code; ///< 消息序列
    int seq;      ///< 开放平台特殊性将SEQ传入上层响应时以msg_seq返回
    char *msg;    ///< 透传消息
} ezDevSDK_thirdparty_transfer_data_msg;

/**
 * \brief   SDK给应用层的响应消息
 */
typedef struct
{
    int ret_code;     ///< 错误码
    char *is_success; ///< 是否成功
    char *message;    ///< 返回的消息
} ezDevSDK_thirdparty_response_msg;

//--------------------------------SDK从应用层获取的消息---------------------------------------------------

//------------------------------------应用层发给SDK（或服务器）的消息-------------------------------------

/**
 * @brief   开放平台认证信息
 */
typedef struct
{
    char *app_key;    ///< 开放平台申请的appkey
    char *dst;        ///< 透传目的地（yun，sdk）
    char *permission; ///< 申请权限（signal、file）
} ezDevSDK_thirdparty_authorize;

/**
 * @brief   设备和开放平台间透传信息
 */
typedef struct
{
    char *app_key;  ///< 开放平台申请的appkey
    char *dst;      ///< 数据发送的目的地
    char *token;    ///< 向开放平台申请的token
    char *msg_type; ///< 消息类型，在发送signal时有效
    char *data;     ///< 发送的数据，如果是发送signal，需要符合jason格式
    int data_len;   ///< 发送数据的长度
} ezDevSDK_thirdparty_data;

/**
 * @brief   ISAPI的文件传输类型
 */
typedef enum
{
    EZDEVSDK_THIRDPARTY_ISAPI_PICTURE, ///< 图片传输
    EZDEVSDK_THIRDPARTY_ISAPI_FILE_TYPE_MAX,
} EZDEVSDK_THIRDPARTY_ISAPI_FILE_TYPE_E;

/** 
 *  \brief APP和设备ISAPI透传文件结构体
 */
typedef struct
{
    EZDEVSDK_THIRDPARTY_ISAPI_FILE_TYPE_E type; ///< 文件类型
    int channel;                                ///< 发送通道
    char *send_data;                            ///< 发送的数据
    int send_len;                               ///< 发送数据大小
    char *out_url;                              ///< 发送文件的url，由萤石云分配，外部预先分配128bytes
    int out_len;                                ///< url缓冲区大小
} ezDevSDK_thirdparty_isapi_file;

/**
 * @brief   文件发送相关属性值
 */
typedef struct
{
    int is_asyn_send; ///< 0同步发送，1:异步发送
    unsigned int seq; ///< 发送序号，接口异步调用会在回调中带回
    int timeout_ms;   ///< 发送超时时间，-1表示无限等待
} ezDevSDK_isapi_send_attrs;

//------------------------------------应用层配置参数--------------------------------------------

typedef struct
{
    void *(*on_connect)(int socket);                  ///< 得到socket并生成相应句柄。如果成功返回handle，失败返回NULL,返回前必须先关闭socket fd,否则会导致fd泄露
    int (*on_send)(void *handle, char *buf, int len); ///< 如果成功返回发送长度，失败返回-1
    int (*on_recv)(void *handle, char *buf, int len); ///< 如果成功返回接收长度，失败返回-1
    int (*on_close)(void *handle);                    ///< 关闭句柄,如果on_connect返回NULL,将不再回调on_close,因此on_connect返回NULL时,先关闭socket fd
} ezDevSDK_thirdparty_ssl_callback;

/*! \} */

#endif
