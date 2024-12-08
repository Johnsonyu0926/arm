/**  @file   HCIoTStorage.h
 *   @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved. 
 *   @brief  外部接口
 *   
 *   @author zhangqing2
 *   @modify 
 *   @date   2020/10/27
 *   
 *   @note:
 */

#ifndef _IOT_CLIENT_STORAGE_INTERFACE_H_
#define _IOT_CLIENT_STORAGE_INTERFACE_H_


#include "HCEBase.h"


#define MAX_IOT_STORAGE_ID_LEN  128
#define MAX_IOT_KEYWORD_LEN     256
#define MAX_IOT_FILE_PATH_LEN   260
#define MAX_IOT_SOTR_URL_LEN    1024
#define MAX_IOT_ALGORITHM_LEN   64
#define MAX_IOT_STORAGE_ENCRYPTION_KEY_LEN   32
#define MAX_IOT_STORAGE_EVENT_TIME_LEN       32

typedef struct tagNET_IOT_STOR_ABILITY_CFG
{
    HPR_UINT32  dwSize;             //[in]结构体大小
    HPR_UINT32  dwUploadCount;      //[in]上传文件总共支持的路数，默认1,最大128
    HPR_UINT32  dwDownloadCount;    //[in]下载文件总共支持的路数，默认1,最大128
    HPR_UINT8   byRes[512];
}NET_IOT_STOR_ABILITY_CFG, *LPNET_IOT_STOR_ABILITY_CFG;

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_IOT_STOR_Init(LPNET_IOT_STOR_ABILITY_CFG lpStruAbility)
*  @brief  初始化库
*  @param (in)	LPNET_IOT_STOR_ABILITY_CFG lpStruAbility
*  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL CALLBACK NET_IOT_STOR_Init(LPNET_IOT_STOR_ABILITY_CFG lpStruAbility);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_IOT_STOR_Fini()
*  @brief  反初始化库
*  @param (in)
*  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL CALLBACK NET_IOT_STOR_Fini();

/** @fn	NET_COM_API HPR_UINT32 CALLBACK NET_IOT_STOR_GetBuildVersion()
*  @brief 获取版本号
*  @return	HPR_UINT32 版本号
*/
NET_COM_API HPR_UINT32 CALLBACK NET_IOT_STOR_GetBuildVersion();


typedef struct tagNET_IOT_STOR_UPLOAD_RET
{
    HPR_UINT32  dwSize;                         //[out]结构体长度
    HPR_UINT8   byStorageId[MAX_IOT_STORAGE_ID_LEN];//[out]存储对象标识
    HPR_UINT32  dwExpireAfterDays;              //[out]文件过期时间，单位天
    HPR_UINT8   byRes[128];                     //[Res]保留，用于字节对齐
}NET_IOT_STOR_UPLOAD_RET, *LPNET_IOT_STOR_UPLOAD_RET;


typedef struct tagNET_IOT_STOR_UPLOAD_SUCC_INFO
{
    HPR_UINT32  dwSize;                         //[out]结构体长度
    HPR_UINT32  dwProcess;                      //[out]上传进度值
    HPR_UINT8   byStorageId[MAX_IOT_STORAGE_ID_LEN];//[out]存储对象标识
    HPR_UINT8   byEncryptionKey[MAX_IOT_STORAGE_ENCRYPTION_KEY_LEN];  //[out]图片加密密钥，若图片未加密，返回为空
    HPR_UINT8   byEventTime[MAX_IOT_STORAGE_EVENT_TIME_LEN];          //[out]图片加密时间，若图片未加密，返回为空
    HPR_UINT8   byRes[64];                     //[Res]保留，用于字节对齐
}NET_IOT_STOR_UPLOAD_SUCC_INFO, *LPNET_IOT_STOR_UPLOAD_SUCC_INFO;


typedef enum enumIoTStorUploadStatus
{
    UPLOAD_STATUS_UNKNOW = -1,          //未知类型， ClassType(pData) = NULL
    UPLOAD_STATUS_EXPECTION = 0,        //出现异常， ClassType(pData) = NET_IOT_STOR_EXCEPTION *，表示错误
    UPLOAD_STATUS_GET_STOR_INFO = 1,    //           ClassType(pData) = LPNET_IOT_STOR_UPLOAD_RET，表示平台返回上传信息
    UPLOAD_STATUS_PROCESSING = 2,       //上传中，   ClassType(pData) = HPR_UINT32 *，表示进度（0-100）
    UPLOAD_STATUS_SUCC = 3,             //上传完成， ClassType(pData) = LPNET_IOT_STOR_UPLOAD_SUCC_INFO，表示上传成功信息
}IoTStorUploadStatus;

