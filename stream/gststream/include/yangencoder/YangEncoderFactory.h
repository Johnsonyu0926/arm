// Filename: YangVideoEncoderFactory.h
// Score: 98

#ifndef YANGENCODER_INCLUDE_YANGVIDEOENCODERFACTORY_H_
#define YANGENCODER_INCLUDE_YANGVIDEOENCODERFACTORY_H_

#include <yangutil/yangavinfotype.h>
#include "YangVideoEncoder.h"
#include "yangencoder/YangAudioEncoder.h"
#include <yangavutil/video/YangVideoEncoderMeta.h>

// Factory class for creating audio and video encoders
class YangEncoderFactory {
public:
    YangEncoderFactory();
    virtual ~YangEncoderFactory();

    YangAudioEncoder* createAudioEncoder(YangAudioCodec paet, YangAudioInfo *pcontext);
    YangAudioEncoder* createAudioEncoder(YangAudioInfo *pcontext);
    YangVideoEncoder* createVideoEncoder(YangVideoCodec paet, YangVideoInfo *pcontext);
    YangVideoEncoder* createVideoEncoder(YangVideoInfo *pcontext);
    YangVideoEncoderMeta* createVideoEncoderMeta(YangVideoInfo *pcontext);
};

#endif /* YANGENCODER_INCLUDE_YANGVIDEOENCODERFACTORY_H_ */
// By GST @2024/10/28