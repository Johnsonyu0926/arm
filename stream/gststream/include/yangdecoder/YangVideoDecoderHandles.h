// Filename: YangVideoDecoderHandles.h
// Score: 98

#ifndef __YangVideoDecoderHandles__
#define __YangVideoDecoderHandles__

#include <yangutil/sys/YangThread2.h>
#include <yangutil/buffer/YangVideoDecoderBuffer.h>
#include <yangutil/buffer/YangVideoBuffer.h>
#include <yangdecoder/YangVideoDecoder.h>
#include <yangutil/yangavinfotype.h>
#include <vector>

using namespace std;

// Class for handling video decoding with callback
class YangVideoDecoderHandles : public YangThread, public YangDecoderCallback {
public:
    YangVideoDecoderHandles(YangContext *pcontext);
    ~YangVideoDecoderHandles();

    void init();
    void stop();
    void setInVideoBuffer(YangVideoDecoderBuffer *pbuf);
    void setOutVideoBuffer(vector<YangVideoBuffer*>* pbuf);

    void onAudioData(YangFrame* pframe) override;
    void onVideoData(YangFrame* pframe) override;

    YangVideoDecoder* getDecoder(int32_t puid);
    YangVideoBuffer* getVideoBuffer(int32_t puid);
    int32_t getDecoderIndex(int32_t puid);
    void parseVideoHeader(uint8_t *p, int32_t pind);
    void addVideoStream(uint8_t *ps, int32_t pind, int32_t pisAdd);
    void removeAllStream();

private:
    int32_t isInit;
    int32_t m_isStart;
    int32_t m_isConvert;

    YangVideoDecoderBuffer *m_in_videoBuffer;
    vector<YangVideoBuffer*> *m_out_videoBuffer;
    vector<YangVideoDecoder*> *m_decs;
    vector<int> m_removeList;

    YangContext *m_context;
    yang_thread_mutex_t m_mutex;

    void run() override;
    void stopLoop();
    void startLoop();
    void saveFile(char *fileName, uint8_t *pBuffer, long BufferLen);
    void removeStream();
    void decode(int32_t isIframe, uint8_t* src, int32_t p_buflen, uint8_t *dest, int32_t *p_destLen);
};

#endif /* __YangVideoDecoderHandles__ */
// By GST @2024/10/28