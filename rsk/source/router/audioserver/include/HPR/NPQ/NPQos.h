#ifndef _NPQOS_H_
#define _NPQOS_H_

#if defined(_WIN32) || defined(_WIN64)
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

#include "IMuxDefine.h"
#include "NPQosAudioDec.h"

constexpr int NPQ_OK = 0x00000000;
constexpr int NPQERR_PARA = 0x80000001;
constexpr int NPQERR_SYSTEM = 0x80000002;
constexpr int NPQERR_GENRAL = 0x80000003;
constexpr int NPQERR_PRECONDITION = 0x80000004;
constexpr int NPQERR_NOTSUPPORT = 0x80000005;
constexpr int NPQERR_DATAFORMAT = 0x80000006;

enum class NPQ_QOS_TYPE {
    NACK = (1 << 0),
    FEC = (1 << 1),
    DEJITTER = (1 << 2),
    BW = (1 << 3),
    PLI = (1 << 4),
    SYNCHRONOUS = (1 << 5),
    ALL = 0xff
};

enum class NPQ_QOS_ROLE {
    RECEIVER,
    SENDER
};

enum class NPQ_MAIN_TYPE {
    VIDEO,
    AUDIO,
    PRIVATE,
    ALL = 0xFF
};

enum class NPQ_DATA_TYPE {
    RTP_VIDEO,
    RTP_AUDIO,
    RTP_PRIVATE,
    RTCP_VIDEO,
    RTCP_AUDIO,
    MULTIPLEX,
    CMD,
    MEDIA_INFO,
    STATE,
    RTCP_PRIVATE,
    HIKINFO
};

enum class NPQ_MEDIA_VIDEO_ENCODE_TYPE {
    H264,
    H265,
    MPEG4,
    SVAC,
    UNKNOWN = 0xff
};

struct NPQ_VIDEO_INFO {
    NPQ_MEDIA_VIDEO_ENCODE_TYPE enVideoCodeType;
    unsigned char res[252];
};

struct NPQ_AUDIO_INFO {
    NPQ_AUDIO_ENCODE_TYPE enAudioCodeType;
    unsigned int uSampleRate;
    unsigned int uBitRate;
    unsigned char res[244];
};

struct NPQ_MEDIA_INFO {
    int iVersion;
    NPQ_MAIN_TYPE enMediaType;
    union MEDIA_INFO {
        NPQ_VIDEO_INFO struVideoInfo;
        NPQ_AUDIO_INFO struAudioInfo;
        unsigned char res[256];
    } unMediaInfo;
};

enum class NPQ_CMD_TYPE {
    ENCODE,
    FORCE_I_FRAME
};

struct NPQ_ECODE_PARAM {
    int iVersion;
    int iMaxBitRate;
    int iResolution;
    int iEstimatedBW;
    unsigned char res[240];
};

struct NPQ_CMD {
    int iVersion;
    NPQ_CMD_TYPE enInfoType;
    union INFO_U {
        NPQ_ECODE_PARAM struEncode;
        unsigned char res[256];
    } unCmd;
};

struct NPQ_STAT {
    int iVersion;
    unsigned int nRttUs;
    unsigned int nRealRttUs;
    unsigned int nBitRate;
    unsigned char cLossFraction;
    unsigned char cLossFraction2;
    unsigned char nVideoPicQ;
    unsigned char nVideoRTQ;
    unsigned char nVideoFluQ;
    unsigned char nAudioTonQ;
    unsigned char nAudioRTQ;
    unsigned char nAudioFluQ;
    unsigned int nFrameRate;
    unsigned int nBitRateFec;
    unsigned char res[232];
};

enum class NPQ_PARAM_VALUE_TYPE {
    NULL_VALUE,
    SDP
};

struct NPQ_SENDER_PARAM {
    int bVaild;
    int iMinBitrate;
    int iMaxBitrate;
    int iStartBitrate;
    char bStopBWProbe;
    char nREDr;
    char res[110];
};

struct NPQ_RECEIVER_PARAM {
    int bVaild;
    int bG7221BigEndian;
    char res[120];
};

struct NPQ_PARAM {
    int iVersion;
    int m_type;
    int bVideo;
    int bAudio;
    NPQ_PARAM_VALUE_TYPE enValueType;
    void* pValue;
    union DIF_PARAM {
        NPQ_SENDER_PARAM struSender;
        NPQ_RECEIVER_PARAM struReceiver;
        unsigned char res[128];
    } unDifParam;
    unsigned char cVideoQosSet;
    unsigned char cAudioQosSet;
    char bEnableCpuSet;
    unsigned int uCpuSet;
    unsigned int uBytesAlign;
    int bPrivate;
    unsigned char res[105];
};

