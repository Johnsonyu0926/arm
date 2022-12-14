/**  @file   EBaseInterface.h
 *   @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved. 
 *   @brief  外部接口
 *   
 *   @author shijianfengyf2
 *   @modify shijianfengyf2
 *   @date   2018/08/09
 *   
 *   @note:
 */

#ifndef _DEVEBASE_SRC_INTERFACE_EBASEINTERFACE_H_
#define _DEVEBASE_SRC_INTERFACE_EBASEINTERFACE_H_

//#include "DevComPublicDef.h"
#include "HPR_Config.h"
#include "HPR_Types.h"

//定义gcc以及g++编译器下，attribute的属性，当前设置为hidden
#ifndef NET_COM_ATTRIBUTE_VISIBILITY
    #define NET_COM_ATTRIBUTE_VISIBILITY //发现后端使用的编译器不支持这个功能，因此先定义成空的，以后有更进一步需求，再使用
    //#ifdef OS_POSIX
    //    #define NET_COM_ATTRIBUTE_VISIBILITY  __attribute__ ((visibility("hidden")))
    //#else  // !OS_POSIX
    //    #define NET_COM_ATTRIBUTE_VISIBILITY
    //#endif // !other os
#endif // NET_COM_ATTRIBUTE_VISIBILITY

#ifndef NET_COM_EXPORT_TYPE
    #ifdef __cplusplus
        #ifdef NETSDK_EXPORTS
            #define NET_COM_EXPORT_TYPE extern "C"__declspec(dllexport)
            #define NET_COM_CLASS_EXPORT_TYPE __declspec(dllexport)
        #else
            #define NET_COM_EXPORT_TYPE extern "C"
            #define NET_COM_CLASS_EXPORT_TYPE
        #endif // !NETSDK_EXPORTS
    #else
        #define NET_COM_EXPORT_TYPE
    #endif // !__cplusplus
#endif // !NET_COM_EXPORT_TYPE


//定义接口以及类导出宏
#ifndef NET_COM_API
    #define NET_COM_API NET_COM_EXPORT_TYPE NET_COM_ATTRIBUTE_VISIBILITY
    #define NET_COM_CLASS NET_COM_CLASS_EXPORT_TYPE NET_COM_ATTRIBUTE_VISIBILITY
#endif  // !NET_COM_API

//定义接口导出顺序宏，现在使用默认的方式
#ifndef CALLBACK
    #define CALLBACK
#endif // !CALLBACK

#if defined(OS_WINDOWS)
    typedef SOCKET HPR_SOCK_T;
#elif defined(OS_POSIX)
    typedef int HPR_SOCK_T;
#else
    #error OS Not Implement Yet.
#endif

/*******************全局错误码 begin**********************/
#define DEV_COM_ERROR_NONE                      0   //没有错误
#define DEV_COM_ERROR_NOINIT                    3   //没有初始化
#define DEV_COM_ERROR_CONNECT		            7	//连接服务器失败
#define DEV_COM_ERROR_SEND			            8	//向服务器发送失败
#define DEV_COM_ERROR_RECV			            9	//从服务器接收数据失败
#define DEV_COM_ERROR_RECV_TIMEOUT		        10	//从服务器接收数据超时
#define DEV_COM_ERROR_DATA                      11  //网络上接收到的数据异常
#define DEV_COM_ERROR_ORDER                     12  //调用次序错误
#define DEV_COM_ERROR_PARAMETER                 17  //参数错误
#define DEV_COM_ERROR_SUPPORT                   23  //不支持该功能
#define DEV_COM_ERROR_FILEOPENFAIL              35  //打开文件出错。可能因为文件不存在或者路径错误
#define DEV_COM_ERROR_OPERNOTFINISH             36  //上次的操作还没有完成
#define DEV_COM_ERROR_ALLOC_RESOURCE            41  //资源分配错误
#define DEV_COM_ERROR_NOENOUGH_BUF              43  //缓冲区太小
#define DEV_COM_ERROR_CREATESOCKET              44  //创建SOCKET出错
#define DEV_COM_ERROR_SETSOCKET                 45  //设置SOCKET出错
#define DEV_COM_ERROR_MAXNUM                    46  //达到最大数量
#define DEV_COM_ERROR_BINDSOCKET    			72	//绑定套接字失败
#define DEV_COM_ERROR_SOCKETCLOSE   			73	//socket连接中断，此错误通常是由于连接中断或目的地不可达

//认证错误码
#define DEV_COM_ERROR_AUTHENTICATION_SUCCESS                     500  //认证成功
#define DEV_COM_ERROR_AUTHENTICATION_FAIL                        501  //认证失败
#define DEV_COM_ERROR_AUTHENTICATION_CMD_ERROR                   502  //非法命令
#define DEV_COM_ERROR_AUTHENTICATION_DATA_INVALID                503  //协议解析失败
#define DEV_COM_ERROR_AUTHENTICATION_DEV_ID_INVALID              504  //DevID非法
#define DEV_COM_ERROR_AUTHENTICATION_QUERY_AUTHCODE_FAIL         505  //平台查询验证码失败
#define DEV_COM_ERROR_AUTHENTICATION_QUERY_REDIS_FAIL            506  //平台查询redis失败
#define DEV_COM_ERROR_AUTHENTICATION_SESSION_KEY_INVALID         507  //SessionKey无效
#define DEV_COM_ERROR_AUTHENTICATION_ENCRYPT_FAIL                508  //协议加密失败
#define DEV_COM_ERROR_AUTHENTICATION_QUERY_DATA_ERROR            509  //查询Das或Stun信息失败
#define DEV_COM_ERROR_AUTHENTICATION_MASTERKEY_INVALID           510  //masterkey非法
#define DEV_COM_ERROR_AUTHENTICATION_SESSION_KEY_INVALID_NOT          //暂不用
#define DEV_COM_ERROR_STUN_INVALID_AUTHENTICATION                512  //查询stun认证失败
#define DEV_COM_ERROR_DAS_INVALID_AUTHENTICATION                 513  //查询das认证失败
#define DEV_COM_ERROR_CODE_ERROR                                 514  //交互流程错误
#define DEV_COM_ERROR_LBS_UPDATE_REDIS_FAIL                      515  //平台更新设备信息失败
#define DEV_COM_ERROR_LBS_MAX_DEV_LINK_ERROR                     516  //平台设备接入数量已达上限

//语音对讲错误码
#define DEV_COM_ERROR_PARSING_FAIL               600  //解析消息失败
#define DEV_COM_ERROR_CHECK_TOKEN                601  //验证token失败

#define DEV_COM_ERROR_HTTP_ERROR                 700  //http错误，收到http响应，但状态不为200，具体状态通过其他方式获取

//萤石微内核错误码
#define DEV_COM_ERROR_EZDEVSDK_INDEX                100000
#define DEV_COM_ERROR_EZDEVSDK_INTERNAL             (DEV_COM_ERROR_EZDEVSDK_INDEX + 1)  //内部错误
#define DEV_COM_ERROR_EZDEVSDK_INVALD_CALL          (DEV_COM_ERROR_EZDEVSDK_INDEX + 2)  //调用流程有误
#define DEV_COM_ERROR_EZDEVSDK_PARAMS_INVALID       (DEV_COM_ERROR_EZDEVSDK_INDEX + 3)  //参数非法
#define DEV_COM_ERROR_EZDEVSDK_BUFFER_TOO_SMALL     (DEV_COM_ERROR_EZDEVSDK_INDEX + 4)  //缓冲区大小不足
#define DEV_COM_ERROR_EZDEVSDK_DATA_LEN_RANGE       (DEV_COM_ERROR_EZDEVSDK_INDEX + 5)  //数据大小超出范围
#define DEV_COM_ERROR_EZDEVSDK_MEMORY               (DEV_COM_ERROR_EZDEVSDK_INDEX + 6)  //内存异常
#define DEV_COM_ERROR_EZDEVSDK_JSON_INVALID         (DEV_COM_ERROR_EZDEVSDK_INDEX + 7)  //非法的json数据
#define DEV_COM_ERROR_EZDEVSDK_JSON_FORMAT          (DEV_COM_ERROR_EZDEVSDK_INDEX + 8)  //json数据有误
#define DEV_COM_ERROR_EZDEVSDK_EXTEND_NO_FIND       (DEV_COM_ERROR_EZDEVSDK_INDEX + 9)  //信令路由找不到对应的注册领域
#define DEV_COM_ERROR_EZDEVSDK_EXTEND_FULL          (DEV_COM_ERROR_EZDEVSDK_INDEX + 10) //扩展注册已满
#define DEV_COM_ERROR_EZDEVSDK_EXTEND_EXISTED       (DEV_COM_ERROR_EZDEVSDK_INDEX + 11) //扩展已经注册
#define DEV_COM_ERROR_EZDEVSDK_QUEUE_FULL           (DEV_COM_ERROR_EZDEVSDK_INDEX + 12) //消息队列已满
#define DEV_COM_ERROR_EZDEVSDK_VALUE_LOAD           (DEV_COM_ERROR_EZDEVSDK_INDEX + 13) //获取设备数据失败
#define DEV_COM_ERROR_EZDEVSDK_VALUE_SAVE           (DEV_COM_ERROR_EZDEVSDK_INDEX + 14) //保存数据至设备失败
#define DEV_COM_ERROR_EZDEVSDK_MSG_STOP_DISTRIBUTE  (DEV_COM_ERROR_EZDEVSDK_INDEX + 15) //设备正在停止,上层消息停止下发
#define DEV_COM_ERROR_EZDEVSDK_MSG_DEFLATE_FAIL     (DEV_COM_ERROR_EZDEVSDK_INDEX + 16) //报文解压失败
#define DEV_COM_ERROR_EZDEVSDK_MSG_INFLATE_FAIL     (DEV_COM_ERROR_EZDEVSDK_INDEX + 17) //报文压缩失败
#define DEV_COM_ERROR_EZDEVSDK_INS_FULL             (DEV_COM_ERROR_EZDEVSDK_INDEX + 18) //客户端数量达到上限

#define DEV_COM_ERROR_EZDEVSDK_NET_INDEX            100300
#define DEV_COM_ERROR_EZDEVSDK_NET_CREATE           (DEV_COM_ERROR_EZDEVSDK_NET_INDEX + 1)  ///< 创建socket失败
#define DEV_COM_ERROR_EZDEVSDK_NET_CONNECT          (DEV_COM_ERROR_EZDEVSDK_NET_INDEX + 2)  ///< 网络连接失败
#define DEV_COM_ERROR_EZDEVSDK_NET_DISCONNECTED     (DEV_COM_ERROR_EZDEVSDK_NET_INDEX + 3)  ///< 网络连接断开
#define DEV_COM_ERROR_EZDEVSDK_NET_TRANSMIT         (DEV_COM_ERROR_EZDEVSDK_NET_INDEX + 4)  ///< 数据传输失败
#define DEV_COM_ERROR_EZDEVSDK_NET_DNS              (DEV_COM_ERROR_EZDEVSDK_NET_INDEX + 5)  ///< 域名解析失败

