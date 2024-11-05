/**
 * \file		ezDevSDK_leave_message_def.h
 *
 * \brief		开放平台对外结构体定义、宏定义
 *
 * \copyright	HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 *
 * \author		liuzhenhao
 *
 * \date		2018/11/29
 */

#ifndef H_EZDEVSDK_LEAVE_MESSAGE_DEF_H_
#define H_EZDEVSDK_LEAVE_MESSAGE_DEF_H_
#include <time.h>
#include "ezDevSDK_error.h"

//----------------------------------错误码--------------------------------------------------
/** 
 *  \brief	留言领域错误码
 */
typedef enum  { 
	EZDEVSDK_LEAVE_MESSAGE_CODE_SUCCESS                     = 0x00000000,   ///< 成功
	EZDEVSDK_LEAVE_MESSAGE_CODE_BASE                        = 0xD3500000,   ///< 留言领域接口错误码起始值，D表示接口错误码，3500，表示云存储领域，最后三位表示具体错误
    EZDEVSDK_LEAVE_MESSAGE_CODE_FAIL                        = EZDEVSDK_LEAVE_MESSAGE_CODE_BASE + EZDEVSDK_ERROR_FAIL,
    EZDEVSDK_LEAVE_MESSAGE_CODE_NOT_INITED                  = EZDEVSDK_LEAVE_MESSAGE_CODE_BASE + EZDEVSDK_ERROR_NOT_INITED,
    EZDEVSDK_LEAVE_MESSAGE_CODE_INVALID_PARAM               = EZDEVSDK_LEAVE_MESSAGE_CODE_BASE + EZDEVSDK_ERROR_INVALID_PARAM,
    EZDEVSDK_LEAVE_MESSAGE_CODE_NOT_SUPPORT                 = EZDEVSDK_LEAVE_MESSAGE_CODE_BASE + EZDEVSDK_ERROR_NOT_SUPPORT,
	EZDEVSDK_LEAVE_MESSAGE_SSL_NO_INIT                      = 0xD3500020,
	EZDEVSDK_LEAVE_MESSAGE_SSL_CONNECT_ERROR                = 0xD3500021,
	EZDEVSDK_LEAVE_MESSAGE_CODE_NO_VALID_SERVER             = 0xD3500022,
	EZDEVSDK_LEAVE_MESSAGE_REPORT_MSG_TO_DAS_ERROR          = 0xD3500023,
	EZDEVSDK_LEAVE_MESSAGE_CODE_TIME_OUT                    = 0xD3500024,
	EZDEVSDK_LEAVE_MESSAGE_CODE_OPEN_READ_ERRROR            = 0xD3500025,
	EZDEVSDK_LEAVE_MESSAGE_CODE_READ_DATA_ERRROR            = 0xD3500026,
	EZDEVSDK_LEAVE_MESSAGE_CODE_GET_DATA_ERRROR             = 0xD3500027,
	EZDEVSDK_LEAVE_MESSAGE_CODE_LEAVE_DATA_NOT_READY        = 0xD3500028,
} EZDEVSDK_LEAVE_MESSAGE_CODE_E;

//--------------------------------初始化信息---------------------------------------------------------
typedef struct {
    int  type;                                   ///< 消息类型 \c EZDEVSDK_LEAVE_MESSAGE_MSG_TO_DEV_TYPE_E
    void *data;                                  ///< 指向消息内容的指针
    int  len;                                    ///< 指向消息内容的长度
} ezDevSDK_leave_message_msg_to_dev;

typedef struct {
    int	 type;                                   ///< \c EZDEVSDK_LEAVE_MESSAGE_RUNTIME_INFO_E
    void *data;                                  ///< 指向具体type类型的数据
    int  len;                                    ///< data数据长度
} ezDevSDK_leave_message_runtime_info;

typedef struct 
{
    int iChannel;  ///<通道号
    int isEndFlag; ///<留言数据获取结束标记(0:未结束 1：结束)
    char *pLeaveData; ///<留言数据
    int iLeaveDataLen; ///<留言大小
    int iLeaveTimeLen;  ///<本次留言数据时长
} ezDevSDK_leave_message_send_data_info;


typedef enum
{
    EZDEVSDK_LEAVE_SEND_SUCCESS = 0,
    EZDEVSDK_LEAVE_SEND_FAILED,
    EZDEVSDK_LEAVE_SEND_MAX,
}EZDEVSDK_LEAVE_NOTIFY_TYPE;

/**
 * \brief   SDK回调给用户层接口
 */
