#ifndef SRC_YANGRTP_YANGRTCPPLI_H_
#define SRC_YANGRTP_YANGRTCPPLI_H_

#include <yangrtp/YangRtcpPsfbCommon.h>

// 创建RTCP PLI（Picture Loss Indication）
void yang_create_rtcpPli(YangRtcpCommon* comm);

// 销毁RTCP PLI
void yang_destroy_rtcpPli(YangRtcpCommon* comm);

// 编码RTCP PLI
int32_t yang_encode_rtcpPli(YangRtcpCommon* comm, YangBuffer* buffer);

// 解码RTCP PLI
int32_t yang_decode_rtcpPli(YangRtcpCommon* comm, YangBuffer* buffer);

// 获取RTCP PLI的字节数
uint64_t yang_rtcpPli_nb_bytes(YangRtcpCommon* comm);

#endif /* SRC_YANGRTP_YANGRTCPPLI_H_ */

// By GST @Date