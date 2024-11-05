/**
 * \file      ezDevSDK_ptz.h  
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     云台功能领域对外接口
 * 
 * \author    xiangbin
 * 
 * \date      2017/8/1
 */
#ifndef H_EZDEVSDK_PTZ_H_
#define H_EZDEVSDK_PTZ_H_

#include "ezDevSDK_error.h"
#include "ezDevSDK_domain_def.h"
#include "ezDevSDK_ptz_def.h"

/**
 * \addtogroup Ptz_Module 云台功能领域模块
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *  \brief      云台领域初始化，使用云台领域时，在系统启动时调用此接口初始化领域
 *  \method     ezDevSDK_ptz_init
 *  \param[in]  info 云台初始化结构体 \c ezDevSDK_ptz_init_info
 *  \return     错误码：
 *                  成功，EZDEVSDK_SUCCESS；
 *                  失败，EZDEVSDK_PTZ_CODE_BASE + EZDEVSDK_ERROR_FAIL；
 *                  参数错误，EZDEVSDK_PTZ_CODE_BASE + EZDEVSDK_ERROR_INVALID_PARAM。
 *  \see        错误码 ： EZDEVSDK_PTZ_CODE_E
 */
GLOBAL_API int ezDevSDK_ptz_init(const ezDevSDK_ptz_init_info *info);

/**
 *  \brief      云台领域反初始化，在系统退出或者领域不使用时调用此接口。
 *  \method     ezDevSDK_ptz_fini
 *  \param[in]  无
 *  \return     错误码：
 *                  成功，EZDEVSDK_SUCCESS；
 *                  失败，EZDEVSDK_PTZ_CODE_BASE + EZDEVSDK_ERROR_FAIL。
 *  \see        错误码 ： EZDEVSDK_PTZ_CODE_E
 */
GLOBAL_API int ezDevSDK_ptz_fini();

/**
 *  \brief      接收来自云台领域的消息，应用层在单独的线程中调用，接口非阻塞
 *  \method     ezDevSDK_ptz_recv_msg
 *  \param[in]  msg 云台初始化结构体 \c ezDevSDK_ptz_msg_to_dev，包含type消息类型、data消息内容指针、len消息内容长度
 *  \return     错误码：
 *                  成功，EZDEVSDK_SUCCESS；
 *                  失败，EZDEVSDK_PTZ_CODE_BASE + EZDEVSDK_ERROR_FAIL；
 *                  缓存长度不够，EZDEVSDK_PTZ_CODE_BUF_NOT_ENOUGH(返回msg->len为需要的长度)。
 *  \see        错误码 ： EZDEVSDK_PTZ_CODE_E
 */
GLOBAL_API int ezDevSDK_ptz_recv_msg(ezDevSDK_ptz_msg_to_dev *msg);

#ifdef __cplusplus
}
#endif

#endif

/*! \} */