#define DEV_COM_ERROR_EZDEVSDK_LBS_INDEX                100600
#define DEV_COM_ERROR_EZDEVSDK_NET_AUTHCODE_MISMATCH    (DEV_COM_ERROR_EZDEVSDK_LBS_INDEX + 1)  ///< 验证码不一致
#define DEV_COM_ERROR_EZDEVSDK_NET_INVALID_CALL         (DEV_COM_ERROR_EZDEVSDK_LBS_INDEX + 2)  ///< 和服务器的交互流程有问题
#define DEV_COM_ERROR_EZDEVSDK_NET_INVALID_DATA         (DEV_COM_ERROR_EZDEVSDK_LBS_INDEX + 3)  ///< 服务器收到错误数据
#define DEV_COM_ERROR_EZDEVSDK_NET_DEVID_MISMATCH       (DEV_COM_ERROR_EZDEVSDK_LBS_INDEX + 4)  ///< devid不一致
#define DEV_COM_ERROR_EZDEVSDK_NET_MASTERKEY_MISMATCH   (DEV_COM_ERROR_EZDEVSDK_LBS_INDEX + 5)  ///< masterkey不一致
#define DEV_COM_ERROR_EZDEVSDK_NET_SESSIONKEY_MISMATCH  (DEV_COM_ERROR_EZDEVSDK_LBS_INDEX + 6)  ///< sessionkey不一致
#define DEV_COM_ERROR_EZDEVSDK_NET_INVALID_DEV          (DEV_COM_ERROR_EZDEVSDK_LBS_INDEX + 7)  ///< 无效设备
#define DEV_COM_ERROR_EZDEVSDK_NET_SERVER_CRYPTO        (DEV_COM_ERROR_EZDEVSDK_LBS_INDEX + 8)  ///< 服务器加解密失败
#define DEV_COM_ERROR_EZDEVSDK_NET_GET_DATA             (DEV_COM_ERROR_EZDEVSDK_LBS_INDEX + 9)  ///< 获取das或者stun信息失败
#define DEV_COM_ERROR_EZDEVSDK_NET_SERVER_EXCEPTION     (DEV_COM_ERROR_EZDEVSDK_LBS_INDEX + 10) ///< 服务器内部异常
#define DEV_COM_ERROR_EZDEVSDK_NET_CHECK_SESSIONKEY_FAIL (DEV_COM_ERROR_EZDEVSDK_LBS_INDEX + 11) ///< sessionkey验证失败
#define DEV_COM_ERROR_EZDEVSDK_NET_SIGN_CHECK_FAIL       (DEV_COM_ERROR_EZDEVSDK_LBS_INDEX + 12)  ///< auth_i校验sign失败、申请device id校验sign失败(可能验证码出错)

#define DEV_COM_ERROR_EZDEVSDK_SECRETKEY_INDEX              100900
#define DEV_COM_ERROR_EZDEVSDK_SECRETKEY_DECRYPT_FAIL       (DEV_COM_ERROR_EZDEVSDK_SECRETKEY_INDEX + 0)    ///< 申请secretkey报文平台解密失败
#define DEV_COM_ERROR_EZDEVSDK_SECRETKEY_OVERFLOW_WINDOWS   (DEV_COM_ERROR_EZDEVSDK_SECRETKEY_INDEX + 1)    ///< 申请secretkey请求不在窗口期
#define DEV_COM_ERROR_EZDEVSDK_SECRETKEY_NO_USER            (DEV_COM_ERROR_EZDEVSDK_SECRETKEY_INDEX + 2)    ///< 设备未绑定用户
#define DEV_COM_ERROR_EZDEVSDK_SECRETKEY_SN_NOT_EXIST       (DEV_COM_ERROR_EZDEVSDK_SECRETKEY_INDEX + 3)    ///< 设备不存在
#define DEV_COM_ERROR_EZDEVSDK_SECRETKEY_AGAIN              (DEV_COM_ERROR_EZDEVSDK_SECRETKEY_INDEX + 4)    ///< 设备重复申请

#define DEV_COM_ERROR_EZDEVSDK_DAS_INDEX                    101000
#define DEV_COM_ERROR_EZDEVSDK_DAS_NOSUPPORT_PROTOCOL_VER   (DEV_COM_ERROR_EZDEVSDK_DAS_INDEX + 1)  ///< 不支持的协议版本
#define DEV_COM_ERROR_EZDEVSDK_DAS_CLIENT_ID_INVALID        (DEV_COM_ERROR_EZDEVSDK_DAS_INDEX + 2)  ///< 不合格的客户端标识符
#define DEV_COM_ERROR_EZDEVSDK_DAS_SERVER_UNUSABLE          (DEV_COM_ERROR_EZDEVSDK_DAS_INDEX + 3)  ///< 服务端不可用(服务器内部异常)
#define DEV_COM_ERROR_EZDEVSDK_DAS_INVALID_USERNAME         (DEV_COM_ERROR_EZDEVSDK_DAS_INDEX + 4)  ///< 无效的用户名或密码（现阶段暂不使用）
#define DEV_COM_ERROR_EZDEVSDK_DAS_UNAUTHORIZED             (DEV_COM_ERROR_EZDEVSDK_DAS_INDEX + 5)  ///< 未授权（现阶段暂不使用）
#define DEV_COM_ERROR_EZDEVSDK_DAS_SESSION_INVAILD          (DEV_COM_ERROR_EZDEVSDK_DAS_INDEX + 6)  ///< 接入会话失效
#define DEV_COM_ERROR_EZDEVSDK_DAS_FORCE_OFFLINE            (DEV_COM_ERROR_EZDEVSDK_DAS_INDEX + 7)  ///< 设备被强制下线
#define DEV_COM_ERROR_EZDEVSDK_DAS_FORCE_DEV_RISK           (DEV_COM_ERROR_EZDEVSDK_DAS_INDEX + 8)  ///< 设备被风控（黑名单）
#define DEV_COM_ERROR_EZDEVSDK_DAS_FORCE_DOMAIN_RISK        (DEV_COM_ERROR_EZDEVSDK_DAS_INDEX + 9)  ///< 领域被风控
#define DEV_COM_ERROR_EZDEVSDK_DAS_FORCE_CMD_RISK           (DEV_COM_ERROR_EZDEVSDK_DAS_INDEX + 10) ///< 指令被风控

/*******************全局错误码 end**********************/

/************************************************************************/
/*                        本地功能所需参数                              */
/************************************************************************/
typedef struct tagNET_EBASE_ABILITY_CFG
{
    HPR_UINT32 dwSize;          //[in]结构体大小
    HPR_UINT32 dwClientCnt;     //[in]作为客户端，支持同时开启几个，默认为1个
    HPR_UINT32 dwAlarmCnt;      //[in]同时支持几路报警，默认不开启
}NET_EBASE_ABILITY_CFG, *LPNET_EBASE_ABILITY_CFG;

typedef struct tagNET_EBASE_ABILITY_CFG_EX
{
    HPR_UINT32 dwSize;
    HPR_UINT32 dwClientCnt;
    HPR_UINT32 dwAlarmCnt;
    HPR_UINT32 dwLFCnt;
    HPR_UINT8  byRes[128];
}NET_EBASE_ABILITY_CFG_EX;

typedef void (CALLBACK *LogFunc)(HPR_UINT32 dwLevel, char const *pBuffer, HPR_VOIDPTR pUserData);
typedef struct tagNET_EBASE_LOGINFO
{
    HPR_UINT32  dwSize;         //[in]结构体大小
    HPR_UINT32  dwLevel;        //[in]日志的等级（默认为0）：0-表示关闭日志，1-表示只输出ERROR错误日志，2-输出ERROR错误信息和DEBUG调试信息，3-输出ERROR错误信息、DEBUG调试信息和INFO普通信息等所有信息
    LogFunc     fnLogFunc;      //[in]日志回调函数
    HPR_VOIDPTR pUserData;      //[in]用户信息
}NET_EBASE_LOGINFO, *LPNET_EBASE_LOGINFO;

typedef struct tagNET_EBASE_TIMEOUT
{
    HPR_UINT32 dwConnectTimeOut;    //链接超时，默认3500ms，之所以不是3000ms，是因为3000ms是很多系统connect第一次重连的时候，多给500ms，相当于给了一次重连的机会
    HPR_UINT32 dwSendTimeOut;       //发送超时，默认5000ms
    HPR_UINT32 dwRecvTimeOut;       //接收超时，默认5000ms
}NET_EBASE_TIMEOUT, LPNET_EBASE_TIMEOUT;

typedef enum enumNET_EBASE_PARAM_TYPE
{
    EBASE_PARAM_UNKNOWN = -1,       //错误类型，无实际含义
    EBASE_PARAM_LOCAL_IP = 0,       //本地IP地址，使用于NET_EBASE_GetParam，对应结构体NET_EBASE_IPINFO
    EBASE_PARAM_ALIVE_INTERVAL= 1,  //设置连接保活心跳间隔，使用于NET_EBASE_SetParam，心跳间隔参数HPR_UINT32，单位ms
    EBASE_PARAM_SESSION_INFO = 2,    //获取SessionKey，使用于NET_EBASE_GetParam，对应结构体NET_EBASE_SESSION_INFO
    EBASE_PARAM_CURINGDATA_CB = 3,       //设置secretKey回调接口，已生产但无产线验证码的设备，接入萤石云时使用，使用NET_EBASE_SetParam，对应结构体NET_EBASE_IOT_CURINGDATA_CB
    EBASE_PARAM_COMPRESS = 4,           //设置是否开启数据压缩 压缩算法；0-表示不压缩；1-表示支持gzip压缩； 参数HPR_UINT8
    EBASE_PARAM_MSG_SIZE = 5,           //用户自定义收发单条消息的最大长度（压缩前）,参数HPR_UINT16，单位kb。   取值范围0-255(超出范围报错)，0表示256kb(默认)    1-255表示1kb-255kb
    EBASE_PARAM_LOAD_THIRD_PARTY = 6,  //[only OTAP]接入萤石云平台使用，透传ISAPI协议，SDK默认加载，用户可设置不加载,对应结构体为HPR_UINT8, 0-不加载，1-加载
    EBASE_PARAM_ALARM_SUBSCRIBE_RULE_WORKING_MODE = 7, //[only OTAP]对应结构体为HPR_UINT8, 默认订阅规则由设备实现，对报警客户端生效。 0-设备程序实现订阅规则，1-SDK库实现订阅规则
    EBASE_PARAM_ALARM_SUBSCRIBE_RULE = 8,        //[only OTAP]设置报警订阅规则；对报警客户端生效。参考枚举值7中描述，0-设备程序实现订阅规则时，无需设置该参数。1-SDK实现订阅规则时，若未设置该参数，报警全部过滤；若通过该参数设置了订阅规则，SDK按设置的订阅规则过滤上报。对应结构体NET_EBASE_ALARM_SUBSCRIBE_RULE_LIST
    EBASE_PARAM_ALARM_SWITCH = 9,         //[only OTAP]接入萤石云平台使用，设置报警开关,默认是开启，对报警客户端生效，对应结构体为HPR_UINT8，0-关闭报警上报，1-开启报警上报
}NET_EBASE_PARAM_TYPE;

#define EHOME_IP_LEN 64 //IPv4和IPv6地址，用字符串表示，64个字节已经足够
#define EHOME_IP_LEN_MAX 256 //IPv4和IPv6地址，用字符串表示，64个字节已经足够,域名长度最大256

typedef struct tagNET_EBASE_IPINFO
{
    HPR_UINT8  szIP[EHOME_IP_LEN/*64*/];//IP地址，支持IPv4，IPv6,必须以结束符结束
    HPR_UINT16 wPort;                   //端口号
    HPR_UINT8  byRes[2];                //字节对齐
}NET_EBASE_IPINFO, *LPNET_EBASE_IPINFO;

#define EHOME_SESSION_KEY_LEN 16 //SessionLey长度，16个字节
typedef struct tagNET_EBASE_SESSION_INFO
{
    HPR_UINT8  szSessionKey[EHOME_SESSION_KEY_LEN/*16*/];//SessionLey
    HPR_UINT32 dwSequence;//序列号，使用时需要加1
    HPR_SOCK_T iSocket;//通信socket
    HPR_UINT8  byRes[32];                //保留字节
}NET_EBASE_SESSION_INFO, *LPNET_EBASE_SESSION_INFO;

typedef enum enumNET_EBASE_LOCALCFG_TYPE
{
    EBASE_LOCALCFG_UNKNOWN = -1,        //错误类型，无实际含义
    EBASE_LOCALCFG_CLIENT_IPINFO = 0,   //OTAP:支持绑定IP地址和端口，使用NET_EBASE_SetIOTLocalCfg；ISUP5.0：支持绑定IP地址，使用NET_EBASE_SetLocalCfg。对应结构体NET_EBASE_IPINFO，作用于客户端连接
                                        //不支持域名方式绑定
    EBASE_LOCALCFG_THREAD_SIZE = 1,     //默认线程栈大小，使用于NET_EBASE_SetLocalCfg/NET_EBASE_SetIOTLocalCfg，对应pParam为HPR_UINT32*，作用于SDK创建线程制定的线程栈大小，单位为B
}NET_EBASE_LOCALCFG_TYPE;

