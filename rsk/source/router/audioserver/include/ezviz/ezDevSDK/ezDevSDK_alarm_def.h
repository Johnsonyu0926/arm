/**
 * \file      ezDevSDK_alarm_def.h  
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     报警领域对外结构体定义、宏定义
 * 
 * \author    panlong
 * 
 * \date      2017/4/8
 */

#ifndef H_EZDEVSDK_ALARM_DEF_H_
#define H_EZDEVSDK_ALARM_DEF_H_

#include <time.h>
#include "ezDevSDK_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /** 
     * \addtogroup Alarm_Module
     * \{
     */

    //--------------------------------------错误码-------------------------------------------------

    /**
     * @brief 萤石SDK报警领域的接口错误码，由SDK返回给调用者
     * @note 公用错误码，请参考ezDevSDK_error.h
     */
    typedef enum
    {
        EZDEVSDK_ALARM_CODE_SUCESS              = 0X00000000, ///< 成功
        EZDEVSDK_ALARM_CODE_BASE                = 0XD2000000, ///< 报警领域接口错误码起始值，D表示接口错误码，2000，表示报警领域，最后三位表示具体错误

        ///< 0XD2000000 - 0XD200001F 公共错误码
        EZDEVSDK_ALARM_CODE_FAIL                = EZDEVSDK_ALARM_CODE_BASE + EZDEVSDK_ERROR_FAIL,
        EZDEVSDK_ALARM_CODE_NOT_INITED          = EZDEVSDK_ALARM_CODE_BASE + EZDEVSDK_ERROR_NOT_INITED,
        EZDEVSDK_ALARM_CODE_INVALID_PARAM       = EZDEVSDK_ALARM_CODE_BASE + EZDEVSDK_ERROR_INVALID_PARAM,
        EZDEVSDK_ALARM_CODE_NOT_SUPPORT         = EZDEVSDK_ALARM_CODE_BASE + EZDEVSDK_ERROR_NOT_SUPPORT,
        EZDEVSDK_ALARM_CODE_NOT_ENOUGH_MEMERY   = EZDEVSDK_ALARM_CODE_BASE + EZDEVSDK_ERROR_NOT_ENOUGH_MEMERY,
        EZDEVSDK_ALARM_CODE_INIT_FAIL           = EZDEVSDK_ALARM_CODE_BASE + EZDEVSDK_ERROR_INIT_FAIL,
        EZDEVSDK_ALARM_CODE_THREAD_CREATE_FAIL  = EZDEVSDK_ALARM_CODE_BASE + EZDEVSDK_ERROR_THREAD_CREATE_FAIL,
        EZDEVSDK_ALARM_CODE_RSP_TIMEOUT         = EZDEVSDK_ALARM_CODE_BASE + EZDEVSDK_ERROR_RSP_TIMEOUT,
        EZDEVSDK_ALARM_CODE_MSG_DATA_INVAILD    = EZDEVSDK_ALARM_CODE_BASE + EZDEVSDK_ERROR_MSG_DATA_INVAILD,

        EZDEVSDK_ALARM_CODE_DEV_NOT_BOUND       = 0XD2000020, ///< 设备未绑定用户
        EZDEVSDK_ALARM_CODE_EMPTY               = 0XD2000021, ///< 设备无可取数据
        EZDEVSDK_ALARM_CODE_STATUS_NOT_ENABLED  = 0XD2000022, ///< 设备报警状态未启用
        EZDEVSDK_ALARM_CODE_GET_TINY_PICS       = 0XD2000023, ///< 向设备获取小图失败
        EZDEVSDK_ALARM_CODE_PUSH_PIC            = 0XD2000024, ///< 上传图片失败
        EZDEVSDK_ALARM_CODE_PUSH_METADATA       = 0XD2000025, ///< 发送报警元数据失败
        EZDEVSDK_ALARM_CODE_PUSH_PIC_NUM_LIMIT  = 0XD2000026, ///< 多图告警上传图片数量超出限制
        EZDEVSDK_ALARM_CODE_PUSH_INTERFACE_ERROR = 0XD2000027, ///<接口调用有误
		
		EZDEVSDK_ALARM_CODE_INTELLI_APP_OUT_OF_MEMORY		    = 0X00100008, ///< 智能应用内存不足
		EZDEVSDK_ALARM_CODE_INTELLI_APP_INSUFFICIENT_FLASH		= 0X00100009, ///< 智能应用FLASH不足
		EZDEVSDK_ALARM_CODE_INTELLI_APP_INSUFFICIENT_CPU		= 0X0010000A, ///< 智能应用CPU不足
		EZDEVSDK_ALARM_CODE_INTELLI_APP_INSUFFICIENT_COMPUTE	= 0X0010000B, ///< 智能应用算力不足
    } EZDEVSDK_ALARM_CODE_E;

    /**
     * @brief 由调用层设置的协议错误码，内部转化后传递到客户端
     */
    typedef enum
    {
        EZDEVSDK_ALARM_PROTOCOL_CODE_BASE = 0XF2000000, ///< 报警领域协议错误码起始值，F表示协议错误码，2000，表示报警领域，最后三位表示具体错误
    } EZDEVSDK_ALARM_PROTOCOL_CODE_E;

    //----------------------------------初始化信息--------------------------------------------------

    /**
     * @brief   SDK通知应用层的消息结构体
     */
    typedef struct
    {
        int type;   ///< SDK给应用的消息类型，@ref EZDEVSDK_ALARM_MSG_TO_DEV_TYPE_E
        void *data; ///< 所给消息内容
        int len;    ///< 所给消息内容的长度
    } ezDevSDK_alarm_msg_to_dev;

    /**
     *  @brief   SDK获取设备信息结构体
     */
    typedef struct
    {
        int type;   ///< 需获取的设备信息类型，@ref EZDEVSDK_ALARM_RUNTIME_INFO_E
        void *data; ///< 信息内容结构体
        int len;    ///< 信息内容长度
    } ezDevSDK_alarm_runtime_info;

    /**
     *  @brief SDK回调给应用层接口, SDK运行时获取信息回调，不能有任何阻塞操作
     *  @note  on_get_runtime_info回调函数中，对于不支持的功能，返回值请填EZDEVSDK_ERROR_NOT_SUPPORT
     */
    typedef struct
    {
        int (*recv_msg)(ezDevSDK_alarm_msg_to_dev *msg);               ///< 消息回调接口，不可以阻塞
        int (*on_get_runtime_info)(ezDevSDK_alarm_runtime_info *info); ///< 获取设备运行时信息回调接口
    } ezDevSDK_alarm_callback;

    /**
     * @brief 报警领域初始化的参数
     */
    typedef struct
    {
        ezDevSDK_alarm_callback cb; ///< 回调函数
        int channel_num_max;        ///< 报警最大通道号，默认为1。(如果最大通道号是2，表示0-2通道可用)
        int thread_num_max;         ///< 异步发送最大线程数量
        int task_num_max;           ///< 异步发送最大任务数,不传该值，默认最大任务数为100
        long timeval_tv_sec;        ///< 设置底层setsockopt函数的timeval.tv_sec(单位秒 seconds,默认值10,最大值100)
    } ezDevSDK_alarm_init_info;

    //----------------------------------SDK通知应用层的消息--------------------------------------------

    /**
     * @brief   SDK通知应用层的消息类型
     */
    typedef enum
    {
        EZDEVSDK_ALARM_ON_THIRD_CAPTION,                 ///< 平台向设备发送第三方报警通知, 消息体: ezDevSDK_alarm_third_caption
        EZDEVSDK_ALARM_ON_ALARM_SOUND,                   ///< 设置报警声音类型, 消息体: ezDevSDK_alarm_sound_type
        EZDEVSDK_ALARM_ON_ALARM_VIDEO,                   ///< 报警小视频开启状态, 消息体: ezDevSDK_alarm_video
        EZDEVSDK_ALARM_ON_ALARM_STATUS,                  ///< 报警上报使能开关, 消息体: ezDevSDK_alarm_status
        EZDEVSDK_ALARM_ON_ALARM_DEFENCE_PLAN,            ///< 报警计划, 消息体: ezDevSDK_alarm_defence_plan
        EZDEVSDK_ALARM_ON_ACTIVE_DEFENCE,                ///< 主动防御, 消息体: ezDevSDK_alarm_active_defence
        EZDEVSDK_ALARM_ON_SET_MOTION_DETECT_AREA,        ///< 移动侦测区域设置, 消息体: ezDevSDK_alarm_motion_detect_area
        EZDEVSDK_ALARM_ON_SWITCH_ENABLE,                 ///< 开关状态量（人形检测、闪烁灯状态等）相关, 消息体: ezDevSDK_alarm_switch_enable
        EZDEVSDK_ALARM_ON_SET_DETECTOR_ALARM_AREA,       ///< 设置探测器报警区域, 消息体: ezDevSDK_alarm_detector_alarm_area
        EZDEVSDK_ALARM_ON_ALARM_LIGHT,                   ///< 设置安防灯类型, 消息体: ezDevSDK_alarm_light_info
        EZDEVSDK_ALARM_ON_CALLING_CHIME,                 ///< 设置Chime 类型, 消息体: ezDevSDK_alarm_calling_chime_info
        EZDEVSDK_ALARM_ON_TRANS_CMD_MSG,                 ///< 由外部实现的请求所对应的平台回应信息，消息体: ezDevSDK_alarm_trans_cmd_msg
        EZDEVSDK_ALARM_ON_PUSH_ACK,                      ///< 消息上报回执, 消息体: ezDevSDK_alarm_push_ack
        EZDEVSDK_ALARM_ON_ALARM_SPAN,                    ///< 平台向设备发送的报警最小间隔时间, 消息体: ezDevSDK_alarm_span
        EZDEVSDK_ALARM_ON_ALARM_SOUND_VOLUME,            ///< 设置报警声音音量大小, 消息体: ezDevSDK_alarm_sound_volume
        EZDEVSDK_ALARM_ON_SET_ALARM_PLAN,                ///< 设置除布撤防计划外的计划，信息体：ezDevSDK_alarm_plan
        EZDEVSDK_ALARM_ON_KV_HMCAR_DETECT,               ///< KEY_VALUE 人车检测开关，信息体：ezDevSDK_alarm_kv_hmcar_detect
        EZDEVSDK_ALARM_ON_SET_BODY_DETECT_TIME_INTERVAL, ///< 平台向设备设置人体检测触发时间间隔，信息体:ezDevSDK_alarm_body_detect_time_interval
		EZDEVSDK_ALARM_ON_SET_TEMPERATURE_THRESHOLD,  ///< 平台向设备设置温度提醒阈值(温感相机),  信息体:  ezDevSDK_alarm_temperature_threshold
		EZDEVSDK_ALARM_ON_SET_HIGH_TEMPERATURE_ALARM_INTERVAL,		///< 平台向设备设置告警时间间隔(温感相机),  信息体:  ezDevSDK_alarm_camera_alarm_interval
		EZDEVSDK_ALARM_ON_SET_REMIND_WAY,             ///< 平台向设备设置高温预警提醒方式(温感相机), 信息体:  ezDevSDK_alarm_camera_remind_way
		EZDEVSDK_ALARM_ON_SET_DETECT_AREA,            ///< 平台向设备设置侦测区域（新）, 信息体: ezDevSDK_alarm_temperature_detect_area
		EZDEVSDK_ALARM_ON_SET_ALARM_LIGHT_DELAY_TIME,           ///< 平台向设备设置灯光延时时间，消息体:ezDevSDK_alarm_light_delay_time
		EZDEVSDK_ALARM_ON_SET_ALARM_SPAN_CUSTERM,         ///< 用户向设备发送的报警最小间隔时间, 消息体: ezDevSDK_alarm_span_custerm
        EZDEVSDK_ALARM_ON_KV_ADVANCED_DETECT,
        EZDEVSDK_ALARM_ON_SET_GESTURE_REMIND_TYPE,          ///< 手势识别类型
        EZDEVSDK_ALARM_ON_SET_SOUND_DETECT_TYPE,
		EZDEVSDK_ALARM_ON_SET_INTELLI_APP,  ///< 平台向设备设置智能算法应用开关，消息体:ezDevSDK_intelligent_application_info
		EZDEVSDK_ALARM_ON_SET_ALARM_MESSAGE_NOTIFY_TYPE,  ///平台向设备设置消息通知类型，消息体：ezDevSDK_alarm_message_notify_type
		EZDEVSDK_ALARM_ON_ADD_TARGET_LIB,   
		EZDEVSDK_ALARM_ON_DEL_TARGET_LIB,
        EZDEVSDK_ALARM_ON_MAX,
    } EZDEVSDK_ALARM_MSG_TO_DEV_TYPE_E;

    typedef enum
    {
        EZDEVSDK_ALARM_UNMANNED_PLAN = 0x0,
        EZDEVSDK_ALARM_CUSTOM_VOICE_PLAN,
        EZDEVSDK_ALARM_TEMPERATURE_REMIND_PLAN,
        EZDEVSDK_ALARM_LIGHT_LINKAGE_PLAN,
        EZDEVSDK_ALARM_REALTIME_DETECT_PLAN,
        EZDEVSDK_ALARM_SECURITY_LIGHT_PLAN,     ///<安防灯计划
		EZDEVSDK_ALARM_NIGHT_DUTY_PLAN,
		EZDEVSDK_ALARM_MESSAGE_NOTIFY_PLAN,
        EZDEVSDK_ALARM_PLAN_MAX,
    } EZDEVSDK_ALARM_PLAN_TYPE_E;

    /**
     * @brief   第三方抓图，录像，对应消息类型: EZDEVSDK_ALARM_ON_THIRD_CAPTION
     */
    typedef struct
    {
        char serial[32];        ///< 设备序列号
        int type;               ///< 0:抓图，1:抓图和录像
        char relation_uuid[64]; ///< 关联uuid
    } ezDevSDK_alarm_third_caption;

    /**
     * @brief   设置报警声音类型，对应消息类型: EZDEVSDK_ALARM_ON_ALARM_SOUND
     */
    typedef struct
    {
        int type;  ///< 声音类型, 0:短，1:长，2:静音，3:自定义语音
        int index; ///< type对应3时才生效，自定义语音的索引号
    } ezDevSDK_alarm_sound_type;

    /**
     * @brief   设置报警声音大小，对应消息类型: EZDEVSDK_ALARM_ON_ALARM_SOUND
     */
    typedef struct
    {
        int speak_volume; ///< 声音音量大小，10.20.30....100
		int microphone_volume;///< 麦克风音量
    } ezDevSDK_alarm_sound_volume;

    /**
     * @brief   设置人车检测类型，对应消息类型: EZDEVSDK_ALARM_ON_KV_HMCAR_DETECT
     */
    typedef struct
    {
        int detect_type; ///< 检测类型，0、1、2、3
    } ezDevSDK_alarm_kv_hmcar_detect;

    /**
     * @brief   设置安防灯信息，对应消息类型: EZDEVSDK_ALARM_ON_ALARM_LIGHT
     */
    typedef struct
    {
        int luminance; ///< 亮度
    } ezDevSDK_alarm_light_info;

    /**
     * @brief   报警小视频开启状态，对应消息类型: EZDEVSDK_ALARM_ON_ALARM_VIDEO
     */
    typedef struct
    {
        int enable; ///< 开关状态
    } ezDevSDK_alarm_video;

    /**
     * @brief   报警状态开关, 对应消息类型: EZDEVSDK_ALARM_ON_ALARM_STATUS
     */
    typedef struct
    {
        int channel; ///< 通道号
        int enable;  ///< 开关状态
    } ezDevSDK_alarm_status;

    /**
     * @brief   报警计划时间
     */
    typedef struct
    {
        time_t begin; ///< 计划开始时间
        time_t end;   ///< 计划结束时间
    } ezDevSDK_alarm_time_segment;

    /**
     * @brief   报警上报计划使能开关, 对应消息类型: EZDEVSDK_ALARM_ON_ALARM_DEFENCE_PLAN
     */
    typedef struct
    {
        int channel;                                ///< 通道号
        int enable_defence_plan;                    ///< 开关状态
        ezDevSDK_alarm_time_segment arm_time[7][4]; ///< 对应7天（一周），每天4个时段的计划时间
    } ezDevSDK_alarm_defence_plan;

    typedef struct
    {
        time_t begin;
        time_t end;
        int index; ///声音文件索引
        char name[64];
    } ezDevSDK_alarm_custom_voice_time_segment;

    typedef struct
    {
        char perform_week_days[8];//""->仅执行一次 "0123456"->0周日 1周一....
        time_t begin;            
        int delay_time;          //侦测事件单位:分钟
        int segment_enable;//与原先的计划不同可以存在A计划使能B计划不使能的场景(单次执行的计划执行之后需要将使能开关关闭)
    } ezDevSDK_alarm_realtime_detect_time_segment;


    typedef struct
    {
        time_t begin;
        time_t end;
        unsigned int alarm_span;
    } ezDevSDK_alarm_night_duty_time_segment;

    typedef struct
    {
        int type;                ///< 计划类型@ref EZDEVSDK_ALARM_PLAN_TYPE_E
        int channel;             ///< 通道号
        int enable_defence_plan; ///< 开关状态
        union {
            struct
            {
                ezDevSDK_alarm_time_segment time[7][4];
            } alarm_unmanned_plan;
            struct
            {
                ezDevSDK_alarm_custom_voice_time_segment time[7][4];
            } alarm_custom_voice_plan;
            struct
            {
                ezDevSDK_alarm_time_segment camera_time[7][4];
            } alarm_temperature_remind_plan;
            struct
            {
                ezDevSDK_alarm_time_segment time[7][4];
            } alarm_light_linkage_plan;
            struct
            {
                ezDevSDK_alarm_realtime_detect_time_segment time[2];
            } alarm_realtime_detect_plan;
            struct
            {
                ezDevSDK_alarm_time_segment time[7][4];
            } alarm_security_light_plan;
			struct
            {
                ezDevSDK_alarm_night_duty_time_segment time[7][4];
            } alarm_night_duty_plan;
			struct
            {
                ezDevSDK_alarm_time_segment time[7][5];
            }alarm_message_notify_plan;
        } plan_info;
    } ezDevSDK_alarm_plan;

    /**
     * @brief   主动防御使能开关, 对应消息类型: EZDEVSDK_ALARM_ON_ACTIVE_DEFENCE
     */
    typedef struct
    {
        int enable; ///< 开关状态
    } ezDevSDK_alarm_active_defence;

    /**
     * @brief    开关的切换类型
     */
    typedef enum
    {
        EZDEVSDK_ALARM_SWITCH_TYPE_HUMAN_DETECT = 1,        ///< 人形检测开关-300
        EZDEVSDK_ALARM_SWITCH_TYPE_LIGHT_FLASH = 2,         ///< 灯光闪烁开关/移动侦测灯光联动-301
        EZDEVSDK_ALARM_SWITCH_TYPE_HUMAN_FILLTRATION = 3,   ///< 人形过滤开关-302
        EZDEVSDK_ALARM_SWITCH_TYPE_ALARM_LIGHT = 4,         ///< 安防灯开关/安防灯开关灯开关 -303
        EZDEVSDK_ALARM_SWITCH_TYPE_HUMAN_FACE_DETECT = 5,   ///< 人脸服务开关-202
        EZDEVSDK_ALARM_SWITCH_TYPE_FACE_SWAPPER = 6,        ///< 人脸抠图开关-304
        EZDEVSDK_ALARM_SWITCH_TYPE_LIGHT_LINKAGE = 7,       ///< 灯光联动开关/PIR灯光联动-305
        EZDEVSDK_ALARM_SWITCH_TYPE_INTELL_HUMAN_DETECT = 8, ///< 智能人体检测开关-200
        EZDEVSDK_ALARM_SWITCH_TYPE_CAMERA_HIGH_TEMPERATURE = 9,  //温感相机高温报警开关602
        EZDEVSDK_ALARM_SWITCH_TYPE_CAMERA_LIGHT_LINKAGE = 10,     //温感相机灯光联动开关603
        EZDEVSDK_ALARM_SWITCH_TYPE_RING = 11,                     //门铃响铃开关 39
        EZDEVSDK_ALARM_SWITCH_TYPE_TAMPER_ALARM = 12,             //防拆报警开关 306
        EZDEVSDK_ALARM_SWITCH_TYPE_HUMAN_FACE_AND_SHADPE = 13,    //人脸\人形过滤开关451
        EZDEVSDK_ALARM_SWITCH_TYPE_HUMAN_LIGHT_LINKAGE = 14,      //人形检测灯光联动 41
        EZDEVSDK_ALARM_SWITCH_TYPE_FALL_DETECT = 15,            //跌倒检测开关 615
        EZDEVSDK_ALARM_SWITCH_TYPE_MAX,
    } EZDEVSDK_ALARM_SWITCH_TYPE_E;

    /**
     * @brief   开关状态（人形检测、闪烁灯状态等）, 对应消息类型：EZDEVSDK_ALARM_ON_SWITCH_ENABLE
     */
    typedef struct
    {
        int type;    ///< 开关的切换类型，@ref EZDEVSDK_ALARM_SWITCH_TYPE_E
        int channel; ///< 通道号
        int enable;  ///< 开关状态
    } ezDevSDK_alarm_switch_enable;

    /**
     * @brief   移动侦测区域，对应消息类型: EZDEVSDK_ALARM_ON_SET_MOTION_DETECT_AREA
     *          只有在设置的区域检测到移动侦测，才会上报报警。
     *          data字段包含要检测的区域，按bit位标识，总共可标识32x18块。
     *          width和height表示data的有效范围，P制为22x18，N制为22x15。
     */
    typedef struct
    {
        int channel;           ///< 通道号
        int width;             ///< 标记区域宽度
        int height;            ///< 标记区域高度
        unsigned int data[18]; ///< 18个UINT32, 标记检测区域
    } ezDevSDK_alarm_motion_detect_area;

    /**
     * @brief   探测器报警区域，对应消息类型: EZDEVSDK_ALARM_ON_SET_DETECTOR_ALARM_AREA
     */
    typedef struct
    {
        int channel;           ///< 通道号
        int degree;            ///< 探测角度
        int nums;              ///< 探测器数量
        int distance;          ///< 探测距离
        int rows;              ///< 行数
        int cols;              ///< 列数
        unsigned int data[18]; ///< 以行数为单位的区域信息，例如3行对应：data[0], data[1], data[2]
    } ezDevSDK_alarm_detector_alarm_area;

    /**
     * @brief   Chime 类型，对应消息类型: EZDEVSDK_ALARM_ON_CALLING_CHIME
     */
    typedef struct
    {
        int type;     ///< 1:机械，2:电子，3:无
        int duration; ///< 响铃持续时长
    } ezDevSDK_alarm_calling_chime_info;

    /**
     * @brief   报警小视频多张小图，对应消息类型: EZDEVSDK_ALARM_RT_INFO_GET_TINY_VIDEO_PIC
     */
    typedef struct
    {
        int channel;   ///< 通道
        int direction; ///< 0:取seq指定帧，1:取seq上一帧，2:取seq下一帧
        int seq;       ///< -1:取最新帧，>0:取当前帧

        struct
        {
            unsigned long long timestamp; ///< 时间，单位毫秒(1970-01-01 00:00:00 +0000 (UTC)再乘以1000)
            int frame_len;                ///< 帧数据长度，按实际长度赋值
            int position_info_len;        ///< 坐标信息数据长度，按实际长度赋值
        } small_video_frame_info;

        char *frame;         ///< 帧数据，按frame_len入参拷贝，NULL不进行数据拷贝，
        char *position_info; ///< 坐标信息，按position_info_len入参拷贝，NULL不进行数据拷贝
    } ezDevSDK_alarm_tiny_video_info;

    /**
     * \brief 外部组装的信令信息，或者平台回复的信息，对应消息类型: EZDEVSDK_ALARM_ON_TRANS_CMD_MSG
     */
    typedef struct
    {
        char *content;          ///< 组装完毕的信令信息，或者平台回应的信息
        int content_len;        ///< 信息长度
        unsigned int cmd_id;    ///< 与平台交互的信令ID
        unsigned char msg_type; ///< 信令交互类型, 0:请求, 1:响应
        unsigned int msg_seq;   ///< 序号
    } ezDevSDK_alarm_trans_cmd_msg;

    /**
     * @brief 报警间隔时间，对应消息类型: EZDEVSDK_ALARM_ON_ALARM_SPAN
     */
    typedef struct
    {
        int alarm_span; ///< 报警时间最小间隔
    } ezDevSDK_alarm_span;

    /**
     * @brief 人体检测触发时间间隔，对应消息类型: EZDEVSDK_ALARM_ON_SET_BODY_DETECT_TIME_INTERVAL
     */
    typedef struct
    {
        int interval; ///< 报警时间最小间隔
    } ezDevSDK_alarm_body_detect_time_interval;
	
	/*
	 * @brief   设置温度提醒阈值，对应EZDEVSDK_ALARM_ON_SET_TEMPERATURE_THRESHOLD
	 */
	typedef struct 
	{
	    int temperature_threshold;                                   ///< 温度提醒阈值 60.70.80
	} ezDevSDK_alarm_temperature_threshold;

	/*
	 * @brief   设置告警时间间隔，对应EZDEVSDK_ALARM_ON_SET_ALARM_INTERVAL
	 */
	typedef struct 
	{
	    int alarm_interval;                                   ///<告警时间间隔10.20.30
	} ezDevSDK_alarm_camera_alarm_interval;

	 /*
	 * @brief 灯光延时时间，对应消息类型:
	 */
	 typedef struct
	 {
	     int delay_time;                           ///< 灯光延时时间
	 }ezDevSDK_alarm_light_delay_time;
	
	/*
	 * @brief   设置温感相机报警提醒方式，对应EZDEVSDK_ALARM_ON_SET_REMIND_WAY
	 */
	typedef struct 
	{
	    int remind_way;                                      ///< 温感相机提醒方式 0.1.2.3..
	} ezDevSDK_alarm_camera_remind_way;

    /**
     * @brief    侦测区域的类型
     */
    typedef enum
    {
        EZDEVSDK_ALARM_DETECT_TYPE_TEMPERATURE = 1,        ///< 温度侦测区域
        EZDEVSDK_ALARM_DETECT_TYPE_HUMANOID,               ///< 人形侦测区域
        EZDEVSDK_ALARM_DETECT_TYPE_MAX,
    } EZDEVSDK_ALARM_DETECT_TYPE_E;
	/*
	 * @brief   平台向设备设置侦测区域，对应EZDEVSDK_ALARM_ON_SET_TEMPERATURE_DETECT_AREA
	 */
	typedef struct 
	{
	    int Type;                ///< EZDEVSDK_ALARM_DETECT_TYPE_E
	    int channel;             ///< 通道号
		int width;               ///< 标记区域宽度
		int height;              ///< 标记区域高度
		unsigned int data[21];   ///< 18个UINT32, 标记检测区域 
		char type[32];           ///<侦测类型
	} ezDevSDK_alarm_detect_area;

    
    /**
    * @brief 报警间隔时间，对应消息类型: EZDEVSDK_ALARM_ON_ALARM_SPAN_CUSTERM
    */
    typedef struct
    {
        int channel;                                      ///<通道号
        int alarm_span_custerm;                           ///< 用户设置的报警时间最小间隔(优先使用)
    }ezDevSDK_alarm_span_custerm;
    
    typedef struct
    {
        int detect_type;
    } ezDevSDK_alarm_kv_advanced_detect;

    typedef struct
    {
        int type;                                       ///< 手势类型0:拍照 1:呼叫
    } ezDevSDK_alarm_gesture_remind_type;

    typedef struct
    {
        int type;                                       ///< 0大噪声   1婴儿啼哭
        int db;                                         ///< 分贝
    } ezDevSDK_alarm_sound_detect_type;
    
    typedef struct
    {
		char appid[256];                                 ///< 智能算法应用id
        int enable;                                      ///< 智能算法应用开关, 1:打开 0:关闭 
        int preset;                                      ///< 智能算法是否预置, 1:预置 0:非预置 
	    char appurl[256];								 ///< 智能算法应用下载定位符
	    char version[64];								 ///< 智能算法应用版本号
	    char filemd5[256];								 ///< 智能算法应用文件摘要
	    char alarm_type[256];							 ///< 智能算法应用报警类型
	    unsigned int size;								 ///< 智能算法应用需要的size
	    unsigned int flash;								 ///< 智能算法应用需要的flash
	    unsigned int mem;								 ///< 智能算法应用需要的内存
	    double ability;								     ///< 智能算法应用需要的算力
	    char event_domain[64];                           ///< 智能算法对应事件所属领域
        char event_identifier[64];                       ///< 智能算法对应事件标识
        int alarm_notify_enable;                         ///< 智能算法对应事件报警通知是否使能
    } ezDevSDK_intelligent_application_info;

	typedef struct
    {
		int num;                                        ///< 智能算法应用个数
        ezDevSDK_intelligent_application_info app_node[16];    
    } ezDevSDK_intelligent_application_array;

	typedef struct
    {
		unsigned int flash;                                 ///< 可用flash
		unsigned int memory;                                ///< 可用内存
		double ability;                                     ///< 可用算力
		char platform[32];                                  ///< 平台
	    unsigned int media_mem; 							///< 多媒体内存	
	    unsigned int channel; 							    ///< 通道数
    } ezDevSDK_dynamic_resource_info;
    #define EZDEVSDK_ALARM_FACE_ID_MAX_LEN                64
    #define EZDEVSDK_ALARM_FACE_MEMBER_MAX_NUM            30
    /**
    * @brief 人脸成员
    */
    typedef struct
    {
        char memberId[EZDEVSDK_ALARM_FACE_ID_MAX_LEN+1];
    }ezDevSDK_alarm_member_id;

    /**
    * @brief 消息通知类型，对应消息类型: EZDEVSDK_ALARM_ON_SET_ALARM_MESSAGE_NOTIFY_TYPE
    */
    typedef struct
    {
        int humanoid;            ///< 人形检测通知 0-关,1-开
        int pir;                 ///< pir检测通知 0-关，1-开
        int face;                ///< 人脸识别通知 0-关，1-开
        int memberNum;           ///< 人脸数,app关闭的
        ezDevSDK_alarm_member_id memberId[EZDEVSDK_ALARM_FACE_MEMBER_MAX_NUM];  ///< 人脸ID,app关闭的
        int loiter;              ///< 徘徊检测通知 0-关,1-开
    }ezDevSDK_alarm_message_notify_type;

    typedef struct
    {
        char target_lib_id[64];
        char target_id[64];
        char target_name[32];
        int target_alarm_action;                            ///< -1 unknow 0 disable 1enable
        char pic_id[64];
        char pic_url[256];
        unsigned char secret_key[32];
    } ezDevSDK_target_lib_info;

    typedef struct
    {
        int num;
        char app_id[256];
        char version[256];
        ezDevSDK_target_lib_info *target_lib_infos;
    } ezDevSDK_target_lib_infos;
    //--------------------------------SDK从应用层获取的消息--------------------------------------------------------------

    /**
     *  @brief   SDK获取设备信息类型
     */
    typedef enum
    {
        //以下参数是萤石云下发的配置保存在设备上，当SDK需要时再从设备处获取。
        EZDEVSDK_ALARM_RT_INFO_GET_ALARM_SOUND,         ///< 获取报警声音，<b>必须实现</b>。信息体: ezDevSDK_alarm_sound_type
        EZDEVSDK_ALARM_RT_INFO_GET_ALARM_VIDEO,         ///< 获取报警小视频状态，信息体: ezDevSDK_alarm_video
        EZDEVSDK_ALARM_RT_INFO_GET_ALARM_STATUS,        ///< 获取报警状态，<b>必须实现</b>。信息体: ezDevSDK_alarm_status
        EZDEVSDK_ALARM_RT_INFO_GET_DEFENCE_PLAN,        ///< 获取布撤防计划。<b>必须实现</b>。信息体: ezDevSDK_alarm_defence_plan
        EZDEVSDK_ALARM_RT_INFO_GET_MOTION_DETECT_AREA,  ///< 获取移动侦测区域。信息体: ezDevSDK_alarm_motion_detect_area
        EZDEVSDK_ALARM_RT_INFO_GET_SWITCH_ENABLE,       ///< 获取开关状态（人形检测、闪烁灯状态等）。<b>必须实现</b>。信息体: ezDevSDK_alarm_switch_enable
        EZDEVSDK_ALARM_RT_INFO_GET_DETECTOR_ALARM_AREA, ///< 获取探测器区域。信息体: ezDevSDK_alarm_detector_alarm_area
        EZDEVSDK_ALARM_RT_INFO_GET_ALARM_LIGHT,         ///< 获取安防灯信息。信息体: ezDevSDK_alarm_light_info
        EZDEVSDK_ALARM_RT_INFO_GET_CALLING_CHIME,       ///< 获取响铃信息。信息体: ezDevSDK_alarm_calling_chime_info


        //以下参数是获取运行时参数，或其他领域配置的参数
        EZDEVSDK_ALARM_RT_INFO_GET_ENCRYPT_KEY,                     ///< 获取音视频加密状态，<b>必须实现</b>。信息体: ezDevSDK_alarm_encrypt_status
        EZDEVSDK_ALARM_RT_INFO_GET_TIME_ZONE,                       ///< 获取时区信息。信息体: ezDevSDK_alarm_time_zone_info
        EZDEVSDK_ALARM_RT_INFO_GET_PLATFORM_ENCRYPT,                ///< 设备是否需要平台加密，<b>必须实现</b>。信息体: ezDevSDK_alarm_platform_encrypt
        EZDEVSDK_ALARM_RT_INFO_GET_AREA_SUPPORT,                    ///< 图片中是否包含区域坐标信息。信息体: ezDevSDK_alarm_area_support
        EZDEVSDK_ALARM_RT_INFO_GET_ALARM_SOUND_VOLUME,              ///< 获取报警声音音量。信息体: ezDevSDK_alarm_sound_volume
        EZDEVSDK_ALARM_RT_INFO_GET_TINY_VIDEO_PIC,                  ///< 获取报警小视频多张小图。信息体: ezDevSDK_alarm_tiny_video_info
        EZDEVSDK_ALARM_RT_INFO_GET_ALARM_PLAN,                      ///< 获取计划。信息体：ezDevSDK_alarm_plan
        EZDEVSDK_ALARM_RT_INFO_GET_KV_HMCAR_DETECT,                 ///< KEY_VALUE 人车检测开关，信息体：ezDevSDK_alarm_kv_hmcar_detect
        EZDEVSDK_ALARM_RT_INFO_GET_ALARM_BODY_DETECT_TIME_INTERVAL, ///< 获取人体检测时间间隔。信息体: ezDevSDK_alarm_body_detect_time_interval
        EZDEVSDK_ALARM_RT_INFO_GET_TEMPERATURE_THRESHOLD,           ///< 获取温度提醒阈值(温感相机)。信息体: ezDevSDK_alarm_temperature_threshold
	    EZDEVSDK_ALARM_RT_INFO_GET_ALARM_INTERVAL,                  ///<获取告警时间间隔(温感相机) 。信息体: ezDevSDK_alarm_camera_alarm_interval
	    EZDEVSDK_ALARM_RT_INFO_GET_REMIND_WAY,                      ///< 获取高温预警提醒方式(温感相机)。信息体:  ezDevSDK_alarm_camera_remind_way
	    EZDEVSDK_ALARM_RT_INFO_GET_DETECT_AREA,                     ///< 获取侦测区域(新)，信息体: ezDevSDK_alarm_detect_area
		EZDEVSDK_ALARM_RT_INFO_GET_ALARM_LIGHT_DELAY_TIME,           ///< 获取灯光延时时间，消息体:ezDevSDK_alarm_light_delay_time
		EZDEVSDK_ALARM_RT_INFO_GET_ALARM_SPAN_CUSTERM,            ///< 获取用户向设备发送的报警最小间隔时间, 消息体: ezDevSDK_alarm_span_custerm
        EZDEVSDK_ALARM_RT_INFO_GET_KV_ADVANCED_DETECT,
        EZDEVSDK_ALARM_RT_INFO_GESTURE_REMIND_TYPE,                 ///< 手势提醒类型0:拍照  1:呼叫
        EZDEVSDK_ALARM_RT_INFO_GET_SOUND_DETECT_TYPE,
		EZDEVSDK_ALARM_RT_INFO_GET_INTELLI_APP_INFO,                ///< 获取智能算法应用信息, 消息体: ezDevSDK_intelligent_application_array
	    EZDEVSDK_ALARM_RT_INFO_GET_DYNAMIC_RESOURCE_INFO,			///< 获取设备可用资源信息, 消息体: ezDevSDK_dynamic_resource_info
        EZDEVSDK_ALARM_RT_INFO_GET_MESSAGE_NOTIFY_TYPE,           ///<获取消息通知类型，消息体：ezDevSDK_alarm_message_notify_type
		EZDEVSDK_ALARM_RT_INFO_GET_CALL_PIC,                     ///< 获取呼叫图片，<b>必须实现</b>。信息体: ezDevSDK_call_pic_data
    } EZDEVSDK_ALARM_RUNTIME_INFO_E;

    /**
     *  @brief   音视频加密状态，对应回调信息类型: EZDEVSDK_ALARM_RT_INFO_GET_ENCRYPT_KEY
     */
    typedef struct
    {
        int encrypt_status;    ///< 音视频加密状态
        char encrypt_code[36]; ///< 音视频加密秘钥
        int encrypt_len;       ///< 加密秘钥长度
    } ezDevSDK_alarm_encrypt_status;

    /**
     *  @brief 呼叫图片数据
     */
    typedef struct
    {
        char *pic_data; ///< 图片数据,内部开辟内存
        int pic_len;    ///< 图片长度
        unsigned long long time;
    } ezDevSDK_call_pic_data;

    /**
     *  @brief   时区信息，对应回调信息类型: EZDEVSDK_ALARM_RT_INFO_GET_TIME_ZONE
     */
    typedef struct
    {
        int hour;
        int minute;
        int no;
    } ezDevSDK_alarm_time_zone_info;

    /**
     *  @brief   设备是否平台加密，对应回调信息类型: EZDEVSDK_ALARM_RT_INFO_GET_PLATFORM_ENCRYPT
     */
    typedef struct
    {
        int enable;
    } ezDevSDK_alarm_platform_encrypt;

    /**
     *  @brief   图片中是否包含区域坐标信息，对应回调信息类型: EZDEVSDK_ALARM_RT_INFO_GET_AREA_SUPPORT
     */
    typedef struct
    {
        int support_method; ///< 0:无，1:小图中包含移动区域位置信息，2:图片(大图和小图)中含有智能坐标信息
    } ezDevSDK_alarm_area_support;

    //------------------------------------应用层发给SDK（或服务器）的消息-------------------------------------

    /**
     *  @brief 报警图片数据
     */
    typedef struct
    {
        char *pic_data; ///< 图片数据
        int pic_len;    ///< 图片长度
    } ezDevSDK_alarm_data;

    /**
     *  @brief 用户自定义数据
     */
    typedef struct
    {
        char *type;        ///< 自定义协议类型
        char *custom_info; ///< 报警自定义信息，长度限制: 不能超过80字节。若某一字段需传递非可见字符或者中文，需要使用base64编码后内容
    } ezDevSDK_alarm_custom_info;

    /**
     *  @brief 互联互通uuid
     */
    typedef struct
    {
        char relation_uuid[64]; ///< 报警关联ID
    } ezDevSDK_alarm_uuid_info;

    /**
     *  @brief 其他信息
     */
    typedef struct
    {
        int sd_state; ///< 本地存储信息
    } ezDevSDK_other_info;

    /**
     *  @brief 报警消息QOS等级
     */
    typedef enum
    {
        EZDEVSDK_ALARM_QOS0, ///< 把消息发出去，不管是否送达
        EZDEVSDK_ALARM_QOS1, ///< 等待服务器ack，确认消息送达
        EZDEVSDK_ALARM_QOS2, ///< 保留字段，暂未实现
    } EZDEVSDK_ALARM_QOS_E;

    /**
     *  @brief 消息发送回执参数
     */
    typedef struct
    {
        unsigned int msg_seq;         ///< 消息sequence
        EZDEVSDK_ALARM_QOS_E msg_qos; ///< 消息QOS等级
    } ezDevSDK_alarm_ack_info;

    /**
     * @brief   消息上报回执，对应EZDEVSDK_ALARM_ON_PUSH_ACK
     */
    typedef struct
    {
        unsigned int last_error;      ///< 错误码
        unsigned int msg_seq;         ///< 消息sequence
        EZDEVSDK_ALARM_QOS_E msg_qos; ///< 消息QOS等级
    } ezDevSDK_alarm_push_ack;

    /**
     * @brief   消息发送相关属性值
     */
    typedef struct
    {
        int cancle_defence_send_enable; ///< 撤防下是否允许发送报警，0:不允许，1:允许
        int asyn_send_flag;             ///< 报警消息发送异步标记，0:同步发送，1:异步发送
        int customFlags;                 ///< 消息类型弹框推送 0：推送 1：不推送 
        int async_pic_flag;              ///< 图片异步发送标记, 0:同步发送，1:异步发送
    } ezDevSDK_alarm_property;

    
    typedef struct
    {
        int cloud_type;                 //云存储类型
        char bucket[64];                //云存储bucket名称，业务消息上报的时候返回
        char pic_uuid[64];              /// 图片UUID    
        int life_cycle;
        char check_sum[33];             //图片秘钥CS
    } ezDevSDK_alarm_ai_pic_info;
    
     /**
     * @brief   报警的user info
     */
    typedef struct
    {
        char* userid_arr;   ///可解析为json数组的字符串,可以包含多个userid ,格式: ["userid1","userid2","userid3"]
        int ai_tags;        ///标记此次业务针对AI标签使用
        ezDevSDK_alarm_ai_pic_info ai_pic_info;
    } ezDevSDK_alarm_user_info;

    /**
     *  @brief 告警业务类型
     */
    typedef enum
    {
        EZDEVSDK_ALARM_BUSINESS_ALARM = 1,       ///< 告警业务
        EZDEVSDK_ALARM_BUSINESS_CALL = 2,        ///< 呼叫业务
        EZDEVSDK_ALARM_BUSINESS_LEAVEMSG = 3,    ///< 留言业务，此功能未实现
        EZDEVSDK_ALARM_BUSINESS_ALARM_VIDEO = 4, ///< 告警业务，带小视频
        EZDEVSDK_ALARM_BUSINESS_ALARM_MULTI_PIC = 5, ///< 多图告警业务，此功能未实现
    } EZDEVSDK_ALARM_BUSINESS_TYPE_E;

    /**
     *@brief 报警通道类型
    */
    typedef enum
    {
        VIDEO_CHANNEL = 1, ///< 视频通道
        IO_CHANNEL = 2,    ///< IO通道。如探测器类没有视频能力
    } EZDEVSDK_CHANNEL_TYPE_E;

    /**
     *  @brief 告警消息详细信息
     */
    typedef struct
    {
        unsigned char business_type; ///< 告警业务类型，@ref EZDEVSDK_ALARM_BUSINESS_TYPE_E
        unsigned char channel;       ///< 通道，0代表设备本身
        time_t timestamp;            ///< 报警时间戳
        char alarm_type[64];         ///< 报警类型
        union {
            struct
            {
                unsigned char channel_type; ///< 报警通道类型，@ref EZDEVSDK_CHANNEL_TYPE_E
                unsigned char sd_state;     ///< sd卡状态
                char location[64];          ///< 报警位置信息
                char sample_name[64];       ///< 智能分析使用的样本名称
                char intelligent_data[128]; ///< 智能元数据
            } alarm_info;
            struct
            {
                unsigned char call_state; ///< 呼叫报警状态，1:呼叫报警，2:呼叫接听，3:呼叫挂断
                char calling_id[65];      ///< 呼叫唯一ID，若外部不关心，请初始化为空串，由内部生成。
                                          ///< 若使用，外部需要保持呼叫和呼叫接听/挂断时，使用相同的calling_id
                char caller[64];          ///< 呼叫信息的发起方
                char handler[64];         ///< 呼叫信息的处理方
                int call_type;            ///< 业务呼叫类型，0:未知，1:儿童手表，2:猫眼，3:门铃
            } calling_info;
        } business_info;
        int customFlags;                 ///< 消息类型弹框推送 0：推送 1：不推送
    } ezDevSDK_alarm_msg_info;

    /**
     *  @brief 附加信息，根据业务的不同，需要添加一些额外的信息，
     */
    typedef struct
    {
        ezDevSDK_alarm_custom_info *custom_info;      ///< 用户自定义的信息，如果没有填NULL
        ezDevSDK_alarm_uuid_info *relation_uuid_info; ///< 局域网内互联互通信息，如果没有填NULL
        ezDevSDK_other_info *other_info;              ///< 本地存储信息，如果没有填NULL
        ezDevSDK_alarm_ack_info *ack_info;            ///< 消息发送回执参数
        ezDevSDK_alarm_property *property;            ///< 消息发送相关属性值
        ezDevSDK_alarm_user_info *user_info; 
    } ezDevSDK_alarm_extra_info;

    typedef struct
    {
        ezDevSDK_alarm_ai_pic_info pic_info;
        char tag_uuid[64];
        unsigned char cid[16];
        time_t begin_time;
        time_t end_time;
        char *aitag;
    } ezDevSDK_alarm_ai_tags;


    #define JPG_OBJ_LENGTH 10
    typedef struct OBJ_POSITION_s
    {
        short x;      //矩形左上角X轴坐标
        short y;      //矩形左上角Y轴坐标
        short width;  //矩形宽度
        short height; //矩形高度
    }  ezDevSDK_OBJ_POSITION_t;

    typedef struct JPG_OBJ_POSITION_INFO_s
    {
        int obj_num; //目标个数
         ezDevSDK_OBJ_POSITION_t obj_position[JPG_OBJ_LENGTH];
    }  ezDevSDK_JPG_OBJ_POSITION_INFO_t;

    typedef struct JPG_MTD_AREA_INFO_s
    {
        int Info[18];
    }  ezDevSDK_JPG_MTD_AREA_INFO_t;

    typedef enum
    {
        EZDEVSDK_JPG_OBJ_POSITION_INFO,
        EZDEVSDK_JPG_MTD_AREA_INFO,
    }ezDevSDK_JPG_APPEND_INFO_TYPE_t;
    
    typedef struct JPG_APPEND_INFO_s
    {
        int type;           //类型
        int length;         //结构体大小

        union {
             ezDevSDK_JPG_OBJ_POSITION_INFO_t ObjPositionInfo; //目标位置信息
             ezDevSDK_JPG_MTD_AREA_INFO_t MtdAreaInfo;         //移动区域信息
        };
    } ezDevSDK_JPG_APPEND_INFO_t;


    typedef struct
    {
        char pic_result[64];                                 /// 图片导入结果
        char model_result[64];                               /// 图片建模结果
        char target_lib_id[64];
        char target_id[64];
        char pic_id[64];
    } ezDevSDK_alarm_target_lib_result;


    //------------------------------------应用层配置参数--------------------------------------------

    /**
     *  @brief    SSL接口，外层须实现
     */
    typedef struct
    {
        void *(*on_connect)(int socket);
        int (*on_send)(void *handle, char *buf, int len);
        int (*on_recv)(void *handle, char *buf, int len);
        int (*on_close)(void *handle);
    } ezDevSDK_alarm_ssl_callback;

    /*! \} */

#ifdef __cplusplus
}
#endif

#endif //H_EZDEVSDK_ALARM_DEF_H_
