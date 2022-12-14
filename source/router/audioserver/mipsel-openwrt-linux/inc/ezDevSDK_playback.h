/**
 * \file      ezDevSDK_playback.h  
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
#ifndef H_EZDEVSDK_PLAYBACK_H_
#define H_EZDEVSDK_PLAYBACK_H_

#include "ezDevSDK_playback_def.h"
#include "ezDevSDK_domain_def.h"

/**
 * \addtogroup Playback_Module 回放领域模块
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *  \brief      回放领域初始化，使用回放领域时，在系统启动时调用此接口初始化领域
 *  \method     ezDevSDK_playback_init
 *  \param[in]  info 回放初始化结构体ezDevSDK_playback_init_info，包含参数回放路数和运行时回调接口
 *  \return     成功：EZDEVSDK_PLAYBACK_CODE_SUCCESS；
 *				失败：EZDEVSDK_PLAYBACK_CODE_FAIL；
 *			    参数错误：EZDEVSDK_PLAYBACK_CODE_INVALID_PARAM。
 *  \see        错误码 ： EZDEVSDK_PLAYBACK_CODE_E
 */
GLOBAL_API int ezDevSDK_playback_init(const ezDevSDK_playback_init_info *info, int b_ezviz, void *ptx);

/**
 *  \brief      回放领域反初始化，在系统退出或者领域不使用时调用此接口。
 *  \method     ezDevSDK_playback_fini
 *  \param[in]  无
 *  \return     成功：EZDEVSDK_PLAYBACK_CODE_SUCCESS；
 *				失败：EZDEVSDK_PLAYBACK_CODE_FAIL。
 *  \see        错误码 ： EZDEVSDK_PLAYBACK_CODE_E
 */
GLOBAL_API int ezDevSDK_playback_fini(int b_ezviz, void *ptx);

/** 
 *  \brief	    回放领域数据写入，上层根据回放的sid写入回放数据，内部由TCP方式发送数据，可能会有阻塞，上层调用者应该单独启动线程发送数据。
 *  \method	    ezDevSDK_playback_write
 *  \param[in]  sid 回放sid标识，一个sid表示一路回放
 *  \param[in]  frame_info 回放帧数据信息
 *  \return     错误码 ：
 *				    成功，EZDEVSDK_PLAYBACK_CODE_SUCCESS；
 *				    失败，EZDEVSDK_PLAYBACK_CODE_FAIL。
 *  \see        错误码 ： EZDEVSDK_PLAYBACK_CODE_E
 */
GLOBAL_API int ezDevSDK_playback_write(int sid, ezDevSDK_playback_frame_info *frame_info);

/** 
 *  \brief	    回放停止，当上层收到回放结束的消息时，调用此接口来结束回放并释放SDK内部资源。
 *  \method	    ezDevSDK_playback_close
 *  \param[in]  sid   回放sid标识，一个sid表示一路回放
 *  \return     成功：EZDEVSDK_PLAYBACK_CODE_SUCCESS；
 *				失败：EZDEVSDK_PLAYBACK_CODE_FAIL。
 *  \see        错误码 ： EZDEVSDK_PLAYBACK_CODE_E
 */
GLOBAL_API int ezDevSDK_playback_close(int sid);

/** 
 *  \brief	    回放领域重置回放控制信息，当回放开始或回放到下一个录像段时，应该调用此接口来复位sdk内部回放控制信息。
 *  \method	    ezDevSDK_playback_reset_ctrl_info
 *  \param[in]  sid 回放sid标识，一个sid表示一路回放
 *  \return     成功：EZDEVSDK_PLAYBACK_CODE_SUCCESS；
 *				失败：EZDEVSDK_PLAYBACK_CODE_FAIL。
 *  \see        错误码 ： EZDEVSDK_PLAYBACK_CODE_E
 */
GLOBAL_API int ezDevSDK_playback_reset_ctrl_info(int sid);

/**
 *  \brief      获取正在回放的路数
 *  \method 	ezDevSDK_playback_get_count
 *  \return     正在回放的路数
 */
GLOBAL_API int ezDevSDK_playback_get_count();

/**
 *  \brief      回放领域显示取流信息
 *  \method	    ezDevSDK_playback_show_stream
 *  \return     成功：EZDEVSDK_PLAYBACK_CODE_SUCCESS；
 *				失败：EZDEVSDK_PLAYBACK_CODE_FAIL。
 *  \see        错误码 ： EZDEVSDK_PLAYBACK_CODE_E
 */
GLOBAL_API int ezDevSDK_playback_show_stream();


#ifdef __cplusplus
}
#endif

#endif

/*! \} */