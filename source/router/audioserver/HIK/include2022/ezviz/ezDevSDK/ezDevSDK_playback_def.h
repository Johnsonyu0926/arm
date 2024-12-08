/**
 * \file	  ezDevSDK_playback_def.h  
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     回放领域对外结构体定义、宏定义
 * 
 * \author    liuzhenhao
 * 
 * \date      2018/5/30
 */

#ifndef H_EZDEVSDK_PLAYBACK_DEF_H_
#define H_EZDEVSDK_PLAYBACK_DEF_H_

#include <time.h>
#include "ezDevSDK_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \addtogroup Playback_Module
 * \{
 */

//----------------------------------错误码--------------------------------------------------

/**
 * \brief 萤石SDK错误码
 */
typedef enum  {
	EZDEVSDK_PLAYBACK_CODE_SUCCESS                      = 0x00000000, 	///< 成功
	
    EZDEVSDK_PLAYBACK_CODE_BASE                         = 0XD3200000,   ///< 回放领域接口错误码起始值，D表示接口错误码，3200，表示回放领域，最后三位表示具体错误
	EZDEVSDK_PLAYBACK_CODE_FAIL                         = EZDEVSDK_PLAYBACK_CODE_BASE + EZDEVSDK_ERROR_FAIL,
    EZDEVSDK_PLAYBACK_CODE_NOT_INITED                   = EZDEVSDK_PLAYBACK_CODE_BASE + EZDEVSDK_ERROR_NOT_INITED,
	EZDEVSDK_PLAYBACK_CODE_INVALID_PARAM                = EZDEVSDK_PLAYBACK_CODE_BASE + EZDEVSDK_ERROR_INVALID_PARAM,
	EZDEVSDK_PLAYBACK_CODE_NOT_SUPPORT                  = EZDEVSDK_PLAYBACK_CODE_BASE + EZDEVSDK_ERROR_NOT_SUPPORT,
	
	EZDEVSDK_PLAYBACK_CODE_BUF_NOT_ENOUGH               = 0XD3200020,
	EZDEVSDK_PLAYBACK_CODE_NO_RESOURCE                  = 0XD3200021,
} EZDEVSDK_PLAYBACK_CODE_E;

//--------------------------------初始化信息---------------------------------------------------------
typedef struct {
	int type;        								///< \c EZDEVSDK_PLAYBACK_INFO_E
	void *data;       								///< 指向具体type类型的数据
	int len;         								///< data数据长度
	void *ptx;									 ///< 多中心标记
} ezDevSDK_playback_runtime_info;

/**
 * \brief SDK接收消息的结构体
 */
typedef struct {
    int type;       								///< \c EZDEVSDK_PLAYBACK_MSG_TO_DEV_TYPE_E
    void *data;     								///< 指向具体type类型的数据
    int len;        								///< data数据长度
	void *ptx;									 ///< 多中心标记
} ezDevSDK_playback_msg_to_dev;

/**
 * \brief SDK回调给用户层接口
 */
typedef struct 
{
    int (*recv_msg)(ezDevSDK_playback_msg_to_dev *msg);             ///< 消息回调接口，不可以阻塞
	int (*get_runtime_info)(ezDevSDK_playback_runtime_info *info);  ///< SDK运行时获取信息回调，不能有任何阻塞操作
} ezDevSDK_playback_callback;

typedef struct 
{
	int link_max;                   				///< 回放连接最大数
	ezDevSDK_playback_callback cb;  				///< 回调函数
} ezDevSDK_playback_init_info;
//-----------------------------------------------------------------------------------------

//--------------------------------SDK通知应用层消息-----------------------------------------

/**
 * \brief SDK给应用的消息
 */
typedef enum {
	EZDEVSDK_PLAYBACK_ON_START_PLAYBACK,            ///< 开始回放
	EZDEVSDK_PLAYBACK_ON_STOP_PLAYBACK,             ///< 停止回放
	EZDEVSDK_PLAYBACK_ON_SET_SWITCH_STATUS,         ///< 设置开关选项
	EZDEVSDK_PLAYBACK_ON_SET_SPEEDCTRL_PARAM,       ///< 设置回放速率控制属性
	EZDEVSDK_PLAYBACK_ON_CHECK_PLAYBACK_RESOURCE,	///< NULL
	EZDEVSDK_PLAYBACK_ON_SET_RECORD_MODE,           ///< 设置录像模式
	EZDEVSDK_PLAYBACK_ON_SET_RECORD_TIME_LIST,      ///< 设置录像播放列表(seek)(暂定一次最多下发二十组)
} EZDEVSDK_PLAYBACK_MSG_TO_DEV_TYPE_E;

/**
 * \brief 录像时间
 */
typedef struct {
    time_t begin;   								///< 录像开始时间
    time_t end;     								///< 录像结束时间
} ezDevSDK_playback_record_time;

typedef struct {
    ezDevSDK_playback_record_time playback_time;
	char type[32];
	char *res;   
	int res_len;  //单位字节
} ezDevSDK_playback_record_search_result;

/**
 * \brief 回放开始所需参数, \c EZDEVSDK_PLAYBACK_ON_START_PLAYBACK
 */
typedef struct {
	int sid;            							///<回放sid
	int channel;        							///<通道
	ezDevSDK_playback_record_time playback_time;    ///<回放的录像时间
	char dev_serial[16];                            ///<回放的设备序列号
	int definition;                                 ///<回放清晰度0-默认1-流畅2-标清3-高清
} ezDevSDK_playback_start_playback;

/**
 * \brief 回放结束所需参数， \c EZDEVSDK_PLAYBACK_ON_STOP_PLAYBACK
 */
typedef struct {
	int sid;        								///<回放sid
} ezDevSDK_playback_stop_playback;

/**
 * \brief 倍速回放参数
 */
typedef struct {
	int sid;        								///<回放sid
	int b_extract_Iframe;							///<是否需要抽取I帧
} ezDevSDK_playback_speedctrl_param;

/**
 * \brief 录像模式
 */
typedef struct {
    int mode;
} ezDevSDK_playback_record_mode;


/**
 * \brief 录像播放列表下发类型
 */
typedef enum{
    EZDEVSDK_PLAYBACK_CONTINUE_LIST,  ///<向设备下发下一段播放列表(上一批播放列表开始播放的时就会下发下一批播放列表)
    EZDEVSDK_PLAYBACK_SEEK_LIST,     ///<向设备下发下一段跳播播放列表(设备直接停止当前播放，直接跳转到该列表播放)
    EZDEVSDK_PLAYBACK_START_LIST,   ///<设备开始回放的时候就带了播放列表(下发的时候已经正在播放录像段了，播完之后继续播该列表)
    EZDEVSDK_PLAYBACK_START_LIST_MAX,
} EZDEVSDK_PLAYBACK_LIST_SET;



/**
 * \brief 智能回放列表, \c EZDEVSDK_PLAYBACK_ON_SET_RECORD_TIME_LIST
 */
typedef struct {
    int sid;        					            ///<回放sid
    int count;                                      ///<回放播放列表个数
    int type;                                       ///<回放播放类型 枚举值:\c EZDEVSDK_PLAYBACK_LIST_SET
    ezDevSDK_playback_record_time *playback_list;   ///<回放播放列表
} ezDevSDK_playback_record_list;
//--------------------------------SDK获取设备信息-----------------------------------------

/**
 * \brief SDK运行时信息枚举和结构体
 */
typedef enum{
	EZDEVSDK_PLAYBACK_RT_INFO_SEARCH_RECORDS,					///< 搜索录像，<b>必须实现</b>，消息体： \c ezDevSDK_playback_search_info
	EZDEVSDK_PLAYBACK_RT_INFO_SEARCH_RECORDS_BY_MONTH,          ///< 按月搜索，<b>必须实现</b>，消息体： \c ezDevSDK_playback_search_by_month_info
	EZDEVSDK_PLAYBACK_RT_INFO_GET_SWITCH_STATUS,                ///< 获取开关选项，消息体： \c ezDevSDK_playback_switch_status
	EZDEVSDK_PLAYBACK_RT_INFO_GET_RECORD_PLAN,                  ///< 获取录像计划，消息体： \c ezDevSDK_playback_record_plan
	EZDEVSDK_PLAYBACK_RT_INFO_GET_RECORD_MODE,                  ///< 获取录像模式，消息体： \c ezDevSDK_playback_record_mode
	EZDEVSDK_PLAYBACK_RT_INFO_NEW_SEARCH_RECORDS,				///< 搜索录像新协议，<b>必须实现</b>，消息体： \c ezDevSDK_playback_new_search_info
    EZDEVSDK_PLAYBACK_RT_INFO_AI_SEARCH_RECORDS,                ///< 智能录像筛选新协议，消息体： \c              ezDevSDK_playback_ai_search_info
	EZDEVSDK_PLAYBACK_RT_INFO_GET_RECORD_COVER,					///< 获取录像封面，消息体： \c ezDevSDK_playback_record_cover_info
	EZDEVSDK_PLAYBACK_RT_INFO_GET_RECORD_LABEL,                 ///< 获取录像标签信息，消息体: \c ezDevSDK_playback_label_info
	EZDEVSDK_PLAYBACK_RT_INFO_GET_RECORD_COVER_STORAGE,         ///< 获取设备封面存储情况，消息体:\c ezDevSDK_playback_record_cover_storage
} EZDEVSDK_PLAYBACK_RUNTIME_INFO_E;

typedef struct {
	int channel;                                             	///< 入参，通道
	ezDevSDK_playback_record_time search_time;               	///< 入参，搜索时间
	int search_count;                                        	///< 入参，搜索的条数
	ezDevSDK_playback_record_search_result result[50];       	///< 出参，搜索结果，最大五十条
	int result_total_count;                                  	///< 出参，搜索结果总数量
	char dev_serial[16];                                        ///< 入参，搜索的设备序列号
	int order;                                                  ///< 入参，顺序查询:0     倒序查询:1
	int search_type;                                            ///< 入参，搜索类型默认:0       定时录像:1    事件录像:2
} ezDevSDK_playback_search_info;

typedef struct {
	int channel;       											///< 入参，通道
	int year;          											///< 入参，年
	int month;         											///< 入参，月
	int day_result[31];											///< 出参，结果标记
	char dev_serial[16];                                        ///< 入参，搜索的设备序列号
} ezDevSDK_playback_search_by_month_info;

typedef struct {
    int channel;
    ezDevSDK_playback_record_time search_time;
    char *label;
    int label_len;                                              ///< 入参label的大小
} ezDevSDK_playback_label_info;
/**
 * \brief 录像搜索类型枚举(定时录像:1/报警录像:0), \c 
 */
typedef enum {
    EZDEVSDK_PLAYBACK_RECORD_TYPE_ALARM = 0,
    EZDEVSDK_PLAYBACK_RECORD_TYPE_TIMING,
    EZDEVSDK_PLAYBACK_RECORD_TYPE_MOTDET_IN_TIMING = 101,
    EZDEVSDK_PLAYBACK_RECORD_TYPE_MAX,
}EZDEVSDK_PLAYBACK_RECORD_TYPE_E;

/**
 * \brief 搜索类型默认:0    定时录像:1    事件录像:2 \c 
 */
typedef enum {
    EZDEVSDK_PLAYBACK_REQ_RECORD_TYPE_TIMING = 1,
    EZDEVSDK_PLAYBACK_REQ_RECORD_TYPE_ALARM,
    EZDEVSDK_PLAYBACK_RECORD_TYPE_REQ_MAX,
}EZDEVSDK_PLAYBACK_RECORD_TYPE_REQ;


/**
 * \brief 录像搜索新协议单条录像信息结构体, \c 
 */
typedef struct {
    ezDevSDK_playback_record_time playback_time; ///< 录像开始/结束时间
	char type;                              ///< 录像类型, 对应枚举\c EZDEVSDK_PLAYBACK_RECORD_TYPE_E
    char res[3];
} ezDevSDK_playback_record_info;



/**
 * \brief 录像搜索新协议单条录像信息结构体, \c 
 */
typedef struct {
    int search_result_num;      //最大搜索条数
    ezDevSDK_playback_record_info *playback_record_info;
} ezDevSDK_playback_new_record_search_result;



/**
 * \brief 录像搜索新协议单条录像信息结构体, \c 
 */
typedef struct {
    unsigned int search_in_max;                           ///<最大搜索条数(入参)
    unsigned int search_out;                              ///<已搜索到的录像条数（出参）
    ezDevSDK_playback_record_info *playback_record_info;  ///<已搜索到的录像信息(出参)
} ezDevSDK_playback_special_record_search_result;


/**
 * \brief 录像搜索新协议结构体, \c 
 */
typedef struct {
	int channel;                                             	///< 入参，通道
	char dev_serial[16];                                        ///< 入参，搜索的设备序列号
	ezDevSDK_playback_record_time search_time;               	///< 入参，搜索时间
	int record_type;                                            ///< 入参，搜索类型 对应枚举\c EZDEVSDK_PLAYBACK_RECORD_TYPE_E
    ezDevSDK_playback_new_record_search_result result;          ///< 出参，搜索结果
	int result_total_count;                                  	///< 出参，本次搜索到的录像总数量
	int is_search_finished;                                     ///< 出参，搜索是否结束(0:未结束 1：已结束)
	ezDevSDK_playback_special_record_search_result special_record;  ///< 出参，特殊录像段搜索结果(SDK内部实现录像段拆分)
} ezDevSDK_playback_new_search_info;



/**
 * \brief 录像筛选结果 0:搜索失败 1：搜索成功 2: 未搜索到对应规则的录像段 \c 
 */
typedef enum {

    EZDEVSDK_PLAYBACK_AI_SERACH_FAILED= 0,
    EZDEVSDK_PLAYBACK_AI_SERACH_SUCCESS,
    EZDEVSDK_PLAYBACK_AI_SERACH_NO_FILE_FOUND,
    EZDEVSDK_PLAYBACK_AI_SEARCH_RESULT_MAX,
}EZDEVSDK_PLAYBACK_AI_SEARCH_RESULT;


/**
 * \brief 智能录像筛选新协议结构体, \c 
 */
typedef struct {
	int channel;                                             	///< 入参，通道
	char dev_serial[16];                                        ///< 入参，搜索的设备序列号
	ezDevSDK_playback_record_time search_time;               	///< 入参，搜索时间
	char *ai_search_filter;                                     ///< 入参，智能筛选规则 
    int record_info_len;                                        ///< 入参，录像信息内存大小：(24*60*60)/8 字节
    unsigned char *record_info;                                 ///< (入/出)参，录像搜索结果（内存由SDK申请和释放）
    int search_result;                                  	    ///< 出参，搜索结果,对应枚举 EZDEVSDK_PLAYBACK_AI_SEARCH_RESULT
} ezDevSDK_playback_ai_search_info;

typedef enum {
    EZDEVSDK_PLAYBACK_SWITCH_TYPE_ALLDAY_RECORD = 0x1, 			///< 全天录像
}EZDEVSDK_PLAYBACK_SWITCH_TYPE_E;

typedef struct {
    int channel;                  								///< 入参，通道  0:代表设备
    int type;                     								///< 入参，类型  1:全天录像
    int enable;                   								///< 入参/出参，开关状态 1:打开 0:关闭
} ezDevSDK_playback_switch_status;

typedef struct  
{
    time_t begin;
    time_t end;
} ezDevSDK_playback_time_segment;

typedef struct  
{
    ezDevSDK_playback_time_segment    time_seg;
    unsigned char   rec_type;                     				///< 0: 定时录像 1:活动录像
    unsigned char   res[3];
} ezDevSDK_playback_record_sched;

typedef struct {
    int channel;                     							///< 通道(入参)  0:代表设备，1开始为通道
    int actor;                       							///< 设备角色；1:视频通道 2: Video 3: io通道
    int enable;                      							///< 使能开关
    ezDevSDK_playback_record_sched week_sched[7][8];
} ezDevSDK_playback_record_plan;

/* 和ezviz_type保持一致 */
typedef enum
{
    EZDEVSDK_PLAYBACK_FRAME_TYPE_I = 0,      ///<视频帧I帧
    EZDEVSDK_PLAYBACK_FRAME_TYPE_P = 1,          ///<视频帧P帧
    EZDEVSDK_PLAYBACK_FRAME_TYPE_B = 2,          ///<视频帧B帧
    EZDEVSDK_PLAYBACK_FRAME_TYPE_AUDIO = 4,      ///<音频帧
    EZDEVSDK_PLAYBACK_FRAME_TYPE_HEADER = 0Xff    //视频帧帧头
}EZDEVSDK_PLAYBACK_FRAME_TYPE_T;


typedef struct {
	char *frame;            									///< 帧数据起始地址
	int len;                									///< 帧长度
	int frame_type;         									///< 帧类型，参见枚举EZDEVSDK_PLAYBACK_FRAME_TYPE_T
	unsigned int time_stamp;									///< 帧绝对时间戳(海康DSP系统时钟为单位1/45000秒)
	int sequence;           									///< 帧序号
}ezDevSDK_playback_frame_info;

/**
 * \brief 录像封面结果 0:搜索失败 1：搜索成功 2: 未搜索到对应规则的录像封面 \c 
 */
typedef enum {

    EZDEVSDK_PLAYBACK_RECORDCOVER_SERACH_FAILED= 0,
    EZDEVSDK_PLAYBACK_RECORDCOVER_SERACH_SUCCESS,
    EZDEVSDK_PLAYBACK_RECORDCOVER_SERACH_NO_COVER_FOUND,
    EZDEVSDK_PLAYBACK_RECORDCOVER_SEARCH_RESULT_MAX,
}EZDEVSDK_PLAYBACK_RECORDCOVE_SEARCH_RESULT;

/**
 * \brief SD 卡封面结构体
 */
typedef struct {
	int channel;                                             	///< 入参，通道
	char dev_serial[16];                                        ///< 入参，搜索的设备序列号
	ezDevSDK_playback_record_time search_time;               	///< 入参，搜索时间
    unsigned int record_cover_buf_len;                          ///< 入参，录像封面缓存最大大小
    unsigned char *record_cover_buf;                            ///< 出参，录像封面缓存起始地址
    unsigned int record_cover_actual_len;                      ///< 出参，录像封面实际长度
	unsigned int time_stamp;									///< 出参，录像封面绝对时间戳
    int search_result;                                  	    ///< 出参，搜索结果,对应枚举 EZDEVSDK_PLAYBACK_AI_SEARCH_RESULT
} ezDevSDK_playback_record_cover_info;

typedef struct
{
    int storage;                            // 1:存储已分配 2:存储未分配
} ezDevSDK_playback_record_cover_storage;

/*! \} */

#ifdef __cplusplus
}
#endif

#endif //H_EZDEVSDK_BASE_FUNCTION_DEF_H_
