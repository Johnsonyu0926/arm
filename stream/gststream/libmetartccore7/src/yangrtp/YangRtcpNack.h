#ifndef SRC_YANGRTP_YANGRTCPNACK_H_
#define SRC_YANGRTP_YANGRTCPNACK_H_

#include <yangrtp/YangRtp.h>
#include <yangutil/yangtype.h>
#include <yangrtp/YangRtcpCommon.h>

typedef struct YangPidBlp {
    uint16_t pid;
    uint16_t blp;
    int32_t in_use;
} YangPidBlp;

#ifdef __cplusplus
extern "C" {
#endif

// 创建RTCP NACK
void yang_create_rtcpNack(YangRtcpCommon* comm, uint32_t pssrc);

// 销毁RTCP NACK
void yang_destroy_rtcpNack(YangRtcpCommon* comm);

// 初始化RTCP NACK
void yang_rtcpNack_init(YangRtcpCommon* comm, uint32_t pssrc);

// 清除RTCP NACK
void yang_rtcpNack_clear(YangRtcpNack* nack);

// 添加序列号到RTCP NACK
void yang_rtcpNack_addSn(YangRtcpNack* nack, uint16_t sn);

// 解码RTCP NACK
int32_t yang_decode_rtcpNack(YangRtcpCommon* comm, YangBuffer* buffer);

// 编码RTCP NACK
int32_t yang_encode_rtcpNack(YangRtcpCommon* comm, YangBuffer* buffer);

// 获取RTCP NACK的字节数
uint64_t yang_rtcpNack_nb_bytes();

#ifdef __cplusplus
}
#endif

#endif /* SRC_YANGRTP_YANGRTCPNACK_H_ */

// By GST @Date