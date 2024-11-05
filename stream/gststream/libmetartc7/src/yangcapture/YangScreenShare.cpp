// Filename: YangScreenShare.h
// Score: 92

#ifndef YANGCAPTURE_SRC_YANGSCREENSHARE_H_
#define YANGCAPTURE_SRC_YANGSCREENSHARE_H_

#include <yangcapture/YangScreenCaptureHandleI.h>
#include <yangutil/buffer/YangVideoBuffer.h>

class YangScreenCapture {
public:
    YangScreenCapture();
    virtual ~YangScreenCapture();
    virtual void run();
    virtual void stop();
protected:
    virtual void startLoop() = 0;
    virtual void stopLoop() = 0;
    int32_t m_isStart;
};

class YangScreenShare {
public:
    YangScreenShare();
    ~YangScreenShare();
    void setOutVideoBuffer(YangVideoBuffer *pbuf);
    void setScreenHandle(YangScreenCaptureHandleI *handle);
    void setInterval(int32_t pinterval);
    void setVideoCaptureStart();
    void setVideoCaptureStop();
    int32_t getVideoCaptureState();
    void initstamp();
    void stopLoop();
    int32_t init();
    void startLoop();

private:
    int32_t m_isloop;
    int32_t m_isCapture;
    int32_t m_interval;
    YangVideoBuffer *m_out_videoBuffer;
    YangScreenCaptureHandleI *m_capture;
};

#endif /* YANGCAPTURE_SRC_YANGSCREENSHARE_H_ */

// By GST @Date