typedef enum enumIoTStorUploadBizType
{
    UPLOAD_BIZ_TYPE_UNKNOW = -1,          //未知类型， ClassType(pData) = NULL
    UPLOAD_BIZ_TYPE_ISAPI = 0,            //图片所属的业务（事件）以isapi协议上传
}IoTStorUploadBizType;

typedef enum enumNET_IOT_STOR_PARAM_TYPE
{
    IOT_STOR_DOWNLOAD_INFO = 0,     //根据storageID获取下载url地址，handle传入注册句柄，输入参数结构体NET_IOT_STOR_DOWNLOAD_PARAM，输出参数结构体NET_IOT_STOR_DOWNLOAD_INFO
}NET_IOT_STOR_PARAM_TYPE;

typedef struct tagNET_IOT_STOR_EXCEPTION
{
    HPR_UINT32 dwErrorCode;     //组件错误码，参考HCEBase.h全局错误码
    HPR_UINT32 dwHttpState;    //http状态，为0或者200表示没有错误，其他请参考http标准错误码，dwErrorCode = DEV_COM_ERROR_HTTP_ERROR时有效
}NET_IOT_STOR_EXCEPTION, *LPNET_IOT_STOR_EXCEPTION;

typedef void (CALLBACK *IoTStorUploadClientCB)(HPR_INT32 lUploadHandle, IoTStorUploadStatus dwStatus, HPR_UINT32 dwLen, HPR_VOIDPTR pData, HPR_VOIDPTR pUserData);
typedef struct tagNET_IOT_STOR_UPLOAD_PARAM
{
    HPR_UINT32  dwSize;                                     //[in]结构体长度
    HPR_UINT8   bySubSerial[MAX_IOT_SERIAL_LEN];            //[in]子设备序列号,无作用，可不填
    HPR_UINT8   byResourceID[MAX_IOT_RESOURCE_ID_LEN];      //[in]资源id,无作用，可不填
    HPR_UINT8   byResourceType[MAX_IOT_RESOURCE_TYPE_LEN];  //[in]资源类型,无作用，可不填
    HPR_UINT8   byDomainID[MAX_IOT_DOMAIN_ID_LEN];          //[in]消息领域id,（1）事件通过OTAP协议上报时，该字段为所属事件的消息领域ID（2）enumBizType=UPLOAD_BIZ_TYPE_ISAPI时，表示事件通过OTAP透传ISAPI协议上报，该字段为字符串"ISAPIDataOverOTAP"
    HPR_UINT8   byIdentifier[MAX_IOT_IDENTIFIER_LEN];       //[in]业务标识（1）事件通过OTAP协议上报时，该字段为所属事件的业务标识（2）enumBizType=UPLOAD_BIZ_TYPE_ISAPI时，表示事件通过OTAP透传ISAPI协议上报，该字段为字符串"ISAPIDataOnEvent"
    HPR_UINT8   byKeyword[MAX_IOT_KEYWORD_LEN];             //[in]（1）事件通过OTAP协议上报时，该字段为存储对象在事件中的路径，采用JSON PATH，如$.payload.alarmPic（2）enumBizType=UPLOAD_BIZ_TYPE_ISAPI时，表示事件通过OTAP透传ISAPI协议上报，该字段为字符串"ISAPIDataOnPicture"
    HPR_UINT8   byReadFileMethod;                           //[in]读取文件方式，0-文件路径，1-完整文件buff，2-回调中分批输入文件buff（适用于大文件，保留，暂未实现）
    char        sFileName[MAX_IOT_FILE_PATH_LEN];           //[in]byReadFileMethod = 0（文件路径）时有效，内容为文件路径，SDK内部读取文件
    char*       pFileBuffer;                                //[in]byReadFileMethod = 1（文件buff）时有效，内容为文件buffer
    HPR_UINT32  dwFileBufLen;                               //[in]byReadFileMethod = 1（文件buff）时有效，内容为文件buffer长度
    HPR_UINT32  dwFileType;                                 //[in]文件类型，0-图片，1-二进制文件
    HPR_VOIDPTR fnCBRes;                                    //[in]byReadFileMethod = 2（回调输入文件buff）时有效（适用于大文件，保留，暂未实现）
    HPR_VOIDPTR pUserDataRes;                               //[in]用户数据（适用于大文件，保留，暂未实现）
    IoTStorUploadClientCB fnCallBack;                       //[out]回调上传进度、状态、结果
    HPR_VOIDPTR pUserData;                                  //[in]用户数据
    HPR_UINT8   byEncrypt;                                  //[in]数据加密, 0-不加密，1-加密
    IoTStorUploadBizType   enumBizType;                     //[in]参见枚举IoTStorUploadBizType
    HPR_UINT8   byRes[507];                                 //[Res]保留，用于字节对齐
}NET_IOT_STOR_UPLOAD_PARAM, *LPNET_IOT_STOR_UPLOAD_PARAM;

