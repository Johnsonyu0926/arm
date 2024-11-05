// Filename: YangRtcPublish.h
// Score: 98

#ifndef SRC_YANGMEETING_INCLUDE_YangRtcPublish_H_
#define SRC_YANGMEETING_INCLUDE_YangRtcPublish_H_

#include <yangrtc/YangPeerConnection2.h>
#include <string>
#include <yangutil/sys/YangThread2.h>
#include <yangutil/sys/YangSysMessageI.h>
#include <yangutil/buffer/YangAudioEncoderBuffer.h>
#include <yangutil/buffer/YangVideoEncoderBuffer.h>
#include <vector>

using namespace std;

// Class for RTC Publish
class YangRtcPublish : public YangThread {
public:
    // Constructor
    YangRtcPublish(YangContext* pcontext);

    // Destructor
    virtual ~YangRtcPublish();

    // Initialize with URL
    int32_t init(char* url);

    // Initialize with network parameters
    int32_t init(int32_t nettype, char* server, int32_t pport, char* app, char* stream);

    // Connect to server
    int32_t connectServer(int32_t puid);

    // Connect to media server
    int32_t connectMediaServer();

    // Disconnect from media server
    int32_t disConnectMediaServer();

    // Reconnect to media server
    int32_t reconnectMediaServer();

    // Set video metadata
    void setInVideoMetaData(YangVideoMeta* pvmd);

    // Set audio buffer list
    void setInAudioList(YangAudioEncoderBuffer* pbuf);

    // Set video buffer list
    void setInVideoList(YangVideoEncoderBuffer* pbuf);

    // Stop publishing audio data
    int32_t stopPublishAudioData();

    // Stop publishing video data
    int32_t stopPublishVideoData();

    // Publish message
    int32_t publishMsg(YangFrame* msgFrame);

    // Receive message
    int32_t receiveMsg(YangFrame* msgFrame);

    // Network state
    int32_t m_netState;

    // Published state
    int32_t isPublished;

    // Start state
    int32_t m_isStart;

    // Stop publishing
    void stop();

protected:
    // Run the thread
    void run();

    // Handle error
    void handleError(int32_t perrCode);

    // Start loop
    void startLoop();

    // Start loop for H265
    void startLoop_h265();

    // Context
    YangContext* m_context;

    // Video metadata
    YangVideoMeta* m_vmd;

    // Video buffer
    YangVideoEncoderBuffer* m_in_videoBuffer;

    // Audio buffer
    YangAudioEncoderBuffer* m_in_audioBuffer;

    // Convert state
    int32_t m_isConvert;

    // Initialization state
    int32_t m_isInit;

    // Audio encoder type
    int32_t m_audioEncoderType;

    // Peer connections
    std::vector<YangPeerConnection2*> m_pushs;

private:
    // Transport type
    int32_t m_transType;

    // Notify state
    int32_t notifyState;
};

#endif /* SRC_YANGMEETING_INCLUDE_YangRtcPublish_H_ */
// By GST @2024/10/28