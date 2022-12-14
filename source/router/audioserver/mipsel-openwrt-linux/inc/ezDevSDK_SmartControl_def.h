/**\filename  ezDevSDK_SmartControl_def.h
*  \filepath  ezDevSDK-V2-Release\ezDevSDK_v2.0.2\userpace\inc\ezDevSDK_SmartControl_def.h
*  \copyright HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*  \brief     智能控制领域对外接口
*  \author    liuxiangchen
*  \date      2018/7/5
*/

#ifndef __EZDEVSDK_SMART_CONTROL_DEF_H__
#define __EZDEVSDK_SMART_CONTROL_DEF_H__


#include "typedef.h"


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


//--------------------------------- 智能控制领域 Def -------------------------------------

#define SMART_CONTROL_DO_ACTION_MAX_PARAMS_NUM  16
#define SMART_CONTROL_RULE_ACTION_MAX_PARAMS_NUM  16

#define SmartControl_module_id				7000	
#define SmartControl_module_name			"smart_control"		///<	智能控制功能的模块名字
#define SmartControl_module_version		    "V2.0.0"			///<	智能控制功能的版本
#define SmartControl_cmd_version			"v1.0.0"			///<	指令版本，先这么用


#define SMART_DEVICE_ID_MAX_SIZE     (32)

// 物模型规则结构数据---------
#define SMART_CONTROL_METHOD_TYPE_MAX_LEN           16
#define SMART_CONTROL_SCENE_TYPE_MAX_LEN            16
#define SMART_CONTROL_RESOURCE_ID_MAX_LEN           32
#define SMART_CONTROL_RESOURCE_TYPE_MAX_LEN         32
#define SMART_CONTROL_DOMAIN_MAX_LEN                64
#define SMART_CONTROL_IDENTIFIER_MAX_LEN            64
#define SMART_CONTROL_ONE_DEVID_MAX_LEN             16

typedef enum{
    ezDevSDK_SmartControl_ModelType_Event,
    ezDevSDK_SmartControl_ModelType_Attribute,
    ezDevSDK_SmartControl_ModelType_Option,

    ezDevSDK_SmartControl_ModelType_Max,
}ezDevSDK_SmartControl_ModelType_E;

typedef enum
{
    ezDevSDK_OTAP_SchemaType_Bool,
    ezDevSDK_OTAP_SchemaType_Integer,
    ezDevSDK_OTAP_SchemaType_Number,
    ezDevSDK_OTAP_SchemaType_String,
    ezDevSDK_OTAP_SchemaType_Array,
    ezDevSDK_OTAP_SchemaType_Object,

    ezDevSDK_OTAP_SchemaType_Max,
}ezDevSDK_OTAP_SchemaType_E;


typedef struct{
    UINT32 uiModelType;                                          // 物模型报文类型 ezDevSDK_SmartControl_ModelType_E
    INT8   acMethodType[SMART_CONTROL_METHOD_TYPE_MAX_LEN];      // 方法类型
    INT8   acSceneType[SMART_CONTROL_SCENE_TYPE_MAX_LEN];        // 业务场景       如 iot\service\manual
    INT8   acResourceId[SMART_CONTROL_RESOURCE_ID_MAX_LEN];      // 设备资源ID
    INT8   acResourceType[SMART_CONTROL_RESOURCE_TYPE_MAX_LEN];  // 设备资源类型
    INT8   acDomain[SMART_CONTROL_DOMAIN_MAX_LEN];               // 业务领域
    INT8   acIdentifier[SMART_CONTROL_IDENTIFIER_MAX_LEN];       // 业务标识
    INT8   acSerial[SMART_CONTROL_ONE_DEVID_MAX_LEN];            // 源设备序列号
    INT8   acSubSerial[SMART_CONTROL_ONE_DEVID_MAX_LEN];         // 源子设备序列号,如果为空则无子设备
}ezDevSDK_SmartControl_OtapBaseInfo_S;

typedef struct
{
    UINT8   ucMsgType;          // 数据类型       SMART_CONTROL_SchemaType_E
    UINT8   reverse[3];
    UINT32  uiPayloadLen;
    VOID    *acPayload;
}ezDevSDK_SmartControl_OtapBody_S;

