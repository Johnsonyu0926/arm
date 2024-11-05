// Filename: YangPlayerHandle.h
// Score: 98

#ifndef INCLUDE_YANGPLAYER_YANGPLAYERHANDLE_H_
#define INCLUDE_YANGPLAYER_YANGPLAYERHANDLE_H_

#include <yangutil/buffer/YangVideoBuffer.h>
#include <yangutil/sys/YangSysMessageI.h>
#include <yangutil/yangavinfotype.h>

// Abstract class for player handle
class YangPlayerHandle {
public:
    YangPlayerHandle() = default;
    virtual ~YangPlayerHandle() = default;

    virtual YangVideoBuffer* getVideoBuffer() = 0;
    virtual int play(char* url) = 0;
    virtual int32_t playRtc(int32_t puid, char* localIp, char* server, int32_t pport, char* app, char* stream) = 0;
    virtual int32_t playRtc(int32_t puid, char* url) = 0;
    virtual void stopPlay() = 0;

    static YangPlayerHandle* createPlayerHandle(YangContext* pcontext, YangSysMessageI* pmessage);
};

#endif /* INCLUDE_YANGPLAYER_YANGPLAYERHANDLE_H_ */
// By GST @2024/10/28