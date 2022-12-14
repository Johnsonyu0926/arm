/**
 * \file		ezDevSDK_base_function.h
 *
 * \brief		基础功能领域模块
 *
 * \copyright	HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 *
 * \author		panlong
 *
 * \date		2017/3/10
 */

#ifndef H_EZDEVSDK_BASE_FUNCTION_H_
#define H_EZDEVSDK_BASE_FUNCTION_H_

#include "ezDevSDK_error.h"
#include "ezDevSDK_domain_def.h"
#include "ezDevSDK_base_function_def.h"

/**
 * \addtogroup BasicFunc_Module 基础功能领域模块
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/** 
 *  \brief		基础功能初始化
 *  \attention  请在调用微内核模块函数ezDevSDK_Start()前调用
 *  \method		ezDevSDK_base_function_init
 *  \param[in] 	info 初始化参数
 *  \return 	成功返回0，失败返回非0
 *  \retval     0 - 成功
 *  \retval     非0 - 失败，\ref EZDEVSDK_BASE_FUNC_CODE_E
 */
GLOBAL_API int ezDevSDK_base_function_init(ezDevSDK_base_function_init_info *info, int b_ezviz, void *ptx);

/** 
 *  \brief		基础功能反初始化
 *  \attention  请在调用微内核模块函数ezDevSDK_Stop()后调用
 *  \method		ezDevSDK_base_function_fini
 *  \return 	成功返回0，失败返回非0
 *  \retval     0 - 成功
 *  \retval     非0 - 失败，\ref EZDEVSDK_BASE_FUNC_CODE_E
 */
GLOBAL_API int ezDevSDK_base_function_fini(int b_ezviz, void *ptx);

/** 
 *  \brief		发送信令接口
 *  \method		ezDevSDK_base_function_send_msg
 *  \param[in] 	msg	往平台发送的信令
 *  \return 	成功返回0，失败返回非0
 *  \retval     0 - 成功
 *  \retval     非0 - 失败，\ref EZDEVSDK_BASE_FUNC_CODE_E
 */
GLOBAL_API int ezDevSDK_base_function_send_msg(const ezDevSDK_base_function_msg* msg);

/** 
 *  \brief		应用层主动接收sdk消息，非阻塞
 *  \method		ezDevSDK_base_function_recv_msg
 *  \deprecated	不建议使用此接口，建议通过回调的方式获取信令
 *  \param[in] 	msg	主动接收SDK的消息
 *  \return 	成功返回0，失败返回非0
 *  \retval     0 - 成功
 *  \retval     非0 - 失败，\ref EZDEVSDK_BASE_FUNC_CODE_E
 */
GLOBAL_API int ezDevSDK_base_function_recv_msg(ezDevSDK_base_function_msg_to_dev *msg);

/** 
 *  \brief		设置日志级
 *  \method		ezDevSDK_base_function_set_log_level
 *  \param[in] 	level 日志等级（0:不打印, 1:只打印错误, 2:打印警告及以上, 3:打印调试信息及以上, 4:打印所有信息）
 *  \return 	成功返回0，失败返回非0
 *  \retval     0 - 成功
 *  \retval     非0 - 失败，\ref EZDEVSDK_BASE_FUNC_CODE_E
 */
GLOBAL_API int ezDevSDK_base_function_set_log_level(int level);

/** 
 *  \brief		修改设备运行模式
 *  \method		ezDevSDK_base_function_change_mode
 *  \param[in] 	dev_mode 模式（6:低功耗模式，1:正常模式）
 *  \return 	成功返回0，失败返回非0
 *  \retval     0 - 成功
 *  \retval     非0 - 失败，\ref EZDEVSDK_BASE_FUNC_CODE_E
 */
GLOBAL_API int ezDevSDK_base_function_change_mode(ezDevSDK_base_function_dev_mode *dev_mode);

/** 
 *  \brief		设置设备操作码(用于萤石云用户添加,如不设置操作码默认使用SAP随机码(验证码))
 *  \method		ezDevSDK_base_function_set_operation_code
 *  \param[in] 	operation_code 设备操作码 
 *  \return 	成功返回0，失败返回非0
 *  \retval     0 - 成功
 *  \retval     非0 - 失败，\ref EZDEVSDK_BASE_FUNC_CODE_E
 */
GLOBAL_API int ezDevSDK_base_function_set_operation_code(ezDevSDK_base_function_operation_code *operation_code);

/**
 *  \brief		获取SDK当前版本号
 *  \method		ezDevSDK_base_function_get_sdk_version
 *  \return 	版本号
 */
GLOBAL_API const char* ezDevSDK_base_function_get_sdk_version();

/**
 * \brief       日志输出当前LBS, DAS, STUN, P2PServer的IP和Port信息
 * \method      ezDevSDK_base_function_show_server
 */
GLOBAL_API void ezDevSDK_base_function_show_server();

/**
 * \brief       日志输出信息master key, dev_id, verification key, p2pv3 link key和version
 * \method      ezDevSDK_base_function_show_key
 */
GLOBAL_API void ezDevSDK_base_function_show_key();


/** 
 *  \brief		dclog 日志直传接口，应用层实现数据组装
 *  \param[in] 	封装好的dclog埋点日志 pcDclogBuf
 *  \return 	GLOBAL_API int				错误码
 *  \asyn
 */
GLOBAL_API int ezDevSDK_base_function_dclog_direct_report(char *pcDclogBuf);

/** 
 *  \brief		升级包下载接口
 *  \param[in] 	upgrade_file  升级包下载的信息
 *  \param[in] 	read_len      指定回调的数据长度
 *  \return 	GLOBAL_API int	错误码
 *  \asyn
 */
GLOBAL_API int ezDevSDK_base_function_ota_download(ezDevSDK_base_function_upgrade_file* upgrade_file, int read_len);

/** 
 *  \brief	    设备通道能力级上报,服务端每隔三分钟允许上报一次,三分钟内上报多次只接收第一次上报的数据
 *  \param[in] 	通道能力级参数 ezDevSDK_base_channel_capacity_array
 *  \return 	GLOBAL_API int				错误码
 *  \asyn
 */
GLOBAL_API int ezDevSDK_base_channel_capacity_report(ezDevSDK_base_channel_capacity_array* channel_capacity_array);

/**
 * \brief       控制SDK日志 初始化base_function会自动启动日志
 * \method      ezDevSDK_base_function_log_ctrl
 * \param[in] 	type 0:停止日志 
 */
GLOBAL_API void ezDevSDK_base_function_log_ctrl(int type);

/** 
 *  \brief		外层触发SDK全量上报接口
 *  \return 	GLOBAL_API int				错误码
 *  \asyn
 */
GLOBAL_API int ezDevSDK_base_function_force_report_all();

#ifdef __cplusplus
}
#endif

/*! \} */

#endif  // H_EZDEVSDK_BASE_FUNCTION_H_