typedef struct{
    ezDevSDK_SmartControl_OtapBaseInfo_S    stBaseInfo;
    ezDevSDK_SmartControl_OtapBody_S        stBody;
}ezDevSDK_SmartControl_OtapMsg_S;

// 智能控制领域 设备事件描述
typedef enum
{
    SMART_CONTROL_DEV_EVENT_OPENDOOR      = 10499,       //开门
    SMART_CONTROL_DEV_EVENT_DOORBELL_RING = 10500,   // 门铃振动
    SMART_CONTROL_DEV_EVENT_PIR           = 10600,   // PIR检测
    SMART_CONTROL_DEV_COREDEV_RST         = 20000,   // doorbell重置，中心设备重置，响铃关系中门铃是中心设备
    SMART_CONTROL_DEV_SUBDEV_RST          = 20001,   // chime重置，执行类设备重置
    SMART_CONTROL_DEV_EVENT_MAX,

} SMART_CONTROL_DEV_EVENT_LIST;

typedef enum
{
    SMART_CONTROL_CMD_REPORT_EVENT        = 0x4E12,
    SMART_CONTROL_CMD_SET_CHIME_ALARM     = 0x4E60,    

} SMART_CONTROL_CMD_LIST;

//----------------------------------错误码--------------------------------------------------

///
/// @brief 萤石SDK错误码
///
typedef enum
{
    EZDEVSDK_SMART_CONTROL_CODE_SUCCESS = 0x0000,       ///< 成功
    EZDEVSDK_SMART_CONTROL_CODE_FAIL,                   ///< 失败
    EZDEVSDK_SMART_CONTROL_CODE_NOT_INITED,             ///< 领域未初始化
    EZDEVSDK_SMART_CONTROL_CODE_INVALID_PARAM,          ///< 非法参数
    EZDEVSDK_SMART_CONTROL_CODE_NOT_SUPPORT,            ///< 命令不支持
} EZDEVSDK_SMART_CONTROL_CODE_E;


//--------------------------------SDK通知应用层消息-----------------------------------------

///
/// @brief   SDK给应用的消息类型
///
typedef enum
{
    EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_CHIME_ALARM_ACTION = 0, //< 平台操作chime响铃，结构体 ezDevSDK_SmartControl_SetChimeAlarmAction
    EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_CONFIG_RULES,           //< 平台下发联动规则，结构体 ezDevSDK_SmartControl_ConfigRules，内存由sdk申请和释放
    EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_CAPTURE_PIC,            //< 平台联动摄像机抓图，结构体 ezDevSDK_SmartControl_device_capture_info
    EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_ICR_DSS,        // 设置(猫眼)日夜切换，结构体 ezDevSDK_SmartControl_msg_IcrDss_S
    EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_SCREEN_OVERTIME,// 设置(猫眼)屏幕超时时间，结构体 unsigned int(单位秒)
    EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_VIDEO_REC_TIME, // 设置(猫眼锁)录像时长，结构体 unsigned int(单位秒)
    EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_SCREEN_AUTO_ROUSE, // 设置(猫眼锁)屏幕自动唤醒，结构体 unsigned int
    EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_SCREEN_LIGHTNESS, // 设置(猫眼锁)屏幕亮度，结构体 unsigned int
	EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_VIDEO,                  //< 平台联动设备录像，结构体 ezDevSDK_SmartControl_Action_Common_S
    EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_PIC_AND_VIDEO,          //< 平台联动设备抓图且录像，结构体 ezDevSDK_SmartControl_Action_Pic_And_Video_S
    EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_SLEEP,                  //< 平台联动设备休眠，结构体 ezDevSDK_SmartControl_Action_Sleep_S
	EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_CONFIG_AIRULES,		  //< 平台下发AI规则 数据类型 ezDevSDK_SmartControl_Rule_S
	EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_CONFIG_SCENE_RULES,	  //<  平台下发场景规则         数据类型      ezDevSDK_SmartControl_Rule_S
	EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_MANUAL_SCENE,           // 平台下发手动场景 ezDevSDK_SmartControl_ManualScene_S
	
    EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_MAX,
} EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_TYPE_E;


