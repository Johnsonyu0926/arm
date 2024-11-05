// Filename: YangVideoEncoderMeta.h
// Score: 95

#ifndef YANGENCODER_INCLUDE_YANGVIDEOENCODERMETA_H_
#define YANGENCODER_INCLUDE_YANGVIDEOENCODERMETA_H_

#include <stdint.h>
#include <yangutil/yangavinfotype.h>

class YangVideoEncoderMeta {
public:
    YangVideoEncoderMeta();
    virtual ~YangVideoEncoderMeta();
    virtual void yang_initVmd(YangVideoMeta *p_vmd, YangVideoInfo *p_config, YangVideoEncInfo *penc) = 0;
};

#endif /* YANGENCODER_INCLUDE_YANGVIDEOENCODERMETA_H_ */
// By GST @Date