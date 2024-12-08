/**
 * \file	  ezDevSDK_DoorLock_def.h
 * 
 * \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 * 
 * \brief     智能门锁领域对外结构体定义、宏定义
 * 
 * \author    liuxiangchen
 * 
 * \date      2019/5/1
 */
#ifndef H_EZDEVSDK_DOOR_LOCK_DEF_H_
#define H_EZDEVSDK_DOOR_LOCK_DEF_H_

#include "typedef.h"
#include <time.h>


/**
 * \addtogroup DoorLock_Module
 * \{
 */

//-------------------------------------错误码------------------------------------------------------

/**
 * \brief 萤石SDK，智能门锁领域错误码
 */
typedef enum
{
    EZDEVSDK_DOORLOCK_CODE_SUCCESS     = 0x0000,     	///< 成功
    EZDEVSDK_DOORLOCK_CODE_FAIL,                        ///< 失败
    EZDEVSDK_DOORLOCK_CODE_NOT_INITED,		        	///< 领域未初始化
    EZDEVSDK_DOORLOCK_CODE_MUL_INITED,		        	///< 领域重复初始化
    EZDEVSDK_DOORLOCK_CODE_INVALID_PARAM,		    	///< 错误参数
    EZDEVSDK_DOORLOCK_CODE_NOT_SUPPORT,                 ///< 功能不支持
    EZDEVSDK_DOORLOCK_CODE_SEND_DATA_ERROR,             ///< 发送数据失败


    EZDEVSDK_DOORLOCK_CODE_SNAP_PASSWORD_FULL      = 0x18001,   ///< 临时密码已达上限
    EZDEVSDK_DOORLOCK_CODE_SNAP_PASSWORD_NOT_EXIST = 0x18002,   ///< 临时密码不存在
    EZDEVSDK_DOORLOCK_CODE_SNAP_PASSWORD_EXIST     = 0x18003,   ///< 临时密码已存在
    EZDEVSDK_DOORLOCK_CODE_USER_DELETE_PROHIBITED  = 0x18004,   ///< 主用户禁止删除
    EZDEVSDK_DOORLOCK_CODE_USER_AUTH_DELETE_PROHIBITED = 0x18005,  ///< 主用户最后一个认证方式禁止删除


    EZDEVSDK_DOORLOCK_CODE_MAX,

} EZDEVSDK_DOORLOCK_CODE_E;


//-------------------------------------初始化信息--------------------------------------------------

/**
 * \brief SDK给应用的消息结构体
 */
typedef struct
{
	int     iType;      ///< SDK给应用的消息类型, 参见 EZDEVSDK_DOORLOCK_MSG2DEV_TYPE_E
	void*   pBuf;       ///< 所给消息内容
	UINT32  uiSize;     ///< 所给消息内容的长度
} ezDevSDK_DoorLock_Msg2Dev_S;

/**
 *  \brief SDK获取设备信息结构体
 */
typedef struct
{
	int     iType;      ///< 需获取的设备信息类型, 参见 EZDEVSDK_DOORLOCK_RUNTIME_INFO_TYPE_E
	void*   pBuf;       ///< 信息内容结构体
	UINT32  uiSize;     ///< 信息内容长度
} ezDevSDK_DoorLock_RuntimeInfo_S;

/**
 *  @brief SDK回调给应用层接口, SDK运行时获取信息回调，不能有任何阻塞操作
 */
typedef struct
{
	int (*recv_msg)(ezDevSDK_DoorLock_Msg2Dev_S *pstMsg);               ///< 消息回调接口
	int (*get_runtime_info)(ezDevSDK_DoorLock_RuntimeInfo_S *pstInfo);	///< 消息回调获取上层状态
} ezDevSDK_DoorLock_Callback_S;

/**
 * @brief 智能门锁领域初始化的参数
 */
typedef struct
{
    ezDevSDK_DoorLock_Callback_S stCb;       ///< 回调函数

    int  iRstRepAllFlag;                     ///< 注销模块时，是否上报所有状态的标志
} ezDevSDK_DoorLock_InitParam_S;


//-------------------------------------SDK通知应用层的消息-----------------------------------------

/**
 * \brief SDK通知应用层的消息类型
 */
