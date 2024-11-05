#ifndef _HC_CHILDDEV_H_
#define _HC_CHILDDEV_H_

#include "HCEBase.h"
#include "HPR_Config.h"
#include "HPR_Types.h"

constexpr int MAX_CHILDDEV_MANAGE_TAGS_KEY_LEN = 16;
constexpr int MAX_CHILDDEV_MANAGE_TAGS_VALUE_LEN = 16;
constexpr int MAX_CHILDDEV_MANAGE_CHILDDEVID_LEN = 40;
constexpr int MAX_CHILDDEV_MANAGE_VERSION_LEN = 72;
constexpr int MAX_CHILDDEV_MANAGE_DEVTYPE_LEN = 72;
constexpr int MAX_CHILDDEV_MANAGE_IP_LEN = 40;
constexpr int MAX_CHILDDEV_MANAGE_FIRMWAREIDCODE_LEN = 136;
constexpr int MAX_CHILDDEV_MANAGE_ENDPOINT_LEN = 264;

constexpr int MAX_OTAP_MSG_RSP_CODE_LEN = 16;
constexpr int MAX_OTAP_MSG_RSP_ERRMSG_LEN = 128;

enum IOT_CHILDDEV_MANAGE_MSG_TYPE {
    CHILDMANAGE_METHOD_UNKNOW = 0,           //未知类型
    CHILDMANAGE_METHOD_FULLCHILD = 1,        //子设备全量上报,pBodyData指向结构体:(1)设备请求IOT_CHILDDEV_REPORT_FULLCHILD（2）平台响应:IOT_CHILDDEV_REPOERT_REPLY
    CHILDMANAGE_METHOD_FULLCHILDPAGE = 2,    //子设备全量上报(分页)，pBodyData指向结构体:(1)设备请求IOT_CHILDDEV_REPORT_FULLCHILDPAGE（2）平台响应:IOT_CHILDDEV_REPOERT_REPLY
    CHILDMANAGE_METHOD_CHANGECHILD = 3,      //子设备变更上报，pBodyData指向结构体:(1)设备请求IOT_CHILDDEV_REPORT_CHANGECHILD（2）平台响应:IOT_CHILDDEV_REPOERT_REPLY
    CHILDMANAGE_METHOD_SEARCHDEVICE = 4,     //搜索网关下能发现的设备，pBodyData指向结构体:(1)平台请求IOT_CHILDDEV_SEARCH_OPERATE_DATA（2）设备响应:IOT_CHILDDEV_GENERAL_OPERATE_REPLY
    CHILDMANAGE_METHOD_ADDCHILD = 5,         //子设备添加，pBodyData指向结构体:(1)平台请求IOT_CHILDDEV_ADD_OPERATE_DATA（2）设备响应:IOT_CHILDDEV_GENERAL_OPERATE_REPLY
    CHILDMANAGE_METHOD_REMOVECHILD = 6,      //子设备删除，pBodyData指向结构体:(1)平台请求IOT_CHILDDEV_REMOVE_OPERATE_DATA（2）设备响应:IOT_CHILDDEV_GENERAL_OPERATE_REPLY
    CHILDMANAGE_METHOD_SEARCHCHILD = 7,      //查询子设备信息, pBodyData指向结构体:(1)平台请求IOT_CHILDDEV_SEARCH_CHILD_OPERATE_DATA（2）设备响应:IOT_CHILDDEV_GENERAL_OPERATE_REPLY
};

struct IOT_CHILDDEV_REPOERT_REPLY {
    HPR_UINT32 dwStatus;                                  
    char szCode[MAX_OTAP_MSG_RSP_CODE_LEN];
    char szErrMsg[MAX_OTAP_MSG_RSP_ERRMSG_LEN];
    HPR_UINT8 byRes[128];
};

struct IOT_CHILDDEV_SEARCH_OPERATE_DATA {
    HPR_UINT32 dwPageSize;
    HPR_UINT32 dwPageIndex;
    HPR_UINT8 byRes[128];
};

struct IOT_CHILDDEV_ADD_OPERATE_DEV_INFO {
    char szChildDevID[MAX_CHILDDEV_MANAGE_CHILDDEVID_LEN]; //子设备局部标识，32字节的数字+字母
    char szEndPoint[MAX_CHILDDEV_MANAGE_ENDPOINT_LEN];     //通过URI方式定义
    char byRes[256];
};

struct IOT_CHILDDEV_ADD_OPERATE_DATA {
    IOT_CHILDDEV_ADD_OPERATE_DEV_INFO* pDevInfo;                
    HPR_UINT32 dwDevNum;                  
    char byRes[128];
};

