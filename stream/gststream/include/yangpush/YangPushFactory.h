// Filename: YangPushFactory.h
// Score: 98

#ifndef INCLUDE_YANGPUSH_YANGPUSHFACTORY_H_
#define INCLUDE_YANGPUSH_YANGPUSHFACTORY_H_

#include <yangpush/YangPushHandle.h>
#include <yangutil/sys/YangSysMessageHandle.h>
#include <yangpush/YangSendVideoI.h>

// Factory class for creating push handles and message handles
class YangPushFactory {
public:
    // Constructor
    YangPushFactory();

    // Destructor
    virtual ~YangPushFactory();

    // Create push handle
    YangPushHandle* createPushHandle(bool hasAudio, bool initVideo, int pvideotype, YangVideoInfo* screenvideo, YangVideoInfo* outvideo, YangContext* pcontext, YangSysMessageI* pmessage);

    // Create push message handle
    YangSysMessageHandle* createPushMessageHandle(bool hasAudio, bool initVideo, int pvideotype, YangVideoInfo* screenvideo, YangVideoInfo* outvideo, YangContext* pcontext, YangSysMessageI* pmessage, YangSysMessageHandleI* pmessagehandle);

    // Get pre video buffer
    YangVideoBuffer* getPreVideoBuffer(YangSysMessageHandle* pmessageHandle);

    // Get send video interface
    YangSendVideoI* getSendVideo(YangSysMessageHandle* pmessageHandle);
};

#endif /* INCLUDE_YANGPUSH_YANGPUSHFACTORY_H_ */
// By GST @2024/10/28