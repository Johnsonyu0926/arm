#ifndef SRC_YANGRTP_YANGRTCPSR_H_
#define SRC_YANGRTP_YANGRTCPSR_H_

#include <yangrtp/YangRtcpCommon.h>

// 创建RTCP SR（Sender Report）
void yang_create_rtcpSR(YangRtcpCommon* comm);

// 销毁RTCP SR
void yang_destroy_rtcpSR(YangRtcpCommon* comm);

// 解码RTCP SR
int32_t yang_decode_rtcpSR(YangRtcpCommon* comm, YangBuffer* buffer);

// 编码RTCP SR
int32_t yang_encode_rtcpSR(YangRtcpCommon* comm, YangBuffer* buffer);

// 获取RTCP SR的字节数
uint64_t yang_get_rtcpSR_nb_bytes(YangRtcpCommon* comm);

#endif /* SRC_YANGRTP_YANGRTCPSR_H_ */

// By GST @Date