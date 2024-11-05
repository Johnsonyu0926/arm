// Filename: YangPlayApp.h
// Score: 98

#ifndef YANGAPP_YANGPLAYAPP_H_
#define YANGAPP_YANGPLAYAPP_H_

#include <yangutil/buffer/YangAudioPlayBuffer.h>
#include <yangutil/buffer/YangVideoBuffer.h>
#include <yangavutil/audio/YangRtcAec.h>
#include <yangdecoder/YangVideoDecoderHandles.h>
#include <yangaudiodev/linux/YangAudioPlayLinux.h>
#include <yangaudiodev/mac/YangAudioPlayMac.h>
#include <yangaudiodev/win/YangWinAudioApiRender.h>
#include <yangaudiodev/android/YangAudioPlayAndroid.h>
#include <vector>

using namespace std;

// Class for handling player play
class YangPlayerPlay {
public:
    YangPlayerPlay();
    virtual ~YangPlayerPlay();
    void initAudioPlay(YangContext* paudio);
    void startAudioPlay();
    void setInAudioList(YangAudioPlayBuffer *paudioList);
    void stopAll();

private:
#if Yang_OS_WIN
    YangWinAudioApiRender *m_audioPlay;
#elif Yang_OS_ANDROID
    YangAudioPlayAndroid *m_audioPlay;
#elif Yang_OS_APPLE
    YangAudioPlayMac *m_audioPlay;
#else
    YangAudioPlayLinux *m_audioPlay;
#endif

    int32_t vm_audio_player_start;
};

#endif /* YANGAPP_YANGPLAYAPP_H_ */
// By GST @2024/10/28