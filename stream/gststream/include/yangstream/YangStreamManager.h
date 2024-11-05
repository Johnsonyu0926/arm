// Filename: YangStreamManager1.h
// Score: 98

#ifndef INCLUDE_YANGSTREAM_YANGSTREAMMANAGER1_H_
#define INCLUDE_YANGSTREAM_YANGSTREAMMANAGER1_H_

#include <yangstream/YangStreamType.h>
#include <yangstream/YangSynBuffer.h>
#include <yangstream/YangSynBufferManager.h>
#include <vector>

// Stream state notification class
class YangStreamStateNotify {
public:
    // Constructor
    YangStreamStateNotify() {};

    // Destructor
    virtual ~YangStreamStateNotify() {};

    // Pure virtual function to notify stream state
    virtual void streamStateNotify(int32_t puid, YangRtcDirection opt, bool isConnect) = 0;
};

// Stream manager class
class YangStreamManager {
public:
    // Constructor
    YangStreamManager(YangSynBufferManager* synMgr);

    // Destructor
    virtual ~YangStreamManager();

    // Public member variables
    YangStreamStateNotify* m_streamState;

    // Public member functions
    int32_t getPlayOnlineCount();
    int32_t getPushOnlineCount();
    void connectNotify(int32_t puid, YangRtcDirection opt, bool isConnect);

    void setMediaConfig(int32_t puid, YangAudioParam* audio, YangVideoParam* video);
    void sendRequest(int32_t puid, uint32_t ssrc, YangRequestType req);

    void setSendRequestCallback(YangSendRequestCallback* request);
    void setDecoderMediaConfigCallback(YangMediaConfigCallback* dec);
    void setRenderMediaConfigCallback(YangMediaConfigCallback* render);

    int32_t getAudioClock();
    int32_t getVideoClock();

private:
    // Private member variables
    YangSendRequestCallback* m_sendRequestCb;
    YangMediaConfigCallback* m_mediaConfig_dec;
    YangMediaConfigCallback* m_mediaConfig_render;
    YangSynBufferManager* m_synMgr;
    int32_t m_videoClock;
    int32_t m_audioClock;

    std::vector<int32_t> m_onlinePlayUsers;
    std::vector<int32_t> m_onlinePushUsers;
};

#endif /* INCLUDE_YANGSTREAM_YANGSTREAMMANAGER1_H_ */
// By GST @2024/10/28