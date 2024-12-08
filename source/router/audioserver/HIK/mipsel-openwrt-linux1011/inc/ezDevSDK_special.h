/**
 * \file      ezDevSDK_special.h
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     特殊功能领域对外接口
 * 
 * \author    liuzhenhao
 * 
 * \date      2017/8/1
 */

#ifndef H_EZDEVSDK_SPEICAL_H_
#define H_EZDEVSDK_SPEICAL_H_

#include "ezDevSDK_error.h"
#include "ezDevSDK_domain_def.h"
#include "ezDevSDK_special_def.h"

/**
 * \addtogroup Special_Module 特殊功能领域模块
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief 特殊领域初始化，使用特殊领域时，在系统启动时调用此接口初始化领域
 * \param[in] 	info 初始化参数
 * \return     成功返回0，失败返回非0。
 * \retval     0 - 成功
 * \retval     非0 - 失败，\ref EZDEVSDK_SPECIAL_CODE_E
 */
GLOBAL_API int ezDevSDK_special_init(const ezDevSDK_special_init_info *info);

/**
 * \brief 特殊领域反初始化，在系统退出或者领域不使用时调用此接口。
 * \return 返回EZDEVSDK_SUCCESS。
 */
GLOBAL_API int ezDevSDK_special_fini();

/**
 * \brief 客流统计使用传输客流信息
 * \param[in] 	msg 客流统计信息列表
 * \return     成功返回0，失败返回非0。
 * \retval     0 - 成功
 * \retval     非0 - 失败，\ref EZDEVSDK_SPECIAL_CODE_E
 */
GLOBAL_API int ezDevSDK_special_report_flow_counting(ezDevSDK_special_flow_counting_seg_count *msg);

/**
 * \brief 发送SDK定义的消息到平台
 * \param[in] 	send_msg 向平台发送的封装信息内容
 * \return     成功返回0，失败返回非0。
 * \retval     0 - 成功
 * \retval     非0 - 失败，\ref EZDEVSDK_SPECIAL_CODE_E
 */
GLOBAL_API int ezDevSDK_special_send_msg(ezDevSDK_special_msg *send_msg);

/**
 * \brief       发送外部组装的信令信息到平台
 * \param[in] 	send_msg 向平台发送的信息
 * \return     成功返回0，失败返回非0。
 * \retval     0 - 成功
 * \retval     非0 - 失败，\ref EZDEVSDK_SPECIAL_CODE_E
 */
GLOBAL_API int ezDevSDK_special_send_msg_to_platform(ezDevSDK_special_trans_cmd_msg* send_msg);

/** 
 *  \brief      应用层传递sdk special事件
 *  \method     ezDevSDK_special_post_event
 *  \access     private 
 *  \param[in]  ezDevSDK_special_post_event_info_S * pstEvent   消息体
 *  \return     成功返回0，失败返回非0。 错误码
 *  \see        错误码 ： EZDEVSDK_SPECIAL_CODE_E
 */
GLOBAL_API int ezDevSDK_special_post_event(ezDevSDK_special_post_event_info_S *pstEvent);


/*
 * \brief 即时上报当前的托管关系
 *
*/
GLOBAL_API int ezDevSDK_special_report_trusteeship(void);


#ifdef __cplusplus
}
#endif

/*! \} */

#endif