#define MAX_ALARM_SUBSCRIBE_EVENT_TYPE_LEN 128
#define MAX_ALARM_SUBSCRIBE_FILEPATH_TYPE_LEN 256
#define MAX_ALARM_SUBSCRIBE_CHANNELS_LEN 1024
#define MAX_ALARM_SUBSCRIBE_IOLIST_LEN 256

#define ALARM_SUBSCRIBE_ALL_EVENT_TYPE "all"
#define ALARM_SUBSCRIBE_ALL_FILEPATH_TYPE "all"
#define ALARM_SUBSCRIBE_ALL_CHANNELS_TYPE "all"
#define ALARM_SUBSCRIBE_ALL_IOLIST_TYPE "all"
typedef struct tagNET_EBASE_ALARM_SUBSCRIBE_RULE_NODE
{
    HPR_UINT8   byAlarmType[MAX_ALARM_SUBSCRIBE_EVENT_TYPE_LEN];         //事件类型，（1）订阅全部事件类型时，设置为ALARM_SUBSCRIBE_ALL_EVENT_TYPE（2）设置一种事件类型
    HPR_UINT8   byFilePathType[MAX_ALARM_SUBSCRIBE_FILEPATH_TYPE_LEN];   //图片上传方式,（1）订阅全部时，设置为ALARM_SUBSCRIBE_ALL_FILEPATH_TYPE（2）设置图片上传方式，若有多种，每种方式以逗号结尾，组合成字符串
    HPR_UINT8   byChannels[MAX_ALARM_SUBSCRIBE_CHANNELS_LEN];            //订阅通道,（1）订阅全部时，设置为ALARM_SUBSCRIBE_ALL_CHANNELS_TYPE（2）设置通道，若有多个，每个通道号以逗号结尾，组合成字符串
    HPR_UINT8   byIOList[MAX_ALARM_SUBSCRIBE_IOLIST_LEN];                //订阅IO通道,（1）订阅全部时，设置为ALARM_SUBSCRIBE_ALL_IOLIST_TYPE（2）设置IO通道，若有多个，每个通道号以逗号结尾，组合成字符串
    HPR_UINT8   byRes[128];              
}NET_EBASE_ALARM_SUBSCRIBE_RULE_NODE, *LPNET_EBASE_ALARM_SUBSCRIBE_RULE_NODE;

typedef struct tagNET_EBASE_ALARM_SUBSCRIBE_RULE_LIST
{
    NET_EBASE_ALARM_SUBSCRIBE_RULE_NODE *pList;
    HPR_UINT32 dwListNum;
    HPR_UINT8  byRes[256];                //保留字节
}NET_EBASE_ALARM_SUBSCRIBE_RULE_LIST, *LPNET_EBASE_ALARM_SUBSCRIBE_RULE_LIST;

/************************************************************************/
/*                          注册所需参数                                */
/************************************************************************/
typedef enum enumEBaseDataType
{
    DATETYPE_UNKNOW     = -1,       //未知类型,           ClassType(pData) = NULL
    DATETYPE_EXPECTION  = 0,        //出现异常,           ClassType(pData) = HPR_UINT32 *,表示错误码

    //服务端主动发起的类型
    DATETYPE_ISAPI                = 1,        //接收到服务端请求    ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATETYPE_EZVIZ                = 2,        //接收到的服务端响应  ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATETYPE_EHOME                = 3,        //接收EHome2.0/4.0的服务端请求  ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATETYPE_HTTP                 = 4,        //接收到服务端HTTP请求 ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATETYPE_VOICESTART           = 5,        //语音对讲开始 ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATETYPE_VOICESTOP            = 6,        //语音对讲结束 ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATATYPE_PREVIEWSTART         = 7,        //预览开始 ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATATYPE_PREVIEWSTOP          = 8,        //预览结束 ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATATYPE_PLAYBACKSTART        = 9,        //回放开始 ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATATYPE_PLAYBACKSTOP         = 10,       //回放结束 ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATATYPE_PUBLISHACK           = 11,       //接收到服务端对应消息ID的确认响应 ClassType(pData) = HPR_UINT32 *,表示消息ID
    DATATYPE_PREVIEWMAKEIFRAME    = 12,       //预览强制I帧 ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATATYPE_PLAYBACKPAUSE        = 13,       //回放暂停 ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATATYPE_PLAYBACKRESTART      = 14,       //回放恢复 ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATATYPE_DEVICE_WAKEUP        = 15,       //唤醒设备 ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATATYPE_PLAYBACKREFAST       = 16,       //回放快放 ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATATYPE_PLAYBACKRESLOW       = 17,       //回放快放 ClassType(pData) = LPNET_EBASE_SERVER_DATA
    DATATYPE_ALARMRECONNECT       = 18,       //报警重连
    DATATYPE_ALARMRECONNECTSUCCESS = 19,      //报警重连成功
    DATATYPE_RCVLFDATA = 20,              //供大文件内部使用

    //分隔符
    DATATYPE_SEPARATOR       = 100,      //无实际意义，为分隔符

    //客户端主动发起的类型
    DATATYPE_ALARM           = 101,      //发送报警数据
    DATATYPE_EZVIZNOTICE     = 102,      //萤石协议主动推送给服务端的publish，目前EHomeSDK仅认识/1100/12345,/30000/1
    DATATYPE_SENDLFDATA = 103,           //供大文件内部使用
    DATATYPE_SENDLFSESSION = 104,       //供大文件内部session使用

    //设备SDK回调给设备APP的状态
    DATATYPE_CONN_SUCC = 200,           //设备异步方式注册成功，目前只支持IOT协议，ClassType(pData) = LPNET_IOT_CONN_SUCC_INFO
    DATATYPE_RECONN_SUCC = 201,         //设备重连成功，目前只支持IOT协议

    //以下是新物联协议的数据信令通道上的类型
    DATATYPE_IOT_KERNEL_READY = 1008,   //SDK内核准备完成， ClassType(pData) = LPNET_EBASE_IOT_KERNEL_READY_INFO
    DATATYPE_IOT_RUNTIME_ERROR = 1009,  //设备SDK运行时错误，此状态并不表示设备下线， ClassType(pData) = HPR_UINT32 *,表示错误码
    DATATYPE_IOT = 1100,                //接收到服务端请求，包含属性和操作 ClassType(pData) = LPNET_EBASE_SERVER_DATA

    //分隔符
    DATATYPE_IOT_SEPARATOR = 1200,      //无实际意义，为分隔符

    //新物联协议客户端主动发起的类型
    DATATYPE_IOT_ALARM = 1201,          //发送报警数据
    DATATYPE_IOT_NOTICE = 1202,         //设备主动推送给服务端的publish
    DATATYPE_IOT_ISAPI_ALARM = 1203     //透传ISAPI的报警
}EBaseDataType;

#define MAX_DEVICE_ID_LEN_V50      256 //设备ID长度
#define FIRMWARE_VER_LEN           24  //固件版本
#define MAX_COMPLETE_SERIAL_LEN    64  //序列号长度
#define MAX_SERIAL_LEN             12  //短序号长度
#define MAX_FIRMWARE_LEN           12  //设备固件长度
#define MAX_FIRMWARE_LEN_OTAP         64 //设备固件长度
#define MAX_DEVNAME_LEN            64  //设备名称长度
#define MAX_EHOME50_KEY_LEN        32  //秘钥长度
#define MAX_ADD_LEN                256 //地址长度
#define MAX_VOICETALK_TOKEN_LEN    256 //Token长度

#define MAX_IP_LEN                 64 //最大IP长度

//新物联协议
#define MAX_IOT_CLIENT_SESSION     256
#define MAX_IOT_PATH_LEN            128 //文件路径长度
#define MAX_IOT_DOMAIN_ID_LEN       64
#define MAX_IOT_RESOURCE_ID_LEN     64
#define MAX_IOT_RESOURCE_TYPE_LEN   64
#define MAX_IOT_IDENTIFIER_LEN      64
#define MAX_IOT_MODULE_LEN     64
#define MAX_IOT_METHOD_LEN          64
#define MAX_IOT_MSG_TYPE_LEN        32
#define MAX_IOT_CHILDID_LEN         64
#define MAX_IOT_SERIAL_LEN          72
#define MAX_IOT_BUSINESS_TYPE_LEN   64
#define MAX_IOT_EZACCESS_REGISTERURL_LEN   128
#define MAX_IOT_EZACCESS_TOKEN_LEN   64
#define MAX_DAS_SERVER_ID_LEN       64
#define MAX_SESSION_KEY_LEN         16

#define MAX_LF_SESSION_ID_LEN         128 //LF sessionid长度

#define MAX_DEV_TYPE_LEN      256 //设备类型长度
#define MAX_DEV_TYPE_DISPLAY_LEN      256 //设备显示类型长度
#define MAX_DEV_MAC_LEN      256 //设备网上物理地址

// #define version_max_len               32     ///< 版本长度
// #define ezdev_sdk_extend_name_len     32     ///< 扩展模块名字长度
// #define ezdev_sdk_ip_max_len          64     ///< ip最长长度
// #define ezdev_sdk_timezone_max_len    32     ///< timezone最长长度
// #define ezdev_sdk_sessionkey_len      16     ///< 设备会话秘钥长度，默认是16个字节
// #define ezdev_sdk_devid_len           32     ///< 设备唯一标识长度  默认是32字节的字符串
// #define ezdev_sdk_masterkey_len       16     ///< 设备mastekey长度 ,默认是16个字节
// #define ezdev_sdk_verify_code_maxlen  48     ///< 设备验证码支持最大长度(对应licence认证中product_key)

typedef void (CALLBACK *EBaseClientFunc)(HPR_INT32 dwHandle, EBaseDataType dwType, HPR_UINT32 dwLen, HPR_VOIDPTR pData, HPR_VOIDPTR pUserData);
typedef struct tagNET_EBASE_REGINFO
{
    HPR_UINT32  dwSize;
    HPR_UINT32  dwDevType;                                       //[in]设备类型
    HPR_UINT32  dwState;                                         //[in]设备工作状态，1:正常工作模式，5：待机（或者睡眠）模式
    HPR_UINT8   byDeviceSerial[MAX_COMPLETE_SERIAL_LEN/*64*/];   //[in]设备序列号，设备型号+日期+短序号+扩展
    HPR_UINT8   bySubSerial[MAX_SERIAL_LEN/*12*/];               //[in]设备短序号,数字序列号
    HPR_UINT8   byDevName[MAX_DEVNAME_LEN/*64*/];                //[in]设备名称
    HPR_UINT8   byDeviceID[MAX_DEVICE_ID_LEN_V50/*256*/];        //[in]EHome协议中的DeviceID
    HPR_UINT8   byShareKey[MAX_EHOME50_KEY_LEN/*32*/];           //[in]sharekey长度，要求设备不存储EHomeKey，而是存sharekey:md5(md5(md5(SHA256(EHomeKey)+ deviceID)+www.88075998.com))  
    HPR_UINT8   byFirmwareVer[MAX_FIRMWARE_LEN/*12*/];           //[in]设备固件版本
    HPR_UINT8   byServerAdd[MAX_ADD_LEN/*256*/];                 //[in]服务端地址，支持IPv4，IPv6，域名
    HPR_UINT16  wServerPort;                                     //[in]服务端端口
    //设备支持的功能情况，支持"与"操作。支持"与"操作。从右到左，
    //第一位为1表示支持NPQ预览(0b00000001)，
    //第二位为1表示支持NPQ回放(0b00000010)。
    //第三位为1表示支持预览、回放和语音对讲链路加密传输(0b00000100)
    //第四位为1表示支持HLS回放(0b00001000)
    //第五位为1表示支持语音对讲链路纯广播传输—即只有下行数据无上行数据(0b00010000)
    HPR_UINT8   bySupport;
    HPR_UINT8   byRes;                                           //[Res]保留，用于字节对齐
    EBaseClientFunc fnCB;                                        //[in]接收到的数据回调
    HPR_VOIDPTR pUserData;                                       //[in]用户数据
}NET_EBASE_REGINFO, *LPNET_EBASE_REGINFO;

