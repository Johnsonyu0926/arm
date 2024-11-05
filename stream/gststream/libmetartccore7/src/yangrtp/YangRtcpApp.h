#ifndef SRC_YANGRTP_YANGRTCPAPP_H_
#define SRC_YANGRTP_YANGRTCPAPP_H_

#include <yangrtp/YangRtcpCommon.h>
#include <yangrtp/YangRtcpPsfbCommon.h>
#include <yangutil/buffer/YangBuffer.h>

// 创建RTCP App
void yang_create_rtcpApp(YangRtcpCommon* comm);

// 销毁RTCP App
void yang_destroy_rtcpApp(YangRtcpCommon* comm);

// 编码RTCP App
int32_t yang_encode_rtcpApp(YangRtcpCommon* comm, YangBuffer* buffer);

// 解码RTCP App
int32_t yang_decode_rtcpApp(YangRtcpCommon* comm, YangBuffer* buffer);

// 判断是否为RTCP App
int32_t yang_rtcpapp_isApp(uint8_t* data, int32_t nb_data);

// 获取RTCP App的字节数
uint64_t yang_rtcpapp_nb_bytes(YangRtcpCommon* comm);

// 设置RTCP App的负载
int32_t yang_rtcpapp_set_payload(YangRtcpCommon* comm, uint8_t* payload, int32_t len);

#endif /* SRC_YANGRTP_YANGRTCPAPP_H_ */

// By GST @Date