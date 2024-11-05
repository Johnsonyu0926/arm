#ifndef _NPQ_DEFINE_H_
#define _NPQ_DEFINE_H_

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "NPQos.h"

enum class ProtoNPQMode {
    DIRECT_CONNECT = 0,
    BY_SMS = 1
};

// 错误码
constexpr int NET_ERR_NPQ_BASE_INDEX = 8000;
enum NPQError {
    PARAM = NET_ERR_NPQ_BASE_INDEX + 1,
    SYSTEM = NET_ERR_NPQ_BASE_INDEX + 2,
    GENERAL = NET_ERR_NPQ_BASE_INDEX + 3,
    PRECONDITION = NET_ERR_NPQ_BASE_INDEX + 4,
    NOTSUPPORT = NET_ERR_NPQ_BASE_INDEX + 5,
    NOTCALLBACK = NET_ERR_NPQ_BASE_INDEX + 100,
    LOADLIB = NET_ERR_NPQ_BASE_INDEX + 101,
    STEAM_CLOSE = NET_ERR_NPQ_BASE_INDEX + 104,
    MAX_LINK = NET_ERR_NPQ_BASE_INDEX + 110,
    STREAM_CFG = NET_ERR_NPQ_BASE_INDEX + 111
};

typedef int32_t (CALLBACK *HC_NPQ_Create)(NPQ_QOS_ROLE enType);
typedef int32_t (CALLBACK *HC_NPQ_Destroy)(int32_t id);
typedef int32_t (CALLBACK *HC_NPQ_Start)(int32_t id);
typedef int32_t (CALLBACK *HC_NPQ_Stop)(int32_t id);
typedef int32_t (CALLBACK *HC_NPQ_RegisterDataCallBack)(int32_t id, void (CALLBACK *fnRegisterDataCallBack)(int32_t id, int32_t iDataType, unsigned char* pData, uint32_t nDataLen, void* pUser), void* pUser);
typedef int32_t (CALLBACK *HC_NPQ_InputData)(int32_t id, int32_t iDataType, unsigned char* pData, uint32_t nDataLen);
typedef int32_t (CALLBACK *HC_NPQ_InputRawData)(int32_t id, MuxParam* pMuxParam, InputBuf* pstInputData);
typedef int32_t (CALLBACK *HC_NPQ_SetParam)(int32_t id, void* pParam);
typedef int32_t (CALLBACK *HC_NPQ_GetStat)(int32_t id, NPQ_MAIN_TYPE enMainType, void* pStat);
typedef int32_t (CALLBACK *HC_NPQ_SetNotifyParam)(int32_t id, NPQ_SET_NOTIFY_PARAM* pNotifyParam);
typedef int32_t (CALLBACK *HC_NPQ_GetVersion)(char* szVersion, int nLen);

struct NetUtilsNPQPackParam {
    uint32_t pack_size;
    uint32_t track_index;
    uint32_t syc_video_track;
    uint32_t system_format;
    uint32_t system_format_subtype;
    uint32_t video_format;
    uint32_t audio_format;
    uint32_t privt_format;
    uint32_t is_hik_stream;
    uint32_t encrypt_type;
    uint32_t frame_type;
    uint32_t time_stamp;
    float duration;
    uint32_t frame_num;
    HKSystemTime global_time;

    struct {
        uint16_t width_orig;
        uint16_t height_orig;
        uint16_t width_play;
        uint16_t height_play;
        float frame_rate;
        uint16_t interlace;
        uint16_t b_frame_num;
        uint32_t is_svc_stream;
    } video;

    struct {
        uint16_t channels;
        uint16_t bits_per_sample;
        uint32_t samples_rate;
        uint32_t bit_rate;
    } audio;

    struct {
        uint32_t privt_type;
        uint32_t data_type;
    } privt;
};

typedef void (CALLBACK *NPQTransCB)(int32_t id, int32_t iDataType, unsigned char* pData, uint32_t nDataLen, void* pUser);

struct NetUtilsNPQStateSingle {
    uint32_t dwRttUs;
    uint32_t dwRealRttUs;
    uint32_t dwBitRate;
    uint8_t byLossFraction;
    uint8_t byLossFraction2;
    uint8_t byRes[126];
};

struct NetUtilsNPQState {
    uint32_t dwSize;
    NetUtilsNPQStateSingle struAudioState;
    NetUtilsNPQStateSingle struVideoState;
    uint8_t byRes[256];
};

struct NetUtilsNPQCompressInfo {
    uint32_t dwSize;
    uint32_t dwMaxBitRate;
    uint8_t byRes[64];
};

struct RTPHeader {
    unsigned char count : 4;
    unsigned char extension : 1;
    unsigned char padding : 1;
    unsigned char version : 2;
    unsigned char payload : 7;
    unsigned char marker : 1;
    unsigned short sequence;
    unsigned int timestamp;
    unsigned int ssrc;
};

constexpr int COMMON_VIDEO_PT = 96;

#endif // _NPQ_DEFINE_H_
