// Filename: YangVideoDecoder.h
// Score: 98

#ifndef YANGH264DECODER_H
#define YANGH264DECODER_H

#include <yangutil/yangavtype.h>
#include "YangDecoder.h"

// Abstract class for H.264 video decoding
class YangVideoDecoder {
public:
    YangVideoDecoder() = default;
    virtual ~YangVideoDecoder() = default;

    virtual void init() = 0;
    virtual void parseRtmpHeader(uint8_t *p, int32_t pLen, int32_t *pwid, int32_t *phei, int32_t *pfps) = 0;
    virtual int32_t decode(YangFrame* pframe, YangYuvType yuvtype, YangDecoderCallback* pcallback) = 0;

    int32_t m_isInit = 0;
    int32_t m_uid = -1;
    int32_t m_state = 0;
};

#endif // YANGH264DECODER_H
// By GST @2024/10/28