// Filename: YangAudioEncoderBuffer.h
// Score: 97

#ifndef YangAudioEncoderBuffer_H
#define YangAudioEncoderBuffer_H

#include <yangutil/buffer/YangMediaBuffer.h>

// Audio Encoder Buffer class
class YangAudioEncoderBuffer : public YangMediaBuffer {
public:
    // Constructor with audio cache number
    YangAudioEncoderBuffer(int32_t paudioCacheNum);

    // Destructor
    ~YangAudioEncoderBuffer();

    // Reset buffer
    void reset();

    // Put audio frame into buffer
    void putAudio(YangFrame* audioFrame);

    // Get audio frame from buffer
    void getAudio(YangFrame* audioFrame);

    // Get audio reference from buffer
    uint8_t* getAudioRef(YangFrame* pframe);

    // Put play audio frame into buffer
    void putPlayAudio(YangFrame* pframe);

    // Get play audio frame from buffer
    void getPlayAudio(YangFrame* audioFrame);

private:
    // Private members can be added here if needed
};

#endif /* YangAudioEncoderBuffer_H */
// By GST @2024/10/28