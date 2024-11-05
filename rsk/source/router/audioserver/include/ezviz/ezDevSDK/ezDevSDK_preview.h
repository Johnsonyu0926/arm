/**
 * \file      ezDevSDK_preview.h  
 * 
 * \filepath  e:\workdir\小项目\ezDevSDK_v2.0.0\userpace\src\domain\base_function\ezDevSDK_base_function.h
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     基础功能领域对外接口
 * 
 * \author    xiangbin
 * 
 * \date      2017/5/22
 */
#ifndef H_EZDEVSDK_PREVIEW_H_
#define H_EZDEVSDK_PREVIEW_H_

#include "ezDevSDK_preview_def.h"
#include "ezDevSDK_domain_def.h"

/**
 * \addtogroup Preview_Module 预览领域模块
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *  \brief      预览领域初始化，使用预览领域时，在系统启动时调用此接口初始化领域
 *  \method	    ezDevSDK_preview_init
 *  \param[in]  info 回放初始化结构体ezDevSDK_preview_init_info，包含参数预览路数、通道数、每个通道码流数和运行时回调接口
 *  \return     错误码：
 *				    成功，EZDEVSDK_PREVIEW_CODE_SUCCESS；
 *				    失败，EZDEVSDK_PREVIEW_CODE_FAIL；
 *				    参数错误，EZDEVSDK_PREVIEW_CODE_INVALID_PARAM。
 *  \see        错误码 ： EZDEVSDK_PREVIEW_ERROR_CODE_E
 */
GLOBAL_API int ezDevSDK_preview_init(const ezDevSDK_preview_init_info *info, int b_ezviz, void *ptx);

/**
 *  \brief      预览领域反初始化，在系统退出或者领域不使用时调用此接口。
 *  \method	    ezDevSDK_preview_fini
 *  \param[in]  无
 *  \return     错误码：
 *				    成功，EZDEVSDK_PREVIEW_CODE_SUCCESS；
 *				    失败，EZDEVSDK_PREVIEW_CODE_FAIL。
 *  \see        错误码 ： EZDEVSDK_PREVIEW_ERROR_CODE_E
 */
GLOBAL_API int ezDevSDK_preview_fini(int b_ezviz, void *ptx);

/**
 *  \brief      预览领域直接发送音视频帧数据，接口为非阻塞
 *  \method	    ezDevSDK_preview_send
 *  \param[in]  sid 会话ID，大于0
 *  \param[in]  frame_info 帧数据信息，ezDevSDK_preview_frame_info结构体描述一帧数据信息
 *  \return     错误码：
 *				    成功，EZDEVSDK_PREVIEW_CODE_SUCCESS；
 *				    失败，EZDEVSDK_PREVIEW_CODE_FAIL，返回失败时，调用者应当停止写入数据。
 *  \see        错误码 ： EZDEVSDK_PREVIEW_ERROR_CODE_E
 */
GLOBAL_API int ezDevSDK_preview_send(int sid, ezDevSDK_preview_frame_info *frame_info);

/**
 *  \brief      预览领域释放会话资源
 *  \method	    ezDevSDK_preview_free
 *  \param[in]  sid 会话ID，大于0
 *  \return     错误码：
 *				    成功，EZDEVSDK_PREVIEW_CODE_SUCCESS；
 *				    失败，EZDEVSDK_PREVIEW_CODE_FAIL，返回失败时，调用者应当停止写入数据。
 *  \see        错误码 ： EZDEVSDK_PREVIEW_ERROR_CODE_E
 */
GLOBAL_API int ezDevSDK_preview_free(int sid);

/**
 *  \brief      预览领域选项参数获取
 *  \method	    ezDevSDK_preview_getopt
 *  \param[in]  opt_type 选项类型，枚举 \c EZDEVSDK_PREVIEW_OPT_TYPE_E
 *  \param[in]  opt_val 选项参数，指向选项内容地址
 *  \param[in]  opt_len 选项内容长度
 *  \return     错误码：
 *				    成功，EZDEVSDK_PREVIEW_CODE_SUCCESS；
 *				    失败，EZDEVSDK_PREVIEW_CODE_FAIL；
 *                  不支持，EZDEVSDK_PREVIEW_CODE_NOT_SUPPORT。
 *  \see        错误码 ： EZDEVSDK_PREVIEW_ERROR_CODE_E
 */
GLOBAL_API int ezDevSDK_preview_getopt(int opt_type, void *opt_val, int opt_len);

/**
 *  \brief      预览领域选项参数设置
 *  \method	    ezDevSDK_preview_setopt
 *  \param[in]  opt_type 选项类型，枚举 \c EZDEVSDK_PREVIEW_OPT_TYPE_E
 *  \param[in]  opt_val 选项参数，指向选项内容地址
 *  \param[in]  opt_len 选项内容长度
 *  \return     错误码：
 *				    成功，EZDEVSDK_PREVIEW_CODE_SUCCESS；
 *				    失败，EZDEVSDK_PREVIEW_CODE_FAIL；
 *                  不支持，EZDEVSDK_PREVIEW_CODE_NOT_SUPPORT。
 *  \see        错误码 ： EZDEVSDK_PREVIEW_ERROR_CODE_E
 */
GLOBAL_API int ezDevSDK_preview_setopt(int opt_type, void *opt_val, int opt_len);

/**
 *  \brief      预览领域显示取流信息
 *  \method	    ezDevSDK_preview_show_stream
 *  \return     错误码：
 *				    成功，EZDEVSDK_PREVIEW_CODE_SUCCESS；
 *				    失败，EZDEVSDK_PREVIEW_CODE_FAIL。
 *  \see        错误码 ： EZDEVSDK_PREVIEW_ERROR_CODE_E
 */
GLOBAL_API int ezDevSDK_preview_show_stream();

/** 
 *  \brief	    设备通道能力级上报,服务端每隔三分钟允许上报一次,三分钟内上报多次只接收第一次上报的数据
 *  \param[in] 	通道能力级参数 ezDevSDK_preview_video_quality_report
 *  \return 	GLOBAL_API int				错误码
 *  \asyn
 */
GLOBAL_API int ezDevSDK_preview_video_quality_report(ezDevSDK_preview_video_quality_array* video_qualitys);

/** 
 *  \brief	    电池P2P预链接设置/由IOT-WIFI完成穿透 CPU启动后设置给CPU侧的SDK
 *  \param[in] 	预链接参数ezDevSDK_preview_valid_p2p_info
 *  \return 	GLOBAL_API int				错误码
 *  \asyn
 */
GLOBAL_API int ezDevSDK_preview_bat_p2p_link_actived(ezDevSDK_preview_valid_p2p_info *p2p_info);

/** 
 *  \brief	    设置电池设备使用的DAS-udp信息，用于打洞时的外网探测
 *  \param[in] 	ip das-ip  port das-udp-port
 *  \return 	GLOBAL_API int				错误码
 *  \asyn
 */
GLOBAL_API int ezDevSDK_preview_bat_das_udp_info(char *ip ,int port);

/** 
 *  \brief	    电池P2P取流传递对应的UDP链路传递的取流信令
 *  \param[in] 	buf 取流信令sessionkey 信令加密的会话秘钥
 *  \return 	GLOBAL_API int				错误码
 *  \asyn
 */
GLOBAL_API int ezDevSDK_preview_bat_p2p_play(char *buf, int buf_len, char *sessionkey);

#ifdef __cplusplus
}
#endif

#endif

/*! \} */