typedef enum
{
    EZDEVSDK_SMART_CONTROL_CONFIG_RULE_STATUS_TYPE_UNKNOWN = 0,
    EZDEVSDK_SMART_CONTROL_CONFIG_RULE_STATUS_TYPE_GLOBAL,

    EZDEVSDK_SMART_CONTROL_CONFIG_RULE_STATUS_TYPE_MAX,

} EZDEVSDK_SMART_CONTROL_CONFIG_RULE_STATUS_TYPE_E;


typedef struct
{
    UINT32  uiDomain;
    UINT32  uiCmdId;
    char    acDevId[SMART_DEVICE_ID_MAX_SIZE];
    UINT8   ucChannel;
    UINT16  usValue;

} ezDevSDK_SmartControl_ConfigRule_Events;

typedef struct
{
    UINT32 uiDomain;
    UINT32 uiCmdId;
    char   acDevId[SMART_DEVICE_ID_MAX_SIZE];
    UINT8  ucChannel;
    UINT8  ucValueNum;
    UINT16 ausValue[SMART_CONTROL_RULE_ACTION_MAX_PARAMS_NUM];
    UINT32 uiDelay;

} ezDevSDK_SmartControl_ConfigRule_Actions;


typedef struct
{
    UINT32  uiRuleId;
    char    acUserId[64];
    char    acRuleName[64];
    UINT8   ucEnable;
    UINT8   aucWeekDaysEn[7];
    UINT32  uiStart_hour;
    UINT32  uiStart_min;
    UINT32  uiEnd_hour;
    UINT32  uiEnd_min;
    UINT16  usCondition;
    UINT8   ucIsCrossDay;   // 规则有效时间是否跨天: 0 - 不跨天，1 - 跨天
    UINT8   reserved[3];

    UINT16                           usEvents_Num;
    ezDevSDK_SmartControl_ConfigRule_Events  *pstEvents;

    UINT16                           usActions_Num;
    ezDevSDK_SmartControl_ConfigRule_Actions *pstActions;
} ezDevSDK_SmartControl_ConfigRules_RuleUnit;

typedef struct
{
    UINT16   usType;
    UINT8    ucChannel;

    UINT16                              usNum;
    ezDevSDK_SmartControl_ConfigRules_RuleUnit  *pstRuleUnit;
} ezDevSDK_SmartControl_ConfigRules_ValueUnit;


//EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_CONFIG_RULES
typedef struct
{
    UINT32    uiVersion;
    UINT16    usNum;
    UINT8     reserved0[2];
    ezDevSDK_SmartControl_ConfigRules_ValueUnit  *pstUnit;

} ezDevSDK_SmartControl_ConfigRules;

typedef struct
{
    UINT32  uiVersion;
    char    *rule;  // 目前shadow v3支持最大数据量为16KB,为压缩后的数据
} ezDevSDK_SmartControl_Rule_S;

typedef struct
{
    UINT32  uiSceneId;  // 手动场景ID
} ezDevSDK_SmartControl_ManualScene_S;

// 平台操作chime响铃
typedef struct
{
    char    acDevId[SMART_DEVICE_ID_MAX_SIZE];
    UINT32  uiEventId;
    UINT32  uiChannel;
    UINT32  uiValueNum;
    UINT16  ausValue[SMART_CONTROL_RULE_ACTION_MAX_PARAMS_NUM];   
} ezDevSDK_SmartControl_SetChimeAlarmAction;

// 平台联动设备，执行动作的通用参数
typedef struct
{
    UINT32 channel;         ///< 通道
    UINT32 delay;           ///< 延迟时间
} ezDevSDK_SmartControl_Action_Common_S;


// 平台联动设备抓图,需上传关联报警
typedef struct
{
    UINT32 channel;         ///< 抓图通道
    UINT32 delay;           ///< 延迟时间
    char relation_id[64];   ///< 关联ID
    UINT32 uiValueNum;
    UINT32 auiValue[SMART_CONTROL_DO_ACTION_MAX_PARAMS_NUM]; 
} ezDevSDK_SmartControl_device_capture_info;

// 平台联动设备抓图且录像
typedef struct
{
    ezDevSDK_SmartControl_Action_Common_S  stComm;
    char     relation_id[64];   ///< 关联ID
} ezDevSDK_SmartControl_Action_Pic_And_Video_S;


// 平台联动设备的休眠状态
typedef struct
{
    UINT8   ucEnable;   ///< 使能休眠: 0 - 关闭休眠; 1 - 开启休眠
    UINT8   reserved[3];
} ezDevSDK_SmartControl_Action_Sleep_S;

