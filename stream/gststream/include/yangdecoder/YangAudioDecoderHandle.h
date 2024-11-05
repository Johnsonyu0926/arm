// Filename: YangAudioDecoderHandle.h
// Score: 97

#ifndef __YangAudioDecoderHandle__
#define __YangAudioDecoderHandle__

#include <yangutil/sys/YangThread2.h>
#include <yangutil/buffer/YangAudioEncoderBuffer.h>
#include <yangutil/buffer/YangAudioPlayBuffer.h>
#include <yangdecoder/YangAudioDecoder.h>
#include <vector>

using namespace std;

// Handles a single audio decoder and manages audio streams
class YangAudioDecoderHandle : public YangThread, public YangDecoderCallback, public YangMediaConfigCallback {
public:
    explicit YangAudioDecoderHandle(YangContext *pcontext);
    ~YangAudioDecoderHandle() override;

    YangAudioPlayBuffer* getOutAudioBuffer();
    void setRemoteParam(YangAudioParam* para);
    void init();
    void stop();
    void setInAudioBuffer(YangAudioEncoderBuffer *pbuf);
    void setOutAudioBuffer(YangAudioPlayBuffer* pbuf);
    void removeAudioStream(int32_t puid);
    void removeAllStream();
    void onAudioData(YangFrame* pframe) override;
    void onVideoData(YangFrame* pframe) override;
    void setMediaConfig(int32_t puid, YangAudioParam *audio, YangVideoParam *video) override;

protected:
    void run() override;
    void stopLoop();
    void startLoop();

private:
    void saveFile(char *fileName, uint8_t *pBuffer, long BufferLen);
    void decode(int32_t isIframe, uint8_t* src, int32_t p_buflen, uint8_t *dest, int32_t *p_destLen);

    int32_t m_isStart = 0;
    int32_t m_isInit = 0;
    int32_t m_isConvert = 0;
    int32_t m_size = 0;
    bool m_is44100 = false;

    YangAudioPlayBuffer* m_out_audioBuffer = nullptr;
    YangAudioDecoder *m_decs = nullptr;
    YangAudioEncoderBuffer *m_in_audioBuffer = nullptr;
    YangContext *m_context = nullptr;
    YangAudioParam* m_param = nullptr;
    uint8_t* m_buf = nullptr;
    YangFrame m_audioFrame;
};

#endif // __YangAudioDecoderHandle__
// By GST @2024/10/28