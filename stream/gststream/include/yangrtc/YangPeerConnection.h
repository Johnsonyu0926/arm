// Filename: YangPeerConnection.h
// Score: 98

#ifndef INCLUDE_YANGRTC_YANGPEERCONNECTION_H_
#define INCLUDE_YANGRTC_YANGPEERCONNECTION_H_

#include <yangutil/yangavinfotype.h>

// Structure for peer connection
typedef struct {
    void* conn;
    YangAVInfo* avinfo;
    YangStreamConfig streamconfig;
} YangPeer;

// Structure for peer connection functions
typedef struct {
    YangPeer peer;

    // Initialize the peer connection
    void (*init)(YangPeer* peer);

    // Add audio track
    int32_t (*addAudioTrack)(YangPeer* peer, YangAudioCodec codec);

    // Add video track
    int32_t (*addVideoTrack)(YangPeer* peer, YangVideoCodec codec);

    // Add transceiver
    int32_t (*addTransceiver)(YangPeer* peer, YangRtcDirection direction);

    // Get ICE candidate type
    YangIceCandidateType (*getIceCandidateType)(YangPeer* peer);

    // Create offer
    int32_t (*createOffer)(YangPeer* peer, char** psdp);

    // Create answer
    int32_t (*createAnswer)(YangPeer* peer, char* answer);

    // Create HTTP answer
    int32_t (*createHttpAnswer)(YangPeer* peer, char* answer);

    // Create data channel
    int32_t (*createDataChannel)(YangPeer* peer);

    // Set local description
    int32_t (*setLocalDescription)(YangPeer* peer, char* sdp);

    // Set remote description
    int32_t (*setRemoteDescription)(YangPeer* peer, char* sdp);

    // Connect to SFU server (e.g., SRS, ZLM)
    int32_t (*connectSfuServer)(YangPeer* peer);

    // Connect to WHIP/WHEP server
    int32_t (*connectWhipWhepServer)(YangPeer* peer, char* url);

    // Close the connection
    int32_t (*close)(YangPeer* peer);

    // Check if the connection is alive
    yangbool (*isAlive)(YangPeer* peer);

    // Check if the connection is connected
    yangbool (*isConnected)(YangPeer* peer);

    // Get connection state
    YangRtcConnectionState (*getConnectionState)(YangPeer* peer);

    // Handle audio frame
    int32_t (*on_audio)(YangPeer* peer, YangFrame* audioFrame);

    // Handle video frame
    int32_t (*on_video)(YangPeer* peer, YangFrame* videoFrame);

    // Handle message frame
    int32_t (*on_message)(YangPeer* peer, YangFrame* msgFrame);

    // Send RTC message
    int32_t (*sendRtcMessage)(YangPeer* peer, YangRtcMessageType mess);
} YangPeerConnection;

#ifdef __cplusplus
extern "C" {
#endif

// Create peer connection
void yang_create_peerConnection(YangPeerConnection* peerconn);

// Destroy peer connection
void yang_destroy_peerConnection(YangPeerConnection* peerconn);

// Get HTTP SDP for P2P connection
int32_t yang_p2p_getHttpSdp(YangIpFamilyType familyType, char* httpIp, int32_t httpPort, char* localSdp, char* remoteSdp);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_YANGRTC_YANGPEERCONNECTION_H_ */
// By GST @2024/10/28