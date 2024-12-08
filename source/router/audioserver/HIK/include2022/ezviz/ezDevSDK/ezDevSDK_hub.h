#ifndef H_EZDEVSDK_HUB_H_
#define H_EZDEVSDK_HUB_H_
#include "ezDevSDK_hub_def.h"
#include "ezDevSDK_domain_def.h"
#include "ezDevSDK_Common_Module_Def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** 
 *  \brief		HUB 领域初始化
 *  \attention  请在调用微内核模块函数ezDevSDK_Start()前调用
 *  \method		ezDevSDK_hub_init
 *  \param[in] 	info 初始化参数
 *  \return 	成功返回0，失败返回非0
 *  \retval     0 - 成功
 *  \retval     非0 - 失败，\ref EZDEVSDK_HUB_CODE_E
 */
GLOBAL_API int ezDevSDK_hub_init(ezDevSDK_hub_init_info *info);

/** 
 *  \brief		HUB 领域反初始化初始化
 *  \method		ezDevSDK_hub_finit
 *  \param[in] 	info 初始化参数
 *  \return 	成功返回0，失败返回非0
 *  \retval     0 - 成功
 *  \retval     非0 - 失败，\ref EZDEVSDK_HUB_CODE_E
 */
GLOBAL_API int ezDevSDK_hub_finit();

/** 
 *  \brief		HUB 领域加载扩展模块
 *  \method		ezDevSDK_hub_extend_load
 *  \param[in] 	info 初始化参数
 *  \return 	成功返回0，失败返回非0
 *  \retval     0 - 成功
 *  \retval     非0 - 失败，\ref EZDEVSDK_HUB_CODE_E
 */
GLOBAL_API int ezDevSDK_hub_extend_load(ezDevSDK_hub_extend *extnd_info);

/** 
 *  \brief		HUB 领域发送子设备消息//INNER USED
 *  \method		ezDevSDK_hub_send_child_msg
 *  \param[in] 	msg 消息内容包含领域ID等
 *  \return 	成功返回0，失败返回非0
 *  \retval     0 - 成功
 *  \retval     非0 - 失败，\ref EZDEVSDK_HUB_CODE_E
 */
GLOBAL_API int ezDevSDK_hub_send_child_msg(ezDevSDK_hub_child_msg *msg);


/** 
 *  \brief		HUB 领域发送子设备透传消息
 *  \method		ezDevSDK_hub_send_child_msg
 *  \param[in] 	transfer_msg 消息内容
 *  \return 	成功返回0，失败返回非0
 *  \retval     0 - 成功
 *  \retval     非0 - 失败，\ref EZDEVSDK_HUB_CODE_E
 */
GLOBAL_API int ezDevSDK_hub_send_transfer_msg(ezDevSDK_hub_transfer_msg *transfer_msg);

#ifdef __cplusplus
}
#endif

#endif
