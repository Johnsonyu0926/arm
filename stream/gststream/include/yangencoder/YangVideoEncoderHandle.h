// Filename: YangVideoEncoderHandle.h
// Score: 98

#ifndef __YANGVIDEOENCODERHANDLE__
#define __YANGVIDEOENCODERHANDLE__

#include <yangutil/sys/YangThread2.h>
#include <yangutil/buffer/YangVideoEncoderBuffer.h>
#include <yangutil/buffer/YangVideoBuffer.h>
#include <yangencoder/YangVideoEncoder.h>
#include <vector>

using namespace std;

// Class for handling video encoding
class YangVideoEncoderHandle : public YangThread, public YangEncoderCallback {
public:
    YangVideoEncoderHandle(YangContext* pcontext, YangVideoInfo* pvideoInfo);
    ~YangVideoEncoderHandle();

    void init();
    void stop();
    void setOutVideoBuffer(YangVideoEncoderBuffer *pvl);
    void setInVideoBuffer(YangVideoBuffer *pvl);
    void setVideoMetaData(YangVideoMeta *pvmd);
    void sendMsgToEncoder(YangRequestType request);
    void onVideoData(YangFrame* pframe);
    void onAudioData(YangFrame* pframe);

protected:
    void run();
    void stopLoop();
    void startLoop();
    void saveFile(char *fileName, uint8_t *pBuffer, long BufferLen);

private:
    YangContext* m_context;
    YangVideoBuffer *m_in_videoBuffer;
    YangVideoEncoderBuffer *m_out_videoBuffer;
    YangVideoMeta *m_vmd;
    YangVideoInfo *m_videoInfo;
    YangVideoEncInfo *m_encInfo;
    YangRtcEncoderMessage m_request;

    int32_t m_isInit;
    int32_t m_isStart;
    int32_t m_uid;
    int32_t m_isConvert;
    int32_t m_sendMsgToEncoder;
};

#endif // __YANGVIDEOENCODERHANDLE__
// By GST @2024/10/28