/** \file devp2p.h
 * \copyright Copyright (c) 2017 by HangZhou Ezviz Network Co., Ltd. All Right Reserved.
 * \brief This file describes all APIs of DevP2P module used by device layer.
 * 
 * \author Jianglin Yang
 * \date   2017/03/06
 *
 * \code history 
 *          [2017/03/06] Jianglin Yang
 *          Initialize the file
 *
 *          [2017/06/07] Jianglin Yang
 *          Add Interface to support Package Drop logic
 *
 *          [2018/03/14] Jianglin Yang
 *          Add Interface to support P2P Link default key setting
 *          Remove ticket function parameter for function P2P_SetP2PServer()
 *
 *          [2018/04/21] Jianglin Yang
 *          Add Interface for up-level to notify inside in case of write data process abnormal abort
 */ 

#ifndef _DEVP2P_H
#define _DEVP2P_H

#include "devp2pdef.h"

// platform dependant macro definition
#if defined (_WIN32) || defined(_WIN64)

#ifdef DEVP2P_EXPORTS
#define DEVP2P_API __declspec(dllexport)
#else
#define DEVP2P_API __declspec(dllimport)
#endif

#define CALLBACK __stdcall

#else

#define DEVP2P_API 
#define CALLBACK

#endif

#ifdef __cplusplus
extern "C" {
#endif 

/**
 * \brief Initialize DevP2P SDK module
 *
 * \return DEVP2P_ER_NoERROR for success, otherwise for error
 *
 * \attention Please call this function in main thread
 */
DEVP2P_API int CALLBACK P2P_Initialize(void);

/**
 * \brief DeInitialize DevP2P SDK module
 *
 * \return DEVP2P_ER_NoERROR for success, otherwise for error
 *
 * \attention Please call this function in main thread
 */

DEVP2P_API int CALLBACK P2P_DeInitialize(void); 

/**
 * \brief Set parameters for DevP2P SDK module
 *
 * \param devParam [in] Parameters need up-level to set
 *
 * \return DEVP2P_ER_NoERROR for success, otherwise for error
 *
 * \attention This fuction should be called before P2P_StartService().
              If parameter is changed outside, it need set again by this function.
 */
DEVP2P_API int CALLBACK P2P_SetParam(const DevParam* devParam);

/**
 * \brief Get parameters from DevP2P SDK module
 *
 * \param devParam [in/out] Parameters need inner to set
 *
 * \return DEVP2P_ER_NoERROR for success, otherwise for error
 *
 * \attention Parameter devParam's memory should allocated by caller.
 */
DEVP2P_API int CALLBACK P2P_GetParam(DevParam* devParam);

/**
 * \brief Set config for DevP2P SDK module
 *
 * \param key [in] the configure key
 *
 * \param value [in] the configure value
 *
 * \return DEVP2P_ER_NoERROR for success, otherwise for error
 *
 * \attention This fuction should be called before P2P_StartService().
              If parameter is changed outside, it need set again by this function.
 */
DEVP2P_API int CALLBACK P2P_SetConfig(ConfigType iType, int iValue);

/**
 * \brief Set callback function for DevP2P SDK module
 *
 * \param cbFuns [in] Callback functions need set by up-level
 *
 * \return DEVP2P_ER_NoERROR for success, otherwise for error
 *
 * \attention This fuction should be called before P2P_StartService().
 */
DEVP2P_API int CALLBACK P2P_SetCallback(const CallbackFuns* cbFuns);

/**
 * \brief Set socket properties for DevP2P SDK module
 *
 * \param sockProperty [in] Socket property parameters need up-level to set
 *
 * \return DEVP2P_ER_NoERROR for success, otherwise for error
 *
 * \attention This fuction should be called before P2P_StartService()
 */
DEVP2P_API int CALLBACK P2P_SetSocketOpt(const SocketProperty* sockProperty);

/**
 * \brief Set p2p server and ticket parameters
 *
 * \param p2pServerList [in] P2P server list information
 * \param serverCount [in] P2P server list count number
 * \param keyInfo [in] Enrypt/decrypt key information
 *
 * \return DEVP2P_ER_NoERROR for success, otherwise for error
 *
 * \attention This fuction should be called before P2P_StartService().
              If parameter is changed outside, it need set again by this function.
 */
DEVP2P_API int CALLBACK P2P_SetP2PServer(const P2PServer* p2pServerList, const int serverCount, const KeyInfo* keyInfo);

/**
 * \brief Set encrypt key for p2p link with client
 *
 * \param encKey [in] Encrypt key for p2p link with client, unsigned char arrary
 * \param keyLen [in] Length of encrypt key array
 * \param keyVer [in] Key version
 *
 * \return DEVP2P_ER_NoERROR for success, otherwise for error
 *
 * \attention This fuction should be called before P2P_StartService().
              If parameter is changed outside, it need set again by this function.
 */

DEVP2P_API int CALLBACK P2P_SetP2PLinkEncKey(const unsigned char* encKey, int keyLen, int keyVer);

/**
 * \brief Start DevP2P Service task
 *
 * \return DEVP2P_ER_NoERROR for success, otherwise for error
 *
 * \attention Please call this function in main thread
 */
DEVP2P_API int CALLBACK P2P_StartService(void);

/**
 * \brief Stop DevP2P Service task
 *
 * \return DEVP2P_ER_NoERROR for success, otherwise for error
 *
 * \attention Please call this function in main thread
 */
DEVP2P_API int CALLBACK P2P_StopService(void);

/**
 * \brief Write stream data to client
 *
 * \param playSessionId [in] Play session id in struct P2PPlaySession, which send to up-level in fnAsynNotify()
 * \param buffer [in] Stream buffer
 * \param bufSize [in] Size of stream buffer
 * \param dropPack [in/out] Whether current buffer send to client success or failed. For preview/playback/talk, current
 *                          package need to drop, and for download, current package need send again.
 *
 * \return DEVP2P_ER_NoERROR for success, otherwise for error
 *
 * \attention The first package should be StreamHeader, then stream data following
 */
DEVP2P_API int CALLBACK P2P_WriteData(const int playSessionId, const char* buffer, const int bufSize, bool* dropPack); 

/**
 * \brief Notify inside in case of up-level write data process abnormal abort. 
 *
 * \param playSessionId [in] Play session id in struct P2PPlaySession, which send to up-level in fnAsynNotify()
 *
 * \return void
 *
 * \attention Up-level need to call this function in case of stream send process breaked by abnormal reason, like bad
              stream data, device upgrading, and so on.
 */
DEVP2P_API void CALLBACK P2P_WriteDataAbort(const int playSessionId);

/**
 * \brief Get SRT performance data by play session id
 *
 * \param playSessionId [in] Play session id in struct P2PPlaySession, which send to up-level in fnAsynNotify()
 * \param srtPerf [in/out] SRT performance data
 *
 * \return DEVP2P_ER_NoERROR for success, otherwise for error
 *
 * \attention Parameter srtPerf's memory should allocated by caller
 */
DEVP2P_API int CALLBACK P2P_GetSRTPerf(const int playSessionId, SRTPerf* srtPerf);

/**
 * \brief Print out link resource and play session information, used for debug
 *
 * \return void
 */
DEVP2P_API void CALLBACK P2P_PrintOutLinkInfo(void);

#ifdef __cplusplus
}
#endif

#endif

