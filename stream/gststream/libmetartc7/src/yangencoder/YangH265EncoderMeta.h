// Copyright (c) 2019-2022 yanggaofeng
//

#ifndef __YangH265EncoderMeta__
#define __YangH265EncoderMeta__

#include <yangavutil/video/YangMeta.h>
#include <yangencoder/YangH265EncoderSoft.h>
#include <yangutil/sys/YangLoadLib.h>
#include <x265.h>

class YangH265EncoderMeta {
public:
    YangH265EncoderMeta();
    ~YangH265EncoderMeta();

    void yang_getSpsPps(YangH2645Conf *p264, YangVideoInfo *p_yvp, YangVideoEncInfo *penc);
    void yang_initVmd(YangVideoMeta *p_vmd, YangVideoInfo *p_config, YangVideoEncInfo *penc);

private:
    void loadLib();
    void unloadLib();

    YangLoadLib m_lib;

    void (*yang_x265_param_default)(x265_param*);
    int32_t (*yang_x265_param_default_preset)(x265_param *, const char *preset, const char *tune);
    x265_encoder* (*yang_x265_encoder_open)(x265_param*);
    x265_param* (*yang_x265_param_alloc)(void);
    void (*yang_x265_param_free)(x265_param*);
    int32_t (*yang_x265_param_apply_profile)(x265_param*, const char *profile);
    int32_t (*yang_x265_encoder_headers)(x265_encoder*, x265_nal **pp_nal, int32_t *pi_nal);
    void (*yang_x265_encoder_close)(x265_encoder*);
};

#endif /* __YangH265EncoderMeta__ */

// By GST @Date