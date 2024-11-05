// Filename: YangAudioPlayBuffer.h
// Score: 97

#ifndef ___YangAudioPlayBuffer1__
#define ___YangAudioPlayBuffer1__

#include <yangutil/buffer/YangMediaBuffer.h>

// Audio Play Buffer class
class YangAudioPlayBuffer : public YangMediaBuffer {
public:
    // Constructor
    YangAudioPlayBuffer();

    // Destructor
    ~YangAudioPlayBuffer();

    // Put audio frame into buffer
    void putAudio(YangFrame* pframe);

    // Get audio frame from buffer
    void getAudio(YangFrame* pframe);

    // Get audio data from buffer
    uint8_t* getAudios(YangFrame* pframe);

    // Get frame timestamp
    int32_t getFrameTimestamp(int64_t* timestamp);

    // Get next frame timestamp
    int64_t getNextTimestamp();

    // Reset buffer
    void reset();

private:
    // Buffer length
    int32_t m_bufLen;
};

#endif /* ___YangAudioPlayBuffer1__ */
// By GST @2024/10/28