struct NPQ_SET_NOTIFY_PARAM {
    int iVersion;
    unsigned int nBitRate;
    bool bHaveBitrate;
    bool bHaveForceIframe;
    bool bForceIframe;
    bool bHaveScale;
    float fScale;
    unsigned char res[240];
};

enum class NPQ_STATE {
    INIT,
    PLAY,
    PAUSE
};

enum class NPQ_DATA_INFO_CODE {
    OK,
    FRAME_INCOMPLETE
};

enum class NPQ_DATA_FRAME_TYPE {
    UNKNOWN,
    VIDEO_I,
    VIDEO_P,
    AUDIO,
    PRIVAT
};

struct NPQ_DATA_INFO {
    int iVersion;
    int iDataType;
    unsigned int nBuffLen;
    unsigned char* pBuff;
    int iTimeStamp;
    int iDataInfoCode;
    int iFrameType;
    int iMainType;
    char res[96];
};

enum class NPQ_LOG_LEVEL {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    STAT,
    ERROR,
    UNKNOWN
};

enum class NPQ_SUB_MOUDLE {
    ALL,
    JITTER,
    NACK,
    FEC,
    BW,
    PACING,
    PLI,
    NETEQ,
    SYN
};

typedef void (__stdcall *NPQCb)(int id, int iDataType, unsigned char* pData, unsigned int nDataLen, void* pUser);
typedef void (__stdcall *NPQDataInfoCb)(int id, void* pDataInfo, unsigned int nDataInfoLen, void* pUser);
typedef void (__stdcall *NPQLogCb)(int iLogLevel, int iNPQSubModule, const char* format, void* varlist, void* pUser, int id);

NPQ_API int __stdcall NPQ_Create(NPQ_QOS_ROLE enType);
NPQ_API int __stdcall NPQ_Destroy(int id);
NPQ_API int __stdcall NPQ_Start(int id);
NPQ_API int __stdcall NPQ_Stop(int id);
NPQ_API int __stdcall NPQ_RegisterDataCallBack(int id, NPQCb pCallback, void* pUser);
NPQ_API int __stdcall NPQ_RegisterDataInfoCallBack(int id, NPQDataInfoCb pCallback, void* pUser);
NPQ_API int __stdcall NPQ_RegisterRecoveriedDataCallBack(int id, NPQCb pCallback, void* pUser);
NPQ_API int __stdcall NPQ_RegisterAudioDecFun(int id, NPQ_AUDIO_DECODE_FUN *pRegisterFun);
NPQ_API int __stdcall NPQ_InputData(int id, int iDataType, unsigned char* pData, unsigned int nDataLen);

NPQ_API int __stdcall NPQ_InputRawData(int id, MuxParam* pMuxParam, InputBuf* pstInputData);
NPQ_API int __stdcall NPQ_SetParam(int id, void* pParam);
NPQ_API int __stdcall NPQ_SetMediaSrcID(int id, NPQ_MAIN_TYPE enMainType, unsigned int nSrcID);
NPQ_API int __stdcall NPQ_SetMinMaxBitrate(int id, NPQ_MAIN_TYPE enMainType, unsigned int nMinBitRate, unsigned int nMaxBitRate);
NPQ_API int __stdcall NPQ_SetMediaDelay(int id, NPQ_MAIN_TYPE enMainType, unsigned int nDelayMs);
NPQ_API int __stdcall NPQ_SetMediaNTPTimeAndTimeStamp(int id, NPQ_MAIN_TYPE enMainType, long long nCaptureTime, unsigned int nTimeStamp);
NPQ_API int __stdcall NPQ_SetNotifyParam(int id, NPQ_SET_NOTIFY_PARAM* pNotifyParam);
NPQ_API int __stdcall NPQ_SetState(int id, NPQ_MAIN_TYPE enMainType, NPQ_STATE enState);
NPQ_API int __stdcall NPQ_GetStat(int id, NPQ_MAIN_TYPE enMainType, void* pStat);
NPQ_API int __stdcall NPQ_GetData(int id, int iDataType, unsigned char* pData, unsigned int nDataLen);
NPQ_API int __stdcall NPQ_GetDataInfo(int id, NPQ_DATA_TYPE enDataType, NPQ_DATA_INFO* pDataInfo);
NPQ_API int __stdcall NPQ_SetLogCallBack(NPQLogCb pLogCb, void* pUser);

#endif // _NPQOS_H_
