// Filename: YangPeerConnection2.h
// Score: 98

#ifndef INCLUDE_YANGRTC_YANGPEERCONNECTION2_H_
#define INCLUDE_YANGRTC_YANGPEERCONNECTION2_H_

#include <yangrtc/YangPeerConnection.h>

class YangPeerConnection2 {
public:
    YangPeerConnection2(YangAVInfo* avinfo, YangStreamConfig* streamConfig);
    virtual ~YangPeerConnection2();

    // Stream configuration
    YangStreamConfig* streamConfig;

    // Initialize the peer connection
    void init();

    // Add audio track
    int32_t addAudioTrack(YangAudioCodec codec);

    // Add video track
    int32_t addVideoTrack(YangVideoCodec codec);

    // Add transceiver
    int32_t addTransceiver(YangRtcDirection direction);

    // Create offer
    int32_t createOffer(char** psdp);

    // Create answer
    int32_t createAnswer(char* answer);

    // Create HTTP answer
    int32_t createHttpAnswer(char* answer);

    // Create data channel
    int32_t createDataChannel();

    // Set remote description
    int32_t setRemoteDescription(char* sdp);

    // Connect to SFU server (e.g., SRS, ZLM)
    int32_t connectSfuServer();

    // Connect to WHIP server
    int32_t connectWhipServer(char* url);

    // Close the connection
    int32_t close();

    // Check if the connection is alive
    int32_t isAlive();

    // Check if the connection is connected
    int32_t isConnected();

    // Handle audio frame
    int32_t on_audio(YangFrame* audioFrame);

    // Handle video frame
    int32_t on_video(YangFrame* videoFrame);

    // Handle message frame
    int32_t on_message(YangFrame* msgFrame);

    // Send RTC message
    int32_t sendRtcMessage(YangRtcMessageType mess);

private:
    // Peer connection instance
    YangPeerConnection m_conn;
};

#endif /* INCLUDE_YANGRTC_YANGPEERCONNECTION2_H_ */
// By GST @2024/10/28