typedef enum
{
    EZDEVSDK_DOORLOCK_ON_SET_SWITCH_ENABLE = 0,    ///< 设置开关，结构体 ezDevSDK_DoorLock_CommSwitch_S
    EZDEVSDK_DOORLOCK_ON_SET_FINGER_STORE,         ///< 设置指纹仓，缓存为无符号整形(unsigned int)，取值 ezDevSDK_DoorLock_FingerStore_Type_E
    EZDEVSDK_DOORLOCK_ON_SET_DOOR_BELL_TONE,       ///< 设置门铃音量，缓存为无符号整形(unsigned int)，取值 ezDevSDK_DoorLock_DoorTone_Type_E
    EZDEVSDK_DOORLOCK_ON_SET_DOOR_PROMPT_TONE,     ///< 设置提示音，缓存为无符号整形(unsigned int)，取值 ezDevSDK_DoorLock_DoorTone_Type_E
    EZDEVSDK_DOORLOCK_ON_SET_USER_INFO,            ///< 设置用户配置，结构体 ezDevSDK_DoorLock_SetUserCfg_S
    EZDEVSDK_DOORLOCK_ON_SET_NOT_BACK_ALARM_PLAN,  ///< 设置未回家提醒计划，结构体 ezDevSDK_DoorLock_SetAlarmPlan_S
    EZDEVSDK_DOORLOCK_ON_SET_SNAP_PASSWORD,        ///< 设置临时密码，结构体 ezDevSDK_DoorLock_SetSnapPwd_S
    EZDEVSDK_DOORLOCK_ON_SET_DOOR_DLPOWER,         ///< 设置电池阈值，结构体 ezDevSDK_DoorLock_DLPower_S
    EZDEVSDK_DOORLOCK_ON_SET_TRIGGER_USER_VERIFY,  ///< 触发用户身份认证，结构体 ezDevSDK_DoorLock_TriggerUserVerify_S
    EZDEVSDK_DOORLOCK_ON_SET_FaceUnlock,           ///< 设置人脸解锁， 结构体 ezDevSDK_DoorLock_FaceUnlock_S 
    EZDEVSDK_DOORLOCK_ON_SET_DOORLOCK_PLAN,        ///< 设置计划，结构体 ezDevSDK_DoorLock_Plan_S

	EZDEVSDK_DOORLOCK_MSG_TO_DEV_TYPE_MAX,

} EZDEVSDK_DOORLOCK_MSG_TO_DEV_TYPE_E;



#define EZDEVSDK_DOORLOCK_USERS_MAX_NUM            (500)  ///< 智能门锁配置里，用户最大条数
#define EZDEVSDK_DOORLOCK_USERS_DATE_MAX_NUM       (4)    ///< 单个用户配置内，时间计划的最大条数

#define EZDEVSDK_DOORLOCK_AlARM_PLAN_DATE_MAX_NUM  (4)    ///< 未回家提醒计划，单个计划内，时间段配置的最大条数
#define EZDEVSDK_DOORLOCK_ALARM_PLAN_MAX_NUM       (8)    ///< 未回家提醒计划，最大计划条数

#define EZDEVSDK_DOORLOCK_SNAP_PWD_MAX_NUM         (3)    ///< 临时密码，最大条数

#define EZDEVSDK_DOORLOCK_UNLOCK_EVT_REC_MAX_NUM   (100)  ///< 开门记录，最大条数

#define EZDEVSDK_DOORLOCK_POWER_MAX_NUM            (2)    ///< 电池最大数量

#define EZDEVSDK_DOORLOCK_USER_NAME_MAX            (64)   ///< 用户名的最大尺寸，包括结束字符'\0'
#define EZDEVSDK_DOORLOCK_TEMP_PWD_MAX             (16)   ///< 临时密码的最大尺寸，包括结束字符'\0'

#define EZDEVSDK_DOORLOCK_VALUE_FOREVER            (-1)   ///< 表示永久



/* 开关类型 */
typedef enum
{
    ezDevSDK_DoorLock_Switch_DoorOpenRemind = 0,  ///< 开门提醒开关
    ezDevSDK_DoorLock_Switch_DoorShockRemind = 1,  ///< 震动提醒开关
    ezDevSDK_DoorLock_Switch_ParyDoorRemind = 2,  ///< 撬门提醒开关
    ezDevSDK_DoorLock_Switch_AntiDoorLockMode = 3, ///<反锁模式开关
	ezDevSDK_DoorLock_Switch_DoorOpenVideo = 4,    ///<开门录像开关
	ezDevSDK_DoorLock_Switch_FaceRecognitionUnlock = 5, ///<人脸识别解锁开关
    ezDevSDK_DoorLock_Switch_IndoorOpenRemind = 6, ///<室内开门告警开关
    ezDevSDK_DoorLock_Switch_DoorAutoWakeup = 7,   ///<按门铃屏幕自动唤醒开关
    ezDevSDK_DoorLock_Switch_Max,
} ezDevSDK_DoorLock_SwitchType_E;