/* 猫眼日夜切换的模式 */
typedef enum
{
    ezDevSDK_SmartControl_IcrDss_Auto      = 0,  ///< 自动
    ezDevSDK_SmartControl_IcrDss_ForceDay,       ///< 强制白天
    ezDevSDK_SmartControl_IcrDss_ForceNight,     ///< 强制夜晚
    ezDevSDK_SmartControl_IcrDss_autoTime,       ///< 自定义白天时间
    ezDevSDK_SmartControl_IcrDss_Mode_Max,

} ezDevSDK_SmartControl_IcrDss_Mode_E;


/**
 * \brief   日夜切换-自定义白天开始和结束时间
 * \note    只在获取默认配置参数的时候用
 */
typedef struct {
    time_t begin;           ///< 开始时间, 高16位表示小时, 低16位表示分钟
    time_t end;             ///< 结束时间, 如0X0017003B, 即"23:59"
} ezDevSDK_SmartControl_IcrDss_auto_time_S;

/* (猫眼锁)日夜切换，参数 */
typedef struct
{
    UINT8  ucMode;           ///< 日夜切换的模式，参见 ezDevSDK_SmartControl_IcrDss_Mode_E
    UINT8  ucSensitivity;    ///< 日夜切换的灵敏度，0-默认;"自动模式"下，1-低，2-中，3-高
    ezDevSDK_SmartControl_IcrDss_auto_time_S autoTime;
} ezDevSDK_SmartControl_msg_IcrDss_S;



///
/// @brief   SDK给应用的消息结构体
///
typedef struct 
{
    int    type; // EZDEVSDK_SMART_CONTROL_MSG_TO_DEV_TYPE_E
    char  *data;
    int    len;
} ezDevSDK_SmartControl_msg_to_dev;


//--------------------------------SDK从应用层获取的消息----------------------------------------

///
/// @brief   SDK获取设备信息类型
///
typedef enum
{
    EZDEVSDK_SMART_CONTROL_RUNTIME_INFO_GET_CONFIG_RULES = 0, //< 获取联动规则，包括shadow版本号(注意，内存由应用层申请，SDK负责释放内存)，结构体 ezDevSDK_SmartControl_ConfigRules
    EZDEVSDK_SMART_CONTROL_RUNTIME_INFO_GET_ICR_DSS,          // 获取(猫眼)日夜切换配置，结构体 ezDevSDK_SmartControl_msg_IcrDss_S
    EZDEVSDK_SMART_CONTROL_RUNTIME_INFO_GET_SCREEN_OVERTIME,  // 获取(猫眼)屏幕超时时间，结构体 unsigned int(单位秒)
    EZDEVSDK_SMART_CONTROL_RUNTIME_INFO_GET_VIDEO_TIME,       // 获取(猫眼锁)录像时间，结构体 unsigned int(单位秒)

	EZDEVSDK_SMART_CONTROL_RUNTIME_INFO_GET_SCREEN_AUTO_ROUSE,	// 获取(猫眼锁)屏幕自动唤醒开关状态，结构体 unsigned int
	EZDEVSDK_SMART_CONTROL_RUNTIME_INFO_GET_SCREEN_LIGHTNESS,	// 获取(猫眼锁)屏幕亮度数值，结构体 unsigned int

    EZDEVSDK_SMART_CONTROL_RUNTIME_INFO_GET_SCENE_RULES,  // 获取联动场景规则(注意，结构体中的rule内存由应用层申请，SDK负责释放内存)，结构体 ezDevSDK_SmartControl_Rule_S
    EZDEVSDK_SMART_CONTROL_RUNTIME_INFO_GET_AI_RULES, //< 获取AI规则，包括shadow版本号(注意，结构体中的rule内存由应用层申请，SDK负责释放内存)，结构体 ezDevSDK_SmartControl_Rule_S
    EZDEVSDK_SMART_CONTROL_RUNTIME_INFO_GET_AP_SIGNAL_WITH_MAC, // 获取扫描到的热点信息，结构体 ezDevSDK_Smartcontrol_ap_signal_result
    EZDEVSDK_SMART_CONTROL_RUNTIME_INFO_MAX,

} EZDEVSDK_SMART_CONTROL_RUNTIME_INFO_TYPE_E;

