// Filename: YangVideoDecoderHandle.h
// Score: 98

#ifndef __YangVideoDecoderHandle__
#define __YangVideoDecoderHandle__

#include <yangutil/buffer/YangVideoDecoderBuffer.h>
#include <yangutil/buffer/YangVideoBuffer.h>
#include <yangutil/sys/YangThread2.h>
#include <yangdecoder/YangVideoDecoder.h>
#include <yangutil/yangavinfotype.h>
#include <vector>

using namespace std;

// Class for handling video decoding with callback
class YangVideoDecoderHandle : public YangThread, public YangDecoderCallback {
public:
    YangVideoDecoderHandle(YangContext *pcontext);
    ~YangVideoDecoderHandle();

    void init();
    void stop();
    void setInVideoBuffer(YangVideoDecoderBuffer *pbuf);
    void setOutVideoBuffer(YangVideoBuffer* pbuf);

    void onAudioData(YangFrame* pframe) override;
    void onVideoData(YangFrame* pframe) override;

    YangVideoDecoder* getDecoder(int32_t puid);
    YangVideoBuffer* getOutVideoBuffer();
    void parseVideoHeader(uint8_t *p, int32_t pind);
    void addVideoStream(uint8_t *ps, int32_t pind, int32_t pisAdd);

private:
    int32_t isInit;
    int32_t m_isStart;
    int32_t m_isConvert;

    YangVideoDecoderBuffer *m_in_videoBuffer;
    YangVideoBuffer *m_out_videoBuffer;
    YangVideoDecoder *m_decs;

    YangContext *m_context;

    void run() override;
    void stopLoop();
    void startLoop();
    void saveFile(char *fileName, uint8_t *pBuffer, long BufferLen);
    void decode(int32_t isIframe, uint8_t* src, int32_t p_buflen, uint8_t *dest, int32_t *p_destLen);
};

#endif /* __YangVideoDecoderHandle__ */
// By GST @2024/10/28