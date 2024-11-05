#ifndef SRC_YANGRTP_YANGRTCPCOMPOUND_H_
#define SRC_YANGRTP_YANGRTCPCOMPOUND_H_

#include <yangrtp/YangRtcpCommon.h>
#include <yangutil/sys/YangVector.h>

yang_vector_declare(YangRtcpCommon)

typedef struct YangRtcpCompound {
    int32_t nb_bytes;
    int32_t nb_data;
    char* data;
    YangRtcpCommonVector rtcpVector;
} YangRtcpCompound;

#ifdef __cplusplus
extern "C" {
#endif

// 创建RTCP Compound
void yang_create_rtcpCompound(YangRtcpCompound* rtcp);

// 销毁RTCP Compound
void yang_destroy_rtcpCompound(YangRtcpCompound* rtcp);

// 解码RTCP Compound
int32_t yang_decode_rtcpCompound(YangRtcpCompound* rtcp, YangBuffer* buffer);

// 编码RTCP Compound
int32_t yang_encode_rtcpCompound(YangRtcpCompound* rtcp, YangBuffer* buffer);

// 获取RTCP Compound的字节数
uint64_t yang_rtcpCompound_nb_bytes();

// 添加RTCP到Compound
int32_t yang_rtcpCompound_add_rtcp(YangRtcpCompound* rtcps, YangRtcpCommon* rtcp);

// 清除RTCP Compound
void yang_rtcpCompound_clear(YangRtcpCompound* rtcps);

#ifdef __cplusplus
}
#endif

#endif /* SRC_YANGRTP_YANGRTCPCOMPOUND_H_ */

// By GST @Date