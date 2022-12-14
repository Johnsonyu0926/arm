/**\filename  ezDevSDK_ota.h 
*  \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*  \brief     ota领域对外接口实现
*  \author    shenhongyin
*  \date      2020/06/02
*/

#ifndef H_EZDEVSDK_OTA_H_
#define H_EZDEVSDK_OTA_H_

#include "ezDevSDK_domain_def.h"
#include "ezDevSDK_ota_def.h"

/**
 * \addtogroup ota_Module 设备升级模块
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief ota领域初始化接口
     * 
     * @param pmodules 升级模块的信息
     * @param pehandler  升级过程通知回调
     * @return  ezDevSDK_ota_code_e
     */
    GLOBAL_API ezDevSDK_ota_code_e ezDevSDK_ota_init(ezDevSDK_ota_init_info *ota_init_info);

    /**
     * @brief 查询升级包信息接口,查询超时时间10s
     * 
     * @param pinfos 升级模块信息
     * @return  ezDevSDK_ota_code_e
     */
    GLOBAL_API ezDevSDK_ota_code_e ezDevSDK_ota_query(const ezDevSDK_ota_query_infos_t *pinfos);

    /**
     * @brief 升级进度上报接口
     * 
     * @param state 
     * @param progress 
     * @param code 
     * @return ezDevSDK_ota_code_e
     */
    GLOBAL_API ezDevSDK_ota_code_e ezDevSDK_ota_report(ezDevSDK_ota_state_e state, int progress, ezDevSDK_ota_report_err_e code);

    /**
     * @brief 升级包下载接口,异步接口
     * 
     * @param ezDevSDK_ota_download_info_t ,下载升级所需的参数结构体
     * @param download_cb    下载包回调
     * @return  如果返回-1,  表示失败
     */
GLOBAL_API int ezDevSDK_ota_file_download(ezDevSDK_ota_download_info_t* download_info, ezDevSDK_ota_download_cb download_cb);

    /**
     * @brief ota反初始化接口
     * 
     * @return ezDevSDK_ota_code_e 
     */
    GLOBAL_API ezDevSDK_ota_code_e ezDevSDK_ota_fini();

#ifdef __cplusplus
}
#endif

/*! \} */

#endif // H_EZDEVSDK_OTA_H_