typedef struct tagNET_IOT_STOR_UPLOAD_SYN_PARAM
{
    HPR_UINT8   bySubSerial[MAX_IOT_SERIAL_LEN];            //[in]子设备序列号,无作用，可不填
    HPR_UINT8   byResourceID[MAX_IOT_RESOURCE_ID_LEN];      //[in]资源id,无作用，可不填
    HPR_UINT8   byResourceType[MAX_IOT_RESOURCE_TYPE_LEN];  //[in]资源类型,无作用，可不填
    HPR_UINT8   byDomainID[MAX_IOT_DOMAIN_ID_LEN];          //[in]消息领域id,（1）事件通过OTAP协议上报时，该字段为所属事件的消息领域ID（2）enumBizType=UPLOAD_BIZ_TYPE_ISAPI时，表示事件通过OTAP透传ISAPI协议上报，该字段为字符串"ISAPIDataOverOTAP"
    HPR_UINT8   byIdentifier[MAX_IOT_IDENTIFIER_LEN];       //[in]业务标识（1）事件通过OTAP协议上报时，该字段为所属事件的业务标识（2）enumBizType=UPLOAD_BIZ_TYPE_ISAPI时，表示事件通过OTAP透传ISAPI协议上报，该字段为字符串"ISAPIDataOnEvent"
    HPR_UINT8   byKeyword[MAX_IOT_KEYWORD_LEN];             //[in]（1）事件通过OTAP协议上报时，该字段为存储对象在事件中的路径，采用JSON PATH，如$.payload.alarmPic（2）enumBizType=UPLOAD_BIZ_TYPE_ISAPI时，表示事件通过OTAP透传ISAPI协议上报，该字段为字符串"ISAPIDataOnPicture"
    HPR_UINT8   byEncrypt;                                  //[in]数据加密, 0-不加密，1-加密
    HPR_UINT8   byRes1[2];                                   //[Res]
    HPR_UINT8   byReadFileMethod;                           //[in]读取文件方式，0-文件路径，1-完整文件buf
    char        sFileName[MAX_IOT_FILE_PATH_LEN];           //[in]byReadFileMethod = 0（文件路径）时有效，内容为文件路径，SDK内部读取文件
    char*       pFileBuffer;                                //[in]byReadFileMethod = 1（文件buf）时有效，内容为文件buffer
    HPR_UINT32  dwFileBufLen;                               //[in]byReadFileMethod = 1（文件buf）时有效，内容为文件buffer长度
    HPR_UINT32  dwFileType;                                 //[in]文件类型，0-图片，1-二进制文件
    HPR_UINT8   byStorageId[MAX_IOT_STORAGE_ID_LEN];        //[out]存储对象标识
    HPR_UINT32  dwExpireAfterDays;                          //[out]文件过期时间，单位天
    HPR_UINT8   byRes2[4];                                   //[Res]]
    HPR_UINT8   byEncryptionKey[MAX_IOT_STORAGE_ENCRYPTION_KEY_LEN];  //[out]加密密钥，byEncrypt=1时有效
    HPR_UINT8   byEventTime[MAX_IOT_STORAGE_EVENT_TIME_LEN];          //[out]加密时间，byEncrypt=1时有效
    IoTStorUploadBizType   enumBizType;                     //[in] 参见枚举IoTStorUploadBizType
    HPR_UINT8   byRes[508];                                 //[Res]保留，用于字节对齐
}NET_IOT_STOR_UPLOAD_SYN_PARAM, *LPNET_IOT_STOR_UPLOAD_SYN_PARAM;
/** @fn	NET_COM_API HPR_INT32 CALLBACK NET_IOT_STOR_CreateUploadClient()
*  @brief  创建上传客户端对象
*  @param (in) HPR_INT32 iLoginHandle 注册返回句柄
*  @return	HPR_INT32 其他/-1 成功/失败
*/
NET_COM_API HPR_INT32   CALLBACK NET_IOT_STOR_CreateUploadClient(HPR_INT32 iLoginHandle);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_IOT_STOR_UploadFile()
*  @brief  上传文件
*  @param (in) HPR_INT32 iUploadHandle 上传句柄
*  @param (in) LPNET_IOT_STOR_UPLOAD_PARAM lpUploadParam 上传参数
*  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL   CALLBACK NET_IOT_STOR_UploadFile(HPR_INT32 iUploadHandle, LPNET_IOT_STOR_UPLOAD_PARAM lpUploadParam);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_IOT_STOR_UploadFileSyn()
*  @brief  上传文件同步接口
*  @param (in) HPR_INT32 iUploadHandle 上传句柄
*  @param (in) LPNET_IOT_STOR_UPLOAD_SYN_PARAM lpUploadSynParam 上传参数
*  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL   CALLBACK NET_IOT_STOR_UploadFileSyn(HPR_INT32 iUploadHandle, LPNET_IOT_STOR_UPLOAD_SYN_PARAM lpUploadSynParam);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_IOT_STOR_UploadClose()
*  @brief  销毁上传客户端对象
*  @param (in) HPR_INT32 iUploadHandle 上传句柄
*  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL   CALLBACK NET_IOT_STOR_UploadClose(HPR_INT32 iUploadHandle);