struct IOT_CHILDDEV_REMOVE_OPERATE_DEV_INFO {
    char szChildDevID[MAX_CHILDDEV_MANAGE_CHILDDEVID_LEN]; //子设备局部标识，32字节的数字+字母
    char byRes[256];
};

struct IOT_CHILDDEV_REMOVE_OPERATE_DATA {
    IOT_CHILDDEV_REMOVE_OPERATE_DEV_INFO* pDevInfo;                
    HPR_UINT32 dwDevNum;              
    char byRes[128];
};

struct IOT_CHILDDEV_SEARCH_CHILD_OPERATE_DATA {
    HPR_UINT32 dwPageSize;
    HPR_UINT32 dwPageIndex;
    HPR_UINT8 byRes[128];
};

struct IOT_CHILDDEV_MANAGE_SERVER_DATA {
    HPR_UINT8 byChildID[MAX_IOT_CHILDID_LEN];             //[out]子设备序列号
    HPR_UINT8 byResourceID[MAX_IOT_RESOURCE_ID_LEN];      //[out]资源id
    HPR_UINT8 byResourceType[MAX_IOT_RESOURCE_TYPE_LEN];  //[out]资源类型
    IOT_CHILDDEV_MANAGE_MSG_TYPE enumMsgType;             //[out]子设备业务消息类型，参见枚举IOT_CHILDDEV_MANAGE_MSG_TYPE 
    HPR_VOIDPTR pData;                                    //[out][readonly]请求体数据，不可在回调之外使用该指针
    HPR_UINT32 dwSequence;                                //[out]序列号，设备端响应本请求时，需将这个值原值设置给接口
    HPR_UINT8 byRes[256];
};

struct IOT_CHILDDEV_MANAGE_CLIENT_DATA {
    HPR_UINT8 byChildID[MAX_IOT_CHILDID_LEN];             //[in]子设备序列号
    HPR_UINT8 byResourceID[MAX_IOT_RESOURCE_ID_LEN];      //[in]资源id
    HPR_UINT8 byResourceType[MAX_IOT_RESOURCE_TYPE_LEN];  //[in]资源类型
    IOT_CHILDDEV_MANAGE_MSG_TYPE enumMsgType;             //[in]子设备业务消息类型，参见枚举IOT_CHILDDEV_MANAGE_MSG_TYPE 
    HPR_VOIDPTR pBodyData;                                //[in]请求体数据，参见枚举IOT_CHILDDEV_MANAGE_MSG_TYPE
    HPR_UINT32 dwSequence;                                //[inout]1.该结构体被NET_EBASE_ChildDevPublish使用，该变量为出参，用来关联响应消息。2.被NET_EBASE_ChildDevResponse使用时，为入参,填入回调中dwSequence
    HPR_UINT8 byRes[256];
};

struct IOT_CHILDDEV_REPORT_ADD_TAGS {
    HPR_UINT8 szKey[MAX_CHILDDEV_MANAGE_TAGS_KEY_LEN];
    HPR_UINT8 szValue[MAX_CHILDDEV_MANAGE_TAGS_VALUE_LEN];
    HPR_UINT8 byRes[32];
};

struct IOT_CHILDDEV_COMMEN_CHILDLIST {  
    char szChildDevID[MAX_CHILDDEV_MANAGE_CHILDDEVID_LEN];            //子设备局部标识，32字节的数字+字母
    char szVersion[MAX_CHILDDEV_MANAGE_VERSION_LEN];                  //设备版本
    char szType[MAX_CHILDDEV_MANAGE_DEVTYPE_LEN];                     //设备型号
    char szFirmwareIdCode[MAX_CHILDDEV_MANAGE_FIRMWAREIDCODE_LEN];    //设备固件标识码
    HPR_BOOL bConnected;                                              //连通状态
    IOT_CHILDDEV_REPORT_ADD_TAGS* pTags;                              //元数据标签，内部为key-value数组，参考结构体 IOT_CHILDDEV_REPORT_ADD_TAGS的数组
    HPR_UINT32 dwTagsNum;                                             //Tags的数目
    char byRes[128];
};

struct IOT_CHILDDEV_REPORT_DELETE_CHILDLIST {
    char szChildDevID[MAX_CHILDDEV_MANAGE_CHILDDEVID_LEN];  //子设备局部标识，32字节的数字+字母
    HPR_UINT32 dwDevIdLen;
    char byRes[124];
};

struct IOT_CHILDDEV_REPORT_FULLCHILD {
    HPR_UINT32 dwVersion;          //如果version=0全量上报
    IOT_CHILDDEV_COMMEN_CHILDLIST* pChildList;         
    HPR_UINT32 dwChildListNum;     //ChildList的数目
    char byRes[128];
};

