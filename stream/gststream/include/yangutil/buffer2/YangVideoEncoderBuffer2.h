// Filename: YangVideoEncoderBuffer2.h
// Score: 97

#ifndef __YangVideoEncoderBuffer2__
#define __YangVideoEncoderBuffer2__

#include <yangutil/buffer2/YangMediaBuffer2.h>

// Video Encoder Buffer structure
typedef struct {
    YangMediaBuffer2 mediaBuffer;
    void (*getEVideo)(YangMediaBuffer2* buf, YangFrame* pframe);
    uint8_t* (*getEVideoRef)(YangMediaBuffer2* buf, YangFrame* frame);
    void (*putEVideo)(YangMediaBuffer2* buf, YangFrame* pframe);
    int32_t (*size)(YangMediaBuffer2* buf);
    void (*resetIndex)(YangMediaBuffer2* buf);
} YangVideoEncoderBuffer2;

// Function to create video encoder buffer
void yang_create_videoEncoderBuffer2(YangVideoEncoderBuffer2* buf, int pcachenum);

// Function to destroy video encoder buffer
void yang_destroy_videoEncoderBuffer2(YangVideoEncoderBuffer2* buf);

#endif /* __YangVideoEncoderBuffer2__ */
// By GST @2024/10/28