// Filename: YangEncoder.h
// Score: 98

#ifndef YANGENCODER_INCLUDE_YANGENCODER_H_
#define YANGENCODER_INCLUDE_YANGENCODER_H_

#include <cstdint>

// Callback class for encoder data
class YangEncoderCallback {
public:
    YangEncoderCallback() = default;
    virtual ~YangEncoderCallback() = default;

    virtual void onVideoData(YangFrame* pframe) = 0;
    virtual void onAudioData(YangFrame* pframe) = 0;
};

#endif /* YANGENCODER_INCLUDE_YANGENCODER_H_ */
// By GST @2024/10/28