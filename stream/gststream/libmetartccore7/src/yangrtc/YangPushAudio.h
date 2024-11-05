#ifndef SRC_YANGRTC_YANGPUSHAUDIO_H_
#define SRC_YANGRTC_YANGPUSHAUDIO_H_

#include <yangrtc/YangPushH.h>

typedef struct {
    YangPushAudioRtp *push;
    int32_t (*on_audio)(void *session, YangPushAudioRtp* rtp, YangFrame* audioFrame);
} YangPushAudio;

void yang_create_pushAudio(YangPushAudio* push, YangRtpBuffer* audioBuffer);
void yang_destroy_pushAudio(YangPushAudio* push);

// By GST @Date

#endif /* SRC_YANGRTC_YANGPUSHAUDIO_H_ */