//注册协议类型
typedef enum enumEBaseRegisterMode
{
    REGISTER_PLATFORM_ISUP = 0,  //以ISUP5.0协议接入ISUP平台
    REGISTER_PLATFORM_IOT = 1,   //以OTAP协议接入萤石云平台
    REGISTER_PLATFORM_OTAP = 2,  //以OTAP协议接入私有化部署平台
}EBaseRegisterMode;


/**
* \brief 存取数据类型
*/
typedef enum enumEBaseIoTKeyValueType
{
    KEYVALUE_DEVICE_ID,         ///<	设备唯一标识  首次设备上线后会分配 一定要写入flash
    KEYVALUE_MASTER_KEY,        ///<	设备masterkey 首次设备上线后会分配 尽量写入flash
    KEYVALUE_COUNT			    ///<	此枚举上限
}EBaseIoTKeyValueType;

typedef enum enumEBaseIoTCurringType
{
    CURINGDATA_SECRETKEY,           ///<	验证码不合规设备重新申请的secretkey，一定要固化
    CURINGDATA_COUNT                   ///<	此枚举上限
}EBaseIoTCurringType;

//下面两个回调函数第五个参数均为NET_EBASE_ConnectToIOTServer接口中的结构体NET_EBASE_IOT_REGINFO中的pUserData
typedef void (CALLBACK *EBaseKeyValueFunc)(HPR_INT32 dwHandle, EBaseIoTKeyValueType dwKeyValueType, unsigned char* pData, HPR_UINT32 dwLen, HPR_VOIDPTR pUserData);
typedef void (CALLBACK *EBaseCuringFunc)(HPR_INT32 dwHandle, EBaseIoTCurringType dwKeyValueType, unsigned char* pData, HPR_UINT32 dwLen, HPR_VOIDPTR pUserData);

typedef struct tagNET_EBASE_IOT_REGINFO_EXTEND
{
    HPR_UINT8   byDevType[MAX_DEV_TYPE_LEN];                      //[in] [only OTAP] NET_EBASE_IOT_REGINFO中byDevInfoType=1时必填。设备真实型号，用作设备静态模型报备使用
    HPR_UINT8   byDevTypeDisplay[MAX_DEV_TYPE_DISPLAY_LEN];       //[in] [only OTAP] NET_EBASE_IOT_REGINFO中byDevInfoType=1时必填。设备显示型号，用作在界面显示设备型号
    HPR_UINT8   byMAC[MAX_DEV_MAC_LEN];                           //[in] [only OTAP] NET_EBASE_IOT_REGINFO中byDevInfoType=1时必填。设备网上物理地址
    HPR_UINT8   byFirmwareVer[MAX_FIRMWARE_LEN_OTAP/*64*/];           //[in][only OTAP] NET_EBASE_IOT_REGINFO中byDevInfoType=1时必填 设备固件版本
    HPR_UINT8   byRes[512];
}NET_EBASE_IOT_REGINFO_EXTEND;

typedef struct tagNET_EBASE_IOT_KERNEL_READY_INFO
{
    HPR_VOIDPTR pKernelPtr;
    HPR_UINT8   byRes[128];
}NET_EBASE_IOT_KERNEL_READY_INFO, *LPNET_EBASE_IOT_KERNEL_READY_INFO;

typedef struct tagNET_EBASE_IOT_REGINFO
{
    HPR_UINT32  dwSize;
    HPR_UINT32  dwDevType;                                       //[in] [only ISUP5.0] 必填 设备类型
    HPR_UINT32  dwState;                                         //[in] [OTAP/ISUP5.0] OTAP保留，暂未用。ISUP5.0:必填。设备工作状态，1:正常工作模式，5：待机（或者睡眠）模式（保留，目前无效，实际按照正常工作模式）
    HPR_UINT8   byDeviceSerial[MAX_COMPLETE_SERIAL_LEN/*64*/];   //[in] [OTAP/ISUP5.0] OTAP：byDevInfoType=1时必填。ISUP5.0:必填。设备序列号，设备型号+日期+短序号+扩展，
    HPR_UINT8   bySubSerial[MAX_SERIAL_LEN/*12*/];               //[in] [only ISUP5.0] 必填 设备短序号,数字序列号
    HPR_UINT8   byDevName[MAX_DEVNAME_LEN/*64*/];                //[in] [OTAP/ISUP5.0] OTAP保留，暂未用。ISUP5.0:必填。设备名称
    HPR_UINT8   byDeviceID[MAX_DEVICE_ID_LEN_V50/*256*/];        //[in] [OTAP/ISUP5.0] OTAP:byDevInfoType=1时必填，可填入设备短序号或唯一标记值，最长支持72字节。ISUP5.0：必填，设备ID。
    HPR_UINT8   byShareKey[MAX_EHOME50_KEY_LEN/*32*/];           //[in] [OTAP/ISUP5.0] 海康设备要求不能存储OTAP dev_verification_code或isup5.0 EHomeKey，而是存sharekey，
                                                                                       //OTAP byDevInfoType=1时必填 sharekey算法: pdkdf2_SHA256(md5(md5(md5(dev_verification_code + dev_subserial) + www.88075998.com)))，详见:otap_generate_sharekey
                                                                                       //ISUP5.0 必填 sharekey算法:：md5(md5(md5(SHA256(EHomeKey)+ deviceID)+www.88075998.com))，详见：generate_sharekey
    HPR_UINT8   byFirmwareVer[MAX_FIRMWARE_LEN/*12*/];           //[in] [only ISUP5.0] ISUP5.0:必填。设备固件版本，
    HPR_UINT8   byServerAdd[MAX_ADD_LEN/*256*/];                 //[in] [OTAP/ISUP5.0] 必填。服务端地址，支持IPv4，IPv6，域名
    HPR_UINT16  wServerPort;                                     //[in] [OTAP/ISUP5.0] 必填。服务端端口，
    HPR_UINT8   byAsynConnect;                                   //[in] [only OTAP]    必填。异步注册，0-同步（接口返回成功表示注册成功），1-异步（回调注册成功）
    HPR_UINT8   byKeyValueMethod;                                //[in] [only OTAP]    必填。平台DevID和MasterKey存储方式
                                                                                       //值为0-dev_id和masterkey通过文件方式存取，对应字段byDevIDFile和byMasterKeyFile,
                                                                                       //值为1-通过回调的方式存取，对应字段fnCBKeyValueLoad和fnCBKeyValueSave
    EBaseClientFunc fnCB;                                        //[out][OTAP/ISUP5.0] 必填。接收数据回调函数
    HPR_VOIDPTR pUserData;                                       //[in] [OTAP/ISUP5.0] 可选。用户数据
    HPR_UINT8   byDevInfoFile[MAX_IOT_PATH_LEN/*128*/];          //[in] [only OTAP]    设备信息文件路径，byDevInfoType=0必填
    HPR_UINT8   byMasterKeyFile[MAX_IOT_PATH_LEN/*128*/];        //[in] [only OTAP]    masterkey文件路径，byKeyValueMethod =0时必填
    HPR_UINT8   byDevIDFile[MAX_IOT_PATH_LEN/*128*/];            //[in] [only OTAP]    平台dev_id文件路径，byKeyValueMethod =0时必填
    EBaseKeyValueFunc fnCBKeyValueLoad;                          //[in] [only OTAP]    SDK通过该回调获取dev_id和masterkey，byKeyValueMethod =1时必填
    EBaseKeyValueFunc fnCBKeyValueSave;                          //[out][only OTAP]    SDK通过该回调将dev_id和masterkey给用户，byKeyValueMethod =1时必填
    HPR_UINT8   byDevInfoType;                                   //[in] [only OTAP]    必填 1-设备信息通过结构体方式传入SDK（建议使用此种方式，文件方式需保存验证码dev_verification_code，不符合安全要求，此种方式保存的是验证码经过摘要算法计算出的byShareKey，具体计算方式参考byShareKey字段说明）
                                                                                       //   0-设备信息通过文件方式传给SDK；此种方式，无需填入byDeviceSerial，byDeviceID，byShareKey，byFirmwareVer，pExDevInfo，这些信息由文件中读入，文件中数据格式参考/Bin/Linux64/fileDepens/dev_info。
    HPR_UINT8   bySupport;                                       //[in] [only ISUP5.0] 可选 设备支持的功能情况，支持"与"操作。支持"与"操作。从右到左，
                                                                                       //第一位为1表示支持NPQ预览(0b00000001)，
                                                                                       //第二位为1表示支持NPQ回放(0b00000010)。
                                                                                       //第三位为1表示支持预览、回放和语音对讲链路加密传输(0b00000100)
                                                                                       //第四位为1表示支持HLS回放(0b00001000)
    HPR_UINT8   byAESIV[17];                                                           
    HPR_VOIDPTR pExDevInfo;                                      //[in] [only OTAP]    byDevInfoType=1时必填,指针指向结构体NET_EBASE_IOT_REGINFO_EXTEND
    HPR_UINT8   byRes[489];                                      //[Res]保留，用于字节对齐
}NET_EBASE_IOT_REGINFO, *LPNET_EBASE_IOT_REGINFO;

typedef struct tagNET_EALARM_IOT_REGINFO
{
    HPR_UINT32  dwSize;                                          //[in] [OTAP/ISUP5.0] 非必填，结构体大小
    HPR_UINT8   byDeviceSerial[MAX_COMPLETE_SERIAL_LEN/*64*/];   //[in] [only ISUP5.0] 必填，设备序列号，设备型号+日期+短序号+扩展
    HPR_UINT8   bySubSerial[MAX_SERIAL_LEN/*12*/];               //[in] [only ISUP5.0] 必填，设备短序号,数字序列号
    HPR_UINT8   byDevName[MAX_DEVNAME_LEN/*64*/];                //[in] [only ISUP5.0] 必填，设备名称
    HPR_UINT8   byDeviceID[MAX_DEVICE_ID_LEN_V50/*256*/];        //[in] [only ISUP5.0] 必填，EHome协议中的DeviceID
    HPR_UINT8   byFirmwareVer[MAX_FIRMWARE_LEN/*12*/];           //[in] [only ISUP5.0] 必填，设备固件版本
    HPR_UINT8   byResourceID[MAX_IOT_RESOURCE_ID_LEN/*64*/];     //[in] [only OTAP]  必填，资源id
    HPR_UINT8   byResourceType[MAX_IOT_RESOURCE_TYPE_LEN/*64*/]; //[in] [only OTAP]  必填，资源类型
    HPR_UINT8   byDomainID[MAX_IOT_DOMAIN_ID_LEN/*64*/];         //[in] [only OTAP]  必填，消息领域id
    HPR_UINT8   byIdentifier[MAX_IOT_IDENTIFIER_LEN/*64*/];      //[in] [only OTAP]  必填，业务标识，OTAP平台注册模式支持
    HPR_UINT8   byServerAdd[MAX_ADD_LEN/*256*/];                 //[in] [OTAP/ISUP5.0] 支持IPv4，IPv6。OTAP:优先连接该报警服务端地址；若为空，连接SDK内部截取平台下发的地址；若平台未下发，报警走注册链路。ISUP5.0:必填
    HPR_UINT16  wServerPort;                                     //[in] [OTAP/ISUP5.0] 同上byServerAdd
    HPR_UINT8   byProtocolType;                                  //[out] [only OTAP]  非必填,报警传输协议，0-tcp
    HPR_UINT8   byRes1[3];
    EBaseClientFunc fnCB;                                        //[in] [OTAP/ISUP5.0] 必填，数据回调函数
    HPR_VOIDPTR pUserData;                                       //[in] [OTAP/ISUP5.0] 非必填，用户数据
    HPR_UINT32  dwkeepAliveSec;                                  //[out][only OTAP] 非必填,mqtt心跳间隔
    HPR_UINT8   byAESIV[17];                                     //[out][only OTAP] 非必填
    HPR_UINT8   byRes[235];
}NET_EALARM_IOT_REGINFO, *LPNET_EALARM_IOT_REGINFO;