///
/// @brief   SDK获取设备信息结构体
///
typedef struct
{
    int   type; // EZDEVSDK_SMART_CONTROL_RUNTIME_INFO_TYPE_E
    char *data;
    int   len;
} ezDevSDK_SmartControl_runtime_info;

typedef enum
{
    ezDevSDK_SmartControl_Ap_DevType_Chime = 0,
    ezDevSDK_SmartControl_Ap_DevType_Route,

    ezDevSDK_SmartControl_Ap_DevType_Max,

} ezDevSDK_SmartControl_Ap_DevType_E;


typedef struct
{
    int iDevType;                  // 参见 ezDevSDK_SmartControl_Ap_DevType_E

    int iSignalStrength;           // 信号强度 0-100

    union
    {
        struct                     // 设备类型为 ezDevSDK_SmartControl_Ap_DevType_Chime
        {
            char  acMac[32];       // 需要查询的AP的mac
            char  acDevSerial[64]; // 此MAC对应的设备序列号
        } stChime;

        struct                     // 设备类型为 ezDevSDK_SmartControl_Ap_DevType_Route
        {
            char  acSSID[64];      // 路由热点的ssid
        } stRoute;

    } unInfo;

} ezDevSDK_Smartcontrol_ap_signal_result;

///
/// @brief
///
typedef struct
{
    int num;                //需要查询的AP个数
    ezDevSDK_Smartcontrol_ap_signal_result *ap_signal_result;
} ezDevSDK_Smartcontrol_ap_signal_info;


//------------------------------------应用层发给服务器的消息------------------------------------


/**
* \brief   应用层给SDK消息，sdk不管理内存，若应用层申请，则应用层释放
*/
typedef enum
{
    EZDEVSDK_SMART_CONTROL_MSG_TO_SERVER_EVENT,           
    EZDEVSDK_SMART_CONTROL_MSG_TO_SERVER_ACTION_RESULT,   
    EZDEVSDK_SMART_CONTROL_MSG_TO_SERVER_UNDONE_ACTIONS,  
    EZDEVSDK_SMART_CONTROL_MSG_TO_SERVER_LAN_CENTOR,  
    EZDEVSDK_SMART_CONTROL_MSG_TO_SERVER_UNDONE_SCENE_V3,  // 未执行场景 内存由应用层申请，sdk释放                 ezDevSDK_SmartControl_UndoneScene_S 
    EZDEVSDK_SMART_CONTROL_MSG_TO_SERVER_UNDONE_ACTIONS_V3,
    EZDEVSDK_SMART_CONTROL_MSG_TO_SERVER_LAN_CENTOR_V3,

	EZDEVSDK_SMART_CONTROL_MSG_TO_SERVER_MAX,
}EZDEVSDK_SMART_CONTROL_MSG_TO_SERVER_TYPE_E;

// 设备上报事件，参见 EZDEVSDK_SMART_CONTROL_MSG_TO_SERVER_EVENT
typedef struct 
{
    UINT32 channel;
    UINT32 value;
} ezDevSDK_SmartControl_Event;

// 设备的执行结果，参见 EZDEVSDK_SMART_CONTROL_MSG_TO_SERVER_ACTION_RESULT
typedef struct
{
    UINT32  uiRuleId;
    char    acDeviceId[SMART_DEVICE_ID_MAX_SIZE];
    UINT32  uiChannelNo;
    UINT32  uiDomain;
    UINT32  uiCmdId;
    UINT8   ucResult;
} ezDevSDK_SmartControl_Info_ActionResult;

// 设备未执行动作，参见 EZDEVSDK_SMART_CONTROL_MSG_TO_SERVER_UNDONE_ACTIONS_V3
typedef struct
{
    UINT16  usNum;
    ezDevSDK_SmartControl_OtapMsg_S pstActions[0];
} ezDevSDK_SmartControl_UndoneActions_V3_S;

// 设备未执行动作，参见 EZDEVSDK_SMART_CONTROL_MSG_TO_SERVER_UNDONE_ACTIONS
typedef struct
{
    UINT32  uiRuleId;

    UINT16  usNum;

    char    reserved0[2];

    char    acExtendVal[128];

    ezDevSDK_SmartControl_ConfigRule_Actions  astUndoneActs[0];

} ezDevSDK_SmartControl_UndoneActions_S;

