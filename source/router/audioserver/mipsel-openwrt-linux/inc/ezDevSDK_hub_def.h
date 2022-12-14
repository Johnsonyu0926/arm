/**
 * \file		ezDevSDK_hub_def.h
 *
 * \brief		开放平台对外结构体定义、宏定义
 *
 * \copyright	HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 *
 * \author		liuzhenhao
 *
 * \date		2019/12/23
 */

#ifndef H_EZDEVSDK_HUB_DEF_H_
#define H_EZDEVSDK_HUB_DEF_H_
#include "ezDevSDK_error.h"
#include "ezdev_sdk_kernel_struct.h"

//----------------------------------错误码--------------------------------------------------
/** 
 *  \brief	留言领域错误码
 */
typedef enum  { 
	EZDEVSDK_HUB_CODE_SUCCESS                     = 0x00000000,   ///< 成功
	EZDEVSDK_HUB_CODE_EXTEND_FULL,
} EZDEVSDK_HUB_CODE_E;

//--------------------------------初始化信息---------------------------------------------------------
typedef struct {
    int  type;                                   ///< 消息类型 \c EZDEVSDK_HUB_MSG_TO_DEV_TYPE_E
    void *data;                                  ///< 指向消息内容的指针
    int  len;                                    ///< 指向消息内容的长度
} ezDevSDK_hub_msg_to_dev;

typedef struct {
    int	 type;                                   ///< \c EZDEVSDK_HUB_RUNTIME_INFO_E
    void *data;                                  ///< 指向具体type类型的数据
    int  len;                                    ///< data数据长度
} ezDevSDK_hub_runtime_info;

/**
 * \brief   SDK回调给用户层接口
 */
typedef struct 
{
    int (*recv_msg)(ezDevSDK_hub_msg_to_dev *msg);              ///< 消息回调接口，不可以阻塞，主动接收消息和回调方式二选一，实现了回调接口，消息就走回调函数
    int (*get_runtime_info)(ezDevSDK_hub_runtime_info *info);   ///< SDK运行时获取信息回调，不能有任何阻塞操作
} ezDevSDK_hub_callback;

/**
 * \brief   SDK初始化接口
 */
typedef struct 
{
    int max;                                                    ///< 支持最大的关联关系个数
    int mode;                                                   ///< HUB工作模式 0:不托管子设备状态 1:托管子设备状态
    ezDevSDK_hub_callback cb;  							        ///< 回调函数
} ezDevSDK_hub_init_info;

typedef struct 
{
    int domain_id;
    void (*extend_data_route)(ezdev_sdk_kernel_submsg *ptr_submsg, char *child_dev_id); ///< 数据路由（领域回调函数）
} ezDevSDK_hub_extend;

typedef struct
{
    char *child_devid;
    char *msg;
    int cmd_id;
    int domain_id;
    unsigned char msg_response;                                 ///<是否为响应
    unsigned int msg_seq;
} ezDevSDK_hub_child_msg;

typedef struct
{
    char *child_dev_id;
    char *buf;
    int len;
} ezDevSDK_hub_transfer_msg;

//--------------------------------SDK通知应用层消息-----------------------------------------
/**
 * \brief   SDK给应用的消息
 */
typedef enum {
    EZDEVSDK_HUB_ON_ADD_CHILD_DEVICE                = 0x1,          ///<主设备添加子设备消息体:ezDevSDK_hub_child_device
    EZDEVSDK_HUB_ON_DELETE_CHILD_DEVICE             = 0x2,          ///< 主设备删除子设备消息体:ezDevSDK_hub_child_device
    EZDEVSDK_HUB_ON_TRANSFER_MSG                    = 0x3,          ///< HUB领域透传协议消息体:ezDevSDK_hub_transfer_msg
} EZDEVSDK_HUB_MSG_TO_DEV_TYPE_E;

typedef struct {
    char serial[64];                                   ///子设备序列号
    char verify_code[64];                              ///子设备验证码/删除关联时此内容为空
} ezDevSDK_hub_child_device;    

//--------------------------------SDK获取设备信息-----------------------------------------
/**
 * \brief SDK运行时信息枚举和结构体
 */
typedef enum {
    EZDEVSDK_HUB_RT_INFO_CHILD_DEVICE_RELATIONSHIP          = 0x1,          ///< 获取子设备的关联关系
    EZDEVSDK_HUB_RT_INFO_CHILD_DEVICE_ONLINE_STATUS         = 0X2,          ///< 获取子设备在线状态
} EZDEVSDK_HUB_RUNTIME_INFO_E;

typedef struct {
    int is_online;                                                  ///< 当前子设备是否在线
    char child_device_serial[64];                                   ///< 子设备序列号
    char child_device_version[64];                                  ///< 子设备版本号
    char child_device_type[64];                                     ///< 子设备型号
    char child_device_fwcode[128];                                  ///< 子设备固件识别码
} ezDevSDK_hub_child_device_relationship;

/**
 * \brief   子设备关联关系列表，对应消息类型: EZDEVSDK_HUB_RT_INFO_CHILD_DEVICE_RELATIONSHIP
 */
typedef struct {
    int num;                                                        ///< 入参，此page最大存储个数
    ezDevSDK_hub_child_device_relationship *relationships;          ///<子设备具体信息
} ezDevSDK_hub_child_device_relationship_page;

//--------------------------------API接口相关定义-----------------------------------------

#endif

