/**\filename  ezDevSDK_SmartControl.h
*  \filepath  ezDevSDK-V2-Release\ezDevSDK_v2.0.2\userpace\inc\ezDevSDK_SmartControl.h
*  \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*  \brief     智能控制领域对外接口
*  \author    liuxiangchen
*  \date      2018/7/5
*/
#ifndef H_EZDEVSDK_SMART_CONTROL_H_
#define H_EZDEVSDK_SMART_CONTROL_H_

#include "ezDevSDK_SmartControl_def.h"
#include "ezDevSDK_domain_def.h"


#ifdef __cplusplus
extern "C"
{
#endif

/** 
 *  \brief		智能控制功能初始化
 *  \method		ezDevSDK_SmartControl_init
 *  \access	    private 
 *  \return 	GLOBAL_API int
 */
GLOBAL_API int ezDevSDK_SmartControl_init(const ezDevSDK_SmartControl_init_info *init_info);


/** 
 *  \brief		发送信令接口
 *  \method		ezDevSDK_SmartControl_Msg2Server
 *  \access	    private
 *  \param[in] 	const ezDevSDK_SmartControl_Msg2Server_S pstMsg		消息
 *  \return 	GLOBAL_API int
 */
GLOBAL_API int ezDevSDK_SmartControl_Msg2Server(const ezDevSDK_SmartControl_Msg2Server_S* pstMsg);



/** 
 *  \brief		智能控制领域，推送埋点
 *  \method		ezDevSDK_SmartControl_PushDclog
 *  \access	    private 
 *  \return 	GLOBAL_API int
 */
GLOBAL_API int ezDevSDK_SmartControl_PushDclog(ezDevSDK_SmartControl_DclogMsg_S *pstMsg);




/** 
 *  \brief		智能控制功能反初始化
 *  \method		ezDevSDK_SmartControl_fini
 *  \access	    private 
 *  \return 	GLOBAL_API int
 */
GLOBAL_API int ezDevSDK_SmartControl_fini(void);



#ifdef __cplusplus
}
#endif

#endif