typedef enum enumIoTStorDownloadStatus
{
    DOWNLOAD_STATUS_UNKNOW = -1,          //未知类型， ClassType(pData) = NULL
    DOWNLOAD_STATUS_EXPECTION = 0,        //出现异常， ClassType(pData) = NET_IOT_STOR_EXCEPTION *，表示错误
    DOWNLOAD_STATUS_DATA_SIZE = 1,       //下载的文件大小，ClassType(pData) = HPR_UINT32 *，表示文件大小，单位Byte
    DOWNLOAD_STATUS_DATA = 2,             //下载的数据，ClassType(pData) = HPR_VOIDPTR，表示下载的文件buff
    DOWNLOAD_STATUS_SUCC = 3,             //下载完成， ClassType(pData) = HPR_UINT32 *，表示进度（值为100）
}IoTStorDownloadStatus;

typedef void (CALLBACK *IoTStorDownloadClientCB)(HPR_INT32 lDownloadHandle, IoTStorDownloadStatus dwStatus, HPR_UINT32 dwLen, HPR_VOIDPTR pData, HPR_VOIDPTR pUserData);

typedef struct tagNET_IOT_STOR_DOWNLOAD_PARAM
{
    HPR_UINT32  dwSize;                                     //[in]结构体长度
    HPR_UINT8   bySubSerial[MAX_IOT_SERIAL_LEN];            //[in]子设备序列号
    HPR_UINT8   byResourceID[MAX_IOT_RESOURCE_ID_LEN];      //[in]资源id
    HPR_UINT8   byResourceType[MAX_IOT_RESOURCE_TYPE_LEN];  //[in]资源类型
    HPR_UINT8   byStorageId[MAX_IOT_STORAGE_ID_LEN];              //[in]存储对象标识
    IoTStorDownloadClientCB fnCallBack;                     //[out]回调下载的数据，仅获取URL时可不设置
    HPR_VOIDPTR pUserData;                                  //[in]用户数据
    HPR_UINT8   byEncryptionKey[MAX_IOT_STORAGE_ENCRYPTION_KEY_LEN];  //[in]图片加密密钥，若图片未加密，不填
    HPR_UINT8   byEventTime[MAX_IOT_STORAGE_EVENT_TIME_LEN];          //[in]图片加密时间，若图片未加密，不填
    HPR_UINT8   byRes[448];                                 //[Res]保留，用于字节对齐
}NET_IOT_STOR_DOWNLOAD_PARAM, *LPNET_IOT_STOR_DOWNLOAD_PARAM;

