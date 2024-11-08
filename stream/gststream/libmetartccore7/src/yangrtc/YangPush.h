#ifndef YANGRTC_YANGPUSH_H_
#define YANGRTC_YANGPUSH_H_

#include <yangrtc/YangRtcSession.h>

int32_t yang_send_avpacket(YangRtcSession *session, YangRtpPacket *pkt, YangBuffer *pbuf);
int32_t yang_send_nackpacket(YangRtcContext *context, char *data, int32_t nb);

// By GST @Date

#endif /* SRC_YANGRTC_YANGPUSH_H_ */