/**	@NPQosAudioDec.h
*	@note HangZhou Hikvision Technology Co., Ltd. All Right Reserved.
*	@brief NPQ库接口定义
*   @apa_common.h、datatypedef.h和mem_tab.h的定义与研究院音频解码库的定义保持一致
*   @音频相关函数接口调用逻辑与研究院音频解码库的保持一致：唯一不同的是，增加了一个资源释放接口
*   @音频相关函数接口返回值含义与研究院音频解码库的保持一致
*
*	@author		huangfanfu
*	@date		2019/03/07
*
*	@note
*
*	@warning 
*/
#ifndef _NPQOSAUDIODEC_H_
#define _NPQOSAUDIODEC_H_

#if defined(WIN32)
#if defined(NPQOS_EXPORTS)
#define NPQ_API extern "C" __declspec(dllexport)
#else
#define NPQ_API extern "C" __declspec(dllimport)
#endif
#else

#ifndef __stdcall
#define __stdcall
#endif

#ifndef NPQ_API
#define NPQ_API extern "C"
#endif

#endif

#include "apa_common.h"
#include "datatypedef.h"
#include "HikCrypt/mem_tab.h"
#include "audio_codec_common.h"

/**	@enum	 NPQ_AUDIO_ENCODE_TYPE 
 *	@brief   音频编码类型
 *	@note	 
 */
enum NPQ_AUDIO_ENCODE_TYPE
{
	NPQ_I_AUDIO_G711A,
	NPQ_I_AUDIO_G711U,
	NPQ_I_AUDIO_G7221,              
	NPQ_I_AUDIO_G726,				
	NPQ_I_AUDIO_AAC,
    NPQ_I_AUDIO_AAC_LC,                 //暂不支持该类型
    NPQ_I_AUDIO_AAC_LD,
	NPQ_I_AUDIO_MPA,
	NPQ_I_AUDIO_OPUS,
	NPQ_I_AUDIO_PCM,
    NPQ_I_AUDIO_G7221C,                 //暂不支持该类型
    NPQ_I_AUDIO_MP3,                    //暂不支持该类型
    NPQ_I_AUDIO_G728,                   //暂不支持该类型
    NPQ_I_AUDIO_G729,                   //暂不支持该类型
	NPQ_I_AUDIO_UNKNOW = 0xff,
};

/**	@fn NPQAudioDec_GetMemSize
*	@brief  获取音频解码所需内存大小
*	@param  NPQ_AUDIO_ENCODE_TYPE audio_type  音频编码格式
*   @param  AUDIODEC_PARAM *param             参数结构指针
*   @param  MEM_TAB *mem_tab                  内存申请表
*	@param  void *pusr                        用户指针
*	@return 返回状态码
*/
typedef HRESULT(__stdcall* NPQAudioDec_GetMemSize)(NPQ_AUDIO_ENCODE_TYPE audio_type, AUDIODEC_PARAM *param, MEM_TAB *mem_tab, void *pusr);

/**	@fn NPQAudioDec_Create
*	@brief  创建音频解码模块
*	@param  NPQ_AUDIO_ENCODE_TYPE audio_type  音频编码格式
*   @param  AUDIODEC_PARAM *param             参数结构指针
*   @param  MEM_TAB *mem_tab                  内存申请表
*   @param  void **handle                     解码句柄
*	@param  void *pusr                        用户指针
*	@return 返回状态码
*/
typedef HRESULT(__stdcall* NPQAudioDec_Create)(NPQ_AUDIO_ENCODE_TYPE audio_type, AUDIODEC_PARAM *param, MEM_TAB *mem_tab, void **handle, void *pusr);

/**	@fn NPQAudioDec_Decode
*	@brief  音频解码
*   @param  void *handle                       解码句柄
*	@param  NPQ_AUDIO_ENCODE_TYPE audio_type   音频编码格式
*   @param  AUDIODEC_PROCESS_PARAM *proc_param 处理参数
*	@param  void *pusr                         用户指针
*	@return 返回状态码
*/
typedef HRESULT(__stdcall* NPQAudioDec_Decode)(void *handle, NPQ_AUDIO_ENCODE_TYPE audio_type, AUDIODEC_PROCESS_PARAM *proc_param, void *pusr);

/**	@fn NPQAudioDec_Release
*	@brief  释放音频解码模块资源
*   @param  void *handle                       解码句柄
*	@param  NPQ_AUDIO_ENCODE_TYPE audio_type   音频编码格式
*	@param  void *pusr                         用户指针
*	@return 返回状态码
*/
typedef HRESULT(__stdcall* NPQAudioDec_Release)(void *handle, NPQ_AUDIO_ENCODE_TYPE audio_type, void *pusr);


/**	@enum	 NPQ_AUDIO_DECODE_FUN 
 *	@brief   音频解码相关函数指针
 *	@note	 
 */
struct NPQ_AUDIO_DECODE_FUN
{
	NPQAudioDec_GetMemSize        pAudioDecGetMemSize;      //获取音频解码所需内存大小函数指针
	NPQAudioDec_Create            pAudioDecCreate;          //创建音频解码模块函数指针
	NPQAudioDec_Decode            pAudioDecDecode;          //音频解码函数指针
	NPQAudioDec_Release           pAudioDecRelease;         //释放音频解码模块资源函数指针
	void*                         pAudioDecUsr;             //音频解码用户指针
	char                          res[128];                 //保留字段
};

#endif//_NPQOSAUDIODEC_H_