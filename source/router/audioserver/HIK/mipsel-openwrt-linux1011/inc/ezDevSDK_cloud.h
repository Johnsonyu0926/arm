/**
 * \file       ezDevSDK_cloud.h  
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     云存储领域对外接口
 * 
 * \author    panlong
 * 
 * \date      2017/4/8
 */

#ifndef H_EZDEVSDK_CLOUD_H_
#define H_EZDEVSDK_CLOUD_H_

#include "ezDevSDK_cloud_def.h"
#include "ezDevSDK_domain_def.h"

/**
 * \addtogroup Cloud_Module 云服务领域模块
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/** 
 *  \brief		云存储初始化
 *  \method		ezDevSDK_cloud_init
 *  \access	    private 
 *  \return 	成功返回0，失败返回非0。
 *  \see        错误码 ： EZDEVSDK_CLOUD_CODE_E
 */
GLOBAL_API int ezDevSDK_cloud_init(const ezDevSDK_cloud_init_info *init_info, const ezDevSDK_cloud_callback* cb);

/** 
 *  \brief		云存储反初始化
 *  \method		ezDevSDK_base_function_fini
 *  \access	    private 
 *  \return 	成功返回0，失败返回非0。
 *  \see        错误码 ： EZDEVSDK_CLOUD_CODE_E
 */
GLOBAL_API int ezDevSDK_cloud_fini();

/**
 *  \brief		开始结束云录像
 *  \method		ezDevSDK_cloud_record
 *  \param[in] 	int status, 1 开始，0 结束            
 *  \param[in]  time_t time          
 *  \param[in]  char alarm_type[32]             
 *  \return 	成功返回0，失败返回非0。            
 *  \see        错误码 ： EZDEVSDK_CLOUD_CODE_E
 */
GLOBAL_API int ezDevSDK_cloud_record(int status, int channel);

/** 
 *  \brief		应用层接收sdk消息
 *  \method		ezDevSDK_cloud_recv_msg
 *  \access	    private 
 *  \param[in] 	cloud_msg_to_dev * msg		  消息体
 *  \return 	成功返回0，失败返回非0。	    错误码
 *  \see        错误码 ： EZDEVSDK_CLOUD_CODE_E
 */
GLOBAL_API int ezDevSDK_cloud_recv_msg(ezDevSDK_cloud_msg_to_dev *msg);

/** 
 *  \brief		设置SSL回调函数
 *  \method		ezDevSDK_cloud_set_ssl_callback
 *  \access	    private 
 *  \param[in] 	ezDevSDK_cloud_ssl_callback ssl_callback
 *  \return 	成功返回0，失败返回非0。		错误码
 *  \see        错误码 ： EZDEVSDK_CLOUD_CODE_E
 */
GLOBAL_API int ezDevSDK_cloud_set_ssl_callback(ezDevSDK_cloud_ssl_callback ssl_callback);



/** 
 *  \brief		应用层传递sdk云存储事件
 *  \method		ezDevSDK_cloud_post_event
 *  \access	    private 
 *  \param[in] 	ezDevSDK_cloud_post_event_info * pstEvent		消息体
 *  \return 	成功返回0，失败返回非0。	    错误码
 *  \see        错误码 ： EZDEVSDK_CLOUD_CODE_E
 */
GLOBAL_API int ezDevSDK_cloud_post_event( ezDevSDK_cloud_post_event_info_S *pstEvent );

/**
 *  \brief      云存储领域选项参数设置
 *  \method	    ezDevSDK_cloud_setopt
 *  \param[in]  opt_type 选项类型，枚举 \c EZDEVSDK_CLOUD_OPT_TYPE_E
 *  \param[in]  opt_val 选项参数，指向选项内容地址
 *  \param[in]  opt_len 选项内容长度
 */
GLOBAL_API int ezDevSDK_cloud_setopt(int opt_type, void *opt_val, int opt_len);

/**
 *  \brief      云存储领域dclog回调设置
 *  \method	    ezDevSDK_cloud_set_dclog_callback
 *  \param[in]  函数指针结构体：ezDevSDK_cloud_dclog_callback
 */
GLOBAL_API int ezDevSDK_cloud_set_dclog_callback(ezDevSDK_cloud_dclog_callback dclog_callback);

/**
 *  \brief      云存储由用户触发推送图片,为异步接口,接口返回存在延时
 *  \method     ezDevSDK_cloud_user_trigger_push_picture
 *  \param[in]  picInfo:推图结构体
 *  \param[in]  timeout:接口超时时间(单位秒)
 *  \return     推送成功返回0 失败返回-1
 */
GLOBAL_API int ezDevSDK_cloud_user_trigger_push_picture(ezdev_cloud_pic_push_S *picInfo, unsigned int timeout);

/**
 *  \brief      云存储由用户触发推送云存储录像功能的使能开关
 *  \method     ezDevSDK_cloud_user_trigger_enable
 *  \param[in]  enable: 0:关闭 1:开启
 *  \param[in]  time: 0:无限制时间        大于0:上报录像时间 (单位秒)
 *  \return     成功返回0  正在运行返回-2  其他失败原因返回-1
 */
GLOBAL_API int ezDevSDK_cloud_user_trigger_enable(int enable,int time);

/**
 *  \brief      云存储录像数据直接推送打开句柄
 *  \method     ezDevSDK_cloud_open_record_data
 *  \param[in]  begin_time：录像开始时间
 *  \return     成功返回0 失败返回-1
 */
GLOBAL_API int ezDevSDK_cloud_open_record_data(long long begin_time);

/**
 *  \brief      云存储录像数据直接推送
 *  \method     ezDevSDK_cloud_send_record_data
 *  \param[in]  type: 2:数据 3:最后一个切片
 *  \param[in]  rec_data: 录像数据 rec_len：录像长度
 *  \param[in]  begin_time：录像开始时间 end_time：录像结束时间
 *  \return     数据推送成功返回0 失败返回-1
 */
GLOBAL_API int ezDevSDK_cloud_send_record_data(int type, char *rec_data, int rec_len, long long begin_time, long long end_time);

/**
 *  \brief      云存储录像数据停止发送
 *  \method     ezDevSDK_cloud_close_record_data
 *  \return     成功返回0 失败返回-1
 */
GLOBAL_API int ezDevSDK_cloud_close_record_data(void);

#ifdef __cplusplus
}
#endif

#endif

/*! \} */