/* 开关通用结构体 */
typedef struct
{
    int  iSwType; ///< 入参参数，参见 ezDevSDK_DoorLock_SwitchType_E
    int  iEnable; ///< 获取时是出参
    int  iChannel;///< 获取时是出参

} ezDevSDK_DoorLock_CommSwitch_S;


/* 指纹仓键值内容 */
typedef enum
{
    ezDevSDK_DoorLock_FingerStore_ForceWork = 0,  ///< 常开
    ezDevSDK_DoorLock_FingerStore_Auto,           ///< 自动
    ezDevSDK_DoorLock_FingerStore_ForceClose,     ///< 常闭

    ezDevSDK_DoorLock_FingerStore_Max,

} ezDevSDK_DoorLock_FingerStore_Type_E;



/* 音量键值内容，如门铃音量、提示音音量 */
typedef enum
{
    ezDevSDK_DoorLock_DoorTone_Slient = 0,       ///< 静音
    ezDevSDK_DoorLock_DoorTone_Low,              ///< 低音
    ezDevSDK_DoorLock_DoorTone_Middle,           ///< 中音
    ezDevSDK_DoorLock_DoorTone_loud,             ///< 高音
    ezDevSDK_DoorLock_DoorTone_Auto,             ///< 自动

    ezDevSDK_DoorLock_DoorTone_Max,

} ezDevSDK_DoorLock_DoorTone_Type_E;



/* 设置操作的类型 */
typedef enum
{
    ezDevSDK_DoorLock_SetOpt_Add = 0,  ///< 新增
    ezDevSDK_DoorLock_SetOpt_Modify,   ///< 修改
    ezDevSDK_DoorLock_SetOpt_Delete,   ///< 删除

    ezDevSDK_DoorLock_SetOpt_Max,

} ezDevSDK_DoorLock_SetOpt_Type_E;


/* 用户类型 */
typedef enum
{
    ezDevSDK_DoorLock_User_Primary = 0,  ///< 主用户
    ezDevSDK_DoorLock_User_General,      ///< 普通用户

    ezDevSDK_DoorLock_User_Type_Max,

} ezDevSDK_DoorLock_User_Type_E;



/* 用户认证方式，设置操作的类型 */
typedef enum
{
    ezDevSDK_DoorLock_UserAuthOpt_NoOperation = -2, ///< 无操作
    ezDevSDK_DoorLock_UserAuthOpt_BulkDelAll,       ///< 删除全部
    ezDevSDK_DoorLock_UserAuthOpt_BulkDelCards,     ///< 删除全部卡
    ezDevSDK_DoorLock_UserAuthOpt_BulkDelPwds,      ///< 删除全部密码
    ezDevSDK_DoorLock_UserAuthOpt_BulkDelFingers,   ///< 删除全部指纹
    ezDevSDK_DoorLock_UserAuthOpt_BulkDelFaces,     ///< 删除全部人脸
    ezDevSDK_DoorLock_UserAuthOpt_UnitOpt,          ///< 对单个认证方式的操作

    ezDevSDK_DoorLock_UserAuthOpt_Max,

} ezDevSDK_DoorLock_UserAuthOptType_E;


/* 用户认证方式，类型 */
typedef enum
{
    ezDevSDK_DoorLock_UserAuth_Card_M1 = 0,  ///< M1卡
    ezDevSDK_DoorLock_UserAuth_Card_CPU,     ///< CPU卡
    ezDevSDK_DoorLock_UserAuth_Card_Country, ///< 国密卡
    ezDevSDK_DoorLock_UserAuth_Password,     ///< 密码
    ezDevSDK_DoorLock_UserAuth_Finger,       ///< 指纹
    ezDevSDK_DoorLock_UserAuth_Face,         ///< 人脸

    ezDevSDK_DoorLock_UserAuth_Max,

} ezDevSDK_DoorLock_UserAuthType_E;



/* 用户配置，日期参数 */
typedef struct
{
    int     iBeginHour;  ///< 起始时间(时)
    int     iBeginMin;   ///< 起始时间(分)
    int     iEndHour;    ///< 结束时间(时)
    int     iEndMin;     ///< 结束时间(分)

    UINT8   ucWeekDay;   ///< 一周内的有效日，比特位置位，[6:1]表示[周六:周一]，[0]表示周日
    UINT8   reserved[3];

    int     iNum;        ///< 有效总数，小于0-永久有效
    int     iAvailable;  ///< 剩余有效次数，小于0-永久有效

} ezDevSDK_DoorLock_UserDate_S;


