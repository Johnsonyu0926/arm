/**
 * \file      ezDevSDK_error.h  
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     对外公共错误码,各领域共用
 * 
 * \author    ezDevSDK Team
 * 
 * \date      2019/3/28
 */

#ifndef H_EZDEVSDK_ERROR_H_
#define H_EZDEVSDK_ERROR_H_

/**
 * \brief 萤石SDK各领域公共错误码，范围0X00~0X1F
 */
#define EZDEVSDK_SUCCESS                   0x0000  ///< 成功
#define EZDEVSDK_ERROR_FAIL                0X0001  ///< 失败
#define EZDEVSDK_ERROR_NOT_INITED          0X0002  ///< 领域未初始化
#define EZDEVSDK_ERROR_INVALID_PARAM       0X0003  ///< 非法参数
#define EZDEVSDK_ERROR_NOT_SUPPORT         0X0004  ///< 功能不支持
#define EZDEVSDK_ERROR_NOT_ENOUGH_MEMERY   0X0005  ///< 内存不足
#define EZDEVSDK_ERROR_INIT_FAIL           0X0006  ///< 领域初始化失败
#define EZDEVSDK_ERROR_THREAD_CREATE_FAIL  0X0007  ///< 线程创建失败
#define EZDEVSDK_ERROR_RSP_TIMEOUT         0X0008  ///< 等待信令响应超时
#define EZDEVSDK_ERROR_MSG_DATA_INVAILD    0X0009  ///< 无效的报文数据
#define EZDEVSDK_ERROR_ALREADY_INITED      0X000A  ///< 领域已初始化
#define EZDEVSDK_ERROR_JSON_ERROR          0X000B  ///< JSON错误
#define EZDEVSDK_ERROR_MSG_SEND_ERROR      0X000C  ///< 消息发送错误

#endif // H_EZDEVSDK_ERROR_H_