struct IOT_CHILDDEV_REPORT_FULLCHILDPAGE {
    HPR_UINT32 dwVersion;          //版本号，如果version=0全量上报
    HPR_UINT32 dwPageSize;         //分页大小，根据分页大小设置定时器对分页上报进行监控，一页10秒
    HPR_UINT32 dwPageIndex;        //分页索引
    IOT_CHILDDEV_COMMEN_CHILDLIST* pChildList;         //参考结构体
    HPR_UINT32 dwChildListNum;     // childList的数目
    char byRes[128];
};

struct IOT_CHILDDEV_REPORT_CHANGECHILD {
    HPR_UINT32 dwDiffVersion; //比较的版本号
    HPR_UINT32 dwVersion;     //版本号
    IOT_CHILDDEV_COMMEN_CHILDLIST* pAddChildList; //新增子设备列表
    HPR_UINT32 dwAddListNum;
    IOT_CHILDDEV_REPORT_DELETE_CHILDLIST* pDelChildList; //删除子设备列表
    HPR_UINT32 dwDelListNum; 
    IOT_CHILDDEV_COMMEN_CHILDLIST* pModChildList; //修改子设备列表
    HPR_UINT32 dwModListNum;
    char byRes[128];
};

struct IOT_CHILDDEV_SEARCH_CHILDLIST_OPERATE_REPLY {
    char szChildDevID[MAX_CHILDDEV_MANAGE_CHILDDEVID_LEN];   //子设备局部标识，32字节的数字+字母
    char szVersion[MAX_CHILDDEV_MANAGE_VERSION_LEN];         //设备版本
    char szType[MAX_CHILDDEV_MANAGE_DEVTYPE_LEN];            //设备型号
    char szIP[MAX_CHILDDEV_MANAGE_IP_LEN];                   //能提供边缘或HUB设备访问的IP
    HPR_UINT32 dwPort;                                       //能提供边缘或HUB设备访问的端口
    char byRes[128];
};

struct IOT_CHILDDEV_GENERAL_OPERATE_REPLY {
    char szCode[MAX_OTAP_MSG_RSP_CODE_LEN];
    char szErrMsg[MAX_OTAP_MSG_RSP_ERRMSG_LEN];
    HPR_UINT32 dwStatus;
    HPR_UINT32 dwPageCount;                                          // 仅response 类型 CHILDMANAGE_METHOD_SEARCHDEVICE、CHILDMANAGE_METHOD_SEARCHCHILD 使用
    IOT_CHILDDEV_SEARCH_CHILDLIST_OPERATE_REPLY* pSearchDevList;     // 仅response 类型 CHILDMANAGE_METHOD_SEARCHDEVICE  使用 IOT_CHILDDEV_SEARCH_CHILDLIST_OPERATE_REPLY  
    IOT_CHILDDEV_COMMEN_CHILDLIST* pSearchChildList;                 // 仅response 类型 CHILDMANAGE_METHOD_SEARCHCHILD   使用 IOT_CHILDDEV_COMMEN_CHILDLIST  
    HPR_UINT32 dwDevListNum;                                         // pSearchDevList数组个数
    HPR_UINT32 dwChildListNum;                                       // pSearchChildList数组个数
    HPR_UINT8 byRes[116];
};

using ChildDevManageMsgCB = void (CALLBACK*)(HPR_INT32 iHandle, IOT_CHILDDEV_MANAGE_SERVER_DATA* pData, HPR_VOIDPTR pUserData);

struct IOT_CHILDDEV_MANAGE_MSG_CB_PARAM {
    ChildDevManageMsgCB fnCB;                  
    HPR_VOIDPTR pUser;                
    HPR_UINT8 byRes[128];
};

struct IOT_CHILDDEV_MANAGE_INIT_PARAM {
    IOT_CHILDDEV_MANAGE_MSG_CB_PARAM struMsgCB;
    HPR_UINT8 byRes[256];
};

NET_COM_API HPR_BOOL CALLBACK IOT_CHILDDEV_MANAGE_Init(IOT_CHILDDEV_MANAGE_INIT_PARAM *pParam);
NET_COM_API HPR_BOOL CALLBACK IOT_CHILDDEV_MANAGE_Fini();
NET_COM_API HPR_BOOL CALLBACK IOT_CHILDDEV_MANAGE_ReponseMsg(HPR_INT32 iHandle, IOT_CHILDDEV_MANAGE_CLIENT_DATA* pParam);
NET_COM_API HPR_BOOL CALLBACK IOT_CHILDDEV_MANAGE_PublishMsg(HPR_INT32 iHandle, IOT_CHILDDEV_MANAGE_CLIENT_DATA* pParam);

#endif // _HC_CHILDDEV_H_
