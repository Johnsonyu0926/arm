/**
 * \file      ezDevSDK_DoorLock.h
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     智能门锁领域对外接口
 * 
 * \author    liuxiangchen
 * 
 * \date      2019/5/1
 */

#ifndef H_EZDEVSDK_DOOR_LOCK_H_
#define H_EZDEVSDK_DOOR_LOCK_H_

#include "ezDevSDK_DoorLock_def.h"
#include "ezDevSDK_domain_def.h"

/**
 * \addtogroup DoorLock_Module 智能门锁领域模块
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief 智能门锁领域初始化接口，启动模块。
 * \param[in] 	pstParam 初始化参数
 * \return     0  - 成功
 *             非0 - 失败，参见 EZDEVSDK_DOORLOCK_CODE_E
 */
GLOBAL_API int ezDevSDK_DoorLock_Init(const ezDevSDK_DoorLock_InitParam_S *pstParam);

/**
 * \brief 智能门锁领域反初始化接口，注销模块。
 * \return     无。
 */
GLOBAL_API void ezDevSDK_DoorLock_Fini(void);


/**
 * \brief 发送SDK定义的消息到平台
 * \param[in] 	pstMsg 向平台发送的封装信息内容
 * \return     0  - 成功
 *             非0 - 失败，参见 EZDEVSDK_DOORLOCK_CODE_E
 */
GLOBAL_API int ezDevSDK_DoorLock_Send2Srv_Msg(ezDevSDK_DoorLock_msg2Srv_S *pstMsg);







#ifdef __cplusplus
}
#endif





#endif
