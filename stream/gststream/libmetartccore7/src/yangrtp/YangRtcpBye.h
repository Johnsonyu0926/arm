#ifndef SRC_YANGRTP_YANGRTCPBYE_H_
#define SRC_YANGRTP_YANGRTCPBYE_H_

#include <yangutil/yangtype.h>
#include <yangrtp/YangRtcpCommon.h>

// 创建RTCP Bye
void yang_create_rtcpBye(YangRtcpCommon* comm);

// 销毁RTCP Bye
void yang_destroy_rtcpBye(YangRtcpCommon* comm);

// 编码RTCP Bye
int32_t yang_encode_rtcpBye(YangRtcpCommon* comm, YangBuffer* buffer);

// 解码RTCP Bye
int32_t yang_decode_rtcpBye(YangRtcpCommon* comm, YangBuffer* buffer);

#endif /* SRC_YANGRTP_YANGRTCPBYE_H_ */

// By GST @Date