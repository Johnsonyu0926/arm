// Filename: YangVideoBuffer.h
// Score: 96

#ifndef __YangVideoBuffer__
#define __YangVideoBuffer__

#include <yangutil/buffer/YangMediaBuffer.h>

#define yang_get_videoBuffer(x) new YangVideoBuffer(x->width, x->height, x->videoCaptureFormat == YangYuy2 ? 16 : 12, x->bitDepth == 8 ? 1 : 2)

// Video Buffer class
class YangVideoBuffer : public YangMediaBuffer {
public:
    // Constructor with bit depth length
    YangVideoBuffer(int32_t pBitDepthLen);

    // Constructor with width, height, YUV type, and bit depth length
    YangVideoBuffer(int32_t pwid, int32_t phei, YangYuvType ptype, int32_t pBitDepthLen);

    // Destructor
    ~YangVideoBuffer();

    // Initialize temporary buffer
    void initTemp();

    // Initialize buffer with width, height, and YUV type
    void init(int32_t pwid, int32_t phei, YangYuvType ptype);

    // Reset buffer
    void reset();

    // Put video frame into buffer
    void putVideo(YangFrame* pframe);

    // Get video frame from buffer
    void getVideo(YangFrame* pframe);

    // Get reference to video frame
    uint8_t* getVideoRef(YangFrame* pframe);

    // Get timestamp
    int64_t getTimestamp(int64_t* timestamp);

    // Get next timestamp
    int64_t getNextTimestamp();

    // Get current video frame
    YangFrame* getCurVideoFrame();

    // Public members
    int32_t isPreview = 0;
    int32_t m_width;
    int32_t m_height;
    int32_t m_length;
    int32_t m_frames;
    int32_t m_bitDepthLen;

private:
    // Private members
    int32_t m_headerLen;
};

#endif /* __YangVideoBuffer__ */
// By GST @2024/10/28