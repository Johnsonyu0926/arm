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

// 音频编码类型
enum class NPQ_AUDIO_ENCODE_TYPE {
    G711A,
    G711U,
    G7221,
    G726,
    AAC,
    AAC_LC,
    AAC_LD,
    MPA,
    OPUS,
    PCM,
    G7221C,
    MP3,
    G728,
    G729,
    UNKNOWN = 0xff
};

// 获取音频解码所需内存大小
using NPQAudioDec_GetMemSize = HRESULT(__stdcall *)(NPQ_AUDIO_ENCODE_TYPE, AUDIODEC_PARAM*, MEM_TAB*, void*);

// 创建音频解码模块
using NPQAudioDec_Create = HRESULT(__stdcall *)(NPQ_AUDIO_ENCODE_TYPE, AUDIODEC_PARAM*, MEM_TAB*, void**, void*);

// 音频解码
using NPQAudioDec_Decode = HRESULT(__stdcall *)(void*, NPQ_AUDIO_ENCODE_TYPE, AUDIODEC_PROCESS_PARAM*, void*);

// 释放音频解码模块资源
using NPQAudioDec_Release = HRESULT(__stdcall *)(void*, NPQ_AUDIO_ENCODE_TYPE, void*);

// 音频解码相关函数指针
struct NPQ_AUDIO_DECODE_FUN {
    NPQAudioDec_GetMemSize pAudioDecGetMemSize;
    NPQAudioDec_Create pAudioDecCreate;
    NPQAudioDec_Decode pAudioDecDecode;
    NPQAudioDec_Release pAudioDecRelease;
    void* pAudioDecUsr;
    char res[128];
};

#endif // _NPQOSAUDIODEC_H_
