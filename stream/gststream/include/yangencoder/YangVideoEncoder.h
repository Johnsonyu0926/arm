// Filename: YangVideoEncoder.h
// Score: 98

#ifndef YANGENCODER_INCLUDE_YANGVIDEOENCODER_H_
#define YANGENCODER_INCLUDE_YANGVIDEOENCODER_H_

#include <yangutil/yangavinfotype.h>
#include <cstdint>
#include "YangEncoder.h"

// Structure for RTC encoder message
typedef struct {
    YangRequestType request;
    int32_t requestValue;
} YangRtcEncoderMessage;

// Base class for video encoder
class YangVideoEncoder {
public:
    YangVideoEncoder();
    virtual ~YangVideoEncoder();

    virtual int32_t init(YangContext* pcontext, YangVideoInfo* pvideoInfo) = 0;
    virtual int32_t encode(YangFrame* pframe, YangEncoderCallback* pcallback) = 0;
    virtual void setVideoMetaData(YangVideoMeta *pvmd) = 0;
    virtual void sendMsgToEncoder(YangRtcEncoderMessage *msg) = 0;

protected:
    int32_t m_isInit;
    YangVideoInfo m_videoInfo;
    YangVideoEncInfo m_enc;
    uint8_t* m_vbuffer;

    void setVideoPara(YangVideoInfo *pap, YangVideoEncInfo *penc);
};

#endif /* YANGENCODER_INCLUDE_YANGVIDEOENCODER_H_ */
// By GST @2024/10/28