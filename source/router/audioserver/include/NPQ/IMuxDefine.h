/** @file       IMuxDefine.h
 *  @note       Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *  @brief      IO相关结构定义
 *
 *  @version    V1.0.1
 *  @author     辛安民
 *  @date       2015/10/13
 *  @note       NEW
 *
 */
#ifndef _IMUX_DEFINE_H_
#define _IMUX_DEFINE_H_

#include "HKMI.h"

// 最大轨道数量
#define MAX_TRACK_COUNT             8

// 输出包类型
#define UNKNOW_PACKET               0
#define VIDEO_PACKET                1
#ifndef AUDIO_PACKET
    #define AUDIO_PACKET            2
#endif
#ifndef PRIVT_PACKET
    #define PRIVT_PACKET            3
#endif
#define HIK_FILE_HEADER             4
#define HIK_FILE_INDEX              5

// 视频信息定义
#define MIN_FRAMERATE               (1.0 / 16)      // 最小帧率
#define COMMON_FRAMERATE            25              // 默认帧率
#define MAX_FRAMERATE               480             // 最大帧率480

// 缓存定义
#define DEFAULT_BUF_SIZE            (1024*1024*2)   // 通用的缓存大小
#define MAX_VALID_BUFSIZE           (1024*1024*32)  // 最大的缓存长度，用于判断数据长度是否合法

// 参数有效性检查
#define ARG_CHECK(arg)\
{\
    if(0 == (arg))\
    {\
        return IMUX_E_PARAMETER;\
    }\
}

// 函数返回值检查
#define ERR_CHECK(err)\
{\
    int ret = (err);\
    if(IMUX_OK != (ret))\
    {\
        return ret;\
    }\
}

// 时间
typedef struct _HK_SYSTEMTIME
{
    unsigned short wYear;
    unsigned short wMonth;
    unsigned short wDayOfWeek;
    unsigned short wDay;
    unsigned short wHour;
    unsigned short wMinute;
    unsigned short wSecond;
    unsigned short wMilliseconds;
}HK_SYSTEMTIME;

// 打包输入参数
typedef struct
{
    // 固定的参数
    unsigned int        pack_size;             // PS、TS和RTP设置最大包长，0为默认长度
    unsigned int        track_index;           // 轨道号
    unsigned int        syc_video_track;       // 关联的视频轨道号

    unsigned int        system_format;         // 目标封装格式
    unsigned int        system_format_subtype; // 目标类型的子格式，目前只使用于MP4，其他封装不适用

    unsigned int        video_format;          // 视频类型
    unsigned int        audio_format;          // 音频类型
    unsigned int        privt_format;          // 私有数据类型

    // 参考帧信息，非固定参数
    unsigned int        is_hik_stream;         // 是否符合海康定义
    unsigned int        encrypt_type;          // 加密类型
    unsigned int        frame_type;            // 当前帧类型 I/P/B/audio/privt
    unsigned int        time_stamp;            // 时间戳
    float               duration;              // 帧时长
    unsigned int        frame_num;             // 帧号
    HK_SYSTEMTIME       global_time;           // 全局时间（海康码流才有）

    // 视频参数
    struct
    {
        unsigned short  width_orig;            // 分辨率，原始宽
        unsigned short  height_orig;           // 分辨率，原始高
        unsigned short  width_play;            // 分辨率，裁剪宽
        unsigned short  height_play;           // 分辨率，裁剪高
        float           frame_rate;            // 帧率
        unsigned short  interlace;             // 是否场编码
        unsigned short  b_frame_num;           // 组模式中，B帧的个数
        unsigned int    is_svc_stream;         // 是否SVC码流
    }video;

    // 音频参数
    struct
    {
        unsigned short  channels;              // 声道
        unsigned short  bits_per_sample;       // 样位
        unsigned int    samples_rate;          // 采样率
        unsigned int    bit_rate;              // 比特率
    }audio;

    // 私有参数
    struct
    {
        unsigned int    privt_type;            // 私有类型
        unsigned int    data_type;             // 子类型
    }privt;
}MUX_PARAM;

// 输入数据结构体
typedef struct INPUT_BUF
{
    unsigned char*      pData;                 // 数据地址
    unsigned int        nDataLen;              // 数据长度
}INPUT_BUF;

// 输出数据结构体
typedef struct OUT_BUF
{
    unsigned char*      pData;                 // 数据地址
    unsigned int        nDataLen;              // 数据长度
}OUT_BUF;

// 打包输出信息
typedef struct
{
    unsigned int        target_id;             // 目标数据序号
    unsigned int        packet_type;           // 包类型
    unsigned int        is_key_frame;          // 0：非关键帧；1：关键帧
    unsigned int        time_stamp;            // 时间戳
}MUX_INFO;

#endif  // _IDEMUX_DEFINE_H_