/***********************设备SDK给设备应用层回调的数据 Start *********************************/
//NET_EBASE_ConnectToIOTServer
typedef struct tagNET_EBASE_SERVER_DATA
{
    HPR_UINT32      dwSize;         //[out]结构体长度
    HPR_UINT32      dwSequence;     //[out]序列号，设备端响应本请求时，需将这个值原值设置给接口
    HPR_VOIDPTR     pCmdIdentify;   //[out][readonly]不可在回调之外使用该指针 
                                    //      (1)当enumEBaseDataType == DATETYPE_ISAPI || enumEBaseDataType == DATETYPE_HTTP，为url
                                    //      (2)当enumEBaseDataType == DATETYPE_EHOME时，为EHome报文中的<CommandType>值
                                    //      (3)当enumEBaseDataType == DATATYPE_EZVIZ，为萤石定义的topic，当前EHomeSDK会发送的topic是/1100/10305，设备端应该响应/1100/10306，详见EHome协议文档（码流加密）
                                    //      (4)当enumEBaseDataType == DATATYPE_IOT，为新物联协议的topic，指向一个NET_IOT_SERVER_COMMAND结构体
    HPR_UINT32      dwBodyLen;      //[out]请求体长度
    HPR_VOIDPTR     pDodyData;      //[out][readonly]请求体数据，不可在回调之外使用该指针
}NET_EBASE_SERVER_DATA, *LPNET_EBASE_SERVER_DATA;

//模块类型
typedef enum enumIotModuleType
{
    MODULE_TYPE_MODEL = 0,      //model
    MODULE_TYPE_OTA = 1,        //升级OTA
    MODULE_TYPE_STRORAGE = 2,   //存储
    MODULE_TYPE_UNKNOWN = 0XFF,    //后续增加模块，通过字符串匹配的方式，不再增加新的枚举，详见NET_IOT_SERVER_COMMAND和NET_IOT_CLIENT_COMMAND字段byModule
}IotModuleType;

//Model模块中的方法
typedef enum enumIotModelMethod
{
    MODEL_METHOD_EVENT = 0,     //事件，设备上报
    MODEL_METHOD_ATTRIBUTE = 1, //属性，设备和平台都可以主动发起
    MODEL_METHOD_SERVICE = 2,   //操作，设备和平台都可以主动发起
}IotModelMethod;

//OTA模块中的方法
typedef enum enumIotOTAMethod
{
    OTA_METHOD_INFORM = 0,      //固件版本信息，设备和平台都可以主动发起
    OTA_METHOD_UPGRADE = 1,     //固件升级通知，平台下发给设备
    OTA_METHOD_PROGRESS = 2,    //固件升级进度，设备上报
}IotOTAMethod;

//STRORAGE模块中的方法
typedef enum enumIotStorageMethod
{
    STOR_METHOD_UPLOAD_URL = 0,     //存储上传地址，设备向平台获取
    STOR_METHOD_UPLOAD_RESULT = 1,  //存储上传结果，设备上报
    STOR_METHOD_DOWNLOAD_URL = 2,   //存储下载地址，设备向平台获取
}IotStorageMethod;

typedef struct tagNET_IOT_SERVER_COMMAND
{
    HPR_UINT32  dwSize;                                     //[out]结构体长度
    HPR_UINT16  wModuleType;                                //[out]用户和萤石云服务约定的模块标识,例如:model（模型）、ota（升级）、storage（存储）等，值为enumIotModuleType
    HPR_UINT16  wMethod;                                    //[out]模块下的方法
                                                            //      (1)当wModuleType = MODULE_TYPE_MODEL，方法有：event（事件）、attribute（属性）、service（操作），值为enumIotModelMethod
                                                            //      (2)当wModuleType = MODULE_TYPE_OTA，方法有：inform、upgrade、progress，值为enumIotOTAMethod
                                                            //      (3)当wModuleType = MODULE_TYPE_STRORAGE，方法有：upload/url、upload/result、download/url，值为enumIotStorageMethod
    //HPR_UINT16  wStorageID;
    HPR_UINT16  wBusinessType;                              //[out]业务类型
    HPR_UINT8   byRes1[2];                                  //[Res]保留，用于字节对齐
    HPR_UINT8   bySubSerial[MAX_IOT_SERIAL_LEN];            //[out]子设备序列号，topic中$CHILDID字段
    HPR_UINT8   byResourceID[MAX_IOT_RESOURCE_ID_LEN];      //[out]资源id，topic中$LOCALINDEX字段
    HPR_UINT8   byResourceType[MAX_IOT_RESOURCE_TYPE_LEN];  //[out]资源类型，topic中$RESOURCETYPE字段
    HPR_UINT8   byDomainID[MAX_IOT_DOMAIN_ID_LEN];          //[out]消息领域id, "model"中{DOMAIN}字段,其他模块无需输入
    HPR_UINT8   byIdentifier[MAX_IOT_IDENTIFIER_LEN];       //[out]业务标识，"model"中{IDENTIFIER}字段,其他模块无需输入
    HPR_UINT8   byModule[MAX_IOT_MODULE_LEN];               //[out]模型,当前有"model" "ota" "storage" "EZVIZAccess" "childmanage"
    HPR_UINT8   byMethod[MAX_IOT_METHOD_LEN];               //[out]方法，topic中module和byMsgType之间的部分
    HPR_UINT8   byMsgType[MAX_IOT_MSG_TYPE_LEN];            //[inout]消息类型"operate" "set" "get" "report" "query" "set_reply" "get_reply" "operate_reply" "report_reply" "query_reply"等
    HPR_UINT32  dwmMulIndex;                                //[out]批量配置索引
    HPR_UINT8   byRes2[348];                                //[Res]保留，用于字节对齐
}NET_IOT_SERVER_COMMAND, *LPNET_IOT_SERVER_COMMAND;


typedef struct tagNET_IOT_CONN_SUCC_INFO
{
    HPR_UINT32  dwSize;                                 //[out]结构体长度
    HPR_UINT16  wDasTcpPort;                            //[out]das服务器的TCP端口号
    HPR_UINT16  wDasUdpPort;                            //[out]das服务器UDP端口号
    char        szDasIP[MAX_IP_LEN];                    //[out]das服务器IP
    char        szLbsIP[MAX_IP_LEN];                    //[out]lbs服务器IP
    HPR_UINT8   bySessionKey[MAX_SESSION_KEY_LEN];      //[out]会话秘钥
    HPR_INT32   iDasSocket;                             //[out]das的socket
    char        szDasDomain[MAX_ADD_LEN];               //[out]das的域名
    char        szDasServerID[MAX_DAS_SERVER_ID_LEN];   //[out]das的serverid
} NET_IOT_CONN_SUCC_INFO, *LPNET_IOT_CONN_SUCC_INFO;
/***********************设备SDK给设备应用层回调的数据 End *********************************/


/***********************设备应用层设置给SDK的数据 Start *********************************/
//NET_EBASE_Reponse
typedef struct tagNET_EBASE_CLIENT_DATA
{
    HPR_UINT32      dwSize;         //[in]结构体长度，未来扩展使用
    EBaseDataType   dwDataType;     //[in]数据类型，见enumEBaseDataType定义，与服务端请求中的dwDataType保持一致
    HPR_UINT32      dwSequence;     //[in]序列号，与服务端请求中的dwSequence保持一致，如果并非响应服务端的请求，填0即可
    HPR_VOIDPTR     pCommandType;   //[in][readonly]，发送给设备的数据类型
                                    //      (1)当enumEBaseDataType == DATETYPE_EHOME时有效，与服务端请求中的pCommandType保持一致
                                    //      (2)当enumEBaseDataType == DATATYPE_EZVIZNOTICE，应当按照萤石的协议，填写对应的topic，目前EHiomeSDK能识别的topic是/1100/10306，/1100/12345,/30000/1
                                    //      (3)当enumEBaseDataType == DATATYPE_IOT，为NET_IOT_CLIENT_COMMAND
    HPR_UINT32      dwBodyLen;      //[in]请求体长度
    HPR_VOIDPTR     pBodyData;      //[in][readonly]请求体数据
    HPR_UINT32      dwID;           //[out]消息ID
}NET_EBASE_CLIENT_DATA, *LPNET_EBASE_CLIENT_DATA;

typedef struct tagNET_IOT_CLIENT_COMMAND
{
    HPR_UINT32  dwSize;                                     //[in]结构体长度，未来扩展使用
    HPR_UINT16  wModuleType;                                //[in]用户和萤石云服务约定的模块标识,例如:model（模型）/ota（升级）/basic/storage（存储）等，值为enumIotModuleType
    HPR_UINT16  wMethod;                                    //[in]模块下的方法
                                                            //      (1)当wModuleType = MODULE_TYPE_MODEL，方法有：event（事件）、attribute（属性）、service（操作），值为enumIotModelMethod
                                                            //      (2)当wModuleType = MODULE_TYPE_OTA，方法有：inform、upgrade、progress，值为enumIotOTAMethod
                                                            //      (3)当wModuleType = MODULE_TYPE_STRORAGE，方法有：upload/url、upload/result、download/url，值为enumIotStorageMethod
    //HPR_UINT16  wStorageID;
    HPR_UINT16  wBusinessType;                              //[in]业务类型
                                                            //  (1)当接口为NET_EBASE_Reponse、wMethod为设备属性时，0-set（平台向设备设置属性），1-get（平台向设备获取属性）
                                                            //  (2)其他情况目前值都为0
                                                            //  即当接口为NET_EBASE_Reponse时，与设备SDK回调给设备应用层数据NET_IOT_SERVER_COMMAND中的wBusinessType值一致
                                                            //  当接口为NET_EBASE_SendPublish/NET_EALARM_SendIOTAlarm时，本参数值为0
    HPR_UINT8   byRes1[2];                                  //[Res]保留，用于字节对齐
    HPR_UINT8   bySubSerial[MAX_IOT_SERIAL_LEN];            //[in]子设备序列号, 如果资源是设备本身，该参数为"global"
    HPR_UINT8   byResourceID[MAX_IOT_RESOURCE_ID_LEN];      //[in]资源id
    HPR_UINT8   byResourceType[MAX_IOT_RESOURCE_TYPE_LEN];  //[in]资源类型
    HPR_UINT8   byDomainID[MAX_IOT_DOMAIN_ID_LEN];          //[in]消息领域id
    HPR_UINT8   byIdentifier[MAX_IOT_IDENTIFIER_LEN];       //[in]业务标识
    HPR_UINT8   byModule[MAX_IOT_MODULE_LEN];           //[in]例如:model（模型）/ota（升级）/basic/storage（存储）
    HPR_UINT8   byMethod[MAX_IOT_METHOD_LEN];           //[in]
    HPR_UINT8   byMsgType[MAX_IOT_MSG_TYPE_LEN];            //[inout]消息类型"operate" "set" "get" "report" "query" "set_reply" "get_reply" "operate_reply" "report_reply" "query_reply"等
    HPR_UINT32  dwmMulIndex;                                //[in]批量配置索引,(1)设备响应平台消息时，传入NET_IOT_SERVER_COMMAND中dwmMulIndex，（2）设备主动发消息时，填0;
    HPR_UINT8   byRes2[348];                                //[Res]保留，用于字节对齐
}NET_IOT_CLIENT_COMMAND, *LPNET_IOT_CLIENT_COMMAND;
/***********************设备应用层设置下来的数据 End *********************************/



