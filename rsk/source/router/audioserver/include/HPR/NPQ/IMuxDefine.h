#ifndef _IMUX_DEFINE_H_
#define _IMUX_DEFINE_H_

#include "HKMI.h"

constexpr int MAX_TRACK_COUNT = 8;

// 输出包类型
enum class PacketType {
    UNKNOWN = 0,
    VIDEO = 1,
    AUDIO = 2,
    PRIVT = 3,
    HIK_FILE_HEADER = 4,
    HIK_FILE_INDEX = 5
};

// 视频信息定义
constexpr double MIN_FRAMERATE = (1.0 / 16);
constexpr int COMMON_FRAMERATE = 25;
constexpr int MAX_FRAMERATE = 480;

// 缓存定义
constexpr size_t DEFAULT_BUF_SIZE = (1024 * 1024 * 2);
constexpr size_t MAX_VALID_BUFSIZE = (1024 * 1024 * 32);

#define ARG_CHECK(arg) if (0 == (arg)) { return IMUX_E_PARAMETER; }
#define ERR_CHECK(err) if (IMUX_OK != (err)) { return (err); }

struct HKSystemTime {
    unsigned short wYear;
    unsigned short wMonth;
    unsigned short wDayOfWeek;
    unsigned short wDay;
    unsigned short wHour;
    unsigned short wMinute;
    unsigned short wSecond;
    unsigned short wMilliseconds;
};

struct MuxParam {
    unsigned int pack_size;
    unsigned int track_index;
    unsigned int syc_video_track;
    unsigned int system_format;
    unsigned int system_format_subtype;
    unsigned int video_format;
    unsigned int audio_format;
    unsigned int privt_format;
    unsigned int is_hik_stream;
    unsigned int encrypt_type;
    unsigned int frame_type;
    unsigned int time_stamp;
    float duration;
    unsigned int frame_num;
    HKSystemTime global_time;

    struct {
        unsigned short width_orig;
        unsigned short height_orig;
        unsigned short width_play;
        unsigned short height_play;
        float frame_rate;
        unsigned short interlace;
        unsigned short b_frame_num;
        unsigned int is_svc_stream;
    } video;

    struct {
        unsigned short channels;
        unsigned short bits_per_sample;
        unsigned int samples_rate;
        unsigned int bit_rate;
    } audio;

    struct {
        unsigned int privt_type;
        unsigned int data_type;
    } privt;
};

struct InputBuf {
    unsigned char* pData;
    unsigned int nDataLen;
};

struct OutBuf {
    unsigned char* pData;
    unsigned int nDataLen;
};

struct MuxInfo {
    unsigned int target_id;
    PacketType packet_type;
    unsigned int is_key_frame;
    unsigned int time_stamp;
};

#endif  // _IMUX_DEFINE_H_