/* 设置用户配置，用户参数 */
typedef struct
{
    UINT16  usIndex;         ///< 用户索引号，从0开始；一般与本地记录索引usLocalIndex相同
    UINT8   ucUserType;      ///< 用户类型，参见 ezDevSDK_DoorLock_User_Type_E
    UINT8   ucEnable;        ///< 有效期使能标志，0-永久，1-非永久
    char    acUserName[EZDEVSDK_DOORLOCK_USER_NAME_MAX];   ///< 用户名
    char    acRemarkName[EZDEVSDK_DOORLOCK_USER_NAME_MAX]; ///< 备注名
    
    time_t  lValidBeginTime; ///< 有效期开始时间
    time_t  lValidEndTime;   ///< 有效期结束时间

    UINT16  usDateListNum;   ///< 有效期的日期参数列表，元素数量
    UINT8   reserved_0[2];
    ezDevSDK_DoorLock_UserDate_S  astDateList[EZDEVSDK_DOORLOCK_USERS_DATE_MAX_NUM];  ///< 日期参数列表

    UINT16  usPwdNum;        ///< 密码开锁，0-不授权，大于1-具体授权的密码个数
    UINT16  usCardNum;       ///< 感应卡开锁，0-不授权，大于1-具体授权的感应卡个数
    UINT16  usFingerNum;     ///< 指纹开锁，0-不授权，大于1-具体授权的指纹个数
    UINT16  usFaceNum;       ///< 人脸开锁，0-不授权，大于1-具体授权的人脸个数
} ezDevSDK_DoorLock_SetUserCfg_UserPara_S;


/* 设置用户参数，用户认证参数 */
typedef struct
{
    int    iOptType;     ///< 操作类型，参见 ezDevSDK_DoorLock_UserAuthOptType_E

    /* 以下参数，只有当"iOptType == ezDevSDK_DoorLock_UserAuthOpt_UnitOpt"时，才有意义，表示对"单个用户认证方式"的 删除/增加/修改 等操作 */

    UINT8  ucUnitAction;  ///< 对单个用户认证方式的操作类型，0 - 删除，1 - 添加，2 - 修改
    UINT8  ucAuthType;    ///< 用户认证方式的类型，参见 ezDevSDK_DoorLock_UserAuthType_E
    UINT16 usUnitIndex;   ///< 用户认证方式的索引，从0开始
    char   acDescBuf[64]; ///< 用户认证方式的描述

} ezDevSDK_DoorLock_SetUserCfg_AuthPara_S;



/* 设置用户配置 */
typedef struct
{
    UINT8    ucOptType;          ///< 操作类型(新增/修改/删除)，参见 ezDevSDK_DoorLock_SetOpt_Type_E
    UINT8    reserved_0[1];
    UINT16   usLocalIndex;       ///< 本地记录索引；注意，如果是"新增"操作，需要回填索引值(出参)

    ezDevSDK_DoorLock_SetUserCfg_UserPara_S  stUserPara;  ///< 用户配置的参数
    ezDevSDK_DoorLock_SetUserCfg_AuthPara_S  stAuthPara;  ///< 用户认证方式的参数

} ezDevSDK_DoorLock_SetUserCfg_S;



/* 未回家提醒计划，日期参数 */
typedef struct
{
    int iBeginHour;   ///< 起始时间(时)
    int iBeginMin;    ///< 起始时间(分)
    int iEndHour;     ///< 结束时间(时)
    int iEndMin;      ///< 结束时间(分)

} ezDevSDK_DoorLock_AlarmPlan_Date_S;


/* 未回家提醒计划，用户参数 */
typedef struct
{
    UINT16  usIndex;     ///< 用户索引号，从0开始
    UINT8   reserved[2];

    char    acUserName[EZDEVSDK_DOORLOCK_USER_NAME_MAX];  ///< 用户名

} ezDevSDK_DoorLock_AlarmPlan_User_S;



/* 设置未回家提醒计划，计划参数 */
typedef struct
{
    UINT8   ucEnable;       ///< 计划使能，0-关闭，1-开启
    UINT8   ucWeekDay;      ///< 一周内的有效日，比特位置位，[6:1]表示[周六:周一]，[0]表示周日

    UINT16  usDataListNum;  ///< 日期参数列表，元素数量
    UINT16  usUserListNum;  ///< 用户参数列表，元素数量

    ezDevSDK_DoorLock_AlarmPlan_Date_S  astDateList[EZDEVSDK_DOORLOCK_AlARM_PLAN_DATE_MAX_NUM];  ///< 日期参数列表
    ezDevSDK_DoorLock_AlarmPlan_User_S  astUserList[EZDEVSDK_DOORLOCK_USERS_MAX_NUM];            ///< 用户参数列表

} ezDevSDK_DoorLock_SetAlarmPlan_Para_S;


