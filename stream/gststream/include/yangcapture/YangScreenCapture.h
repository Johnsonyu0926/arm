// Filename: YangScreenCapture.h
// Score: 97

#ifndef INCLUDE_YANGCAPTURE_YANGSCREENCAPTURE_H_
#define INCLUDE_YANGCAPTURE_YANGSCREENCAPTURE_H_

#include "yangutil/buffer/YangVideoBuffer.h"
#include <yangutil/sys/YangThread2.h>
#include "YangScreenCaptureHandleI.h"

// Abstract class for screen capturing
class YangScreenCapture : public YangThread {
public:
    YangScreenCapture();
    virtual ~YangScreenCapture();

    virtual void setDrawmouse(bool isDraw) = 0;
    virtual bool getisDrawmouse() const = 0;

    virtual void setInterval(int32_t pinterval) = 0;
    virtual int32_t init() = 0;
    virtual void setVideoCaptureStart() = 0;
    virtual void setVideoCaptureStop() = 0;
    virtual YangVideoBuffer* getOutVideoBuffer() = 0;
    virtual YangVideoBuffer* getPreVideoBuffer() = 0;
    virtual int32_t getVideoCaptureState() const = 0;

    void stop();

    int32_t m_isStart = 0;

protected:
    void run() override;
    virtual void startLoop() = 0;
    virtual void stopLoop() = 0;
};

#endif /* INCLUDE_YANGCAPTURE_YANGSCREENCAPTURE_H_ */
// By GST @2024/10/28