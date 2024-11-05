// Filename: YangSendVideoI.h
// Score: 98

#ifndef INCLUDE_YANGPUSH_YANGSENDVIDEOI_H_
#define INCLUDE_YANGPUSH_YANGSENDVIDEOI_H_

#include <stdint.h>

// Interface for sending video
class YangSendVideoI {
public:
    YangSendVideoI() = default;
    virtual ~YangSendVideoI() = default;

    // Pure virtual function to put RGBA video data
    virtual void putVideoRgba(uint8_t* data, int len, int64_t timestamp) = 0;

    // Pure virtual function to put I420 video data
    virtual void putVideoI420(uint8_t* data, int len, int64_t timestamp) = 0;
};

#endif /* INCLUDE_YANGPUSH_YANGSENDVIDEOI_H_ */
// By GST @2024/10/28