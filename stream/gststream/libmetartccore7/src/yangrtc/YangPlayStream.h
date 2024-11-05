#ifndef YANGWEBRTC_YANGRTCPLAYSTREAM_H_
#define YANGWEBRTC_YANGRTCPLAYSTREAM_H_

#include <yangrtc/YangRtcContext.h>
#include <yangrtc/YangPlay.h>

void yang_create_rtcplay(YangRtcContext *context, YangRtcPlay* play, YangRtpBuffer *rtpBuffer);
void yang_destroy_rtcplay(YangRtcPlay* play);

#endif /* YANGWEBRTC_YANGRTCPLAYSTREAM_H_ */

// By GST @Date