/* 设置未回家提醒计划 */
typedef struct
{
    UINT8    ucOptType;     ///< 操作类型(新增/修改/删除)，参见 ezDevSDK_DoorLock_SetOpt_Type_E
    UINT8    reserved_0[1];
    UINT16   usLocalIndex;  ///< 本地记录索引；注意，如果是"新增"操作，需要回填索引值(出参)

    ezDevSDK_DoorLock_SetAlarmPlan_Para_S  stPlan;  ///< 设置计划的参数

} ezDevSDK_DoorLock_SetAlarmPlan_S;


/* 设置临时密码，参数*/
typedef struct
{
    char     acUserName[EZDEVSDK_DOORLOCK_USER_NAME_MAX];    ///< 临时密码对应用户名

    time_t   lBeginTime;  ///< 临时密码使用开始时间
    time_t   lEndTime;    ///< 临时密码使用结束时间

    int      iNum;        ///< 有效总数，小于0-永久有效
    int      iAvailable;  ///< 剩余有效次数，小于0-永久有效

} ezDevSDK_DoorLock_SetSnapPwd_Para_S;



/* 设置临时密码 */
typedef struct
{
    UINT8    ucOptType;     ///< 操作类型(新增/删除)，参见 ezDevSDK_DoorLock_SetOpt_Type_E
    UINT8    reserved_0[1];
    UINT16   usLocalIndex;  ///< 本地记录索引；注意，如果是"新增"操作，需要回填索引值(出参)

    ezDevSDK_DoorLock_SetSnapPwd_Para_S  stSnapPwd;     ///< 设置临时密码的参数

    char     acTempPwd[EZDEVSDK_DOORLOCK_TEMP_PWD_MAX]; ///< 临时密码值，注意，如果是"新增"操作，需要回填(出参)，通知平台产生的临时密码值

} ezDevSDK_DoorLock_SetSnapPwd_S;



/* 门锁电池阈值 */
typedef struct
{
    UINT8  ucLowPercent;  ///< 低电量百分比，0-100
    UINT8  reserved[3];

} ezDevSDK_DoorLock_DLPower_S;


/* 触发用户身份认证的结果 */
typedef enum
{
    ezDevSDK_DoorLock_TriggerUserVerify_Rslt_Success = 0,   ///< 成功
    ezDevSDK_DoorLock_TriggerUserVerify_Rslt_By_Other_User, ///< 已经被其它用户触发身份认证，隔断时间再试
    ezDevSDK_DoorLock_TriggerUserVerify_Rslt_Failed,        ///< 未知的失败

    ezDevSDK_DoorLock_TriggerUserVerify_Rslt_Max,

} ezDevSDK_DoorLock_TriggerUserVerify_Result_E;


/* 触发用户身份认证，目前只有"智能门锁产品"使用到该信令 */
typedef struct
{
    char     acUserId[32];   ///< user id
    UINT32   uiTimeout;      ///< 身份认证的超时时间，默认为120秒
    int      iResult;        ///< 回填处理结果(出参)，参见 ezDevSDK_DoorLock_TriggerUserVerify_Result_E

} ezDevSDK_DoorLock_TriggerUserVerify_S;


typedef struct
{
    int iType;            ///< 解锁方式 0-自动 1-手动
    int iSensitivity;     ///< 灵敏度    0-高  1-低 
}ezDevSDK_DoorLock_FaceUnlock_S;

typedef enum 
{
    EZDEVSDK_DOORLOCK_AUTO_FACE_UNLOCK_PLAN = 0x0,   ///< 自动人脸解锁计划
    EZDEVSDK_DOORLOCK_PLAN_MAX,
}EZDEVSDK_DOORLOCK_PLAN_TYPE_E;
/**
 *\brief 自动人脸解锁计划时间段
 */
typedef struct
{
    time_t begin;
    time_t end;
}ezDevSDK_DoorLock_AutoFaceUnlock_Time_Segment;

/**
 *\brief 计划
 */
typedef struct{
    int type;         ///< 计划类型 EZDEVSDK_DOORLOCK_PLAN_TYPE_E
    int channel;      ///< 通道号
    int enable_plan;  ///< 开关状态
    struct
    {
        ezDevSDK_DoorLock_AutoFaceUnlock_Time_Segment time[7][4];
    }doorlock_auto_face_unlock_plan;
}ezDevSDK_DoorLock_Plan_S;

//-------------------------------------SDK从应用层获取的消息---------------------------------------

