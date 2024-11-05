// Filename: YangVideoDecoderBuffer.h
// Score: 97

#ifndef __YangVideoDecoderBuffer__
#define __YangVideoDecoderBuffer__

#include <stdint.h>
#include <yangutil/buffer/YangMediaBuffer.h>

// Video Decoder Buffer class
class YangVideoDecoderBuffer : public YangMediaBuffer {
public:
    // Constructor
    YangVideoDecoderBuffer(int num);

    // Destructor
    ~YangVideoDecoderBuffer();

    // Put encoded video frame
    void putEVideo(YangFrame* pframe);

    // Get encoded video frame
    void getEVideo(YangFrame* pframe);

private:
    // Private members can be added here if needed
};

#endif /* __YangVideoDecoderBuffer__ */
// By GST @2024/10/28