/************************************************************************/
/*                            本地接口                                  */
/************************************************************************/

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EBASE_Init(LPNET_EBASE_ABILITY_CFG lpStruAbility)
 *  @brief  初始化库
 *  @param (in)	LPNET_EBASE_ABILITY_CFG lpStruAbility    
 *  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
 */
NET_COM_API HPR_BOOL   CALLBACK NET_EBASE_Init(LPNET_EBASE_ABILITY_CFG lpStruAbility);

NET_COM_API HPR_BOOL   CALLBACK NET_EBASE_InitEx(NET_EBASE_ABILITY_CFG_EX* lpStruAbilityEx);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EBASE_Cleanup()
 *  @brief 反初始化库
 *  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
 */
NET_COM_API HPR_BOOL   CALLBACK NET_EBASE_Fini();
/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EBASE_SetLogInfo(LPNET_EBASE_LOGINFO lpLogInfo)
 *  @brief  设置日志信息
 *  @param (in)	LPNET_EBASE_LOGINFO lpLogInfo    
 *  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
 */
NET_COM_API HPR_BOOL   CALLBACK NET_EBASE_SetLogInfo(LPNET_EBASE_LOGINFO lpLogInfo);
/** @fn	NET_COM_API HPR_UINT32 CALLBACK NET_EBASE_GetBuildVersion()
*  @brief 获取版本号
*  @return	HPR_UINT32 版本号
*/
NET_COM_API HPR_UINT32 CALLBACK NET_EBASE_GetBuildVersion();
/** @fn	NET_COM_API HPR_UINT32 CALLBACK NET_EBASE_GetLastError()
 *  @brief 获取错误码
 *  @return HPR_UINT32 错误码值，0：无错误，其他值为错误码
 */
NET_COM_API HPR_UINT32 CALLBACK NET_EBASE_GetLastError();

/** @fn	NET_COM_API HPR_UINT32 CALLBACK NET_EBASE_SetLocalCfg(HPR_UINT32 dwType, HPR_VOIDPTR pParam)
 *  @brief 本地配置，全局设置
 *  @param (in)	HPR_UINT32 dwType，参考类型NET_EBASE_LOCALCFG_TYPE 
 *  @param (in)	HPR_VOIDPTR pParam
 *  @return	NET_COM_API HPR_UINT32 CALLBACK
 */
NET_COM_API HPR_BOOL CALLBACK NET_EBASE_SetLocalCfg(HPR_UINT32 dwType, HPR_VOIDPTR pParam);


/** @fn	NET_COM_API HPR_UINT32 CALLBACK NET_EBASE_SetIOTLocalCfg(HPR_INT32 iHandle, HPR_UINT32 dwType, HPR_VOIDPTR pParam)
*  @brief 本地配置,针对单个设备
*  @param (in)	HPR_UINT32 dwType，参考类型NET_EBASE_LOCALCFG_TYPE
*  @param (in)	HPR_VOIDPTR pParam
*  @return	NET_COM_API HPR_UINT32 CALLBACK
*/
NET_COM_API HPR_BOOL CALLBACK NET_EBASE_SetIOTLocalCfg(HPR_INT32 iHandle, HPR_UINT32 dwType, HPR_VOIDPTR pParam);

/************************************************************************/
/*                            功能接口                                  */
/************************************************************************/
/** @fn	NET_COM_API HPR_INT32 CALLBACK NET_EBASE_CreateClient()
 *  @brief  创建一个EHome客户端
 *  @return	HPR_INT32  句柄，客户端标识
 */
NET_COM_API HPR_INT32  CALLBACK NET_EBASE_CreateClient();


/************************************************************************/
/*                            功能接口                                  */
/************************************************************************/
/** @fn	NET_COM_API HPR_INT32 CALLBACK NET_EBASE_CreateIOTClient(HPR_UINT32 dwClientType)
*  @brief  创建一个IOT客户端，兼容NET_EBASE_CreateClient
*  @param (in)	HPR_UINT32 dwClientType，
                    对应EBaseRegisterMode，
                    REGISTER_PLATFORM_ISUP表示兼容NET_EBASE_CreateClient，此模式可以创建多个ISUP客户端，由NET_EBASE_Init设置最大值，默认一个
                    REGISTER_PLATFORM_IOT，此模式创建IOT客户端，接入萤石云平台
                    REGISTER_PLATFORM_OTAP，此模式创建IOT客户端，接入海康私有平台
*  @return	HPR_INT32  句柄，客户端标识，从0开始，在一个进程中一个客户端对应一个设备
*/
NET_COM_API HPR_INT32  CALLBACK NET_EBASE_CreateIOTClient(HPR_UINT32 dwClientType);


/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EBASE_ConnectToServer(HPR_INT32 dwHandle, LPNET_EBASE_REGINFO lpStruRegInfo)
 *  @brief  链接至服务端
 *  @param (in)	HPR_INT32 dwHandle    
 *  @param (in)	LPNET_EBASE_REGINFO lpRegInfo    
 *  @return HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
 */
NET_COM_API HPR_BOOL   CALLBACK NET_EBASE_ConnectToServer(HPR_INT32 iHandle, LPNET_EBASE_REGINFO lpRegInfo);


/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EBASE_ConnectToIOTServer(HPR_INT32 dwHandle, LPNET_EBASE_IOT_REGINFO lpStruRegInfo)
*  @brief  链接至服务端，兼容NET_EBASE_ConnectToServer
*  @param (in)	HPR_INT32 dwHandle为NET_EBASE_CreateIOTClient()返回值
*  @param (in)	LPNET_EBASE_IOT_REGINFO lpIOTRegInfo
*  @return HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL   CALLBACK NET_EBASE_ConnectToIOTServer(HPR_INT32 iHandle, LPNET_EBASE_IOT_REGINFO lpIOTRegInfo);


/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EBASE_DeystoryClient(HPR_INT32 dwHandle)
 *  @brief  销毁Ehome客户端
 *  @param (in)	HPR_INT32 dwHandle    
 *  @return HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
 */
NET_COM_API HPR_BOOL   CALLBACK NET_EBASE_DeystoryClient(HPR_INT32 iHandle);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EBASE_SetParam(HPR_UINT32 dwHandle, HPR_UINT32 dwType, HPR_VOIDPTR lpParam)
 *  @brief 设置参数，具体可设置项待定,萤石IOT协议不支持设备端本地设置心跳间隔（EBASE_PARAM_ALIVE_INTERVAL）
 *  @param (in)	HPR_INT32 dwHandle   NET_EBASE_CreateClient返回的句柄 
 *  @param (in)	HPR_UINT32 dwType     待定
 *  @param (in)	HPR_VOIDPTR lpParam   待定
 *  @param (in)	HPR_UINT32 dwSize     待定
 *  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
 */
NET_COM_API HPR_BOOL   CALLBACK NET_EBASE_SetParam(HPR_INT32 iHandle, HPR_UINT32 dwType, HPR_VOIDPTR lpParam, HPR_UINT32 dwSize);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EBASE_GetParam(HPR_INT32 iHandle, HPR_UINT32 dwType, HPR_VOIDPTR lpParam)
 *  @brief 获取参数,萤石IOT协议不支持设备端本地获取设备网络地址（EBASE_PARAM_LOCAL_IP）
 *  @param (in)	HPR_INT32 iHandle   NET_EBASE_CreateClient返回的句柄 
 *  @param (in)	HPR_UINT32 dwType   需要获取的参数类型 
 *  @param (in)	HPR_VOIDPTR lpParam 获取参数返回结果   
 *  @param (in)	HPR_UINT32 dwSize   lpParam指向的结构体长态度
 *  @return	NET_COM_API HPR_BOOL CALLBACK HPR_TRUE/HPR_FALSE 成功/失败
 */
NET_COM_API HPR_BOOL   CALLBACK NET_EBASE_GetParam(HPR_INT32 iHandle, HPR_UINT32 dwType, HPR_VOIDPTR lpParam, HPR_UINT32 dwSize);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EBASE_Reponse(HPR_UINT32 dwHandle, LPNET_EBASE_CLIENT_DATA lpClientData)
 *  @brief 响应服务端请求
 *  @param (in)	HPR_INT32 dwHandle    句柄，NET_EBASE_CreateClient返回值
 *  @param (in)	LPNET_EBASE_CLIENT_DATA lpClientData   待响应的数据 
 *  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
 */
NET_COM_API HPR_BOOL   CALLBACK NET_EBASE_Reponse(HPR_INT32 iHandle, LPNET_EBASE_CLIENT_DATA lpClientData);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EBASE_CheckHTTPSession(HPR_INT32 iHandle, char const * lpSession,HPR_UINT32 dwLen)
*  @brief 校验HTTP session
*  @param (in)	HPR_INT32 dwHandle    句柄，NET_EBASE_CreateClient返回值
*  @param (in)	char const * lpSession   待校验的数据
*  @param (in)	HPR_UINT32 dwLen   待校验数据的长度
*  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL   CALLBACK NET_EBASE_CheckHTTPSession(HPR_INT32 iHandle, char const * lpSession,HPR_UINT32 dwLen);


/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EBASE_SendPublish(HPR_INT32 iHandle, LPNET_EBASE_CLIENT_DATA lpClientData)
 *  @brief 在非响应服务端请求，并且不需要服务端响应的情况下，用于发送publish的接口
 *  @param (in)	HPR_INT32 iHandle    
 *  @param (in)	LPNET_EBASE_CLIENT_DATA lpClientData    
 *  @return	NET_COM_API HPR_BOOL CALLBACK
 */
NET_COM_API HPR_BOOL   CALLBACK NET_EBASE_SendPublish(HPR_INT32 iHandle, LPNET_EBASE_CLIENT_DATA lpClientData);


#define MAX_OTAP_ERROR_MSG_LEN 256

typedef struct tagNET_EBASE_OTAP_ERROR_INFO
{
    HPR_UINT32  dwSize;                             //[out]结构体长度
    HPR_UINT32  dwStatusCode;                       //[out]传输层错误码
    HPR_UINT8   szErrorMsg[MAX_OTAP_ERROR_MSG_LEN]; //[out]错误码描述,与ErrorNo对应
    HPR_UINT8   byRes[128];
}NET_EBASE_OTAP_ERROR_INFO, *LPNET_EBASE_OTAP_ERROR_INFO;


/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EBASE_GetOTAPErrorMsg(HPR_UINT32 dwErrorNo, LPNET_EBASE_OTAP_ERROR_INFO lpStruErrorInfo)
*  @brief 物联协议错误码转换为错误信息(错误码描述 + 传输层错误码)
*  @param (in)	HPR_UINT32 dwErrorNo
*  @param (out)	LPNET_EBASE_OTAP_ERROR_INFO lpStruErrorInfo
*  @return	NET_COM_API HPR_BOOL CALLBACK
*/
NET_COM_API HPR_BOOL CALLBACK NET_EBASE_GetOTAPErrorMsg(HPR_UINT32 dwErrorNo, LPNET_EBASE_OTAP_ERROR_INFO lpStruErrorInfo);

/** @fn	typedef HPR_BOOL(CALLBACK *OTAPGetErrMsgFun)(HPR_UINT32 dwErrorNo, LPNET_EBASE_OTAP_ERROR_INFO lpStruErrorInfo, HPR_VOIDPTR pUserData);
*  @brief 物联协议错误码转换为错误信息回调函数(错误码描述 + 传输层错误码)
*  @param (out)	HPR_UINT32 dwErrorNo
*  @param (in)	LPNET_EBASE_OTAP_ERROR_INFO lpStruErrorInfo
*  @param (out)	HPR_VOIDPTR pUserData
*  @return	NET_COM_API HPR_BOOL CALLBACK
*/
typedef HPR_BOOL(CALLBACK *OTAPGetErrMsgFun)(HPR_UINT32 dwErrorNo, LPNET_EBASE_OTAP_ERROR_INFO lpStruErrorInfo, HPR_VOIDPTR pUserData);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EBASE_SetOTAPErrorMsgCB(OTAPGetErrMsgFun pGetErrMsgCB, HPR_VOIDPTR pUserData);
*  @brief 设置物联协议错误码转换为错误信息回调函数，设置后不再使用库中的转换表，通过回调函数向用户获取错误信息
*  @param (in)	OTAPGetErrMsgFun pGetErrMsgCB 回调函数
*  @param (in)	HPR_VOIDPTR pUserData  用户数据
*  @return	NET_COM_API HPR_BOOL CALLBACK
*/
NET_COM_API HPR_BOOL CALLBACK NET_EBASE_SetOTAPErrorMsgCB(OTAPGetErrMsgFun pGetErrMsgCB, HPR_VOIDPTR pUserData);