/**
 * \brief   智能门锁领域从设备获取信息
 */
typedef enum
{
    EZDEVSDK_DOORLOCK_ON_GET_SWITCH_ENABLE = 0,    ///< 获取开关状态，结构体 ezDevSDK_DoorLock_CommSwitch_S
    EZDEVSDK_DOORLOCK_ON_GET_FINGER_STORE,         ///< 获取指纹仓信息，缓存为无符号整形(unsigned int)，取值 ezDevSDK_DoorLock_FingerStore_Type_E
    EZDEVSDK_DOORLOCK_ON_GET_DOOR_BELL_TONE,       ///< 获取门铃音量信息，缓存为无符号整形(unsigned int)，取值 ezDevSDK_DoorLock_DoorTone_Type_E
    EZDEVSDK_DOORLOCK_ON_GET_DOOR_PROMPT_TONE,     ///< 获取提示音信息，缓存为无符号整形(unsigned int)，取值 ezDevSDK_DoorLock_DoorTone_Type_E
    EZDEVSDK_DOORLOCK_ON_GET_LOCK_STATUS,          ///< 获取门锁的状态，结构体 ezDevSDK_DoorLock_LockStatus_S
    EZDEVSDK_DOORLOCK_ON_GET_UNLOCK_EVT_RECORDS,   ///< 获取开门记录，结构体 ezDevSDK_DoorLock_UnlockEvt_Record_S
    EZDEVSDK_DOORLOCK_ON_GET_USER_INFO,            ///< 获取用户配置，结构体 ezDevSDK_DoorLock_UserInfo_S
    EZDEVSDK_DOORLOCK_ON_GET_NOT_BACK_ALARM_PLANS, ///< 获取未回家提醒计划，结构体 ezDevSDK_DoorLock_AlarmPlans_S
    EZDEVSDK_DOORLOCK_ON_GET_SNAP_PASSWORD_INFO,   ///< 获取临时密码，结构体 ezDevSDK_DoorLock_SnapPwdInfo_S
    EZDEVSDK_DOORLOCK_ON_GET_MUL_POWER_STATUS,     ///< 获取电池状态(复数)，结构体 ezDevSDK_DoorLock_PowerStatus_S
    EZDEVSDK_DOORLOCK_ON_GET_Face_Unlock,          ///< 获取人脸解锁，结构体，ezDevSDK_DoorLock_FaceUnlock_S
    EZDEVSDK_DOORLOCK_ON_GET_DOORLOCK_PLAN,        ///< 获取计划, 结构体 ezDevSDK_DoorLock_Plan_S
	EZDEVSDK_DOORLOCK_RUNTIME_INFO_TYPE_MAX,

} EZDEVSDK_DOORLOCK_RUNTIME_INFO_TYPE_E;


/**
 * \brief   门的状态
 */
typedef enum
{
    ezDevSDK_DoorLock_DoorStatus_Close = 0, ///<  门关着
    ezDevSDK_DoorLock_DoorStatus_Open,      ///<  门开着

    ezDevSDK_DoorLock_DoorStatus_Max,

} ezDevSDK_DoorLock_DoorStatus_E;


/**
 * \brief   锁的状态
 */
typedef enum
{
    ezDevSDK_DoorLock_LockStatus_Normal = 0, ///<  正常
    ezDevSDK_DoorLock_LockStatus_Lock,       ///<  反锁
    ezDevSDK_DoorLock_LockStatus_KeepUnlock, ///<  常开

    ezDevSDK_DoorLock_LockStatus_Max,

} ezDevSDK_DoorLock_LockStatus_E;



/**
 * \brief   智能门锁的状态
 */
typedef struct
{
    UINT8  ucDoorStatus;     ///< 门的状态，参见 ezDevSDK_DoorLock_DoorStatus_E
    UINT8  ucLockStatus;     ///< 锁的状态，参见 ezDevSDK_DoorLock_LockStatus_E
    UINT8  ucSingalPercent;  ///< 门锁和网关间的无线信号，百分比表示: [0,100]
    UINT8  reserved[1];

} ezDevSDK_DoorLock_LockStatus_S;


/**
* \brief 开锁方式
*/
typedef enum
{
    ezDevSDK_DoorLock_Unlock_Finger = 0,  ///< 指纹
    ezDevSDK_DoorLock_Unlock_Password,    ///< 密码
    ezDevSDK_DoorLock_Unlock_Card,        ///< 卡
    ezDevSDK_DoorLock_Unlock_TempPassword,  ///<临时密码
    ezDevSDK_DoorLock_Unlock_Face,        ///< 人脸
    ezDevSDK_DoorLock_Unlock_IndoorOpen,   ///<室内开门

    ezDevSDK_DoorLock_Unlock_Max,

} ezDevSDK_DoorLock_Unlock_Type_E;


