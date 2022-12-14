/**
 * \file		ezDevSDK_leave_message.h
 *
 * \brief		开放平台对外结构体定义、宏定义
 *
 * \copyright	HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 *
 * \author		liuzhenhao
 *
 * \date		2018/11/19
 */

#ifndef H_EZDEVSDK_LEAVE_MESSAGE_H_
#define H_EZDEVSDK_LEAVE_MESSAGE_H_

#include "ezDevSDK_leave_message_def.h"
#include "ezDevSDK_domain_def.h"

/**
 * @addtogroup Leave_Message_Module 留言领域模块
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/** 
 *  \brief		留言领域初始化
 *  \method		ezDevSDK_leave_message_init
 *	\param[in]	info 始化参数
 *  \return 	成功返回0 失败返回非0
 *	\see		错误码 ： EZDEVSDK_LEAVE_MESSAGE_CODE_E
 */
GLOBAL_API int ezDevSDK_leave_message_init(const ezDevSDK_leave_message_init_info* info);

/** 
 *  \brief		留言领域反初始化
 *  \method		ezDevSDK_thirdparty_fini
 *  \return 	成功返回0 失败返回非0
 *	\see		错误码 ： EZDEVSDK_LEAVE_MESSAGE_CODE_E
 */
GLOBAL_API int ezDevSDK_leave_message_fini();

/** 
 *  \brief		设置ssl回调接口
 *  \method		ezDevSDK_leave_message_set_ssl_callback
 *	\note		发送留言消息之前需要先设置ssl回调函数。SDK会把socket交给上层，然后由上层执行数据发送
 *  \param[in] 	ssl_callback ssl回调函数
 *  \return 	成功返回0 失败返回非0
 *	\see		错误码 ： EZDEVSDK_LEAVE_MESSAGE_CODE_E
 */
 GLOBAL_API int ezDevSDK_leave_message_set_ssl_callback(ezDevSDK_leave_message_ssl_callback* ssl_callback);

/** 
 *  \brief		发送留言接口
 *  \method		ezDevSDK_leave_message_send_msg
 *	\note		发送留言数据
 *  \param[in] 	ssl_callback ssl回调函数
 *  \return 	成功返回0 失败返回非0
 *	\see		错误码 ： EZDEVSDK_LEAVE_MESSAGE_CODE_E
 */
 GLOBAL_API int ezDevSDK_leave_message_send_msg(ezDevSDK_leave_message_send_msg_info* msg_info);

 
 /** 
  *  \brief      留言数据查询接口
  *  \method     ezDevSDK_leave_message_query
  *  \note       向平台查询留言数据
  *  \return     成功返回0 失败返回非0
  *  \see        错误码 ： EZDEVSDK_LEAVE_MESSAGE_CODE_E
  */
  GLOBAL_API int ezDevSDK_leave_message_query();

   
 /** 
  *  \brief      留言数据接收接口
  *  \method     ezDevSDK_leave_message_recv_msg
  *  \note       向平台查询获取留言数据
  *  \return     成功返回0 失败返回非0
  *  \see        错误码 ： EZDEVSDK_LEAVE_MESSAGE_CODE_E
  */
  GLOBAL_API int ezDevSDK_leave_message_recv_msg(leave_message_query_info *query_info);


#ifdef __cplusplus
}
#endif

#endif
