// Filename: YangPlayFactory.h
// Score: 98

#ifndef YANGPLAYER_INCLUDE_YANGPLAYFACTORY_H_
#define YANGPLAYER_INCLUDE_YANGPLAYFACTORY_H_

#include <yangaudiodev/YangAudioPlay.h>

// Enum for audio play types
enum YangAudioPlayType {
    Yang_AP_SDL,
    Yang_AP_ALSA,
};

// Factory class for creating audio play instances
class YangPlayFactory {
public:
    YangPlayFactory();
    virtual ~YangPlayFactory();
    YangAudioPlay* createAudioPlay(YangAudioInfo *pcontext);
    YangAudioPlay* createAudioPlay(YangAudioPlayType paet, YangAudioInfo *pcontext);
};

#endif /* YANGPLAYER_INCLUDE_YANGPLAYFACTORY_H_ */
// By GST @2024/10/28