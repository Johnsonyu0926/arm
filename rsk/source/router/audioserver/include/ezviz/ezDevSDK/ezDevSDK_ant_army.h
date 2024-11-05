/**
 * \file      ezDevSDK_ant_army.h  
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     蚁兵控制领域领域对外接口
 * 
 * \author    dengzihui
 * 
 * \date      2017/11/13
*/

#ifndef H_EZDEVSDK_ANT_ARMY_H_
#define H_EZDEVSDK_ANT_ARMY_H_

#include "ezDevSDK_error.h"
#include "ezDevSDK_domain_def.h"
#include "ezDevSDK_ant_army_def.h"

/**
 * \addtogroup Antarmy_Module 蚁兵领域模块
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief       蚁兵控制领域选项参数设置
 * \note        在ezDevSDK_antarmy_init()后调用
 * \param[in]   opt_type 选项类型，\ref EZDEVSDK_ANTARMY_OPT_TYPE_E
 * \param[in]   opt_val 选项参数，指向选项内容地址
 * \param[in]   opt_len 选项内容长度
 * \return      成功则返回0，失败返回非0
 * \retval      0 - 成功
 * \retval      非0 - 失败，\ref EZDEVSDK_ANT_ARMY_CODE_E
 */
int ezDevSDK_antarmy_setopt(int opt_type, void *opt_val, int opt_len);

/**
 * \brief       蚁兵控制领域初始化接口
 * \note        初始化蚁兵控制领域时会向微内核注册领域并设置信令处理函数
 * \param[in]   info 初始化信息
 * \return      成功则返回0，失败返回非0
 * \retval      0 - 成功
 * \retval      非0 - 失败，\ref EZDEVSDK_ANT_ARMY_CODE_E
 */
GLOBAL_API int ezDevSDK_antarmy_init(ezDevSDK_antarmy_init_info *info);

/**
 * \brief       蚁兵控制领域反初始化接口
 * \note        蚁兵控制领域反初始化时若蚁兵进程正在运行，反初始化接口会停止正在运行的蚁兵进程。蚁兵进程停止后,蚁兵控制线程会退出。
 * \return      成功则返回0，失败返回非0
 * \retval      0 - 成功
 * \retval      非0 - 失败，\ref EZDEVSDK_ANT_ARMY_CODE_E
 */
GLOBAL_API int ezDevSDK_antarmy_fini(void);

#ifdef __cplusplus
}
#endif

/*! \} */

#endif  /* H_EZDEVSDK_ANT_ARMY_H_ */
