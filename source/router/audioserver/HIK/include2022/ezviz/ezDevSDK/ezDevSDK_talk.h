/**
 * \file      ezDevSDK_talk.h  
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     对讲领域对外接口
 * 
 * \author    panlong
 * 
 * \date      2017/4/8
 */
#ifndef H_EZDEVSDK_TALK_H_
#define H_EZDEVSDK_TALK_H_

#include "ezDevSDK_talk_def.h"
#include "ezDevSDK_domain_def.h"
#include "ez_model_def.h"
/**
 * \addtogroup Talk_Module 对讲领域模块
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /** 
     *  \brief      对讲初始化
     *  \method     ezDevSDK_talk_init
     *  \access     
     *  \return     GLOBAL_API int
     */
    GLOBAL_API int ezDevSDK_talk_init(ezDevSDK_talk_init_info *init_info, int b_ezviz, void *ptx);

    /** 
     *  \brief      对讲反初始化
     *  \method     ezDevSDK_talk_fini
     *  \access
     *  \return     GLOBAL_API int
     */
    GLOBAL_API int ezDevSDK_talk_fini(int b_ezviz, void *ptx);

    /** 
     *  \brief      设置对讲码流数据
     *  \method     ezDevSDK_talk_set_data
     *  \access
     *  \param[in]
     *  \return     GLOBAL_API int
     */
    GLOBAL_API int ezDevSDK_talk_set_data(int channel, char *frame, int len);

    /** 
     *  \brief      获取对讲路数
     *  \method     ezDevSDK_talk_get_count
     *  \access
     *  \param[in]
     *  \return     GLOBAL_API int
     */
    GLOBAL_API int ezDevSDK_talk_get_count();

    /**
     *  \brief      对讲领域显示对讲状态信息
     *  \method     ezDevSDK_talk_show_state
     *  \return     错误码：
     *  \retval     成功，EZDEVSDK_TALK_CODE_SUCCESS；
     *  \retval     失败，EZDEVSDK_TALK_CODE_FAIL。
     *  \see        错误码 ： EZDEVSDK_TALK_CODE_E
     */
    GLOBAL_API int ezDevSDK_talk_show_state();

     /**
     *  \brief      对讲领域otap回复消息请求
     *  \method     ezDevSDK_talk_otap_reply_to_das
     *  \return     错误码：
     *  \see        错误码 EZ_ERR_CODE_E
     *  \param[in]  basic_info 通用消息信息
     *  \param[in]  msg_type 消息类型 set/operate/query等
     *  \param[in]  msg_seq 消息seq值
     *  \param[in]  err_code 16进制code值
     *  \param[in]  rsp  data结构体内的响应值
     */
    GLOBAL_API int ezDevSDK_talk_otap_reply_to_das(ez_basic_info* basic_info, char* msg_type, unsigned int msg_seq, int err_code, char* rsp);
#ifdef __cplusplus
}
#endif

#endif

/*! \} */