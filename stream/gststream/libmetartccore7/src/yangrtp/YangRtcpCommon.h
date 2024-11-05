#ifndef SRC_YANGRTP_YANGRTCPCOMMON_H_
#define SRC_YANGRTP_YANGRTCPCOMMON_H_

#include <yangrtp/YangRtcp.h>
#include <yangrtp/YangRtpConstant.h>
#include <yangutil/buffer/YangBuffer.h>

#if Yang_OS_WIN
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// 创建RTCP Common
void yang_create_rtcpCommon(YangRtcpCommon* comm);

// 销毁RTCP Common
void yang_destroy_rtcpCommon(YangRtcpCommon* comm);

// 解码RTCP Common
int32_t yang_decode_rtcpCommon(YangRtcpCommon* comm, YangBuffer* buffer);

// 编码RTCP Common
int32_t yang_encode_rtcpCommon(YangRtcpCommon* comm, YangBuffer* buffer);

// 获取RTCP Common的字节数
uint64_t yang_get_rtcpCommon_nb_bytes(YangRtcpCommon* comm);

// 解码RTCP Common头部
int32_t yang_decode_header_rtcpCommon(YangRtcpCommon* comm, YangBuffer* buffer);

// 编码RTCP Common头部
int32_t yang_encode_header_rtcpCommon(YangRtcpCommon* comm, YangBuffer* buffer);

// 编码RTCP头部
int32_t yang_encode_header_rtcpHeader(YangRtcpHeader* header, uint32_t ssrc, YangBuffer* buffer);

#ifdef __cplusplus
}
#endif

#endif /* SRC_YANGRTP_YANGRTCPCOMMON_H_ */

// By GST @Date