/*
typedef struct
{
    INT8    acUUID[32];
} ezDevSDK_SmartControl_UndoneSceneNode_S;

typedef struct
{
    UINT16  usNum;  // 未执行场景数量
    ezDevSDK_SmartControl_UndoneSceneNode_S pstSceneList[0];
} ezDevSDK_SmartControl_UndoneSceneList_S;

*/

typedef struct
{
    ezDevSDK_SmartControl_OtapMsg_S stSrcEvent;
    UINT32  uiNum;
    UINT32  uiSceneList[0];    //场景ID列表
} ezDevSDK_SmartControl_UndoneSceneList_S;


// 上报局域网互联互通中心节点，
typedef struct
{
    char acDeviceId[SMART_DEVICE_ID_MAX_SIZE];
    UINT32 uiVersion;
    UINT32 obligate;
}ezDevSDK_SmartControl_LanDevice;

typedef struct 
{
    char acCenterDeviceId[SMART_DEVICE_ID_MAX_SIZE];
    UINT32 uiVersion;
    char acExtend[64];
    UINT32 uiEnable;
    UINT32 uiLanDeviceNum;
    ezDevSDK_SmartControl_LanDevice *pLanDevice;
} ezDevSDK_SmartControl_ReportLanCentor;


/**
* \brief   应用主动推送消息给云端
*/
typedef struct
{
	int    iType;	///<	消息类型，参见EZDEVSDK_SMART_CONTROL_MSG_TO_SERVER_TYPE_E
	void*  pData;	///<	指向消息内容的指针
	UINT32 uiLen;   ///<  数据长度
} ezDevSDK_SmartControl_Msg2Server_S;


//------------------------------------应用层推送的数据埋点------------------------------------

/* 智能家居，埋点消息的类型 */
typedef enum
{
    ezDevSDK_SmartControl_DCLogMsg_EntireDCLog = 0, ///< 上层推送完整的dclog，结构体 ezDevSDK_SmartControl_DCLogParam_S

    ezDevSDK_SmartControl_DCLogMsg_Type_Max,

} ezDevSDK_SmartControl_DCLogMsg_Type_E;



/* 智能家居 -- 局域网互联互通 -- 运行事件埋点 */
typedef struct
{
    char   acSdkVer[16];   // sdk版本号
    char   acDevType[16];  // 设备类型(全功能/精简)描述
    char   acEvent[64];    // 运行事件描述

} ezDevSDK_SmartControl_DCLog_LanLinkRunEvt_S;


/* 智能家居 -- 局域网互联互通 -- 网络参数埋点 */
typedef struct
{
    UINT8  ucNetIf_IsActive;      // 网络接口是否活动标记
    UINT8  ucUserid_IsSwitched;   // userid是否变化标记
    UINT8  ucDas_IsOnline;        // 云端是否在线标记
    UINT8  ucSock_ReInit;         // 网络通信套接字重初始化标记: 0 - 没有重初始化; 1 - 多播套接字重初始化; 2 - 单播套接字重初始化

    char   acIpAddr[64];          // IP地址的字符串描述
    char   acNetMask[64];         // IP子网掩码
    char   acNetIfName[32];       // 网络接口名称

} ezDevSDK_SmartControl_DCLog_LanLinkNetPara_S;


/* 智能家居 -- 局域网互联互通 -- 中心节点设备智能联动统计埋点 */
typedef struct
{
    UINT8  ucHasRule;          // 是否已有规则的标记
    UINT8  reserved[3];

    UINT32 uiRuleRootNum;      // 当前规则的根元素数量

    UINT32 uiSelfEvtCnt;       // 本设备下发的智能事件数量
    UINT32 uiLanEvtCnt;        // 收到来自局域网组网设备的智能事件数量
    UINT32 uiSelfActCnt;       // 中心节点执行的动作数量
    UINT32 uiLanActSuccCnt;    // 成功发送给局域网组网设备的执行动作数量
    UINT32 uiLanActFailCnt;    // 失败发送给局域网组网设备的执行动作数量
    UINT32 uiUndoActCnt;       // 需要通过云端联动的执行动作数量
    UINT32 uiSelfActRetCnt;    // 中心节点的执行动作结果的反馈数量
    UINT32 uiLanActRetCnt;     // 收到来自局域网组网设备的执行动作结果的反馈数量

} ezDevSDK_SmartControl_DCLog_LanLinkCentStatis_S;


