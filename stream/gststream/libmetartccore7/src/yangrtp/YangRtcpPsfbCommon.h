#ifndef SRC_YANGRTP_YANGRTCPPSFBCOMMON_H_
#define SRC_YANGRTP_YANGRTCPPSFBCOMMON_H_

#include <yangrtp/YangRtcpCommon.h>

#ifdef __cplusplus
extern "C" {
#endif

// 创建RTCP PSFB（Payload-Specific Feedback）
void yang_create_rtcpPsfb(YangRtcpCommon* comm);

// 解码RTCP PSFB
int32_t yang_decode_rtcpPsfb(YangRtcpCommon* comm, YangBuffer* buffer);

// 编码RTCP PSFB
int32_t yang_encode_rtcpPsfb(YangRtcpCommon* comm, YangBuffer* buffer);

// 获取RTCP PSFB的字节数
uint64_t yang_rtcpPsfb_nb_bytes(YangRtcpCommon* comm, YangBuffer* buffer);

#ifdef __cplusplus
}
#endif

#endif /* SRC_YANGRTP_YANGRTCPPSFBCOMMON_H_ */

// By GST @Date