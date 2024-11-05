/** @file       HKMI.h
 *  @note       Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *  @brief      海康媒体信息定义
 *
 *  @version    V1.0.1
 *  @author     辛安民
 *  @date       2015/08/13
 *  @note       NEW
 *
 */
#ifndef _HKMI_H_
#define _HKMI_H_


// 系统封装层定义
#define MI_SYSTEM_NULL         0x0000            // 不支持的封转格式
#define MI_SYSTEM_HIK          0x0001            // 海康封装层
#define MI_SYSTEM_MPEG2_PS     0x0002            // PS封装，Program Stream
#define MI_SYSTEM_MPEG2_TS     0x0003            // TS封装，Transport Stream
#define MI_SYSTEM_RTP          0x0004            // RTP封装
#define MI_SYSTEM_MPEG4        0x0005            // ISO Base Media Format and Extension，.MP4, .AVC, .MOV, .3GP
#define MI_SYSTEM_ASF          0x0006            // ASF
#define MI_SYSTEM_AVI          0x0007            // AVI
#define MI_SYSTEM_FLV          0x0008            // FLV
#define MI_SYSTEM_RTMP         0x0009            // RTMP

// 视频编码类型定义
#define MI_VIDEO_NULL          0x0000            // 无视频编码
#define MI_VIDEO_HIK264        0x0001            // 海康H.264和部分较早标注H264都可以用这个定义
#define MI_VIDEO_MPEG2         0x0002            // 标准MPEG2
#define MI_VIDEO_MPEG4         0x0003            // 标准MPEG4
#define MI_VIDEO_MJPEG         0x0004            // Motion Jpeg
#define MI_VIDEO_AVC264        0x0100            // 标准H264/AVC
#define MI_VIDEO_AVC265        0x0005            // 标准H.265保留
#define MI_VIDEO_SVAC          0x0006            // 标准SVAC保留
#define MI_VIDEO_SVC264        0x0110            // SVC保留
#define MI_VIDEO_WMV9          0x0200            // WMV9
#define MI_VIDEO_VC1           0x0201            // VC1
#define MI_VIDEO_REAL          0x0300            // REAL保留
#define MI_VIDEO_MSMPEG4V1     0x0811            // 微软定义的mpeg编码格式version1
#define MI_VIDEO_MSMPEG4V2     0x0812            // 微软定义的mpeg编码格式version2
#define MI_VIDEO_MSMPEG4V3     0x0813            // 微软定义的mpeg编码格式version3
#define MI_VIDEO_WMV1          0x0821            // 微软定义的WMV编码格式version1
#define MI_VIDEO_WMV2          0x0822            // 微软定义的WMV编码格式version2

// 音频编码类型定义
#define MI_AUDIO_NULL          0x0000            // 没有音频
#define MI_AUDIO_ADPCM         0x1000            // ADPCM
#define MI_AUDIO_MPEG          0x2000            // MPEG系列音频，解码器能自适应各种MPEG音频
#define MI_AUDIO_AAC           0x2001            // AAC编码
#define MI_AUDIO_RAW_DATA8     0x7000            // 采样率为8k的原始数据
#define MI_AUDIO_RAW_UDATA16   0x7001            // 采样率为16k的原始数据，即L16
#define MI_AUDIO_G711_U        0x7110            // G711 U Law
#define MI_AUDIO_G711_A        0x7111            // G711 A Law
#define MI_AUDIO_G722_1        0x7221            // G722.1
#define MI_AUDIO_G723_1        0x7231            // G723.1
#define MI_AUDIO_G726_U        0x7260            // G726 U Law
#define MI_AUDIO_G726_A        0x7261            // G726 A Law
#define MI_AUDIO_G726_16       0x7262
#define MI_AUDIO_G729          0x7290            // G729
#define MI_AUDIO_AMR_NB        0x3000
#define MI_AUDIO_WMA_V2        0x8011            // WMA_V2

// 私有数据
#define MI_PRIVT_IVS_INFO      0x0003
#define MI_PRIVT_ITS_INFO      0x0004
#define MI_PRIVT_IAS_INFO      0x0005
#define MI_PRIVT_MD_INFO       0x0008
#define MI_INTER_DECODER       0xBDBF            // 智能码流

#endif  // _HKMI_H_
