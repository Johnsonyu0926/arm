// Filename: YangStreamType.h
// Score: 98

#ifndef YangStreamType_YangStreamType_H_
#define YangStreamType_YangStreamType_H_

#include <yangutil/yangavtype.h>

#ifdef __cplusplus

// Media configuration callback class
class YangMediaConfigCallback {
public:
    // Constructor
    YangMediaConfigCallback() {};

    // Destructor
    virtual ~YangMediaConfigCallback() {};

    // Pure virtual function to set media configuration
    virtual void setMediaConfig(int32_t puid, YangAudioParam* audio, YangVideoParam* video) = 0;
};

// Send request callback class
class YangSendRequestCallback {
public:
    // Constructor
    YangSendRequestCallback() {};

    // Destructor
    virtual ~YangSendRequestCallback() {};

    // Pure virtual function to send request
    virtual void sendRequest(int32_t puid, uint32_t ssrc, YangRequestType req) = 0;
};

#endif // __cplusplus

#endif // YangStreamType_YangStreamType_H_
// By GST @2024/10/28