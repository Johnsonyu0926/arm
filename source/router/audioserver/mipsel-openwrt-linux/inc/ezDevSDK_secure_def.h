/**
 * \file	  ezDevSDK_secure_def.h  
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     安全领域对外结构体定义、宏定义
 * 
 * \author    jinrongshun
 * 
 * \date      2021/02/22
 */

#ifndef H_EZDEVSDK_SECURE_DEF_H_
#define H_EZDEVSDK_SECURE_DEF_H_

#include "ezDevSDK_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

//----------------------------------错误码--------------------------------------------------

/**
 * \brief 萤石SDK错误码
 */
typedef enum  {
	EZDEVSDK_SECURE_CODE_SUCCESS                      = 0x00000000, 	///< 成功
	
    EZDEVSDK_SECURE_CODE_BASE                         = 0XD8000000,   ///< 安全领域接口错误码起始值，D表示接口错误码，8000，表示安全领域，最后三位表示具体错误
	EZDEVSDK_SECURE_CODE_FAIL                         = EZDEVSDK_SECURE_CODE_BASE + EZDEVSDK_ERROR_FAIL,
    EZDEVSDK_SECURE_CODE_NOT_INITED                   = EZDEVSDK_SECURE_CODE_BASE + EZDEVSDK_ERROR_NOT_INITED,
	EZDEVSDK_SECURE_CODE_INVALID_PARAM                = EZDEVSDK_SECURE_CODE_BASE + EZDEVSDK_ERROR_INVALID_PARAM,
	EZDEVSDK_SECURE_CODE_NOT_SUPPORT                  = EZDEVSDK_SECURE_CODE_BASE + EZDEVSDK_ERROR_NOT_SUPPORT,
	EZDEVSDK_SECURE_CODE_NOT_ENOUGH_MEMERY            = EZDEVSDK_SECURE_CODE_BASE + EZDEVSDK_ERROR_NOT_ENOUGH_MEMERY,
    EZDEVSDK_SECURE_CODE_INIT_FAIL                    = EZDEVSDK_SECURE_CODE_BASE + EZDEVSDK_ERROR_INIT_FAIL,
    EZDEVSDK_SECURE_CODE_THREAD_CREATE_FAIL           = EZDEVSDK_SECURE_CODE_BASE + EZDEVSDK_ERROR_THREAD_CREATE_FAIL,
    EZDEVSDK_SECURE_CODE_RSP_TIMEOUT                  = EZDEVSDK_SECURE_CODE_BASE + EZDEVSDK_ERROR_RSP_TIMEOUT,
    EZDEVSDK_SECURE_CODE_MSG_DATA_INVAILD             = EZDEVSDK_SECURE_CODE_BASE + EZDEVSDK_ERROR_MSG_DATA_INVAILD,
    EZDEVSDK_SECURE_CODE_JSON_ERROR                   = EZDEVSDK_SECURE_CODE_BASE + EZDEVSDK_ERROR_JSON_ERROR,
    EZDEVSDK_SECURE_CODE_MSG_SEND_ERROR               = EZDEVSDK_SECURE_CODE_BASE + EZDEVSDK_ERROR_MSG_SEND_ERROR,
} EZDEVSDK_SECURE_CODE_E;

//--------------------------------初始化信息---------------------------------------------------------
typedef struct {
	int type;        								///< \c EZDEVSDK_SECURE_INFO_E
	void *data;       								///< 指向具体type类型的数据
	int len;         								///< data数据长度
} ezDevSDK_secure_runtime_info;

/**
 * \brief SDK接收消息的结构体
 */
typedef struct {
    int type;       								///< \c EZDEVSDK_SECURE_MSG_TO_DEV_TYPE_E
    void *data;     								///< 指向具体type类型的数据
    int len;        								///< data数据长度
} ezDevSDK_secure_msg_to_dev;

/**
 * \brief SDK回调给用户层接口
 */
typedef struct 
{
    int (*recv_msg)(ezDevSDK_secure_msg_to_dev *msg);             ///< 消息回调接口，不可以阻塞
	int (*get_runtime_info)(ezDevSDK_secure_runtime_info *info);  ///< SDK运行时获取信息回调，不能有任何阻塞操作
} ezDevSDK_secure_callback;

typedef struct 
{
	ezDevSDK_secure_callback cb;  				///< 回调函数
	int ca_download_request;                    ///< CA证书申请下载 0:不申请 1:申请
} ezDevSDK_secure_init_info;
//--------------------------------SDK通知应用层消息-----------------------------------------

#define CA_NAME_MAX 24 
/**
 * \brief SDK给应用的消息
 */
typedef enum {
	EZDEVSDK_SECURE_ON_DOWNLOAD_CA,      ///< 下载CA根证书
}EZDEVSDK_SECURE_MSG_TO_DEV_TYPE_E;

typedef struct 
{
    char ca_file_name[CA_NAME_MAX];  ///< CA根证书文件名
    char *ca;               ///< CA根证书
}ezDevSDK_secure_ca_list_info;

typedef struct 
{
    char busi_name[CA_NAME_MAX];      ///业务名
    char ca_name[CA_NAME_MAX];        ///使用CA根证书文件名
    int  action;                      ///执行动作，0-不验证 1-验证，失败放过 2-强制验证 
}ezDevSDK_secure_busi_list_info;

typedef struct
{
    int ca_num;
    int busi_num;
    ezDevSDK_secure_ca_list_info *ca_list;
    ezDevSDK_secure_busi_list_info *busi_list;
}ezDevSDK_secure_ca_info;

//--------------------------------SDK获取设备信息-----------------------------------------

/**
 * \brief SDK运行时信息枚举和结构体
 */
typedef enum{
	EZDEVSDK_SECURE_ON_GET_CA_EXIST,	        ///< 判断CA文件是否存在, 消息体：ezDevSDK_secure_ca_exist
	EZDEVSDK_SECURE_ON_GET_CA_FILE_PATH,       ///<  获取CA根证书路径,消息体：\c ezDevSDK_secure_ca_file_path
} EZDEVSDK_SECURE_RUNTIME_INFO_E;

typedef struct 
{
    int enable;  //0-不存在 1-存在
}ezDevSDK_secure_ca_exist;


typedef struct 
{
    int  action;              //执行动作，0-不验证 1-验证，失败放过 2-强制验证
    char name[CA_NAME_MAX];   //业务名
    char file_path[64];       //ca 根证书路径    
}ezDevSDK_secure_ca_file_path;
//--------------------------------应用层配置参数-----------------------------------------

/**
 * \brief    SSL接口
 * \param[in] server_ip：服务域名（如果数字证书认证不为空） file_path:ca根证书路径 
 */
typedef struct {
    void* (*on_connect)(int socket, char* server_ip, char* file_path, int *error_status);///< 得到socket并生成相应句柄。如果成功返回handle，失败返回NULL
    int (*on_send)(void *handle, char* buf, int len, int *error_status);///< 如果成功返回发送长度，失败返回-1
    int (*on_recv)(void *handle, char* buf, int len);///< 如果成功返回接收长度，失败返回-1
    int (*on_close)(void *handle);///< 关闭句柄
} ezDevSDK_secure_ssl_callback;


#ifdef __cplusplus
}
#endif

#endif 


