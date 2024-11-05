// Filename: YangDecoder.h
// Score: 98

#ifndef YANGDECODER_INCLUDE_YANGDECODER_H_
#define YANGDECODER_INCLUDE_YANGDECODER_H_

// Callback interface for decoder events
class YangDecoderCallback {
public:
    YangDecoderCallback() = default;
    virtual ~YangDecoderCallback() = default;

    virtual void onAudioData(YangFrame* pframe) = 0;
    virtual void onVideoData(YangFrame* pframe) = 0;
};

#endif /* YANGDECODER_INCLUDE_YANGDECODER_H_ */
// By GST @2024/10/28