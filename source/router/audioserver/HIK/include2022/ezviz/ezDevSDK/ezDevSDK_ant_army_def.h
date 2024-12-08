/**
 * \file      ezDevSDK_ant_army_def.h  
 * 
 *  \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     蚁兵控制模块对外结构体定义、宏定义
 * 
 * \author    dengzihui
 * 
 * \date      2017/10/24
 */

#ifndef H_EZDEVSDK_ANT_ARMY_DEF_H_
#define H_EZDEVSDK_ANT_ARMY_DEF_H_
#include "ezDevSDK_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \addtogroup Antarmy_Module 
 * \{
 */

//-------------------------------------错误码------------------------------------------------------

/**
 * \brief 萤石SDK蚁兵控制领域的接口错误码，由SDK返回给调用者
 * \note 公用错误码，请参考ezDevSDK_error.h
 */
typedef enum  {
    EZDEVSDK_ANT_ARMY_SUCESS                    = 0X00000000, ///< 成功
    EZDEVSDK_ANT_ARMY_CODE_BASE                 = 0XD6000000,   ///< 蚁兵领域接口错误码起始值，D表示接口错误码，6000，表示蚁兵领域，最后三位表示具体错误

    ///< 0XD6000000 - 0XD6000001F 公共错误码
    EZDEVSDK_ANT_ARMY_CODE_FAIL                 = EZDEVSDK_ANT_ARMY_CODE_BASE + EZDEVSDK_ERROR_FAIL,
    EZDEVSDK_ANT_ARMY_CODE_NOT_INITED           = EZDEVSDK_ANT_ARMY_CODE_BASE + EZDEVSDK_ERROR_NOT_INITED,
    EZDEVSDK_ANT_ARMY_CODE_INVALID_PARAM        = EZDEVSDK_ANT_ARMY_CODE_BASE + EZDEVSDK_ERROR_INVALID_PARAM,
    EZDEVSDK_ANT_ARMY_CODE_NOT_SUPPORT          = EZDEVSDK_ANT_ARMY_CODE_BASE + EZDEVSDK_ERROR_NOT_SUPPORT,
    EZDEVSDK_ANT_ARMY_CODE_NOT_ENOUGH_MEMERY    = EZDEVSDK_ANT_ARMY_CODE_BASE + EZDEVSDK_ERROR_NOT_ENOUGH_MEMERY,
    EZDEVSDK_ANT_ARMY_CODE_INIT_FAIL            = EZDEVSDK_ANT_ARMY_CODE_BASE + EZDEVSDK_ERROR_INIT_FAIL,
    EZDEVSDK_ANT_ARMY_CODE_THREAD_CREATE_FAIL   = EZDEVSDK_ANT_ARMY_CODE_BASE + EZDEVSDK_ERROR_THREAD_CREATE_FAIL,
    EZDEVSDK_ANT_ARMY_CODE_RSP_TIMEOUT          = EZDEVSDK_ANT_ARMY_CODE_BASE + EZDEVSDK_ERROR_RSP_TIMEOUT,
    EZDEVSDK_ANT_ARMY_CODE_MSG_DATA_INVAILD     = EZDEVSDK_ANT_ARMY_CODE_BASE + EZDEVSDK_ERROR_MSG_DATA_INVAILD,

} EZDEVSDK_ANT_ARMY_CODE_E;

//-------------------------------------初始化信息--------------------------------------------------

/**
 * \brief 蚁兵控制领域获取设备信息结构体
 */
typedef struct {
    int     type;       ///< 需获取的运行时信息类型, \ref EZDEVSDK_ANTARMY_RUNTIME_INFO_E
    void*   data;       ///< 信息内容结构体
    int     len;        ///< 信息内容长度
} ezDevSDK_antarmy_runtime_info;

/**
 * \brief 蚁兵控制领域回调函数
 */
typedef struct
{
    int (*get_runtime_info)(ezDevSDK_antarmy_runtime_info* info);      ///< 消息回调接口，不可以阻塞
} ezDevSDK_antarmy_callback;

/**
 * \brief 蚁兵控制领域初始化参数
 */
typedef struct
{
    ezDevSDK_antarmy_callback cb;
    int is_delete_antarmy_file;                      ///< 是否删除已下载的蚁兵程序标记,初始化传入,值为1表示不需要删除 
} ezDevSDK_antarmy_init_info;


//-------------------------------------SDK通知应用层的消息-----------------------------------------


//-------------------------------------SDK从应用层获取的消息---------------------------------------

/**
 * \brief 蚁兵控制领域获取设备信息类型
 */
typedef enum {
    EZDEVSDK_ANTARMY_RT_INFO_GET_NET_CONNECT,   ///< 获取设备运行时的网络连接数，消息体：int类型。蚁兵进程根据此来判断设备是否正在发流(预览、回放、对讲、云存储、或发送告警图片)
}EZDEVSDK_ANTARMY_RUNTIME_INFO_E;


//-------------------------------------应用层发给SDK（或服务器）的消息------------------------------

/**
 * \brief 蚁兵控制领域参数设置选项
 */
typedef enum{
    EZDEVSDK_ANTARMY_OPT_TYPE_SET_INIT_INFO     ///< 设置蚁兵启动信息，包括名称和路径，<b>必须实现</b>。消息体: ezDevSDK_antarmy_opt_start_info
} EZDEVSDK_ANTARMY_OPT_TYPE_E;

/**
 * \brief 蚁兵进程启动信息，对应 \c EZDEVSDK_ANTARMY_OPT_TYPE_SET_INIT_INFO
 */
typedef struct
{
    char name[64];                              ///< 蚁兵进程可执行文件的名称，按工具链命名，例如antarmy_fuhan, antarmy_hisiv300
    char path[64];                              ///< 蚁兵进程可执行文件的绝对路径，例如 /home
    char version[64];                           ///< 蚁兵进程可执行文件的版本，可不填
} ezDevSDK_antarmy_opt_start_info;

/*! \} */

#ifdef __cplusplus
}
#endif

#endif  /* H_EZDEVSDK_ANT_ARMY_DEF_H_ */
