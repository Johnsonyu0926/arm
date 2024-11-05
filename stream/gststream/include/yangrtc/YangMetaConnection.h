// Filename: YangMetaConnection.h
// Score: 98

#ifndef INCLUDE_YANGRTC_YANGMETACONNECTION_H_
#define INCLUDE_YANGRTC_YANGMETACONNECTION_H_

#include <yangutil/yangavctype.h>

// Structure for AEC (Acoustic Echo Cancellation)
typedef struct {
    void* session;

    // Initialize AEC
    void (*init)(void* session, int32_t sample, int32_t channel, int32_t echopath);

    // Close AEC
    void (*closeAec)(void* session);

    // Capture echo
    void (*echoCapture)(void* session, short* rec, short* out);

    // Run preprocessing
    void (*preprocessRun)(void* session, short* pcm);

    // Reset echo state
    void (*echoStateReset)(void* session);

    // Playback echo
    void (*echoPlayback)(void* session, short* play);

    // Cancel echo
    void (*echoCancellation)(void* session, const short* rec, const short* play, short* out);
} YangAec;

// Structure for Meta Connection
typedef struct {
    void* session;

    // Initialize meta connection
    void (*init)(void* session, YangStreamConfig* stream, void* user);

    // Initialize parameters
    int32_t (*initParam)(void* session, char* url, YangStreamDirection opt);

    // Parse header
    void (*parseHeader)(YangVideoCodec codec, uint8_t* buf, uint8_t* src, int32_t* hLen);

    // Connect to SFU server
    int32_t (*connectSfuServer)(void* session, int32_t mediaServer);

    // Close connection
    int32_t (*close)(void* session);

    // Set extra data
    void (*setExtradata)(void* session, YangVideoCodec codec, uint8_t* extradata, int32_t extradata_size);

    // Handle audio frame
    int32_t (*on_audio)(void* session, YangFrame* audioFrame);

    // Handle video frame
    int32_t (*on_video)(void* session, YangFrame* videoFrame);

    // Handle message frame
    int32_t (*on_message)(void* session, YangFrame* videoFrame);

    // Check if connected
    int32_t (*isConnected)(void* session);

    // Notify video reception
    int32_t (*recvvideoNotify)(void* session, YangRtcMessageType mess);
} YangMetaConnection;

// Create meta connection
void yang_create_metaConnection(YangMetaConnection* metaconn);

// Destroy meta connection
void yang_destroy_metaConnection(YangMetaConnection* metaconn);

// Create AEC
void yang_create_aec(YangAec* aec);

// Destroy AEC
void yang_destroy_aec(YangAec* aec);

#endif /* INCLUDE_YANGRTC_YANGMETACONNECTION_H_ */
// By GST @2024/10/28