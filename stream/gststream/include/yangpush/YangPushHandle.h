// Filename: YangPushHandle.h
// Score: 98

#ifndef INCLUDE_YANGPUSH_YANGPUSHHANDLE_H_
#define INCLUDE_YANGPUSH_YANGPUSHHANDLE_H_

#include <yangutil/buffer/YangVideoBuffer.h>
#include <yangutil/yangavinfotype.h>
#include <yangutil/sys/YangSysMessageI.h>
#include <yangpush/YangPushCommon.h>
#include <yangpush/YangSendVideoI.h>
#include <string>

// Interface for Push Handle
class YangPushHandle {
public:
    // Constructor
    YangPushHandle();

    // Destructor
    virtual ~YangPushHandle();

    // Publish video
    virtual int publish(char* url, yangbool isWhip) = 0;

    // Disconnect
    virtual void disconnect() = 0;

    // Initialize
    virtual void init() = 0;

    // Change source
    virtual void changeSrc(int videoSrcType, bool pisinit) = 0;

    // Get pre video buffer
    virtual YangVideoBuffer* getPreVideoBuffer() = 0;

    // Get send video interface
    virtual YangSendVideoI* getSendVideo() = 0;
};

#endif /* INCLUDE_YANGPUSH_YANGPUSHHANDLE_H_ */
// By GST @2024/10/28