/**
* \brief 单个开门事件的记录信息
*/
typedef struct
{
    char    acName[EZDEVSDK_DOORLOCK_USER_NAME_MAX];   ///< 用户名

    UINT8   ucUnlockType; ///< 开锁方式，参见 ezDevSDK_DoorLock_Unlock_Type_E
    UINT8   reserved[3];

    time_t  lTime;        ///< 开锁时间

} ezDevSDK_DoorLock_UnlockEvt_S;



/**
* \brief 开门记录
*/
typedef struct
{
    UINT16   usRecNum;    ///< 开门事件记录列表，元素数量
    UINT8    reserved[2];

    ezDevSDK_DoorLock_UnlockEvt_S astRec[EZDEVSDK_DOORLOCK_UNLOCK_EVT_REC_MAX_NUM];  ///< 开门事件记录列表

} ezDevSDK_DoorLock_UnlockEvt_Record_S;



/* 单个用户配置，参数 */
typedef struct
{
    UINT16  usIndex;         ///< 用户索引号，从0开始
    UINT8   ucUserType;      ///< 用户类型，参见 ezDevSDK_DoorLock_User_Type_E
    UINT8   ucEnable;        ///< 有效期使能标志，0-永久，1-非永久
    char    acUserName[EZDEVSDK_DOORLOCK_USER_NAME_MAX];   ///< 用户名
    char    acRemarkName[EZDEVSDK_DOORLOCK_USER_NAME_MAX]; ///< 备注名
    
    time_t  lValidBeginTime; ///< 有效期开始时间
    time_t  lValidEndTime;   ///< 有效期结束时间

    UINT16  usDateListNum;   ///< 有效期的日期参数列表，元素数量
    UINT8   reserved_0[2];
    ezDevSDK_DoorLock_UserDate_S  astDateList[EZDEVSDK_DOORLOCK_USERS_DATE_MAX_NUM];  ///< 日期参数列表

    UINT16  usPwdNum;        ///< 密码开锁，0-不授权，大于1-具体授权的密码个数
    UINT16  usCardNum;       ///< 感应卡开锁，0-不授权，大于1-具体授权的感应卡个数
    UINT16  usFingerNum;     ///< 指纹开锁，0-不授权，大于1-具体授权的指纹个数
    UINT16  usFaceNum;       ///< 人脸开锁，0-不授权，大于1-具体授权的人脸个数
} ezDevSDK_DoorLock_UserInfo_Unit_S;


/* 用户配置，信息 */
typedef struct
{
    UINT16  usNum;        ///< 用户列表，元素数量
    UINT8   reserved[2];

    ezDevSDK_DoorLock_UserInfo_Unit_S  astUserList[EZDEVSDK_DOORLOCK_USERS_MAX_NUM];  ///< 用户列表
     
} ezDevSDK_DoorLock_UserInfo_S;


/* 单个未回家提醒计划，参数 */
typedef struct
{
    UINT16  usIndex;        ///< 计划索引号，从0开始
    UINT8   ucEnable;       ///< 计划使能，0-关闭，1-开启
    UINT8   ucWeekDay;      ///< 一周内的有效日，比特位置位，[6:1]表示[周六:周一]，[0]表示周日

    UINT16  usDataListNum;  ///< 日期参数列表，元素数量
    UINT16  usUserListNum;  ///< 用户参数列表，元素数量

    ezDevSDK_DoorLock_AlarmPlan_Date_S  astDateList[EZDEVSDK_DOORLOCK_AlARM_PLAN_DATE_MAX_NUM];  ///< 日期参数列表
    ezDevSDK_DoorLock_AlarmPlan_User_S  astUserList[EZDEVSDK_DOORLOCK_USERS_MAX_NUM];            ///< 用户参数列表

} ezDevSDK_DoorLock_AlarmPlan_Unit_S;


/* 未回家提醒计划，信息 */
typedef struct
{
    UINT16   usPlanListNum;   ///< 未回家提醒计划列表，元素数量
    UINT8    reserved[2];
    
    ezDevSDK_DoorLock_AlarmPlan_Unit_S  astPlanList[EZDEVSDK_DOORLOCK_ALARM_PLAN_MAX_NUM];  ///< 未回家提醒计划列表

} ezDevSDK_DoorLock_AlarmPlans_S;


