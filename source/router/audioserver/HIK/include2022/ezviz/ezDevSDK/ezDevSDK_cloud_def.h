/**
 * \file      ezDevSDK_cloud_def.h  
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     云存储领域对外结构体定义、宏定义
 * 
 * \author    panlong
 * 
 * \date      2017/4/8
*/
#ifndef H_EZDEVSDK_CLOUD_DEF_H_
#define H_EZDEVSDK_CLOUD_DEF_H_

#include <time.h>
#include "ezDevSDK_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \addtogroup Cloud_Module
 * \{
 */

/**
 *  @brief 萤石SDK错误码
 */
typedef enum  {                 
	EZDEVSDK_CLOUD_CODE_SUCCESS                     = 0x00000000,   ///< 成功
	EZDEVSDK_CLOUD_CODE_BASE                        = 0xD3000000,   ///< 云存储领域接口错误码起始值，D表示接口错误码，3000，表示云存储领域，最后三位表示具体错误
	EZDEVSDK_CLOUD_CODE_EXIT                        = 0xD3000001,   ///< 云存储强制退出
	EZDEVSDK_CLOUD_CODE_SEND_PIC_ERROR              = 0xD3000020,
	EZDEVSDK_CLOUD_CODE_REC_ERROR                   = 0xD3000021,
	EZDEVSDK_CLOUD_CODE_ALARM_VIDEO_ERROR           = 0xD3000022,
	EZDEVSDK_CLOUD_CODE_INTERRUPT                   = 0xD3000023,   ///<强制中断退出:例如硬盘错误等
} EZDEVSDK_CLOUD_CODE_E;

//--------------------------------初始化信息---------------------------------------------------------
typedef struct {
    int max_channel;
    int mq_relative_timeout;//mq消息队列是否采用相对时间
} ezDevSDK_cloud_init_info;

//--------------------------------SDK通知应用层消息-----------------------------------------
/**
 * \brief   SDK给应用的消息类型
 */
typedef enum  {
    EZDEVSDK_CLOUD_ON_PRE_AFTER,                ///< 预录延录时间
    EZDEVSDK_CLOUD_ON_ENABLE,                   ///< 使能开关
    EZDEVSDK_CLOUD_SET_RECORD_PLAN,             ///< 设置录像计划
    EZDEVSDK_CLOUD_SPACE_INFO,                  ///< 云空间信息,见EZDEVSDK_CLOUD_SPACE_INFO_E
    EZDEVSDK_CLOUD_SET_TRIGGER_RULES,           ///< 触发规则,见ezDevSDK_cloud_trigger_rules_array
    EZDEVSDK_CLOUD_MANUAL_TRIGGER,              ///< 手动触发云存储,见ezDevSDK_cloud_manual_trig_param
    EZDEVSDK_CLOUD_SET_BACKUP_RULES,            ///< 备份云存储规则,ezDevSDK_cloud_backup_rules
    EZDEVSDK_CLOUD_ON_MAX
} EZDEVSDK_CLOUD_MSG_TO_DEV_TYPE_E;

typedef enum {
    EZDEVSDK_CLOUD_SPACE_INFO_NOT_ENABLE,
    EZDEVSDK_CLOUD_SPACE_INFO_EXPIRE,
    EZDEVSDK_CLOUD_SPACE_INFO_FULL,
}EZDEVSDK_CLOUD_SPACE_INFO_E;
/**
 * \brief   SDK向设备推送的dclog类型
 */
typedef enum  {
    EZDEVSDK_CLOUD_STATISTICS_DCOLG,              ///<保存云存储帧数据统计埋点，信息体:ezDevSDK_cloud_statistics_info
    EZDEVSDK_CLOUD_SET_MAX
} EZDEVSDK_CLOUD_DCLOG_INFO_TO_DEV_TYPE_E;


/**
 * \brief   SDK给应用的消息结构体(dclog)
 */
typedef struct {
	int              type;    ///< SDK给应用的消息类型,信息体:EZDEVSDK_CLOUD_DCLOG_INFO_TO_DEV_TYPE_E
	void*            data;    ///< 所给消息内容
	int              len;      ///< 所给消息内容的长度
} ezDevSDK_cloud_dclog_info_to_dev;


/**
 * \brief   SDK给应用的消息结构体
 */
typedef struct {
	EZDEVSDK_CLOUD_MSG_TO_DEV_TYPE_E    type;
	void*                               data;
	int                                 len;
} ezDevSDK_cloud_msg_to_dev;

/**
 * \brief   预录延录信息
 */
typedef struct {
    int pre;
    int after;
} ezDevSDK_cloud_pre_after_time;

/**
 * \brief   使能开关
 */
typedef struct {
    int channel;                                 ///< 入参
    int enable;                                  ///< 出参
} ezDevSDK_cloud_enable;

/**
 * \brief   云存储录像计划时间
 */
typedef struct {
    time_t begin;                                ///< 计划开始时间
    time_t end;                                  ///< 计划结束时间
} ezDevSDK_cloud_time_segment;

/**
 * \brief   云存储录像计划
 */
typedef struct {
    int plan_type;                               ///< 计划类型0:当前计划 1:待生效计划
    int channel;                                 ///< 通道号
    int status;                                  ///< 存储计划状态0:关闭1:开启
    ezDevSDK_cloud_time_segment time[7][4];      ///< 对应7天（一周），每天4个时段的计划时间
} ezDevSDK_cloud_record_plan;

/**
 * \brief   云存储触发规则
 */
typedef struct {
    char rule_type[32];                           ///< 触发规则
    int channel;                                  ///< 通道号
    int enable;                                   ///< 使能状态 0:关闭 1:开始
}ezDevSDK_cloud_trigger_rule;

/**
 * \brief   云存储触发规则数组
 */
typedef struct {
	int num;									  ///<规则个数
    ezDevSDK_cloud_trigger_rule rules[32];        ///< 云存储触发规则
}ezDevSDK_cloud_trigger_rules_array;

/**
 * \brief   手动触发云存储指令参数
 */
typedef struct
{
	int channel;       ///< 通道号
	char reqId[64];    ///< 信令ID
	int cmd;		   ///< 0(结束录像), 11(开始录像),
	int recordTime;    ///< "录像时间 精确到秒
} ezDevSDK_cloud_manual_trig_param;


/* DCLOG公共参数 */
typedef struct
{
    char  acSystemName[64];   /* dclog文件类型 */
    char  acDevId[64];        /* 设备序列号 */
    char  acDevType[64];      /* 设备类型 */
    char  acFwVer[64];        /* 固件版本信息 */
    int   iLogTime;           /* 此log产生的时间 */
    int   iSequence;          /* log序号 */
} ezDevSDK_DCLog_CommParam;



/* 云存储统计埋点数据，包括I帧/P帧 总数统计等 */
typedef struct
{
    ezDevSDK_DCLog_CommParam   stComm;   /* 公共参数 */
    int      iCloudType;            /* 云存储服务器类型 */
    char     acFileId[64];          /* 云录像文件ID */
    time_t   lBeginTime;            /* 录像开始时间 */
    time_t   lEndTime;              /* 录像结束时间 */
    int      iIFrame_TotalNum;      /* 云录像文件总的I帧数 */
    int      iPFrame_TotalNum;      /* 云录像文件总的P帧数 */
    int      iIFrame_SendNum;       /* 设备发送的I帧总数 */
    int      iPFrame_SendNum;       /* 设备发送的P帧总数 */
    int      iSegSuccNum;           /* 录像文件上报成功片段数 */
    int      iSegFailNum;           /* 录像文件上报失败片段数 */
    int      iHigh_Br;              /* 高码率切片数 */
    int      iLow_Br;               /* 低码率切片数 */
} ezDevSDK_DCLog_CloudStatisticsParam;

/**
 * \brief  云存储统计埋点数据，包括I帧/P帧 总数统计等
 */
typedef struct
{
	int      uiDclogResetFlag;       /* dclog重置标志 0-不清楚，只添加  1-重置当前埋点数据(dclog推送成功) --入参*/
	int      current_num;             /* 结构体ezDevSDK_cloud_statistics_dclog 的个数*/
	ezDevSDK_DCLog_CloudStatisticsParam *stCloudStatistics;   
} ezDevSDK_cloud_statistics_info;


/**
 * \brief  云存储备份规则
 */
typedef struct
{
    int enable;         ///< 规则使能开关 0-关闭 1-开启
    int trigger_type;   ///< 触发类型 1-计划推流 2-本地录像即将覆盖时推流
    int video_type;     ///< 推流录像类型 -1-全部录像 1-事件录像 2-连续录像
    int start_time;     ///< 上传开始时间（偏移秒数）,触发类型为1有效
    int start_percent;  ///< 从SD卡容量指定百分比之后开始推流，触发类型为2有效
    ezDevSDK_cloud_time_segment time[5][7];  ///<对应5个时间段，分别为7天（当天偏移秒数）
}ezDevSDK_cloud_backup_rules;

//--------------------------------SDK获取设备信息-----------------------------------------
/**
 * \brief   SDK获取设备信息类型，对应 \c on_get_runtime_info回调函数
 */
typedef enum {
    //以下参数是萤石云下发的配置保存在设备上，当SDK需要时再从设备处获取
    EZDEVSDK_CLOUD_RT_INFO_GET_CFG_ENABLE,      ///< 使能开关, \c ezDevSDK_cloud_enable
    EZDEVSDK_CLOUD_RT_INFO_GET_TRIGGER_RULES,   ///< 云录像触发规则, \c ezDevSDK_cloud_trigger_rules_array

    //以下参数是设备运行时信息
    EZDEVSDK_CLOUD_RT_INFO_GET_ENCRYPT_KEY,     ///< 音频加密状态， \c ezDevSDK_cloud_encrypt_status
    EZDEVSDK_CLOUD_RT_INFO_GET_TIME_ZONE,       ///< 时区信息， \c ezDevSDK_cloud_time_zone_info
    
    EZDEVSDK_CLOUD_RT_INFO_GET_CURRENT_SLICE_INFO, ///< 获取当前录像切片的帧信息
    EZDEVSDK_CLOUD_RT_INFO_GET_CURRENT_SLICE_TOTAL_SIZE, ///< 获取当前录像切片总长度
    EZDEVSDK_CLOUD_RT_INFO_GET_CLOUD_RECORD_PLAN,   ///< 获取当前的云存储录像计划
    EZDEVSDK_CLOUD_RT_INFO_GET_DSP_SLICE_INFO,	///< 获取本次云存储推送中dsp产生的录像切片的帧信息
    EZDEVSDK_CLOUD_RT_INFO_GET_BACKUP_RULES,    ///< 获取备份云存储规则, \c ezDevSDK_cloud_backup_rules
} EZDEVSDK_CLOUD_RUNTIME_INFO_E;


/**
 * \brief   SDK获取设备信息类型，对应 \c on_get_runtime_info回调函数
 */
typedef enum {
	EZDEVSDK_CLOUD_GET_STATISTICS_DCOLG_COUNT,  ///<获取云存储帧数据统计埋点，信息体:ezDevSDK_cloud_statistics_info
    EZDEVSDK_CLOUD_GET_STATISTICS_DCOLG,  ///<获取云存储帧数据统计埋点，信息体:ezDevSDK_cloud_statistics_info
	EZDEVSDK_CLOUD_GET_MAX,
} EZDEVSDK_CLOUD_DCLOG_INFO_E;


/**
 * \brief   SDK获取应用的消息结构体(dclog)
 */
typedef struct {
	int              type; ///< 需获取的设备信息类型 信息体:EZDEVSDK_CLOUD_DCLOG_INFO_E
	void*            data; ///< 信息内容结构体
	int              len;  ///< 信息内容长度
} ezDevSDK_cloud_get_dev_dclog_info;



/**
 * \brief   应用传递SDK的事件类型，对应接口ezDevSDK_cloud_post_event()
 */
typedef enum
{
    EZDEVSDK_CLOUD_POST_EVENT_REC_FLUSH = 0,

    EZDEVSDK_CLOUD_POST_EVENT_MAX,

} EZDEVSDK_CLOUD_POST_EVENT_INFO_E;


/**
 * \brief   音视频加密状态
 */
typedef struct {
    int     encrypt_status;//出参
    char    encrypt_code[36];//出参
    int     encrypt_len;    //出参
    int     channel;        //入参
} ezDevSDK_cloud_encrypt_status;

/**
 * \brief   时区信息
 */
typedef struct {
    int hour;
    int minute;
    int no;
} ezDevSDK_cloud_time_zone_info;

/**
 * \brief   SDK获取设备信息结构体
 */
typedef struct {
	EZDEVSDK_CLOUD_RUNTIME_INFO_E   type;
	void*                           data;
	int                             len;
} ezDevSDK_cloud_runtime_info;


/**
 * \brief   设备传递事件给SDK 结构体
 */
typedef struct {
	int    iEvent;  // 参见EZDEVSDK_CLOUD_POST_EVENT_INFO_E
	char*  pcData;
	int    iLen;
} ezDevSDK_cloud_post_event_info_S;

typedef enum{
    CLOUD_NOTIFY_HEAD = 1,
    CLOUD_NOTIFY_MID,
    CLOUD_NOTIFY_END,
    CLOUD_NOTIFY_RESET,
    CLOUD_NOTIFY_USER_PUSH,
} ezDevSDK_cloud_notify_type;
/**
 * \brief   云存储取流类型
 */
typedef struct  {
  int type;///< 1.云存储流头,2.云存储数据分片,3 云存储最后一个数据分片,5 主动推送 结构体:ezDevSDK_cloud_notify_type
  int channel; ///< 通道
} ezDevSDK_cloud_stream_type;

/**
 * \brief   每一帧的帧头
 */
typedef struct {
    unsigned int length;		                ///< 数据实际长度
	unsigned int type;                          ///< 码流类型:0:I帧,1:P帧
	unsigned long long timestamp;               ///< 时间，单位毫秒(1970-01-01 00:00:00 +0000 (UTC)再乘以1000)
} ezDevSDK_cloud_stream_header;

typedef struct{
    unsigned long long time;    ///< 创建时间
    int encrypt_status;         ///< 加密状态 0-不加密；3-用户加密；4-平台加密
    char encrypt_code[36];      ///< 加密秘钥
    int encrypt_len;            ///< 加密秘钥长度
    int tzOffset;               ///< 时区偏移
    char *picData;              ///< 图片数据
    unsigned int picDatalen;    ///< 图片数据长度
}ezdev_cloud_pic_push_S;

/**
 * \brief   码流类型
 */
typedef enum
{
    ezDevSDK_cloud_SliceVideo_StreamType_Invalid,
    ezDevSDK_cloud_SliceVideo_StreamType_High,  ///< 高码率
    ezDevSDK_cloud_SliceVideo_StreamType_Low,   ///< 低码率

    ezDevSDK_cloud_SliceVideo_StreamType_MAX,
} ezDevSDK_cloud_SliceVideo_StreamType;


/**
 * \brief   录像切片的数据
 */
typedef struct
{
    unsigned char   ucStreamType;    					///< 码流类型, \cezDevSDK_cloud_SliceVideo_StreamType
    unsigned int  uiIFrameNum;     					///< 切片中I帧数量
    unsigned int  uiNFrameNum;     					///< 切片中其它帧数量
} ezDevSDK_cloud_SliceVideo_Info;



typedef enum {
    EZDEVSDK_DEVICE_IPC_CLOUD = 0x0,
    EZDEVSDK_DEVICE_DCLOG_MAX,
} EZDEVSDK_CLOUD_DCLOG_TYPE_E;


/**
 * \brief  获取将要发送切片数据的总长度
 */
typedef struct {
    char  enable;
    int   len;
	int   channel;
} ezDevSDK_cloud_slice_total_size;

/**
 * \brief  云存储期间dsp产生的录像帧数据总数
 */
typedef struct
{
    unsigned int  uiTotalIFrameNum;     					///< I帧总数
    unsigned int  uiTotalNFrameNum;     					///< 其它帧总数
} ezDevSDK_cloud_TotalSliceVideo_Info;

typedef struct {
    ezDevSDK_cloud_stream_type type;
    unsigned int seq;              //序号
    long long begin_time;          //begin_time  1970-01-01 00:00:00 +0000 (UTC)
    long long end_time;            //end_time 1970-01-01 00:00:00 +0000 (UTC)
    char *frame;                    //录像数据
    int len;                       //录像长度
    long long event_time;          //事件触发事件
    char *ai_labels;                //AI标签数据
    unsigned int ai_labels_len;     //AI标签数据长度
    char is_get_url;                //是否为获取url
}ezDevSDK_cloud_get_slice_data_S;

/**
 * \brief   SDK回调给用户层接口, SDK运行时获取信息回调，不能有任何阻塞操作
 */
typedef struct {
    /** 
     * \brief		    获取云存储分片数据，数据要求按设置的分片时长返回
     * \method		    sliceData 切片数据
     * \param[in]	    type
     * \return 	        成功返回0，失败返回非0。只有成功之后才会往萤石云推送数据，
                        如果录像数据没有达到10s应该返回失败
     * \see             错误码 ： EZDEVSDK_CLOUD_CODE_E
     */
    int (*on_get_slice_data)(ezDevSDK_cloud_get_slice_data_S *sliceData);
    
    /** 
     * \brief		    通知录像发送状态
     * \method		    on_stream_data_notify
     * \param[in]	    type
     * \param[in]       seq 序号
     * \param[in]       notify_type 获取云存储分片数据发送结果，0 数据发送成功, 1 数据没有发送成功
     * \return 	        成功返回0，失败返回非0。只有成功之后才会往萤石云推送数据，
                        如果录像数据没有达到10s，应该返回失败
     * \see             错误码 ： EZDEVSDK_CLOUD_CODE_E
     */
    int (*on_stream_data_notify)(ezDevSDK_cloud_stream_type *type, unsigned int seq, int notify_type); 

    /** 
     * \brief		    获取运行时信息对应EZDEVSDK_CLOUD_RUNTIME_INFO_E
     * \method		    on_get_runtime_info
     * \param[in]	    info           
     * \return 	        成功返回0，失败返回非0。
     * \see             错误码 ： EZDEVSDK_CLOUD_CODE_E
     */
	int (*on_get_runtime_info)(ezDevSDK_cloud_runtime_info *info);
} ezDevSDK_cloud_callback;

/**
 * \brief    SSL接口
 */
typedef struct {
    void* (*on_connect)(int socket);
    int (*on_send)(void *handle, char* buf, int len, int *error_status);
    int (*on_recv)(void *handle, char* buf, int len);
    int (*on_close)(void *handle);
} ezDevSDK_cloud_ssl_callback;


/**
 *	@brief SDK回调给应用层接口, SDK运行时获取和保存dclog埋点数据回调，不能有任何阻塞操作
 */
typedef struct
{
	int (*save_cloud_dclog_info)(ezDevSDK_cloud_dclog_info_to_dev *msg);	       ///<dclog埋点回调保存接口，不可以阻塞
	int (*get_cloud_dclog_info)(ezDevSDK_cloud_get_dev_dclog_info *info);      ///< dclog埋点回调获取接口，不可以阻塞
} ezDevSDK_cloud_dclog_callback;


//--------------------------------应用层设置/获取SDK配置信息-----------------------------------------
typedef enum {
    EZDEVSDK_CLOUD_OPT_TYPE_SET_SLICE_DATA_LEN = 0x1,       //ezDevSDK_cloud_slice_data_len
}EZDEVSDK_CLOUD_OPT_TYPE_E;

typedef struct {
    int time_len;       //单个分片时长，单位S
}ezDevSDK_cloud_slice_data_len;

#ifdef __cplusplus
}
#endif

#endif //H_EZDEVSDK_CLOUD_DEF_H_