typedef struct 
{
    int (*recv_msg)(ezDevSDK_leave_message_msg_to_dev *msg);           ///< 消息回调接口，不可以阻塞，主动接收消息和回调方式二选一，实现了回调接口，消息就走回调函数
    int (*get_runtime_info)(ezDevSDK_leave_message_runtime_info *info); ///< SDK运行时获取信息回调，不能有任何阻塞操作
    int (*get_leave_data)(ezDevSDK_leave_message_send_data_info *data_info, int *pDataSeq);  ///<留言数据序列号); ///< 留言推送时获取留言数据回调，不能有任何阻塞操作
    int (*on_leave_data_notify)(int iChannel,unsigned int iDataSeq, int notify_type);       //通知留言发送状态 notify_type参见枚举值 EZDEVSDK_LEAVE_NOTIFY_TYPE
} ezDevSDK_leave_message_callback;

/**
 * \brief   SDK回调给用户层SSL接口
 */
typedef struct {
    void* (*on_connect)(int socket);								///< 得到socket并生成相应句柄。如果成功返回handle，失败返回NULL
    int (*on_send)(void *handle, char* buf, int len);				///< 如果成功返回发送长度，失败返回-1
    int (*on_recv)(void *handle, char* buf, int len);				///< 如果成功返回接收长度，失败返回-1
    int (*on_close)(void *handle);									///< 关闭句柄
}ezDevSDK_leave_message_ssl_callback;

/**
 * \brief   SDK初始化接口
 */
typedef struct 
{
    int max_channel;                                                ///< 最大通道数 
    ezDevSDK_leave_message_callback cb;  							///< 回调函数
} ezDevSDK_leave_message_init_info;

//--------------------------------SDK通知应用层消息-----------------------------------------
/**
 * \brief   SDK给应用的消息
 */
typedef enum {
    EZDEVSDK_LEAVE_MESSAGE_ON_QUERY_INFO= 0x1,                      ///<接收留言查询信息,通知设备平台对应的留言信息，对应结构体：ezDevSDK_leave_message_query_Info
    EZDEVSDK_LEAVE_MESSAGE_ON_CONTENT= 0x2,                         ///<留言数据，对应结构体：ezDevSDK_leave_message_content
} EZDEVSDK_LEAVE_MESSAGE_MSG_TO_DEV_TYPE_E;

//--------------------------------SDK获取设备信息-----------------------------------------
/**
 * \brief SDK运行时信息枚举和结构体
 */
typedef enum {
    EZDEVSDK_LEAVE_MESSAGE_RT_INFO_ENCRYPT_STATUS = 0x1,            ///< 获取视频加密密码，<b>必须实现</b>
} EZDEVSDK_LEAVE_MESSAGE_RUNTIME_INFO_E;

/**
 * \brief \c EZDEVSDK_LEAVE_MESSAGE_RT_INFO_ENCRYPT_STATUS
 */
typedef struct {
    int encrypt_status;                                             ///< 音视频加密状态
    char encrypt_code[36];                                          ///< 音视频加密秘钥
    int encrypt_len;                                                ///< 加密秘钥长度
} ezDevSDK_leave_message_encrypt_status;

//--------------------------------API接口相关定义-----------------------------------------

/**
 * \brief 留言类型
 */
typedef enum {
    EZDEVSDK_LEAVE_AUDIO_MSEEAGE = 0,  ///<音频留言
    EZDEVSDK_LEAVE_VIDEO_MSEEAGE = 1,  ///< 视频留言
} EZDEVSDK_LEAVE_MESSAGE_TYPE_E;


typedef struct 
{
    int dev_type;                                                   ///<设备类型 莹宝1
    int type;                                                       ///<留言类型:EZDEVSDK_LEAVE_MESSAGE_TYPE_E
    int channel;                                                    ///<通道号
    time_t time_stamp;                                              ///<时间戳 单位:S
    char *pic_data;                                                 ///<留言封面数据
    int pic_len;                                                    ///<留言封面大小
    char *leave_message_data;                                       ///<留言数据
    int leave_message_len;                                          ///<留言大小
    int leave_message_time_len;                                     ///<留言时长
} ezDevSDK_leave_message_send_msg_info;


typedef struct 
{
     char acFileID[128];  ///<留言文件ID
     int iLength;         ///<留言文件长度
     time_t iTimestamp;  ///<留言文件时间戳信息
     int iFileType;       ///<要接收的文件类型,当前只支持音频留言(0:普通文件, 1:视频流, 2:图片, 3:音频, 4:视频留言 5:音频留言)  
     int iChannel;        ///<通道号(留言接收功能目前不区分通道号，默认0)
} leave_message_query_info;


typedef struct {
    int query_info_count;  ///<留言信息体个数
    leave_message_query_info *query_info; ///<留言信息体
} ezDevSDK_leave_message_query_info;

typedef struct {
    int iType;          ///<留言数据类型(0:视频头 1：数据)
    char *acLeaveBuf;  ///<留言数据
    int iLength;         ///<留言数据长度
} ezDevSDK_leave_message_content;


#endif