/* 单个临时密码，参数 */
typedef struct
{
    UINT16   usIndex;        ///< 临时密码索引号，从0开始
    UINT8    reserved[2];
    
    char     acUserName[EZDEVSDK_DOORLOCK_USER_NAME_MAX];    ///< 临时密码对应用户名

    time_t   lBeginTime;  ///< 临时密码使用开始时间
    time_t   lEndTime;    ///< 临时密码使用结束时间

    int      iNum;        ///< 有效总数，小于0-永久有效
    int      iAvailable;  ///< 剩余有效次数，小于0-永久有效
     
} ezDevSDK_DoorLock_SnapPwdInfo_Unit_S;



/* 临时密码，信息 */
typedef struct
{
    UINT16  usNum;        ///< 临时密码列表，元素数量
    UINT8   reserved[2];

    ezDevSDK_DoorLock_SnapPwdInfo_Unit_S  astSnapPwdList[EZDEVSDK_DOORLOCK_SNAP_PWD_MAX_NUM];  ///< 临时密码列表
     
} ezDevSDK_DoorLock_SnapPwdInfo_S;


/* 电源类型 */
typedef enum
{
    ezDevSDK_DoorLock_Power_Main = 0,  // 主电源
    ezDevSDK_DoorLock_Power_Alter,     // 备用电源

    ezDevSDK_DoorLock_PowerType_Max,

} ezDevSDK_DoorLock_PowerType_E;


/* 电池宿主类型 */
typedef enum
{
    ezDevSDK_DoorLock_BatteryHost_DoorLock = 0,  // 门锁的电池
    ezDevSDK_DoorLock_BatteryHost_CatEye,        // 猫眼的电池/视频模块的电池
    ezDevSDK_DoorLock_BatteryHost_VideoLock,     //门锁和猫眼共用一块电池

    ezDevSDK_DoorLock_BatteryHost_Type_Max,

} ezDevSDK_DoorLock_BatteryHost_Type_E;


/* 电池状态 */
typedef enum
{
    ezDevSDK_DoorLock_BatteryStatus_Default_Normal = 0,// 电池默认正常运行状态，非充电
    ezDevSDK_DoorLock_BatteryStatus_IsCharging,        // 电池正在充电
    ezDevSDK_DoorLock_BatteryStatus_IsFull,            // 电池已充满电量
    ezDevSDK_DoorLock_BatteryStatus_NoBattery,         // 没有电池
    ezDevSDK_DoorLock_BatteryStatus_IsDamaged,         // 电池已损坏

    ezDevSDK_DoorLock_BatteryStatus_Max,

} ezDevSDK_DoorLock_BatteryStatus_E;


/* 电池状态:单元 */
typedef struct
{
    UINT8  ucType;           // 电池类型，参见 ezDevSDK_DoorLock_PowerType_E
    UINT8  ucRemain;         // 剩余电量百分比，0-100
    UINT8  ucBatteryHost;    // 电池宿主类型，用来区分"门锁的电池/猫眼的电池"，参见 ezDevSDK_DoorLock_BatteryHost_Type_E
    UINT8  ucBatteryStat;    // 电池状态，参见 ezDevSDK_DoorLock_BatteryStatus_E

    UINT16 usCurrVolt;       // 当前电压，暂不使用
    UINT16 usStandVolt;      // 标准电压，暂不使用

    char   acSubSerail[16];  // 子通道的短序列号，设备自身状态时与DevSerial相同
    char   acDesc[64];       // 当前电源的描述信息

} ezDevSDK_DoorLock_PowerUnit_S;



/* 电池状态 信息 */
typedef struct
{
    UINT16    usNum;        ///< 电池列表，元素数量
    UINT8     reserved[2];

    ezDevSDK_DoorLock_PowerUnit_S  astPower[EZDEVSDK_DOORLOCK_POWER_MAX_NUM];   ///< 电池列表
} ezDevSDK_DoorLock_PowerStatus_S;



//-------------------------------------应用层发给SDK（或服务器）的消息------------------------------

/**
 * \brief SDK定义的设备发给服务器的信令集合
 */
typedef enum
{
    ///< 预留

    EZDEVSDK_DOORLOCK_MSG2SRV_TYPE_MAX,

} EZDEVSDK_DOORLOCK_MSG2SRV_TYPE_E;


/**
* \brief 智能门锁领域，设备通过SDK发送消息给服务器的通用结构体
*/
typedef struct
{
	int     iType;		    ///< 消息类型，参见 EZDEVSDK_DOORLOCK_MSG2SRV_TYPE_E
	void*   pBuf;	        ///< 指向消息内容的指针
    UINT32  uiSize;         ///< 消息内容长度
} ezDevSDK_DoorLock_msg2Srv_S;



#endif



