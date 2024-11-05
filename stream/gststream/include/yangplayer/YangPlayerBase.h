// Filename: MeetingPlayoBase.h
// Score: 98

#ifndef MEETINGPLAYOBASE_H
#define MEETINGPLAYOBASE_H

#include <yangplayer/YangPlayerDecoder.h>
#include <yangplayer/YangPlayerPlay.h>
#include <vector>
#include "yangutil/buffer/YangAudioEncoderBuffer.h"
#include "yangutil/buffer/YangAudioPlayBuffer.h"
#include "yangutil/buffer/YangVideoDecoderBuffer.h"

using namespace std;

// Base class for meeting player
class YangPlayerBase {
public:
    YangPlayerBase();
    virtual ~YangPlayerBase();

    void startAudioDecoder(YangAudioEncoderBuffer *prr);
    void startVideoDecoder(YangVideoDecoderBuffer *prr);
    void init(YangContext* audio);
    void startAudioPlay(YangContext* paudio);
    void stopAll();

protected:
    YangPlayerDecoder *m_ydb;
    YangPlayerPlay *m_ypb;

private:
    // Add any private members here if needed
};

#endif // MEETINGPLAYOBASE_H
// By GST @2024/10/28