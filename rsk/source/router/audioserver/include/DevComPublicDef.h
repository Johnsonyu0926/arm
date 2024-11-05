/**  @file   DevComPublicDef.h
 *   @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved. 
 *   @brief  设备端协议组件一些公共的定义，不定义每个组件独有内容，不做实现
 *   
 *   @author shijianfengyf2
 *   @modify shijianfengyf2
 *   @date   2018/08/10
 *   
 *   @note:
 */
#ifndef _DEVECOM_INCLUDE_PUBLICDEF_H_
#define _DEVECOM_INCLUDE_PUBLICDEF_H_

#include "HPR/HPR_Config.h"
#include "HPR/HPR_Types.h"
#include "HPR/HPR_Utils.h"

#define MAX_PRINT_BYTES             1024               //一次打印最大字节数

//定义gcc以及g++编译器下，attribute的属性，当前设置为hidden
#ifndef NET_COM_ATTRIBUTE_VISIBILITY
    #define NET_COM_ATTRIBUTE_VISIBILITY //发现后端使用的编译器不支持这个功能，因此先定义成空的，以后有更进一步需求，再使用
    //#ifdef OS_POSIX
    //    #define NET_COM_ATTRIBUTE_VISIBILITY  __attribute__ ((visibility("hidden")))
    //#else  // !OS_POSIX
    //    #define NET_COM_ATTRIBUTE_VISIBILITY
    //#endif // !other os
#endif // NET_COM_ATTRIBUTE_VISIBILITY

//定义接口以及类导出宏
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

#define EHOME_COM_BASE_VERSION_LOG              ("version:EHomeCom version is %d.%d.%d.%d, %s.")
#define EHOME_COM_STREAM_VERSION_LOG            ("version:EHomeCom version is %d.%d.%d.%d, %s.") 
#define EHOME_COM_STORAGE_VERSION_LOG           ("version:EHomeCom version is %d.%d.%d.%d, %s.") 

#define EHOME_COM_BASE_BUILDTIME                     ("2020_12_21")
#define EHOME_COM_STREAM_BUILDTIME                   ("2020_12_21")
#define EHOME_COM_STORAGE_BUILDTIME                  ("2020_12_21")

#define EHOME_COM_BASE_BUILDVERSION             (MAKESDKBUILDVERSION(2, 2, 2, 6))
#define EHOME_COM_STREAM_BUILDVERSION           (MAKESDKBUILDVERSION(2, 2, 2, 6))
#define EHOME_COM_STORAGE_BUILDVERSION          (MAKESDKBUILDVERSION(2, 2, 2, 6))
#define EHOME_COM_MODEL_BUILDVERSION            "V2.0.0.0"

//生成sdk的build版本号
#define MAKESDKBUILDVERSION(main_version, sub_version, build_no1, build_no2) \
    HPR_UINT32( \
    (HPR_UINT32(main_version & 0xff) << 24) | \
    (HPR_UINT32(sub_version & 0xff) << 16) | \
    (HPR_UINT32(build_no1 & 0xff) << 8) | \
    (build_no2 & 0xff) \
                )

#define GET_SUB_FOUR_VERSION(dwBuildVersion) (0xff000000 & (dwBuildVersion))>>24, (0x00ff0000 & (dwBuildVersion))>>16,\
        (0x0000ff00 & (dwBuildVersion))>>8, (0x000000ff & (dwBuildVersion))

#ifdef OS_POSIX
#ifdef _DEBUG
#define VERSION_BASE            ("The HCEBase_Version is 2.2.2.6 build2022_08_03 debug")
#define VERSION_STREAM          ("The HCEStream_Version is 2.2.2.6 build2022_08_03 debug")
#define VERSION_MODEL          ("The HCIoTModel_Version is 2.2.2.6 build2022_08_03 debug")
#define VERSION_STORAGE          ("The HCIoTStorage_Version is 2.2.2.6 build2022_08_03 debug")
#else //release
#define VERSION_BASE            ("The HCEBase_Version is 2.2.2.6 build2022_08_03 release")
#define VERSION_STREAM          ("The HCEStream_Version is 2.2.2.6 build2022_08_03 release")
#define VERSION_MODEL          ("The HCIoTModel_Version is 2.2.2.6 build2022_08_03 release")
#define VERSION_STORAGE          ("The HCIoTStorage_Version is 2.2.2.6 build2022_08_03 release")
#endif //_DEBUG
#endif //OS_POSIX

//含有源长度的字符串格式化函数，windows/linux各提供了不同版本
#if defined OS_WINDOWS
    #define LOG_SNPRINTF        _snprintf
    #define LOG_VSNPRINTF       _vsnprintf
#elif defined OS_POSIX
    #define LOG_SNPRINTF        snprintf
    #define LOG_VSNPRINTF       vsnprintf
#endif

#ifndef LOG_LEVEL_MACRO
#define LOG_LEVEL_MACRO
typedef enum log_level
{
    ERROR_LEVEL = 1, DEBUG_LEVEL = 2, INFO_LEVEL = 3
}DEVCOM_LOG_LEVEL;
#endif