/* 智能家居 -- 局域网互联互通 -- 组网设备(非中心节点)智能联动统计埋点 */
typedef struct
{
    UINT32 uiLanEvtSuccCnt;    // 发送给中心节点的智能事件，成功发送的数量
    UINT32 uiLanEvtFailCnt;    // 发送给中心节点的智能事件，失败发送的数量
    UINT32 uiLanActCnt;        // 收到来自中心节点的执行动作的数量
    UINT32 uiLanActRetSuccCnt; // 反馈给中心节点的执行动作结果，成功发送的数量
    UINT32 uiLanActRetFailCnt; // 反馈给中心节点的执行动作结果，失败发送的数量

} ezDevSDK_SmartControl_DCLog_LanLinkMembStatis_S;


/* 智能家居 -- 局域网互联互通 -- 局域网通信统计埋点 */
typedef struct
{
    UINT32 uiReqRetrySuccCnt;  // 支持重试的命令，通信成功的数量
    UINT32 uiReqRetryFailCnt;  // 支持重试的命令，通信失败的数量

} ezDevSDK_SmartControl_DCLog_LanLinkCommStatis_S;


/* 智能家居 -- 局域网互联互通，私有埋点 */
typedef union
{
    ezDevSDK_SmartControl_DCLog_LanLinkRunEvt_S      stRunEvent;   // "device_lan_smart_run"
    ezDevSDK_SmartControl_DCLog_LanLinkNetPara_S     stNetParam;   // "device_lan_smart_network"
    ezDevSDK_SmartControl_DCLog_LanLinkCentStatis_S  stCentStatis; // "device_lan_smart_center_statistics"
    ezDevSDK_SmartControl_DCLog_LanLinkMembStatis_S  stMembStatis; // "device_lan_smart_member_statistics"
    ezDevSDK_SmartControl_DCLog_LanLinkCommStatis_S  stCommStatis; // "device_lan_smart_comm_statistics"

} ezDevSDK_SmartControl_DCLog_LanLink_PrivParam_U;


/* 智能家居，私有埋点 */
typedef union
{
    ezDevSDK_SmartControl_DCLog_LanLink_PrivParam_U  unLanLinkPriv;

} ezDevSDK_SmartControl_DCLog_PrivParam_U;


/* 智能家居，上层推送完整的埋点的结构体 */
typedef struct
{
    char    acSystemName[64];
    UINT32  uiStartTime_sec;
    UINT32  uiStartTime_usec;
    UINT32  uiEndTime_sec;
    UINT32  uiEndTime_usec;
    char    acDevId[32];        /* 设备序列号 */
    UINT32  uiLog_Seq;          /* log序号 */

    ezDevSDK_SmartControl_DCLog_PrivParam_U  unPrivPara;

} ezDevSDK_SmartControl_DCLogParam_S;


/**
* \brief   应用主动推送埋点数据
*/
typedef struct
{
	int    iType;	///<	消息类型，参见 ezDevSDK_SmartControl_DCLogMsg_Type_E
	void*  pData;	///<	指向消息内容的指针
	UINT32 uiLen;   ///<  数据长度
} ezDevSDK_SmartControl_DclogMsg_S;


//----------------------------------初始化信息--------------------------------------------------

///
/// @brief   SDK回调给用户层接口, SDK运行时获取信息回调，不能有任何阻塞操作
///
typedef struct {
    int (*recv_msg)(ezDevSDK_SmartControl_msg_to_dev *msg); ///消息回调接口，不可以阻塞，主动接收消息和回调方式二选一，实现了回调接口，消息就走回调函数
	int (*on_get_runtime_info)(ezDevSDK_SmartControl_runtime_info *info);
} ezDevSDK_SmartControl_callback;


typedef struct {
    ezDevSDK_SmartControl_callback cb;
    int resetReportAll;
} ezDevSDK_SmartControl_init_info;


//------------------------------------应用层配置参数--------------------------------------------


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __EZDEVSDK_SMART_CONTROL_DEF_H__ */

