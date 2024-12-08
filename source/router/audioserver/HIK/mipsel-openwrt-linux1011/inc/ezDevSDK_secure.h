/**\filename  ezDevSDK_secure.h 
*  \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*  \brief     安全领域对外接口实现
*  \author    jinrongshun
*  \date      2021/02/21
*/

#ifndef H_EZDEVSDK_SECURE_H_
#define H_EZDEVSDK_SECURE_H_

#include "ezDevSDK_domain_def.h"
#include "ezDevSDK_secure_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief 安全领域初始化接口
 * 
 * @param init_info  安全通知回调
 * @return  EZDEVSDK_SECURE_CODE_E
 */
GLOBAL_API EZDEVSDK_SECURE_CODE_E ezDevSDK_secure_init(ezDevSDK_secure_init_info* init_info);

/**
 * @brief 安全领域反初始化接口
 * 
 * @return  EZDEVSDK_SECURE_CODE_E
 */
GLOBAL_API EZDEVSDK_SECURE_CODE_E ezDevSDK_secure_fini();

/**
 * @brief CA证书更新接口
 * 
 * @param error_status  错误码
 * @param business  业务名称
 * @return  EZDEVSDK_SECURE_CODE_E
 */
GLOBAL_API EZDEVSDK_SECURE_CODE_E ezDevSDK_secure_ca_update_request(int error_status, char *business);


/** 
 *  \brief		设置SSL回调函数
 *  \param[in] 	ezDevSDK_secure_ssl_callback ssl_callback
 *  \return 	成功返回0，失败返回非0。		错误码
 *  \see        错误码 ： EZDEVSDK_SECURE_CODE_E
 */
GLOBAL_API EZDEVSDK_SECURE_CODE_E ezDevSDK_secure_set_ssl_callback(ezDevSDK_secure_ssl_callback ssl_callback);




#ifdef __cplusplus
}
#endif

/*! \} */

#endif // H_EZDEVSDK_SECURE_H_

