#ifndef _HKMI_H_
#define _HKMI_H_

// 系统封装层定义
enum class MISystem {
    NULL_SYSTEM = 0x0000,
    HIK = 0x0001,
    MPEG2_PS = 0x0002,
    MPEG2_TS = 0x0003,
    RTP = 0x0004,
    MPEG4 = 0x0005,
    ASF = 0x0006,
    AVI = 0x0007,
    FLV = 0x0008,
    RTMP = 0x0009
};

// 视频编码类型定义
enum class MIVideo {
    NULL_VIDEO = 0x0000,
    HIK264 = 0x0001,
    MPEG2 = 0x0002,
    MPEG4 = 0x0003,
    MJPEG = 0x0004,
    AVC264 = 0x0100,
    AVC265 = 0x0005,
    SVAC = 0x0006,
    SVC264 = 0x0110,
    WMV9 = 0x0200,
    VC1 = 0x0201,
    REAL = 0x0300,
    MSMPEG4V1 = 0x0811,
    MSMPEG4V2 = 0x0812,
    MSMPEG4V3 = 0x0813,
    WMV1 = 0x0821,
    WMV2 = 0x0822
};

// 音频编码类型定义
enum class MIAudio {
    NULL_AUDIO = 0x0000,
    ADPCM = 0x1000,
    MPEG = 0x2000,
    AAC = 0x2001,
    RAW_DATA8 = 0x7000,
    RAW_UDATA16 = 0x7001,
    G711_U = 0x7110,
    G711_A = 0x7111,
    G722_1 = 0x7221,
    G723_1 = 0x7231,
    G726_U = 0x7260,
    G726_A = 0x7261,
    G726_16 = 0x7262,
    G729 = 0x7290,
    AMR_NB = 0x3000,
    WMA_V2 = 0x8011
};

// 私有数据
enum class MIPrivt {
    IVS_INFO = 0x0003,
    ITS_INFO = 0x0004,
    IAS_INFO = 0x0005,
    MD_INFO = 0x0008,
    INTER_DECODER = 0xBDBF
};

#endif  // _HKMI_H_
