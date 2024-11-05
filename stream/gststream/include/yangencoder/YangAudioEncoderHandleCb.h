// Filename: YangAudioEncoderHandleCb.h
// Score: 98

#ifndef YANGENCODER_INCLUDE_YANGAUDIOENCODERHANDLECB_H_
#define YANGENCODER_INCLUDE_YANGAUDIOENCODERHANDLECB_H_

#include <yangutil/sys/YangThread2.h>
#include <yangutil/buffer/YangAudioEncoderBuffer.h>
#include <yangutil/buffer/YangAudioPlayBuffer.h>
#include <yangencoder/YangAudioEncoder.h>
#include <vector>

using namespace std;

// Class for handling audio encoder with callback
class YangAudioEncoderHandleCb : public YangThread {
public:
    YangAudioEncoderHandleCb(YangAudioInfo *pcontext);
    ~YangAudioEncoderHandleCb();

    void init();
    void stop();
    void setInAudioBuffer(YangAudioBuffer *pbuf);
    void setCallback(YangEncoderCallback* pcb);

private:
    int32_t m_isInit;
    int32_t m_isStart;
    int32_t m_uid;
    int32_t m_isConvert;

    YangEncoderCallback* m_cb;
    YangAudioEncoder *m_enc;
    YangAudioBuffer *m_in_audioBuffer;
    YangAudioInfo *m_context;

    void run() override;
    void stopLoop();
    void startLoop();
    void saveFile(char *fileName, uint8_t *pBuffer, long BufferLen);
    void Encoder(int32_t isIframe, uint8_t* src, int32_t p_buflen, uint8_t *dest, int32_t *p_destLen);
};

#endif /* YANGENCODER_INCLUDE_YANGAUDIOENCODERHANDLECB_H_ */
// By GST @2024/10/28