// Filename: YangCaptureCallback.h
// Score: 94



#ifndef YANGCAPTURE_WIN_API_YANGCAPTURECALLBACK_H_
#define YANGCAPTURE_WIN_API_YANGCAPTURECALLBACK_H_

#include <yangutil/yangavtype.h>

class YangCaptureCallback {
public:
    YangCaptureCallback() {};
    virtual ~YangCaptureCallback() {};
    virtual void captureAudioData(YangFrame* audioFrame) = 0;
};

#endif /* YANGCAPTURE_WIN_API_YANGCAPTURECALLBACK_H_ */

// By GST @Date