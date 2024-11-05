// Filename: YangPushPublish.h
// Score: 98

#ifndef YangPushPublish_H
#define YangPushPublish_H

#include <yangutil/buffer/YangAudioEncoderBuffer.h>
#include <yangutil/buffer/YangVideoEncoderBuffer.h>
#include <yangutil/buffer/YangAudioBuffer.h>
#include <yangutil/buffer/YangVideoBuffer.h>
#include <yangpush/YangPushCapture.h>
#include <yangpush/YangPushEncoder.h>
#include <yangpush/YangRtcPublish.h>
#include <vector>

using namespace std;

// Class for Push Publish
class YangPushPublish : public YangSendRequestCallback {
public:
    // Constructor
    YangPushPublish(YangContext* pcontext);

    // Destructor
    virtual ~YangPushPublish();

    // Set capture type
    void setCaptureType(int pct);

    // Start camera
    void startCamera();

    // Stop camera
    void stopCamera();

    // Set screen interval
    void setScreenInterval(int32_t pinterval);

    // Set draw mouse
    void setDrawmouse(bool isDraw);

    // Set RTC net buffer
    void setRtcNetBuffer(YangRtcPublish* prr);

    // Start publishing video
    void startPubVideo();

    // Start publishing audio
    void startPubAudio();

    // Initialize audio encoding
    void initAudioEncoding();

    // Initialize video encoding
    void initVideoEncoding();

    // Set video information
    void setVideoInfo(YangVideoInfo* pvideo);

    // Start audio capture
    int32_t startAudioCapture();

    // Start video capture
    int32_t startVideoCapture();

    // Initialize video meeting
    void initVideoMeeting();

    // Start audio encoding
    void startAudioEncoding();

    // Start video encoding
    void startVideoEncoding();

    // Delete video encoding
    void deleteVideoEncoding();

    // Start audio capture state
    void startAudioCaptureState();

    // Start video capture state
    void startVideoCaptureState();

    // Stop audio capture state
    void stopAudioCaptureState();

    // Stop video capture state
    void stopScreenCaptureState();

    // Get pre video buffer
    YangVideoBuffer* getPreVideoBuffer();

    // Get out pre video buffer
    YangVideoBuffer* getOutPreVideoBuffer();

    // Get out video buffer
    YangVideoBuffer* getOutVideoBuffer();

    // Stop all
    void stopAll();

    // Set in audio buffer
    void setInAudioBuffer(vector<YangAudioPlayBuffer*>* pbuf);

    // Change state
    void change(int32_t st);

    // Send request
    void sendRequest(int32_t puid, uint32_t ssrc, YangRequestType req);

    // Send message to encoder
    void sendMsgToEncoder(YangRequestType req);

    // Get push capture
    YangPushCapture* getPushCapture();

protected:

private:
    // Out video buffer
    YangVideoBuffer* m_outVideoBuffer;

    // Out pre video buffer
    YangVideoBuffer* m_outPreVideoBuffer;

    // Context
    YangContext* m_context;

    // Encoder
    YangPushEncoder* m_encoder;

    // Capture
    YangPushCapture* m_capture;

    // Video information
    YangVideoInfo* m_videoInfo;

    // Capture states
    int32_t isStartAudioCapture, isStartVideoCapture, isStartScreenCapture;
    int32_t isStartAudioEncoder, isStartVideoEncoder;

    // Stop audio state
    void stopAudioState();

    // Stop video state
    void stopVideoState();

    // Initialize capture
    void initCapture();

    // Capture type
    int m_captureType;
};

#endif /* YangPushPublish_H */
// By GST @2024/10/28