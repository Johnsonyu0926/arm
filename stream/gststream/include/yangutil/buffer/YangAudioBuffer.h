// Filename: YangAudioBuffer.h
// Score: 97

#ifndef ___YangAudioBuffer__
#define ___YangAudioBuffer__

#include <yangutil/buffer/YangMediaBuffer.h>

// Audio Buffer class
class YangAudioBuffer : public YangMediaBuffer {
public:
    // Constructor with cache number
    YangAudioBuffer(int32_t pcacheNum);

    // Destructor
    ~YangAudioBuffer();

    // Put audio frame into buffer
    void putAudio(YangFrame* pframe);

    // Get audio frame from buffer
    int32_t getAudio(YangFrame* pframe);

    // Get audio reference from buffer
    uint8_t* getAudioRef(YangFrame* pframe);

    // Reset buffer
    void reset();

private:
    // Buffer length
    int32_t m_bufLen;
};

#endif /* ___YangAudioBuffer__ */
// By GST @2024/10/28