// Filename: YangAudioEncoderHandle.h
// Score: 98

#ifndef __YangAudioEncoderHandle__
#define __YangAudioEncoderHandle__

#include <yangutil/sys/YangThread2.h>
#include <yangutil/buffer/YangAudioEncoderBuffer.h>
#include <yangutil/buffer/YangAudioPlayBuffer.h>
#include <yangencoder/YangAudioEncoder.h>
#include <vector>

using namespace std;

// Class for handling audio encoding with callback
class YangAudioEncoderHandle : public YangThread, public YangEncoderCallback {
public:
    YangAudioEncoderHandle(YangAudioInfo *pcontext);
    ~YangAudioEncoderHandle();

    void init();
    void stop();
    void setInAudioBuffer(YangAudioBuffer *pbuf);
    void setOutAudioBuffer(YangAudioEncoderBuffer *pbuf);

    void onVideoData(YangFrame* pframe) override;
    void onAudioData(YangFrame* pframe) override;

private:
    int32_t m_isInit;
    int32_t m_isStart;
    int32_t m_uid;
    int32_t m_isConvert;

    YangAudioEncoder *m_enc;
    YangAudioBuffer *m_in_audioBuffer;
    YangAudioEncoderBuffer *m_out_audioBuffer;
    YangAudioInfo *m_context;

    void run() override;
    void stopLoop();
    void startLoop();
    void saveFile(char *fileName, uint8_t *pBuffer, long BufferLen);
    void Encoder(int32_t isIframe, uint8_t* src, int32_t p_buflen, uint8_t *dest, int32_t *p_destLen);
};

#endif /* __YangAudioEncoderHandle__ */
// By GST @2024/10/28