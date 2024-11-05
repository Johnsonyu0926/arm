// Filename: YangDecoderApp.h
// Score: 98

#ifndef YANGAPP_YANGDECODERAPP_H_
#define YANGAPP_YANGDECODERAPP_H_

#include <yangdecoder/YangAudioDecoderHandle.h>
#include <yangdecoder/YangVideoDecoderHandle.h>
#include <vector>

using namespace std;

// Class for handling player decoder
class YangPlayerDecoder {
public:
    YangPlayerDecoder(YangContext* pcontext);
    virtual ~YangPlayerDecoder();

    void initAudioDecoder();
    void initVideoDecoder();
    void setInVideoBuffer(YangVideoDecoderBuffer *pvel);
    void setInAudioBuffer(YangAudioEncoderBuffer *pael);
    YangVideoBuffer* getOutVideoBuffer();
    YangAudioPlayBuffer* getOutAudioBuffer();
    void startAudioDecoder();
    void startVideoDecoder();
    void setRoomState(int32_t pst);
    void stopAll();

private:
    YangContext *m_context;
    YangVideoBuffer* m_out_videoBuffer;
    YangAudioPlayBuffer* m_out_audioBuffer;
    YangAudioDecoderHandle *m_audioDec;
    YangVideoDecoderHandle *m_videoDec;
};

#endif /* YANGAPP_YANGDECODERAPP_H_ */
// By GST @2024/10/28