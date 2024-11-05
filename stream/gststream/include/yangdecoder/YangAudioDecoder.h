// Filename: YangAudioDecoder.h
// Score: 98

#ifndef YANGDECODER_INCLUDE_YANGAUDIODECODER_H_
#define YANGDECODER_INCLUDE_YANGAUDIODECODER_H_

#include <yangutil/yangavinfotype.h>
#include "yangstream/YangStreamType.h"
#include "YangDecoder.h"

// Abstract class for audio decoding
class YangAudioDecoder {
public:
    YangAudioDecoder();
    virtual ~YangAudioDecoder();

    virtual void init() = 0;
    virtual int32_t decode(YangFrame* pframe, YangDecoderCallback* pcallback) = 0;

    int32_t m_uid = 0;

protected:
    int32_t m_isInit = 0;
    int32_t m_frameSize = 0;
    int32_t m_alen = 0;
    YangAudioParam *m_context = nullptr;
    uint8_t* m_dstBuffer = nullptr;
    int32_t m_dstLen = 0;
};

#endif /* YANGDECODER_INCLUDE_YANGAUDIODECODER_H_ */
// By GST @2024/10/28