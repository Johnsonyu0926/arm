#ifndef SRC_YANGRTP_YANGRTCPRR_H_
#define SRC_YANGRTP_YANGRTCPRR_H_

#include <yangrtp/YangRtcpCommon.h>
#include <yangutil/buffer/YangBuffer.h>

// 创建RTCP RR（Receiver Report）
void yang_create_rtcpRR(YangRtcpCommon* comm);

// 销毁RTCP RR
void yang_destroy_rtcpRR(YangRtcpCommon* comm);

// 解码RTCP RR
int32_t yang_decode_rtcpRR(YangRtcpCommon* comm, YangBuffer* buffer);

// 编码RTCP RR
int32_t yang_encode_rtcpRR(YangRtcpCommon* comm, YangBuffer* buffer);

// 获取RTCP RR的字节数
uint64_t yang_rtcpRR_nb_bytes(YangRtcpCommon* comm);

// 设置发送者的NTP时间戳
void yang_rtcpRR_set_sender_ntp(YangRtcpCommon* comm, uint64_t ntp);

// 设置发送者的NTP时间戳（别名）
void yang_set_sender_ntp(YangRtcpCommon* comm, uint64_t ntp);

#endif /* SRC_YANGRTP_YANGRTCPRR_H_ */

// By GST @Date