typedef struct tagNET_IOT_STOR_DOWNLOAD_SYN_PARAM
{
    HPR_UINT32  dwSize;                                     //[in]结构体长度
    HPR_UINT8   bySubSerial[MAX_IOT_SERIAL_LEN];            //[in]子设备序列号
    HPR_UINT8   byResourceID[MAX_IOT_RESOURCE_ID_LEN];      //[in]资源id
    HPR_UINT8   byResourceType[MAX_IOT_RESOURCE_TYPE_LEN];  //[in]资源类型
    HPR_UINT8   byStorageId[MAX_IOT_STORAGE_ID_LEN];              //[in]存储对象标识
    HPR_UINT32   dwBufferSize;              //[in]用户传入的，用于接收下载数据的buffer大小
    HPR_VOIDPTR pDataBuffer;            //[out]下载数据
    HPR_UINT32 dwDownloadSize;      //[out]下载数据的实际大小
    HPR_UINT8   byEncryptionKey[MAX_IOT_STORAGE_ENCRYPTION_KEY_LEN];  //[in]图片加密密钥，若图片未加密，不填
    HPR_UINT8   byEventTime[MAX_IOT_STORAGE_EVENT_TIME_LEN];          //[in]图片加密时间，若图片未加密，不填
    HPR_UINT8   byRes[480];                                 //[Res]保留，用于字节对齐
}NET_IOT_STOR_DOWNLOAD_SYN_PARAM, *LPNET_IOT_STOR_DOWNLOAD_SYN_PARAM;

typedef struct tagNET_IOT_STOR_DOWNLOAD_INFO
{
    HPR_UINT8   byUrl[MAX_IOT_SOTR_URL_LEN];
    HPR_BOOL    bEncrypt;
    HPR_UINT8   byAlgorithm[MAX_IOT_ALGORITHM_LEN];
    HPR_UINT8   byRes[256];
}NET_IOT_STOR_DOWNLOAD_INFO, *LPNET_IOT_STOR_DOWNLOAD_INFO;

/** @fn	NET_COM_API HPR_INT32 CALLBACK NET_IOT_STOR_CreateDownloadClient()
*  @brief  创建下载客户端对象
*  @param (in) HPR_INT32 iLoginHandle 注册返回句柄
*  @return	HPR_INT32 其他/-1 成功/失败
*/
NET_COM_API HPR_INT32   CALLBACK NET_IOT_STOR_CreateDownloadClient(HPR_INT32 iLoginHandle);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_IOT_STOR_DownloadFile()
*  @brief  下载文件
*  @param (in) HPR_INT32 iDownloadHandle 下载句柄
*  @param (in) LPNET_IOT_STOR_DOWNLOAD_PARAM lpDownloadParam 下载参数
*  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL   CALLBACK NET_IOT_STOR_DownloadFile(HPR_INT32 iDownloadHandle, LPNET_IOT_STOR_DOWNLOAD_PARAM lpDownloadParam);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_IOT_STOR_DownloadFile()
*  @brief  下载文件同步接口
*  @param (in) HPR_INT32 iDownloadHandle 下载句柄
*  @param (in) LPNET_IOT_STOR_DOWNLOAD_SYN_PARAM lpDownloadParam 下载参数
*  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL   CALLBACK NET_IOT_STOR_DownloadFileSyn(HPR_INT32 iDownloadHandle, LPNET_IOT_STOR_DOWNLOAD_SYN_PARAM lpDownloadParam);

/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_IOT_STOR_DownloadClose()
*  @brief  销毁下载客户端对象
*  @param (in) HPR_INT32 iUploadHandle 下载句柄
*  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL   CALLBACK NET_IOT_STOR_DownloadClose(HPR_INT32 iDownloadHandle);


/** @fn	NET_COM_API HPR_BOOL CALLBACK NET_IOT_STOR_GetParam()
*  @brief  存储模块获取参数接口
*  @param (in) HPR_INT32 iHandle 注册句柄或者存储句柄，与参数类型有关
*  @param (in) HPR_UINT32 dwType 参数类型，参考NET_IOT_STOR_PARAM_TYPE枚举
*  @param (in) HPR_VOIDPTR pParamIn 输入参数指针
*  @param (in) HPR_UINT32 dwInLen 输入参数大小
*  @param (out) HPR_VOIDPTR pParamOut 输出参数指针
*  @param (in) HPR_UINT32 dwOutLen 输出参数大小
*  @return	HPR_BOOL HPR_TRUE/HPR_FALSE 成功/失败
*/
NET_COM_API HPR_BOOL   CALLBACK NET_IOT_STOR_GetParam(HPR_INT32 iHandle, HPR_UINT32 dwType, HPR_VOIDPTR pParamIn, HPR_UINT32 dwInLen, HPR_VOIDPTR pParamOut, HPR_UINT32 dwOutLen);


#endif //_IOT_CLIENT_STORAGE_INTERFACE_H_
