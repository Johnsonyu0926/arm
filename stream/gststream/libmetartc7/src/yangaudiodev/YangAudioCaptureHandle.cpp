// Filename: YangAudioCaptureHandle.h
// Score: 94



#ifndef ___YangAudioCaptureHandle__
#define ___YangAudioCaptureHandle__

#include <yangutil/buffer/YangAudioBuffer.h>
#include <yangavutil/audio/YangAudioUtil.h>
#include <yangavutil/audio/YangRtcAec.h>
#include <yangutil/yangavinfotype.h>

class YangAudioCaptureHandle {
public:
    YangAudioCaptureHandle(YangAVInfo *avinfo);
    virtual ~YangAudioCaptureHandle();

    void putBuffer(uint8_t *pBuffer, int32_t plen);
    void putBuffer2(uint8_t *pBuffer, int32_t plen);
    void putEchoBuffer(uint8_t *pBuffer, int32_t plen);
    void putEchoBuffer2(uint8_t *pBuffer, int32_t plen);
    void putEchoPlay(short *pbuf, int32_t plen);
    void startRecordWave(char *filename);
    void stopRecordWave();
    void setOutAudioBuffer(YangAudioBuffer *plist);

private:
    YangRtcAec *m_aec;
    YangAudioBuffer *m_aecPlayBuffer;
    YangAudioBuffer *m_audioList;
    YangAudioResample m_resample;
    YangFrame m_audioFrame;
    short *pcm;
    int32_t hasPlayData;
    int32_t m_aecBufferFrames;
    int32_t isFirst;
    int32_t m_audioLen;
    bool m_enableBuf;
};

#endif // ___YangAudioCaptureHandle__
// By GST @Date