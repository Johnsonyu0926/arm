、
#ifndef SRC_YANGRTP_YANGRTCPRPSI_H_
#define SRC_YANGRTP_YANGRTCPRPSI_H_

#include <yangrtp/YangRtcpPsfbCommon.h>
#include <yangutil/buffer/YangBuffer.h>

// 创建RTCP RPSI（Reference Picture Selection Indication）
void yang_create_rtcpRpsi(YangRtcpCommon* comm);

// 销毁RTCP RPSI
void yang_destroy_rtcpRpsi(YangRtcpCommon* comm);

// 解码RTCP RPSI
int32_t yang_decode_rtcpRpsi(YangRtcpCommon* comm, YangBuffer* buffer);

// 编码RTCP RPSI
int32_t yang_encode_rtcpRpsi(YangRtcpCommon* comm, YangBuffer* buffer);

// 获取RTCP RPSI的字节数
uint64_t yang_rtcpRpsi_nb_bytes();

#endif /* SRC_YANGRTP_YANGRTCPRPSI_H_ */

// By GST @Date