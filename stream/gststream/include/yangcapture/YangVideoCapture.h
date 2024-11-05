// Filename: YangVideoCapture.h
// Score: 97

#ifndef INCLUDE_YANGCAPTURE_YANGVIDEOCAPTURE_H_
#define INCLUDE_YANGCAPTURE_YANGVIDEOCAPTURE_H_

#include <yangutil/buffer/YangVideoBuffer.h>
#include <yangutil/sys/YangLog.h>
#include <yangutil/sys/YangThread2.h>
#include <yangutil/yangavinfotype.h>
#include <string>
#include <vector>

// Function to get camera indices
void yang_get_camera_indexs(std::vector<int> *pvs, const std::string& pcamindex);

// Abstract class for video capturing
class YangVideoCapture : public YangThread {
public:
    YangVideoCapture();
    virtual ~YangVideoCapture();

    virtual int32_t init() = 0;
    virtual void setVideoCaptureStart() = 0;
    virtual void setVideoCaptureStop() = 0;
    virtual void setOutVideoBuffer(YangVideoBuffer *pbuf) = 0;
    virtual void setPreVideoBuffer(YangVideoBuffer *pbuf) = 0;
    virtual int32_t getVideoCaptureState() = 0;
    virtual void initstamp() = 0;

    void stop();

    int32_t cameraIndex = 0;
    int32_t m_isStart = 0;

protected:
    void run() override;
    virtual void startLoop() = 0;
    virtual void stopLoop() = 0;

    YangVideoInfo *m_para = nullptr;
};

#endif /* INCLUDE_YANGCAPTURE_YANGVIDEOCAPTURE_H_ */
// By GST @2024/10/28