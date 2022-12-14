/**
 * \file      ezDevSDK_alarm.h  
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     报警领域对外接口
 * 
 * \author    panlong
 * 
 * \date      2017/4/8
 */

#ifndef H_EZDEVSDK_ALARM_H_
#define H_EZDEVSDK_ALARM_H_

#include "ezDevSDK_error.h"
#include "ezDevSDK_domain_def.h"
#include "ezDevSDK_alarm_def.h"

/**
 * \addtogroup Alarm_Module 报警领域模块
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /** 
     *  \brief      报警领域初始化
     *  \attention  请在调用微内核模块函数ezDevSDK_Start()前调用
     *  \method     ezDevSDK_alarm_init
     *  \param[in] 	init_info 初始化参数
     *  \return     成功返回0，失败返回非0。
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_ALARM_CODE_E
     */
    GLOBAL_API int ezDevSDK_alarm_init(const ezDevSDK_alarm_init_info *init_info);

    /** 
     *  \brief      报警领域反初始化
     *  \attention  请在调用微内核模块函数ezDevSDK_Stop()后调用
     *  \method     ezDevSDK_alarm_fini  
     *  \return     成功返回0，失败返回非0
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_ALARM_CODE_E
     */
    GLOBAL_API int ezDevSDK_alarm_fini();

    /** 
     *  \brief      上报报警，不带图片信息
     *  \method     ezDevSDK_alarm_push
     *  \param[in] 	business_type 业务类型，详见: \ref EZDEVSDK_ALARM_BUSINESS_TYPE_E
     *  \param[in]  alarm_type  报警类型
     *  \param[in]  channel 通道，0代表设备本身
     *  \param[in]  timestamp 报警时间戳
     *  \param[in]  extra_info 附加信息
     *  \return     成功返回0，失败返回非0
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_ALARM_CODE_E
     */
    GLOBAL_API int ezDevSDK_alarm_push(int business_type, char *alarm_type, int channel, time_t timestamp,
                                       ezDevSDK_alarm_extra_info extra_info);

    /** 
     *  \brief      上报报警，带图片信息
     *  \method     ezDevSDK_alarm_push_picture
     *  \param[in] 	business_type 业务类型，详见: \ref EZDEVSDK_ALARM_BUSINESS_TYPE_E
     *  \param[in]  alarm_type 报警类型
     *  \param[in]  channel 通道，0代表设备本身
     *  \param[in]  timestamp 报警时间戳
     *  \param[in]  data 报警图片数据
     *  \param[in]  extra_info 附加信息
     *  \return     成功返回0，失败返回非0
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_ALARM_CODE_E
     */
    GLOBAL_API int ezDevSDK_alarm_push_picture(int business_type, char *alarm_type, int channel, time_t timestamp,
                                               ezDevSDK_alarm_data data, ezDevSDK_alarm_extra_info extra_info);

    /** 
     *  \brief      上报报警，是否带图片信息，由参数data决定。此函数支持更为丰富的外部参数设置
     *  \method     ezDevSDK_alarm_push_EX
     *  \param[in]  alarm_msg_info  报警消息具体信息，其中sd_state，请设置在此结构体中
     *  \param[in]  data 报警图片数据，若无，传入NULL
     *  \param[in]  extra_info 附加信息，其中ezDevSDK_other_info中的sd_state无需再设置
     *  \return     成功返回0，失败返回非0
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_ALARM_CODE_E
     */
    GLOBAL_API int ezDevSDK_alarm_push_EX(ezDevSDK_alarm_msg_info *alarm_msg_info, ezDevSDK_alarm_data *data,
                                          ezDevSDK_alarm_extra_info extra_info);
    
    /** 
     *  \brief      上报报警，由参数data决定。支持多图报警需求
     *  \method     ezDevSDK_alarm_push_multi_pic
     *  \param[in]  alarm_msg_info  报警消息具体信息，其中sd_state，请设置在此结构体中
     *  \param[in]  data 报警图片数据，不能传空
     *  \param[in]  pic_num 报警图片张数
     *  \param[in]  extra_info 附加信息，其中ezDevSDK_other_info中的sd_state无需再设置
     *  \return     成功返回0，失败返回非0
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_ALARM_CODE_E
     */
    GLOBAL_API int ezDevSDK_alarm_push_multi_pic(ezDevSDK_alarm_msg_info *alarm_msg_info, ezDevSDK_alarm_data *data, int pic_num,
                                          ezDevSDK_alarm_extra_info extra_info);

    /** 
     *  \brief      设置ssl回调函数
     *  \method     ezDevSDK_alarm_set_ssl_callback
     *  \param[in]  ssl_callback ssl回调函数
     *  \return     EZDEVSDK_SUCCESS
     */
    GLOBAL_API int ezDevSDK_alarm_set_ssl_callback(ezDevSDK_alarm_ssl_callback ssl_callback);

    /** 
     *  \brief      设置SDK内部自动布撤防使能，默认SDK内部控制布撤防计划
     *  \method     ezDevSDK_alarm_set_auto_defense_enable
     *  \param[in]  enable 自动布撤防使能控制
     *              - 0: 外层控制布撤防计划
     *              - 1: SDK内部控制布撤防计划
     *  \return     EZDEVSDK_SUCCESS
     */
    GLOBAL_API int ezDevSDK_alarm_set_auto_defense_enable(int enable);

    GLOBAL_API int ezDevSDK_alarm_push_ai_tags(ezDevSDK_alarm_ai_tags *tags);
    
    GLOBAL_API int ezDevSDK_alarm_report_target_lib_add_result(int num, char *app_id,ezDevSDK_alarm_target_lib_result *lib_result);

    GLOBAL_API int ezDevSDK_alarm_report_target_lib_update(char *app_id, char *version);
    
#ifdef __cplusplus
}
#endif

#endif

/*! \} */