typedef struct tagNET_EBASE_OBERSER
{
    HPR_UINT8   szModule[MAX_IOT_MODULE_LEN];            //模块
    HPR_UINT8   szDomain[MAX_IOT_DOMAIN_ID_LEN];         //领域类型，属于模块的子属性。（1）若szDomain为空，表示注册整个模块，（2）若szDomain不为空，则注册具体模块的领域，目前只有model模块使用本参数
    HPR_UINT8   byIdentifier[MAX_IOT_IDENTIFIER_LEN];    //[in]业务功能标识
    EBaseClientFunc fnCB;
    HPR_VOIDPTR pUserData;
}NET_EBASE_OBERSER, *LPNET_EBASE_OBERSER;

typedef struct tagNET_EBASE_REMOVE_OBERSER
{
    HPR_UINT8   szModule[MAX_IOT_MODULE_LEN];       //模块
    HPR_UINT8   szDomain[MAX_IOT_DOMAIN_ID_LEN];    //领域类型，属于模块的子属性。（1）若szDomain为空，表示注销整个模块，（2）若szDomain不为空，则注册具体模块的领域，目前只有model模块使用本参数
    HPR_UINT8   byIdentifier[MAX_IOT_IDENTIFIER_LEN];    //[in]业务功能标识
}NET_EBASE_REMOVE_OBERSER, *LPNET_EBASE_REMOVE_OBERSER;


NET_COM_API HPR_INT32   CALLBACK NET_EBASE_SetMessageCallBack(LPNET_EBASE_OBERSER lpMsgInfo);
NET_COM_API HPR_INT32   CALLBACK NET_EBASE_RemoveMsgCallBack(LPNET_EBASE_REMOVE_OBERSER lpMsgInfo);


/************************************************************************/
/*                            报警接口                                  */
/************************************************************************/
#define ALARM_SEND_SUCCESS 0
#define ALARM_IS_FILTERED  1
#define ALARM_SWITCH_CLOSE  2

typedef struct tagNET_EALARM_AlARMINFO
{
    HPR_UINT32  dwSize;         //[in]结构体长度
    HPR_UINT32  dwAlarmLen;     //[in]报警信息长度
    HPR_VOIDPTR pAlarmInfo;     //[in]报警信息
    HPR_UINT32  dwAlarmID;      //[out]报警ID
}NET_EALARM_AlARMINFO, *LPNET_EALARM_AlARMINFO;

typedef struct tagNET_EALARM_IOT_AlARMINFO
{
    HPR_UINT32  dwSize;             //[in]结构体长度
    HPR_UINT32  dwAlarmLen;         //[in]报警信息长度
    HPR_VOIDPTR pAlarmInfo;         //[in]报警信息
    HPR_UINT32  dwAlarmID;          //[out]报警ID
    NET_IOT_CLIENT_COMMAND struCmd; //[in]若byIsISAPIData为0，struCmd为报警topic信息,若byIsISAPIData为1，即透传ISAPI报文，struCmd为空
    HPR_UINT8   byIsISAPIData;      //[in]是否是ISAPI协议报文，0-否，1-是
    HPR_UINT8   byResult;           //[out]发送结果，设置SDK使能订阅规则有效。1.ALARM_SEND_SUCCESS-发送成功。2.ALARM_IS_FILTERED-未订阅该类型报警，被过滤。3.ALARM_SWITCH_CLOSE-萤石报警开关处于关闭状态
    HPR_UINT8   byRes[510];         //[Res]保留，用于字节对齐
}NET_EALARM_IOT_AlARMINFO, *LPNET_EALARM_IOT_AlARMINFO;


/** @fn	NET_COM_API HPR_UINT32 CALLBACK NET_EALARM_CreateClient(LPNET_EBASE_REGINFO lpRegInfo)
 *  @brief  创建报警客户端
 *  @param (in) iLoginHandle 调用NET_EBASE_CreateClient产生的句柄
 *  @param (in)	LPNET_EBASE_REGINFO lpRegInfo 创建EHome报警所需参数
 *  @return	HPR_INT32  -1表示失败，其他值表示句柄
 */
NET_COM_API HPR_INT32 CALLBACK NET_EALARM_CreateClient(HPR_INT32 iLoginHandle, LPNET_EBASE_REGINFO lpRegInfo);

/** @fn	NET_COM_API HPR_UINT32 CALLBACK NET_EALARM_CreateIOTClient(LPNET_EBASE_REGINFO lpRegInfo)
*  @brief  创建报警客户端
*  @param (in) iLoginHandle 调用NET_EBASE_CreateClient产生的句柄
*  @param (in)	LPNET_EBASE_REGINFO lpRegInfo 创建EHome报警所需参数
*  @return	HPR_INT32  -1表示失败，其他值表示句柄
*/
NET_COM_API HPR_INT32 CALLBACK NET_EALARM_CreateIOTClient(HPR_INT32 iLoginHandle, LPNET_EALARM_IOT_REGINFO lpRegInfo);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EALARM_DestroyClient(HPR_INT32 dwHandle)
 *  @brief 销毁报警客户端
 *  @param (in)	HPR_INT32 dwHandle  NET_EALARM_CreateClient返回值
 *  @return HPR_BOOL  HPR_TRUE/HPR_FALSE 成功/失败
 */
NET_COM_API HPR_BOOL CALLBACK NET_EALARM_DestroyClient(HPR_INT32 iHandle);
/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EALARM_SendAlarm(HPR_INT32 dwHandle, LPNET_EALARM_AlARMINFO lpAlarmInfo)
 *  @brief 发送报警信息
 *  @param (in)	HPR_INT32 dwHandle NET_EALARM_CreateClient返回值   
 *  @param (in)	LPNET_EALARM_AlARMINFO lpAlarmInfo  报警信息  
 *  @return HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
 */
NET_COM_API HPR_BOOL CALLBACK NET_EALARM_SendAlarm(HPR_INT32 iHandle, LPNET_EALARM_AlARMINFO lpAlarmInfo);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_EALARM_SendAlarm(HPR_INT32 dwHandle, LPNET_EALARM_AlARMINFO lpAlarmInfo)
*  @brief 发送报警信息，兼容NET_EALARM_SendAlarm
*  @param (in)	HPR_INT32 dwHandle NET_EALARM_CreateClient返回值
*  @param (in)	LPNET_EALARM_IOT_AlARMINFO lpAlarmInfo  报警信息
*  @return HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL CALLBACK NET_EALARM_SendIOTAlarm(HPR_INT32 iHandle, LPNET_EALARM_IOT_AlARMINFO lpAlarmInfo);

/************************************************************************/
/*                            接入萤石平台物理添加接口                                  */
/************************************************************************/
//pData对应结构体NET_EBASE_IOT_MSG
typedef HPR_VOID(CALLBACK *EBaseEZAccessCB)(HPR_INT32 dwHandle, HPR_VOIDPTR pData, HPR_VOIDPTR pUserData);

typedef struct tagNET_EBASE_EZACCESS_REG
{
    EBaseEZAccessCB fnCB;
    HPR_VOIDPTR pUserData;
    HPR_UINT8   byRes[256];                                //[Res]保留，用于字节对齐
}NET_EBASE_EZACCESS_REG;

//pBuf 对应结构体
//GetDeviceToken query NET_EBASE_IOT_EZACCESS_GET_DEV_TOKEN_REQ
//GetDeviceToken query_reply NET_EBASE_IOT_EZACCESS_GET_DEV_TOKEN_RSP
typedef struct tagNET_EBASE_IOT_MSG
{
    HPR_UINT8   byModule[MAX_IOT_MODULE_LEN];           //[in]
    HPR_UINT8   byChildID[MAX_IOT_CHILDID_LEN];              //[inout]子设备序列号, 如果资源是设备本身，这个参数不需要填写
    HPR_UINT8   byResourceID[MAX_IOT_RESOURCE_ID_LEN];      //[inout]资源id
    HPR_UINT8   byResourceType[MAX_IOT_RESOURCE_TYPE_LEN];  //[inout]资源类型
    HPR_UINT8   byMethod[MAX_IOT_METHOD_LEN];               //[inout]扩展内容，例如"model"中的 "domainid/identifier"字段
    HPR_UINT8   byMsgType[MAX_IOT_MSG_TYPE_LEN];            //[inout]消息类型"operate" "set" "get" "report" "query" "set_reply" "get_reply" "operate_reply" "report_reply" "query_reply"等
    HPR_UINT32  dwID;                                       //[inout]消息ID
    HPR_UINT32  dwLen;
    char        *pBuf;                                     //由具体的业务对应相应的结构体
    HPR_UINT8   byRes[512];                                //[Res]保留，用于字节对齐
}NET_EBASE_IOT_MSG, *LPNET_EBASE_IOT_MSG;

typedef enum NET_EBASE_IOT_EZACCESS_EXPIRE_TYPE
{
    ENUM_PARAM_PERMANENT = 0,       //永久有效,token 16位
    ENUM_PARAM_ONE_DAY = 1,         //24小时有效,token 6位
    ENUM_PARAM_THIRTY_DAY = 2,      //30天有效,token 8位
    ENUM_PARAM_NINETY_DAY = 3,      //90天有效,token 12位
}NET_EBASE_IOT_EZACCESS_EXPIRE_TYPE;

typedef struct tagNET_EBASE_IOT_EZACCESS_GET_DEV_TOKEN_REQ
{
    HPR_BOOL    bForce;                                      //[in]强制刷新 （默认 false）
    HPR_UINT32  dwExpire;                                    //[in]参见枚举NET_EBASE_IOT_EZACCESS_EXPIRE_TYPE
    HPR_UINT8   byRes[128];                                  //[Res]保留，用于字节对齐
}NET_EBASE_IOT_EZACCESS_GET_DEV_TOKEN_REQ;

typedef struct tagNET_EBASE_IOT_EZACCESS_GET_DEV_TOKEN_RSP
{
    HPR_UINT32  dwError;                                             //[out]0表示成功
    HPR_UINT8   byRegisterUrl[MAX_IOT_EZACCESS_REGISTERURL_LEN];     //[out]
    HPR_UINT8   byToken[MAX_IOT_EZACCESS_TOKEN_LEN];                 //[out]
    HPR_UINT8   byRes[128];                                          //[Res]保留，用于字节对齐
}NET_EBASE_IOT_EZACCESS_GET_DEV_TOKEN_RSP;


typedef struct tagNET_EBASE_IOT_EZACCESS_TOKEN_BIND_REQ
{
    HPR_UINT8   byToken[MAX_IOT_EZACCESS_TOKEN_LEN];         //[in]输入token
    HPR_UINT8   byRes[128];                                  //[Res]保留，用于字节对齐
}NET_EBASE_IOT_EZACCESS_TOKEN_BIND_REQ;

typedef struct tagNET_EBASE_IOT_EZACCESS_TOUCH_BIND_REQ
{
    HPR_UINT8   byToken[MAX_IOT_EZACCESS_TOKEN_LEN];         //[in]输入token
    HPR_UINT8   byRes[128];                                  //[Res]保留，用于字节对齐
}NET_EBASE_IOT_EZACCESS_TOUCH_BIND_REQ;

