#ifndef SRC_YANGRTP_YANGRTCPSLI_H_
#define SRC_YANGRTP_YANGRTCPSLI_H_

#include <yangrtp/YangRtcpPsfbCommon.h>
#include <yangutil/buffer/YangBuffer.h>

// 创建RTCP SLI（Slice Loss Indication）
void yang_create_rtcpSli(YangRtcpCommon* comm, uint32_t sender_ssrc);

// 销毁RTCP SLI
void yang_destroy_rtcpSli(YangRtcpCommon* comm);

// 编码RTCP SLI
int32_t yang_encode_rtcpSli(YangRtcpCommon* comm, YangBuffer* buffer);

// 解码RTCP SLI
int32_t yang_decode_rtcpSli(YangRtcpCommon* comm, YangBuffer* buffer);

#endif /* SRC_YANGRTP_YANGRTCPSLI_H_ */

// By GST @Date