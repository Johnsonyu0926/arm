/**
 * \file		ezDevSDK_thirdparty.h
 *
 * \brief		开放平台对外接口
 *
 * \copyright	HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
 *
 * \author		liuzhenhao
 *
 * \date		2017/3/10
 */

#ifndef H_EZDEVSDK_THIRDPARTY_H_
#define H_EZDEVSDK_THIRDPARTY_H_

#include "ezDevSDK_error.h"
#include "ezDevSDK_domain_def.h"
#include "ezDevSDK_thirdparty_def.h"

/**
 * @addtogroup Thirdparty_Module 开放平台领域模块
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /** 
     *  \brief		开放平台领域初始化
     *  \note       请在调用微内核模块函数ezDevSDK_Start()前调用
     *  \method		ezDevSDK_thirdparty_init
     *	\param[in]	info 始化参数
    *  \return 	成功返回0，失败返回非0
    *  \retval     0 - 成功
    *  \retval     非0 - 失败，\ref EZDEVSDK_THIRDPARTY_CODE_E
    */
    GLOBAL_API int ezDevSDK_thirdparty_init(const ezDevSDK_thirdparty_init_info *info);

    /** 
     *  \brief		开放平台领域反初始化
     *  \note       请在调用微内核模块函数ezDevSDK_Stop()后调用
     *  \method		ezDevSDK_thirdparty_fini
     *  \return 	成功返回0，失败返回非0
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_THIRDPARTY_CODE_E
     */
    GLOBAL_API int ezDevSDK_thirdparty_fini();

    /** 
     *  \brief		应用层获取开放平台的token
     *  \method		ezDevSDK_thirdparty_get_token
     *  \param[in] 	authorize 开放平台认证信息
     *  \return 	成功返回0，失败返回非0
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_THIRDPARTY_CODE_E
     */
    GLOBAL_API int ezDevSDK_thirdparty_get_token(ezDevSDK_thirdparty_authorize *authorize);

    /** 
     *  \brief		应用层主动向开放平台发送数据请求
     *  \method		ezDevSDK_thirdparty_send_data
     *  \deprecated	不再支持，不建议使用此接口
     *  \param[in] 	send_data 需要发送给开放平台的透传信息
     *  \param[out] msg_id 本次透传发送对应的唯一msg_id
     *  \param[out] msg_id_len 本次透传对应的msg_id的长度
     *  \return 	成功返回0，失败返回非0
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_THIRDPARTY_CODE_E
     */
    GLOBAL_API int ezDevSDK_thirdparty_send_data(ezDevSDK_thirdparty_data *send_data, char *msg_id, unsigned int *msg_id_len);

    /** 
     *  \brief		应用层向开放平台发送文件
     *  \method		ezDevSDK_thirdparty_send_file
     *  \deprecated	不再支持，不建议使用此接口
     *  \param[in] 	send_data 需要发送给开放平台的透传文件信息
     *  \param[out] url 本次透传发送开放平台返回的URL信息
     *  \param[out] url_len 本次透传发送开放平台返回的URL内容长度
     *  \param[out] ret_msg 本次透传发送开放平台返回的信息
     *  \param[out] ret_msg_len 本次透传发送开放平台返回的信息长度
     *  \return 	成功返回0，失败返回非0
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_THIRDPARTY_CODE_E
     */
    GLOBAL_API int ezDevSDK_thirdparty_send_file(ezDevSDK_thirdparty_data *send_data, char *url, int *url_len, char *ret_msg, int *ret_msg_len);

    /** 
     *  \brief		应用层向开放平台发送信令消息一般用于异步响应
     *  \method		ezDevSDK_thirdparty_send_transfer_data_msg
     *  \deprecated	不再支持，不建议使用此接口
     *  \param[in] 	msg 向开放平台发送的透传消息体
     *  \return 	成功返回0，失败返回非0
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_THIRDPARTY_CODE_E
     */
    GLOBAL_API int ezDevSDK_thirdparty_send_transfer_data_msg(ezDevSDK_thirdparty_transfer_data_msg *msg);

    /**
     * @brief 应用层向开放平台发送ISAPI信息
     * 
     * @param[in] send_isapi_msg 透传的消息体
     * @return 接口调用错误码
     * @retval 0 - 成功
     * @retval 非0 - 失败，\ref EZDEVSDK_THIRDPARTY_CODE_E
     */
    GLOBAL_API int ezDevSDK_thirdparty_send_isapi_msg(ezDevSDK_thirdparty_isapi_msg *send_isapi_msg);

    /** 
     *  \brief      应用层向开放平台发送ISAPI文件
     *  \method     ezDevSDK_thirdparty_send_isapi_file
     *  \param[in]  send_isapi_file 透传文件信息消息体
     *  \return     成功返回0，失败返回非0
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_THIRDPARTY_CODE_E
     */
    GLOBAL_API int ezDevSDK_thirdparty_send_isapi_file(ezDevSDK_thirdparty_isapi_file *send_isapi_file);

    /**
     * @brief 应用层向开放平台发送ISAPI文件
     * 
     * @param[in] send_isapi_file 发送的文件数据
     * @param[in] pattrs 文件发送相关属性
     * @return 接口调用错误码
     * @retval 0 - 成功
     * @retval 非0 - 失败，\ref EZDEVSDK_THIRDPARTY_CODE_E
     */
    GLOBAL_API int ezDevSDK_thirdparty_send_isapi_file_ex(ezDevSDK_thirdparty_isapi_file *send_isapi_file, ezDevSDK_isapi_send_attrs *pattrs);

    /** 
     *  \brief      设置ssl回调接口
     *  \method     ezDevSDK_thirdparty_set_ssl_callback
     *  \note       透传文件之前需要先设置ssl回调函数。SDK会把socket交给上层，然后由上层执行文件发送
     *  \param[in]   ssl_callback ssl回调函数
     *  \return  成功返回0，失败返回非0
     *  \retval     0 - 成功
     *  \retval     非0 - 失败，\ref EZDEVSDK_THIRDPARTY_CODE_E
     */
    GLOBAL_API int ezDevSDK_thirdparty_set_ssl_callback(ezDevSDK_thirdparty_ssl_callback *ssl_callback);
     /** 
     *  \brief      获取上传的服务器地址和端口信息
     *  \method     ezDevSDK_thirdparty_get_upload_server_info
     *  \note       透传文件之前需要获取服务器信息
     *  \param[out]  server 返回的服务器地址,预先传入128字节
     *  \param[in]   server_len,传入的server buf长度,长度不足时会返回 EZDEVSDK_THIRDPARTY_CODE_NOT_ENOUGH_MEMERY
     *  \param[in]  port  返回的端口信息
     *  \param[in]  channel通道号
     *  \param[in]  timeout_ms超时时间
     *  \return     成功返回0，失败返回非0
     *  \retval     非0 - 失败，\ref EZDEVSDK_THIRDPARTY_CODE_E
     */
    GLOBAL_API int ezDevSDK_thirdparty_get_upload_server_info(char* server, int server_len, int* port, int channel, int timeout_ms);
     /** 
     *  \brief      连接上传服务器
     *  \method     ezDevSDK_thirdparty_connect_server
     *  \param[in]  server 服务器地址
     *  \param[in]  port  服务器端口
     *  \note       
     *  \return     失败   返回NULL
     *  \retval     成功   返回handle
     */
    GLOBAL_API void* ezDevSDK_thirdparty_connect_server(const char *server, int port);

        /** 
     *  \brief      关闭连接())
     *  \method     ezDevSDK_thirdparty_connect_close
     *  \param[in]  handle 连接成功服务器返回的句柄
     *  \return     成功返回0，失败返回非0
     *  \retval     非0 - 失败
     */
    GLOBAL_API int  ezDevSDK_thirdparty_connect_close(void* handle);

    /** 
     *  \brief      上传文件接口(内部不做连接功能)
     *  \method     ezDevSDK_thirdparty_upload_file
     *  \param[in]  handle 连接成功服务器返回的句柄
     *  \param[in]  send_isapi_file  待发送的文件信息
     *  \param[in]  pattrs           接口发送属性
     *  \note       调用该接口前需要先连接服务器
     *  \return     handle 连接服务器返回的句柄handle
     *  \return     成功返回0，失败返回非0
     */
    GLOBAL_API int ezDevSDK_thirdparty_upload_file(void* handle, ezDevSDK_thirdparty_isapi_file *send_isapi_file, ezDevSDK_isapi_send_attrs *pattrs);

#ifdef __cplusplus
}
#endif

/*! \} */

#endif // H_EZDEVSDK_THIRDPARTY_H_
