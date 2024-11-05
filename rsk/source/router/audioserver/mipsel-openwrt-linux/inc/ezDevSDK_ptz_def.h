/**
 * \file      ezDevSDK_ptz_def.h  
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     云台控制领域对外结构体定义、宏定义
 * 
 * \author    xiangbin
 * 
 * \date      2017/5/22
 */
#ifndef H_EZDEVSDK_PTZ_DEF_H_
#define H_EZDEVSDK_PTZ_DEF_H_
#include "ezDevSDK_error.h"
#include <time.h>


#ifdef __cplusplus
extern "C"
{
#endif

/* 预置点相关常量定义 */
#define EZDEVSDK_PTZ_MAX_PRESET_NUM 300
#define EZDEVSDK_PTZ_SERIAL_NO_NUM 32
#define EZDEVSDK_PTZ_LINKAGE_PRESET_NUM 36
#define EZDEVSDK_PTZ_MAX_CHANNEL_NUM 12

    /**
     * \addtogroup Ptz_Module
     * \{
     */

    //-------------------------------------错误码------------------------------------------------------

    /**
     * \brief 萤石SDK云台控制领域的接口错误码，由SDK返回给调用者
     * \note  公用错误码，请参考ezDevSDK_error.h
     */
    typedef enum
    {
        EZDEVSDK_PTZ_SUCESS                     = 0X00000000,    ///< 成功
        EZDEVSDK_PTZ_CODE_BASE                  = 0XD3600000, ///< 云台控制领域接口错误码起始值，D表示接口错误码，3600，表示云台控制领域，最后三位表示具体错误

        ///< 0XD3600000 - 0XD3600001F 公共错误码
        EZDEVSDK_PTZ_CODE_FAIL                  = EZDEVSDK_PTZ_CODE_BASE + EZDEVSDK_ERROR_FAIL,
        EZDEVSDK_PTZ_CODE_NOT_INITED            = EZDEVSDK_PTZ_CODE_BASE + EZDEVSDK_ERROR_NOT_INITED,
        EZDEVSDK_PTZ_CODE_INVALID_PARAM         = EZDEVSDK_PTZ_CODE_BASE + EZDEVSDK_ERROR_INVALID_PARAM,
        EZDEVSDK_PTZ_CODE_NOT_SUPPORT           = EZDEVSDK_PTZ_CODE_BASE + EZDEVSDK_ERROR_NOT_SUPPORT,
        EZDEVSDK_PTZ_CODE_NOT_ENOUGH_MEMERY     = EZDEVSDK_PTZ_CODE_BASE + EZDEVSDK_ERROR_NOT_ENOUGH_MEMERY,
        EZDEVSDK_PTZ_CODE_INIT_FAIL             = EZDEVSDK_PTZ_CODE_BASE + EZDEVSDK_ERROR_INIT_FAIL,
        EZDEVSDK_PTZ_CODE_THREAD_CREATE_FAIL    = EZDEVSDK_PTZ_CODE_BASE + EZDEVSDK_ERROR_THREAD_CREATE_FAIL,
        EZDEVSDK_PTZ_CODE_RSP_TIMEOUT           = EZDEVSDK_PTZ_CODE_BASE + EZDEVSDK_ERROR_RSP_TIMEOUT,
        EZDEVSDK_PTZ_CODE_MSG_DATA_INVAILD      = EZDEVSDK_PTZ_CODE_BASE + EZDEVSDK_ERROR_MSG_DATA_INVAILD,

        ///< 0XD3600020 - 0XD36000FF 领域普通错误码
        EZDEVSDK_PTZ_CODE_BUF_NOT_ENOUGH        = 0XD3600020, ///< 缓存长度不够

        ///< 0XD3600100 - 0XD3600FFF 回调交互错误码
        EZDEVSDK_PTZ_CODE_CB_CALLING_PRESET_FAIL                = 0XD3600100, ///< 正在调用预置点
        EZDEVSDK_PTZ_CODE_CB_SOUND_LOCALIZATION_FAIL            = 0XD3600101, ///< 当前正在声源定位
        EZDEVSDK_PTZ_CODE_CB_CRUISE_TRACK_FAIL                  = 0XD3600102, ///< 当前正在轨迹巡航
        EZDEVSDK_PTZ_CODE_CB_PRESET_INVALID_POSITION_FAIL       = 0XD3600103, ///< 预置点无效
        EZDEVSDK_PTZ_CODE_CB_PRESET_CURRENT_POSITION_FAIL       = 0XD3600104, ///< 已在当前预置点
        EZDEVSDK_PTZ_CODE_CB_RESPONSE_SOUND_LOCALIZATION_FAIL   = 0XD3600105, ///< 正在响应声源定位
        EZDEVSDK_PTZ_CODE_CB_OPENING_PRIVACY_FAIL               = 0XD3600106, ///< 当前正在打开隐私遮蔽
        EZDEVSDK_PTZ_CODE_CB_CLOSING_PRIVACY_FAIL               = 0XD3600107, ///< 当前正在关闭隐私遮蔽
        EZDEVSDK_PTZ_CODE_CB_PRESET_EXCEED_MAXNUM_FAIL          = 0XD3600108, ///< 预置点超过最大数
        EZDEVSDK_PTZ_CODE_CB_PRIVACYING_FAIL                    = 0XD3600109, ///< 设备处于隐私模式
        EZDEVSDK_PTZ_CODE_CB_MIRRORING_FAIL                     = 0XD360010a, ///< 正在镜像操作
        EZDEVSDK_PTZ_CODE_CB_KEY_CONTROLING_FAIL                = 0XD360010b, ///< 正在键控操作
        EZDEVSDK_PTZ_CODE_CB_TTSING_FAIL                        = 0XD360010c, ///< 正在语音对讲
        EZDEVSDK_PTZ_CODE_CB_ROTATING_UP_LIMIT_FAIL             = 0XD360010d, ///< 云台旋转到上限
        EZDEVSDK_PTZ_CODE_CB_ROTATING_DOWN_LIMIT_FAIL           = 0XD360010e, ///< 云台旋转到下限
        EZDEVSDK_PTZ_CODE_CB_ROTATING_LEFT_LIMIT_FAIL           = 0XD360010f, ///< 云台旋转到左限
        EZDEVSDK_PTZ_CODE_CB_ROTATING_RIGHT_LIMIT_FAIL          = 0XD3600110, ///< 云台旋转到右限
        EZDEVSDK_PTZ_CODE_CB_CRUISE_PRESET_ANGLE_ERROR          = 0XD3600111, ///< 巡航预置点角度间隔太小(小余15度)
    } EZDEVSDK_PTZ_CODE_E;

    /**
     * \brief 由调用层设置的协议错误码，内部转化后传递到客户端
     */
    typedef enum
    {
        EZDEVSDK_PTZ_PROTOCOL_CODE_BASE = 0XF3600000,   ///< 云台控制领域协议错误码起始值，F表示协议错误码，3600，表示云台控制领域，最后三位表示具体错误
    } EZDEVSDK_PTZ_PROTOCOL_CODE_E;

    //--------------------------------初始化信息---------------------------------------------------------

    typedef struct
    {
        int type;   ///< \c EZDEVSDK_PTZ_INFO_E
        void *data; ///< 指向具体type类型的数据
        int len;    ///< data数据长度
    } ezDevSDK_ptz_runtime_info;
    
    /**
    * \brief SDK 接收消息的结构体
    */
    typedef struct
    {
        int type; ///< \c EZDEVSDK_PTZ_MSG_TO_DEV_TYPE_E
        void *data;
        int len;
    } ezDevSDK_ptz_msg_to_dev;

    /**
     *  \brief 云台控制命令相关描述
     */
    typedef enum
    {
        EZDEVSDK_PTZ_CTRL_CMD_UP = 1,             ///< 上
        EZDEVSDK_PTZ_CTRL_CMD_DOWN,               ///< 下
        EZDEVSDK_PTZ_CTRL_CMD_LEFT,               ///< 左
        EZDEVSDK_PTZ_CTRL_CMD_RIGHT,              ///< 右
        EZDEVSDK_PTZ_CTRL_CMD_UPLEFT,             ///< 上左
        EZDEVSDK_PTZ_CTRL_CMD_DOWNLEFT,           ///< 下左
        EZDEVSDK_PTZ_CTRL_CMD_UPRIGHT,            ///< 上右
        EZDEVSDK_PTZ_CTRL_CMD_DOWNRIGHT,          ///< 下右
        EZDEVSDK_PTZ_CTRL_CMD_ZOOMIN,             ///< 调焦拉近
        EZDEVSDK_PTZ_CTRL_CMD_ZOOMOUT,            ///< 调焦拉远
        EZDEVSDK_PTZ_CTRL_CMD_FOCUSNEAR,          ///< 聚焦近焦
        EZDEVSDK_PTZ_CTRL_CMD_FOCUSFAR,           ///< 聚焦远焦
        EZDEVSDK_PTZ_CTRL_CMD_IRISSTARTUP,        ///< 上滤光片
        EZDEVSDK_PTZ_CTRL_CMD_IRISSTARTDOWN,      ///< 下滤光片
        EZDEVSDK_PTZ_CTRL_CMD_AUTO,               ///< 云台水平自动模式
        EZDEVSDK_PTZ_CTRL_CMD_GOTO_PRESET,        ///< 调用预置点
        EZDEVSDK_PTZ_CTRL_CMD_3D_POSITION,        ///< 云台3D定位
        EZDEVSDK_PTZ_CTRL_CMD_PANORAMIC_POSITION, ///< 全景云台定位

        EZDEVSDK_PTZ_CTRL_CMD_END
    } EZDEVSDK_PTZ_CTRL_CMD_E;

    typedef enum
    {
        EZDEVSDK_PTZ_ACTION_INVALID,
        EZDEVSDK_PTZ_ACTION_START,
        EZDEVSDK_PTZ_ACTION_STOP
    } EZDEVSDK_PTZ_ACTION_E;

    /**
     *  \brief 云台控制参数相关描述
     */
    typedef struct
    {
        int action;          ///< 云台启停动作, 枚举中取值 \c EZDEVSDK_PTZ_ACTION_E
        int speed;           ///< 云台速度
        int preset_index;    ///< 预置点索引
        char uuid[32];       ///< 云台控制uuid
        int http_p2p_method; ///< 标志云台控制是http方式还是P2P 高速通道方式，1:http, 2:p2p
    } ezDevSDK_ptz_param;

    typedef struct
    {
        double x; ///< x坐标
        double y; ///< y坐标
    } ezDevSDK_ptz_position;

    typedef struct
    {
        ezDevSDK_ptz_position start; ///< 开始坐标
        ezDevSDK_ptz_position end;   ///< 结束坐标
    } ezDevSDK_ptz_3D_position;

    /**
     * \brief   SDK回调给用户层接口
     */
    typedef struct 
    {
    /**
     *  \brief      云台控制回调
     *  \param[in]  param cmd 云台命令，枚举 \c EZDEVSDK_PTZ_CTRL_CMD_E
     *  \param[in]  channel 通道号，0表示本设备
     *  \param[in]  param ptz控制参数，根据命令cmd参数识别是ezDevSDK_ptz_param或ezDevSDK_ptz_3D_position
     *  \return     错误码，错误码必须是EZDEVSDK_PTZ_PROTOCOL_CODE_E中的值
     *  \see        错误码 ： EZDEVSDK_PTZ_PROTOCOL_CODE_E
     */
        int (*ptz_ctrl)(int cmd, int channel, void *param, int param_len);
        int (*recv_msg)(ezDevSDK_ptz_msg_to_dev *msg);             ///< 消息回调接口，设备通过此接口接收云台领域的消息
        int (*get_runtime_info)(ezDevSDK_ptz_runtime_info *info); ///< SDK运行时获取信息回调，不能有任何阻塞操作
    } ezDevSDK_ptz_callback;

    typedef struct
    {
        ezDevSDK_ptz_callback cb;                                   ///< 回调函数
    } ezDevSDK_ptz_init_info;

    // 云台预置点管理(单一巡航路径)
    typedef struct
    {
        int channel;    // 通道号
        int status;     // 0:停止    1:开始
        int patrolId;   // 巡航路径ID
    } ezDevSDK_ptz_set_one_patrol_s;
//-----------------------------------------------------------------------------------------

    //--------------------------------SDK通知应用层消息-----------------------------------------

    /**
* \brief   SDK给应用的消息
*/
    typedef enum
    {
        EZDEVSDK_PTZ_ON_ADD_PRESET = 1,        ///< 增加预置点
        EZDEVSDK_PTZ_ON_DEL_PRESET,            ///< 删除预置点
        EZDEVSDK_PTZ_ON_SET_CRUISE_POSITION,   ///< 设置巡航路径
        EZDEVSDK_PTZ_ON_CLEAR_CRUISE_POSITION, ///< 清除巡航路径
        EZDEVSDK_PTZ_ON_ADD_LINKAGE,           ///< 增加探测器云台关联关系
        EZDEVSDK_PTZ_ON_DEL_LINKAGE,           ///< 删除探测器云台关联关系
        EZDEVSDK_PTZ_ON_CLEAR_LINKAGE,         ///< 清除探测器云台关联关系
        EZDEVSDK_PTZ_ON_SET_SWITCH_STATUS,     ///< 设置云台开关状态
        EZDEVSDK_PTZ_ON_POSITION_ADJUST,       ///< 云台位置校准
        EZDEVSDK_PTZ_ON_TRANS_CMD_MSG,         ///< 由外部实现的请求所对应的平台回应信息
        EZDEVSDK_PTZ_ON_SET_CUSTOM_HOME_POINT_STATUS,      ///< 设置自定义归位点。信息体：ezDevSDK_ptz_custom_home_point_status
        EZDEVSDK_PTZ_ON_SET_HOME_POINT_MODE,      ///< 设置归位点模式（0：默认归位点 1：自定义归位点）。信息体：ezDevSDK_ptz_custom_home_point_mode
        EZDEVSDK_PTZ_ON_SET_PTZ_PLAN,           ///< 设置云台计划。信息体：ezDevSDK_ptz_plan
        EZDEVSDK_PTZ_ON_SET_CRUISE_PRESET,        ///< 设置巡航预置点。信息体 \c ezDevSDK_ptz_preset_info
        EZDEVSDK_PTZ_ON_CLE_CRUISE_PRESET,        ///< 清空巡航预置点。信息体：ezDevSDK_ptz_preset_info
        EZDEVSDK_PTZ_ON_SET_CRUISE_PRESET_EFFECT,  ///< 巡航预置点生效索引值。信息体 \c ezDevSDK_ptz_set_cruise_preset_effect
        EZDEVSDK_PTZ_ON_SET_ONE_PATROL,        ///< 单一巡航路径 ezDevSDK_ptz_set_one_patrol_s
    } EZDEVSDK_PTZ_MSG_TO_DEV_TYPE_E;

    /**
     * \brief 预置点参数配置结构体
     */
    typedef struct
    {
        int channel;      ///< 通道(入参)  0:代表设备
        int preset_index; ///< 预置点索引(入参或出参)，根据具体命令定义
    } ezDevSDK_ptz_preset_info;

    /**
     * \brief 巡航路径配置结构体
     */
    typedef struct
    {
        int channel;                                   ///< 通道(入参)  0:代表设备
        int cruise_num;                                ///< 巡航路径号(入参)
        int preset_num;                                ///< 预置点索引(入参)，根据具体命令定义
        int preset_index[EZDEVSDK_PTZ_MAX_PRESET_NUM]; ///< 预置点索引信息(入参)，取前preset_num值
    } ezDevSDK_ptz_set_cruise_position;

    /**
     * \brief 巡航路径清除结构体
     */
    typedef struct
    {
        int channel;    ///< 通道(入参)  0:代表设备
        int cruise_num; ///< 巡航路径号(入参)
    } ezDevSDK_ptz_clear_cruise_position;

    /**
     * \brief 云台探测器关联结构体
     */
    typedef struct
    {
        int channel;                                ///< 通道(入参)  0:代表设备
        char serial_no[EZDEVSDK_PTZ_SERIAL_NO_NUM]; ///< 探测器序列号(人参)
        ezDevSDK_ptz_position position;             ///< 云台坐标
    } ezDevSDK_ptz_linkage_info;

#define EZDEVSDK_PTZ_SWITCH_TYPE_MOTION_TRACE 1
#define EZDEVSDK_PTZ_SWITCH_TYPE_AUTO_RESET 2
#define EZDEVSDK_PTZ_SWITCH_TYPE_SOUND_SOURCE_LOCATE 3 ///<声源定位
#define EZDEVSDK_PTZ_SWITCH_TYPE_HUMAN_TRACK 4         ///<人形/人脸跟踪
#define EZDEVSDK_PTZ_SWITCH_TYPE_CRUISE_TRACK 5        ///<巡航跟踪
#define EZDEVSDK_PTZ_SWITCH_TYPE_CRUISE_PRESET_DEBUG   6        ///<云台设置巡航点调试开关



    typedef struct
    {
        int channel; ///< 通道(入参)  0:代表设备
        int type;    ///< 类型(入参/出参)
        int enable;  ///< 开关状态 1:打开 0:关闭
    } ezDevSDK_ptz_switch_status;

    typedef struct
    {
        int channel; ///< 通道(入参)  0:代表设备
    } ezDevSDK_ptz_position_adjust;

    /**
     * \brief 外部组装的信令信息，或者平台回复的信息
     */
    typedef struct
    {
        char *content;          ///< 组装完毕的信令信息，或者平台回应的信息
        int content_len;        ///< 信息长度
        unsigned int cmd_id;    ///< 与平台交互的信令ID
        unsigned char msg_type; ///< 信令交互类型, 0-请求, 1-响应
        unsigned int msg_seq;   ///< 序号
    } ezDevSDK_ptz_trans_cmd_msg;

     /**
     * \brief 巡航自定义归位点状态
     */
    typedef struct
    {
        int home_point_status;    ///<自定义归位点状态 (0:未设置 1：已设置)
    } ezDevSDK_ptz_custom_home_point_status;

     /**
     * \brief 巡航归位点模式
     */
    typedef struct
    {
        int home_point_mode;    ///<巡航归位点模式 (0:默认归位点 1：自定义归位点)
    } ezDevSDK_ptz_home_point_mode;

     /**
     * \brief 巡航预置点设置生效命令
     */
    typedef struct
    {
        int channel;   ///<通道号
        char Index[16];  ///<已成功设置的预置点，例如：“1111”
    } ezDevSDK_ptz_set_cruise_preset_effect;


     /**
     * \brief 云台领域计划枚举
     */
    typedef enum {
        EZDEVSDK_PTZ_AUTO_CRUISE_PLAN = 0x0,     ///< 云台自动巡航计划
        EZDEVSDK_PTZ_PLAN_MAX,
    } EZDEVSDK_PTZ_PLAN_TYPE_E;

    /**
     * @brief   巡航计划时间
     */
    typedef struct
    {
        time_t begin;                               ///< 计划开始时间
        time_t end;                                 ///< 计划结束时间
    } ezDevSDK_ptz_time_segment;


    typedef struct 
    {
        int type;                                   ///< 计划类型@ref EZDEVSDK_PTZ_PLAN_TYPE_E
        int channel;                                ///< 通道号
        int enable_defence_plan;                    ///< 开关状态
        union
        {
            struct
            {
                ezDevSDK_ptz_time_segment time[7][4];
            } ptz_auto_cruise_plan;
        } plan_info;
    } ezDevSDK_ptz_plan;

    //--------------------------------SDK从应用层获取的消息-----------------------------------------

    /**
    * \brief   SDK运行时信息枚举和结构体
    */
    typedef enum
    {
        EZDEVSDK_PTZ_RT_INFO_GET_NEW_PRESET = 1,          ///< 获取新的预置点
        EZDEVSDK_PTZ_RT_INFO_GET_PRESET_CFG,              ///< 获取预置点配置
        EZDEVSDK_PTZ_RT_INFO_GET_LINKAGE_CFG,             ///< 获取探测器云台关联配置
        EZDEVSDK_PTZ_RT_INFO_CAPTURE_PICTURE,             ///< 获取抓图
        EZDEVSDK_PTZ_RT_INFO_PANORAMIC_PIC_NUM,           ///< 获取全景抓图张数
        EZDEVSDK_PTZ_RT_INFO_CHANNEL_PTZ_CFG,             ///< 获取通道云台配置
        EZDEVSDK_PTZ_RT_INFO_GET_ENCRYPT_INFO,            ///< 获取加密信息
        EZDEVSDK_PTZ_RT_INFO_GET_SWITCH_STATUS,           ///< 获取开关状态
        EZDEVSDK_PTZ_RT_INFO_GET_AUTO_RESET_CFG,          ///< 获取自动复位上报通道配置
        EZDEVSDK_PTZ_RT_INFO_GET_MOTION_TRACK_CFG,        ///< 获取移动跟踪配置
        EZDEVSDK_PTZ_RT_INFO_GET_SOUND_SOURCE_LOCATE_CFG, //获取声源定位通道配置 。信息体：ezDevSDK_ptz_sound_source_locate_cfg
        EZDEVSDK_PTZ_RT_INFO_GET_HUMAN_TRACK_CFG,         //获取人形/人脸跟踪通道配置。信息体：ezDevSDK_ptz_human_track_cfg
        EZDEVSDK_PTZ_RT_INFO_GET_CRUISE_TRACK_CFG,        //获取巡航跟踪通道配置。信息体：ezDevSDK_ptz_cruise_track_cfg
        EZDEVSDK_PTZ_RT_INFO_GET_CRUISE_PRESET_DEBUG_CFG, //获取巡航预置点调试开关结构体。信息体/c ezDevSDK_ptz_cruise_preset_debug_cfg
        EZDEVSDK_PTZ_RT_INFO_GET_CUSTOM_HOME_POINT_STATUS,  ///< 获取自定义归位点状态。信息体：ezDevSDK_ptz_custom_home_point_status
        EZDEVSDK_PTZ_RT_INFO_GET_HOME_POINT_MODE,      ///< 获取设置归位点模式（0：默认归位点 1：自定义归位点）。信息体：ezDevSDK_ptz_custom_home_point_mode
        EZDEVSDK_PTZ_RT_INFO_GET_PTZ_PLAN,              ///<获取云台计划。信息体：ezDevSDK_ptz_plan
        EZDEVSDK_PTZ_RT_INFO_GET_PTZ_CRUISE_PRESET_EFFECT,  ///<获取巡航预置点生效索引值。信息体：ezDevSDK_ptz_set_cruise_preset_effect
    } EZDEVSDK_PTZ_RUNTIME_INFO_E;

    ///
    /// \brief 预置点配置结构体
    ///
    typedef struct
    {
        int channel;                                   ///< 通道(入参)  0:代表设备
        char serial_no[EZDEVSDK_PTZ_SERIAL_NO_NUM];    ///< 通道序列号(出参)
        int preset_num;                                ///< 预置点个数(出参)
        int preset_index[EZDEVSDK_PTZ_MAX_PRESET_NUM]; ///< 预置点索引信息(出参)，取前preset_num值
    } ezDevSDK_ptz_preset_cfg;

    ///
    /// \brief 探测器联动关系配置结构体
    ///
    typedef struct
    {
        char serial_no[EZDEVSDK_PTZ_SERIAL_NO_NUM]; ///< 探测器序列号(出参)
        ezDevSDK_ptz_position position;             ///< 云台位置信息(出参)
    } ezDevSDK_ptz_linkage_preset;
    ///
    /// \brief 探测器联动关系配置结构体
    ///
    typedef struct
    {
        int linkage_preset_num;                                            ///< 预置点个数(出参)
        ezDevSDK_ptz_linkage_preset info[EZDEVSDK_PTZ_LINKAGE_PRESET_NUM]; ///< 云台探测器信息(出参),前linkage_preset_num有效
    } ezDevSDK_ptz_linkage_cfg;

    typedef enum
    {
        EZDEVSDK_PTZ_CAPTURE_PIC_STAT_START = 1,
        EZDEVSDK_PTZ_CAPTURE_PIC_STAT_CAPUTRE,
        EZDEVSDK_PTZ_CAPTURE_PIC_STAT_END
    } EZDEVSDK_PTZ_CAPTURE_PIC_STAT_E;

    ///
    /// \brief 全景抓图信息结构体
    ///
    typedef struct
    {
        int channel;                            ///< 通道号(入参)
        EZDEVSDK_PTZ_CAPTURE_PIC_STAT_E status; ///< 抓图状态(入参)
        int num;                                ///< 抓图第几张数(入参)
        char *data;                             ///< 图片地址(入参)
        int len;                                ///< 图片长度(入参为data可存储长度地址，出参为实际的图片长度)
    } ezDevSDK_ptz_capture_picture_info;

    ///
    /// \brief 全景抓图信息结构体
    ///
    typedef struct
    {
        int channel;     ///< 通道号(入参)
        int pic_max_num; ///< 抓图最大张数(出参)
    } ezDevSDK_ptz_panoramic_picture_info;

    ///
    /// \brief 通道云台配置结构体
    ///
    typedef struct
    {
        int channel_num;                                 ///< 通道数
        short ptz_channel[EZDEVSDK_PTZ_MAX_CHANNEL_NUM]; ///< 云台通道号，前channel_num有效
    } ezDevSDK_ptz_channel_cfg;

    ///
    /// \brief 云台自动复位通道配置结构体
    ///
    typedef struct
    {
        int channel_num;                             ///< 通道数
        short channel[EZDEVSDK_PTZ_MAX_CHANNEL_NUM]; ///< 上报通道号，前channel_num有效
    } ezDevSDK_ptz_auto_reset_cfg;

    ///
    /// \brief 云台移动跟踪通道配置结构体
    ///
    typedef struct
    {
        int channel_num;                             ///< 通道数
        short channel[EZDEVSDK_PTZ_MAX_CHANNEL_NUM]; ///< 上报通道号，前channel_num有效
    } ezDevSDK_ptz_motion_track_cfg;

    ///
    /// @brief 声源定位通道配置结构体
    ///
    typedef struct
    {
        int channel_num;                             ///<通道数
        short channel[EZDEVSDK_PTZ_MAX_CHANNEL_NUM]; ///<上报通道号，前channel_num有效
    } ezDevSDK_ptz_sound_source_locate_cfg;

    ///
    /// @brief 人形/人脸跟踪通道配置结构体
    ///
    typedef struct
    {
        int channel_num;                             ///<通道数
        short channel[EZDEVSDK_PTZ_MAX_CHANNEL_NUM]; ///<上报通道号，前channel_num有效
    } ezDevSDK_ptz_human_track_cfg;

    ///
    /// @brief 巡航跟踪通道配置结构体
    ///
    typedef struct
    {
        int channel_num;                             ///<通道数
        short channel[EZDEVSDK_PTZ_MAX_CHANNEL_NUM]; ///<上报通道号，前channel_num有效
    } ezDevSDK_ptz_cruise_track_cfg;

    ///
    /// @brief 巡航预置点调试开关结构体
    ///
    typedef struct
    {
        int channel_num;                             ///<通道数
        short channel[EZDEVSDK_PTZ_MAX_CHANNEL_NUM]; ///<上报通道号，前channel_num有效
    } ezDevSDK_ptz_cruise_preset_debug_cfg;


    
    /// \brief 加密信息配置结构体
    ///
    typedef struct
    {
        int channel_num;   ///< 通道号，0代码设备
        int enable;        ///< 开关状态 1:打开 0:关闭
        char password[64]; ///< 密码(出参)
    } ezDevSDK_ptz_encrypt_info;

    /*! \} */

#ifdef __cplusplus
}
#endif

#endif