typedef struct tagNET_EBASE_IOT_CURINGDATA_CB
{
    EBaseCuringFunc fnCBCuringDataLoad;                         //[in]用户通过该回调接口将secretKey传给SDK，IOT模式有效
    EBaseCuringFunc fnCBCuringDataSave;                         //[in]SDK通过该回调接口将secretKey回调给用户，用户需将secretKey固化，任何情况下均不能丢失，否则无法再接入萤石云平台，IOT模式有效
}NET_EBASE_IOT_CURINGDATA_CB, *LPNET_EBASE_IOT_CURINGDATA_CB;

NET_COM_API HPR_BOOL CALLBACK NET_EBASE_SetEZVIZAccessMsgCB(NET_EBASE_EZACCESS_REG *pInfo);
NET_COM_API HPR_BOOL CALLBACK NET_EBASE_RemoveEZVIZAccessMsgCB();

NET_COM_API HPR_BOOL CALLBACK NET_EBASE_RegisterEZVIZAccess(HPR_INT32 iLoginHandle);
//暂不支持注销，萤石微内核不支持，对实际使用影响较小，
//NET_COM_API HPR_BOOL CALLBACK NET_EBASE_UnRegisterEZVIZAccess(HPR_INT32 iLoginHandle);

NET_COM_API HPR_BOOL CALLBACK NET_EBASE_SendEZVIZAccessMsg(HPR_INT32 iLoginHandle, LPNET_EBASE_IOT_MSG lpMsgInfo);


/************************************************************************/
/*                            大文件传输接口                                  */
/************************************************************************/
#define MAX_LF_SEND_BUF_LEN        4 * 1024 * 1024    // 大文件单次发送的最大buffer长度 4M


typedef enum enumLFCallBackType
{
    TYPE_LF_UNKNOW = 0,                //未知类型,               ClassType(pData) = NULL
    TYPE_LF_EXCEPTION = 1,             //出现异常,               ClassType(pOutBuffer) = HPR_UINT32 *,表示错误码
    TYPE_LF_UPLOAD_URL = 2,            //大文件上传URL           ClassType(pOutBuffer) = NET_ISUP_BASE_LF_UPLOAD_URL_INFO 其中buf为URL
    TYPE_LF_UPLOAD_DATA = 3,           //大文件上传              ClassType(pOutBuffer) = NET_ISUP_BASE_LF_UPLOAD_DATA_INFO 其中buf为文件数据
    TYPE_LF_UPLOAD_END = 4,            //大文件上传成功           ClassType(pOutBuffer) = HPR_UINT32 * , 传出sequence， ClassType(pInBuffer) = NET_ISUP_BASE_LF_UPLOAD_END_INFO  
    TYPE_LF_DOWNLOAD_URL = 5,          //大文件下载URL            ClassType(pOutBuffer) = NET_ISUP_BASE_LF_DOWNLOAD_CB_INFO
}NET_ISUP_BASE_LF_CALLBACK_TYPE;

/*当enumType为数据类型，具体参见NET_ISUP_BASE_LF_CALLBACK_TYPE */
typedef HPR_VOID(CALLBACK *LF_Func)(HPR_INT32 iHandle, NET_ISUP_BASE_LF_CALLBACK_TYPE enumType, HPR_VOIDPTR pOutBuffer, HPR_UINT32 dwOutLen, HPR_VOIDPTR pInBuffer, HPR_UINT32* dwInLen, HPR_VOIDPTR pUserData);

typedef struct tagNET_ISUP_BASE_LF_UPLOAD_URL_INFO
{
    void*                     pFileId;     //[out] 上传的文件fileid
    HPR_UINT32                dwFileIdLen; //[out] 上传fileid的
    HPR_UINT32                dwSequence;  //[out] 上传的sequence 用于是否判断同一个文件
    HPR_BOOL                  bResume;     //[out] 断点续传标记
    HPR_UINT32                dwStart;     //[out] 当bResume = True, 断点续传请求的开始位置，按字节算
    HPR_UINT32                dwEnd;       //[out] 当bResume = True, 断点续传请求的结束位置，按字节算
    HPR_UINT8                 byRes[128];
}NET_ISUP_BASE_LF_UPLOAD_URL_INFO, *LPNET_ISUP_BASE_LF_UPLOAD_URL_INFO;

typedef struct tagNET_ISUP_BASE_LF_UPLOAD_DATA_INFO
{
    void*                    pBuf;       //[out] 上传响应数据
    HPR_UINT32               dwBufLen;   //[out] buf长度
    HPR_UINT32               dwSequence; //[out] 上传文件标识sequence
    HPR_UINT8                byRes[124];
}NET_ISUP_BASE_LF_UPLOAD_DATA_INFO;


typedef struct tagNET_ISUP_BASE_LF_UPLOAD_END_INFO
{
    HPR_UINT32               dwResult;      //[in]  上传结果
    HPR_VOIDPTR              pUrl;          //[in]  上传成功之后返回的URL
    HPR_UINT32               dwUrlLen;      //[in]  URL长度
    HPR_UINT8                byRes[124];
}NET_ISUP_BASE_LF_UPLOAD_END_INFO; //由pInBuffer指向改结构体


typedef struct tagNET_ISUP_BASE_LF_DOWNLOAD_CB_INFO
{
    void*                     pUrl;       //[out] 请求下载的文件 uri
    HPR_UINT32                dwUrlLen;   //[out] uri长度
    HPR_UINT32                dwSequence; //[out] 下载的sequence 
    HPR_BOOL                  bResume;    //[out] 断点续传标记
    HPR_UINT32                dwStart;    //[out] 当bResume = True, 断点续传请求的开始位置，按字节算
    HPR_UINT32                dwEnd;      //[out] 当bResume = True, 断点续传请求的结束位置，按字节算
    HPR_UINT8                 byRes[128];
}NET_ISUP_BASE_LF_DOWNLOAD_CB_INFO;


typedef enum enumNET_ISUP_BASE_LF_BUSINESS_TYPE
{
    LF_SEND_DOWNLOAD_RSP_DATA = 0,    //下载发数据,对应传参结构体 NET_ISUP_BASE_LF_SEND_DATA
    LF_SEND_DOWNLOAD_RSP_RESULT = 1,  //下载结果， 对应传参结构体 NET_ISUP_BASE_LF_SEND_RESULT
}NET_ISUP_BASE_LF_BUSINESS_TYPE;

#define TAG_LF_TRANS_START 0x01  //开始标记（预留，用不到）
#define TAG_LF_TRANS_END   0x10  //结束标记, 最后一包数据时与数据buffer一起传入
#define TAG_LF_TRANS_DATA  0x00  //数据传输标记

//大文件传输结果

#define RESULT_LF_SUCCESS           0 //成功
#define RESULT_LF_UNKNOW_EXCEPTION     1 //未知异常
#define RESULT_LF_FILE_SIZE_LIMIT       2 //文件大小限制
#define RESULT_LF_INVALID_FILE        3 //非法文件
#define RESULT_LF_DEVICE_RESOURCE_ERROR       4  //设备空间不足
#define RESULT_LF_FILE_NOT_FOUND       5 // 文件不存在
#define RESULT_LF_RESUME_POSITION_ERROR       6 //断点续传位置错误
#define RESULT_LF_CALLBACK_PARAM_ERROR     7  //回调参数错误
#define RESULT_LF_TRANS_CHANNEL_BUSY      8 // 通道忙碌
#define RESULT_LF_TRANS_CHANNEL_NOT_FOUND     9 //通道不存在
#define RESULT_LF_TRANS_CHANNEL_NOT_ESTABLISH   10 //通道未建立
#define RESULT_LF_TRANS_READ_DATA_ERROR     11//读取数据失败
#define RESULT_LF_TRANS_SEND_DATA_ERROR     12 //发送数据失败


typedef struct tagNET_ISUP_BASE_LF_SEND_DATA
{
    void*                    pbuf;           //[in] 下载文件数据
    HPR_UINT32               dwbufLen;       //[in] buf长度，由信令端判断实际接收的长度
    HPR_UINT32               dwSequence;     //[in] 用户生成或下载请求URL的sequence，用同一个sequence代表同一个文件          
    HPR_UINT8                byTransStage;   //[in] 文件传输标志 0x010 结束标记  0x001 开始标记(其实不需要) 0x000 数据传输
    HPR_UINT8                byRes[127];
}NET_ISUP_BASE_LF_SEND_DATA, *LPNET_ISUP_BASE_LF_SEND_DATA;


typedef struct tagNET_ISUP_BASE_LF_SEND_RESULT
{
    HPR_UINT32               dwResult;  // [in] 当文件不存在时 返回 result = 6
    HPR_UINT32               dwSequence; //[in] 用户生成或下载请求URL的sequence，用同一个sequence代表同一个文件 
    HPR_UINT8                byRes[128];
}NET_ISUP_BASE_LF_SEND_RESULT, *LPNET_ISUP_BASE_LF_SEND_RESULT;



typedef struct tagNET_ISUP_BASE_LFINFO
{
    HPR_UINT32  dwSize;
    HPR_UINT8   byDeviceSerial[MAX_COMPLETE_SERIAL_LEN/*64*/];   //[in]设备序列号，设备型号+日期+短序号+扩展
    HPR_UINT8   bySubSerial[MAX_SERIAL_LEN/*12*/];               //[in]设备短序号,数字序列号
    HPR_UINT8   byDevName[MAX_DEVNAME_LEN/*64*/];                //[in]设备名称
    HPR_UINT8   byDeviceID[MAX_DEVICE_ID_LEN_V50/*256*/];        //[in]EHome协议中的DeviceID
    HPR_UINT8   byShareKey[MAX_EHOME50_KEY_LEN/*32*/];           //[in]sharekey长度，要求设备不存储EHomeKey，而是存sharekey:md5(md5(md5(SHA256(EHomeKey)+ deviceID)+www.88075998.com))  
    HPR_UINT8   byFirmwareVer[MAX_FIRMWARE_LEN/*12*/];           //[in]设备固件版本
    HPR_UINT8   byServerAdd[MAX_ADD_LEN/*256*/];                 //[in]服务端地址，支持IPv4，IPv6，域名
    HPR_UINT8   bySessionId[MAX_LF_SESSION_ID_LEN/*128*/];          //[in]CMS下发的sessionid
    HPR_UINT16  wServerPort;                                     //[in]服务端端口
    HPR_UINT8   byRes1[2];                                       //[Res]保留，用于字节对齐
    LF_Func     fnCB;                                            //[in]接收到的数据回调
    HPR_VOIDPTR pUserData;                                       //[in]用户数据
    HPR_UINT8   byRes[128];
}NET_ISUP_BASE_LFINFO, *LPNET_ISUP_BASE_LFINFO;


typedef enum tagENUM_SET_LF_PARAM_TYPE
{
    TYPE_LF_UNKNOW_PARAM = 0,
    TYPE_LF_SEND_TIMEOUT = 1,
    TYPE_LF_ALIVE_INTERVAL = 2,
}ENUM_SET_LF_PARAM_TYPE;

NET_COM_API HPR_INT32 CALLBACK  NET_ISUP_BASE_LFCreateClient(HPR_INT32 iLoginHandle, LPNET_ISUP_BASE_LFINFO lpLFInfo);

NET_COM_API HPR_BOOL CALLBACK NET_ISUP_BASE_LFDestroyClient(HPR_INT32 iHandle);

NET_COM_API HPR_BOOL CALLBACK NET_ISUP_BASE_LFSendData(HPR_INT32 iHandle, HPR_UINT32 dwType, HPR_VOIDPTR lpLFParam, HPR_UINT32 dwSize);

NET_COM_API HPR_BOOL CALLBACK NET_ISUP_BASE_LFSetParam(HPR_INT32 iHandle, HPR_UINT32 dwType, HPR_VOIDPTR lpLFParam, HPR_UINT32 dwSize);


#endif //_DEVEBASE_SRC_INTERFACE_EBASEINTERFACE_H_
