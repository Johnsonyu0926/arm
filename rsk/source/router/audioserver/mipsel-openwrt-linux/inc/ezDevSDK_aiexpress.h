/**
 * \file ezDevSDK_aiexpress.h
 * \author xurongjun (xurongjun\hikvision.com)
 * \brief AI高速通道模块，供有图片AI分析需求并且对延迟有比较高要求的业务使用
 * \version 0.1
 * \date 2019-06-14
 * 
 * \copyright Copyright (c) 2019
 * 
 */

#ifndef H_EZDEVSDK_AIEXPRESS_H_
#define H_EZDEVSDK_AIEXPRESS_H_

#include "ezDevSDK_error.h"
#include "ezDevSDK_domain_def.h"
#include "ezDevSDK_aiexpress_def.h"

/**
 * \addtogroup aiexpress_Module AI高速通道模块
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif
    /** 
     *  \brief      AI高速通道领域初始化
     *  \note       请在调用微内核模块函数ezDevSDK_Start()前调用
     *  \method     ezDevSDK_aiexpress_init
     *  \param[in]  info 始化参数
     *  \return     成功返回0，失败返回非0
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_AIEXPRESS_CODE_E
     */
    GLOBAL_API int ezDevSDK_aiexpress_init(const ezDevSDK_aiexpress_init_info_t *pinfo);

    /** 
     *  \brief      AI高速通道领域反初始化
     *  \note       请在调用微内核模块函数ezDevSDK_Stop()后调用
     *  \method     ezDevSDK_aiexpress_fini
     *  \return     成功返回0，失败返回非0
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_AIEXPRESS_CODE_E
     */
    GLOBAL_API int ezDevSDK_aiexpress_fini();

    /**
     * \brief 应用层通过此接口向AI服务发数据
     * 
     * \param pinfo 发送的文件数据附属相关信息
     * \param pdata 发送的文件数据
     * \param seq 文件发送的seq，如果选择异步发送，会在发送确认中带回
     * \param pattrs 文件发送配置
     * \return 接口调用错误码
     * \retval 0 - 成功
     * \retval 非0 - 失败，\ref EZDEVSDK_AIEXPRESS_CODE_E
     */
    GLOBAL_API int ezDevSDK_aiexpress_send(const ezDevSDK_aiexpress_info_t *pinfo ,const ezDevSDK_aiexpress_data_t *pdata,
                                           const ezDevSDK_aiexpress_attrs_t *pattrs);

#ifdef __cplusplus
}
#endif

/*! \} */

#endif // H_EZDEVSDK_THIRDPARTY_H_
