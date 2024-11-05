// Filename: YangAudioDecoderHandles.h
// Score: 96

#ifndef __YangAudioDecoderHandles__
#define __YangAudioDecoderHandles__

#include <yangutil/sys/YangThread2.h>
#include <yangutil/buffer/YangAudioEncoderBuffer.h>
#include <yangutil/buffer/YangAudioPlayBuffer.h>
#include <yangdecoder/YangAudioDecoder.h>
#include <vector>
#include <map>

using namespace std;

// Handles multiple audio decoders and manages audio streams
class YangAudioDecoderHandles : public YangThread, public YangDecoderCallback {
public:
    explicit YangAudioDecoderHandles(YangContext *pcontext);
    ~YangAudioDecoderHandles() override;

    YangAudioDecoder* getDecoder(int32_t puid);
    YangAudioPlayBuffer* getAudioBuffer(int32_t puid);
    int32_t getDecoderIndex(int32_t puid);
    void init();
    void stop();
    void setInAudioBuffer(YangAudioEncoderBuffer *pbuf);
    void setOutAudioBuffer(vector<YangAudioPlayBuffer*>* pbuf);
    void removeAudioStream(int32_t puid);
    void removeAllStream();
    void onAudioData(YangFrame* pframe) override;
    void onVideoData(YangFrame* pframe) override;
    void setRemoteParam(int32_t puid, YangAudioParam* para);

protected:
    void run() override;
    void stopLoop();
    void startLoop();

private:
    void saveFile(char *fileName, uint8_t *pBuffer, long BufferLen);
    void decode(int32_t isIframe, uint8_t* src, int32_t p_buflen, uint8_t *dest, int32_t *p_destLen);

    int32_t m_isInit = 0;
    int32_t m_isStart = 0;
    int32_t m_isConvert = 0;
    int32_t m_frameSize = 0;
    int32_t m_channel = 0;

    std::map<int, YangAudioParam*> m_paramMap;
    vector<YangAudioPlayBuffer*> *m_out_audioBuffer = nullptr;
    vector<YangAudioDecoder*> *m_decs = nullptr;
    YangAudioEncoderBuffer *m_in_audioBuffer = nullptr;
    YangContext *m_context = nullptr;
};

#endif // __YangAudioDecoderHandles__
// By GST @2024/10/28