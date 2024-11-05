// Filename: YangVideoEncoderBuffer.h
// Score: 97

#ifndef __YangVideoEncoderBuffer__
#define __YangVideoEncoderBuffer__

#include <yangutil/buffer/YangMediaBuffer.h>

// Video Encoder Buffer class
class YangVideoEncoderBuffer : public YangMediaBuffer {
public:
    // Constructor
    YangVideoEncoderBuffer(int32_t pcachenum);

    // Destructor
    ~YangVideoEncoderBuffer();

    // Get encoded video frame
    void getEVideo(YangFrame* pframe);

    // Get reference to encoded video frame
    uint8_t* getEVideoRef(YangFrame* frame);

    // Put encoded video frame
    void putEVideo(YangFrame* pframe);

private:
    // Private members can be added here if needed
};

#endif /* __YangVideoEncoderBuffer__ */
// By GST @2024/10/28