#define LOG_NOERROR                              0
#define LOG_ALLOCMEM_FAILED                     -1
#define LOG_SERVICE_NOTSTART                    -2
#define LOG_LEVEL_TOOLOW                        -3
#define LOG_BUFFER_NOTENOUGH                    -4
#define LOG_PARAMETER_ERROR                     -5
#define LOG_OVER_MAXPRINTBYTES                  -6
#define LOG_NOT_SUPPORTED                       -7

#define NEWOBJECT(p, x)            {p = new(::std::nothrow) x; }

#define NEW_PRIVATE_OBJECT(Class)  NEWOBJECT(m_pPrivate, Class##Private);
#define DEL_PRIVATE_OBJECT \
if (m_pPrivate != NULL)\
{\
    delete m_pPrivate; \
    m_pPrivate = NULL; \
    }


//#define EHOME_IP_LEN 64 //IPv4和IPv6地址，用字符串表示，64个字节已经足够

#define GET_DATA_NOTNULL(a, b)   (a != 0 ? a : b)   //按照参数顺序，优先取非0的数值

#define EHOME_MTU_LEN 1500

#ifndef  EBASE_NEWOBJECT
    #ifdef OS_WINDOWS
        #pragma warning(disable: 4291) //消除new(::std::nothrow)的VC编译警告
    #endif

    #define EBASE_NEWOBJECT
    #define NEWOBJECT(p, x)            {p = new(::std::nothrow) x; }
#endif

#define THREADSTACKSIZE             (128 * 1024)
#define BUF_SIZE_COMMON             (4 * 1024)

#define NEWARRAY(p, x, size)    {p = new(::std::nothrow) x[size];}
#define DELARRAY(p)             {if(NULL != p) delete[] p;} 

//定义此宏是为在某参数暂不使用时将其框选以使编译不出警告
#define UNREFERENCEDPARAMETER_INSDK(x)    {(x) = (x);}

#define SDK_INVALID_ID (-1)

#define SESSIONKEY_LEN		                                       16		    //	设备会话秘钥长度 ,默认是16个字节 

#define ARRAY_SIZE(A)    (sizeof(A)/sizeof((A)[0]))    // 数组大小

#define TOPIC_OBERSER_CNT 64
#define SSL_AUTH_INFO_PATH_LEN_MAX        128

//连接类型，与socket密切相关
typedef enum tagInterLinkTypeEnum
{
    ENUM_LINK_TYPE_ERROR = -1,
    ENUM_LINK_TYPE_TCP = 0,
    ENUM_LINK_TYPE_UDP =1,
    ENUM_LINK_TYPE_TCP_TLS = 2,
    ENUM_LINK_TYPE_UDP_DTLS = 3,
    ENUM_LINK_MULTICAST  =4
}LINK_TYPE_ENUM;

typedef enum enumSSL_AUTH_TYPE
{
    SSL_AUTH_TYPE_UNKNOOW = -1,
    SSL_AUTH_TYPE_TWO_WAY = 0,     //双向认证
    SSL_AUTH_TYPE_ONE_WAY = 1,   //单向认证，客户端认证服务端
    SSL_AUTH_TYPE_NO_AUTH = 2,   //无认证
}SSL_AUTH_TYPE;

//这个结构体可以扩展，因为只在core内部使用了
typedef struct tagConnectCond
{
    char*      pServerIP;
    HPR_UINT16 wServerPort;
    HPR_UINT16 wLocalPort;
    char*      pLocalIP;
    HPR_BOOL   bIsNeedBind;
    HPR_UINT32 dwConnectTimeOut;    //链接超时，默认3500ms，之所以不是3000ms，是因为3000ms是很多系统connect第一次重连的时候，多给500ms，相当于给了一次重连的机会
    HPR_UINT32 dwSendTimeOut;       //发送超时，默认5000ms
    HPR_UINT32 dwRecvTimeOut;       //接收超时，默认5000ms
    LINK_TYPE_ENUM linkType;        //连接类型
    char       szClientCertPath[SSL_AUTH_INFO_PATH_LEN_MAX];
    char       szClientPriKeyPath[SSL_AUTH_INFO_PATH_LEN_MAX];
    char       szCACert[SSL_AUTH_INFO_PATH_LEN_MAX];
    SSL_AUTH_TYPE  bySSLAuthType;  
}CONNECT_COND, *PCONNECT_COND;

typedef enum tagRecvErrorType
{
    ENUM_SOCKETCLOSE_ERROR = 0,     //NET_DVR_SOCKETCLOSE_ERROR
    ENUM_NETWORK_RECV_ERROR = -1,   //NET_DVR_NETWORK_RECV_ERROR
    ENUM_NETWORK_RECV_TIMEOUT = -2, //NET_DVR_NETWORK_RECV_TIMEOUT
}RECV_ERROR_TYPE_ENUM;

#endif // !_DEVECOM_INCLUDE_PUBLICDEF_H_
