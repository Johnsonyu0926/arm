// Filename: YangH265EncoderMeta.cpp
// Score: 100

//
// Copyright (c) 2019-2022 yanggaofeng
//

#include "YangH265EncoderMeta.h"

YangH265EncoderMeta::YangH265EncoderMeta() {
    m_lib.loadObject("libx265");
    loadLib();
}

YangH265EncoderMeta::~YangH265EncoderMeta() {
    unloadLib();
    m_lib.unloadObject();
}

void YangH265EncoderMeta::loadLib() {
    yang_x265_param_default = (void (*)(x265_param*)) m_lib.loadFunction("x265_param_default");
    yang_x265_param_default_preset = (int32_t (*)(x265_param *, const char *preset, const char *tune)) m_lib.loadFunction("x265_param_default_preset");

    char s[30];
    memset(s, 0, 30);
    sprintf(s, "x265_encoder_open_%d", X265_BUILD);
    yang_x265_encoder_open = (x265_encoder* (*)(x265_param*)) m_lib.loadFunction(s);

    yang_x265_param_alloc = (x265_param* (*)(void)) m_lib.loadFunction("x265_param_alloc");
    yang_x265_param_free = (void (*)(x265_param*)) m_lib.loadFunction("x265_param_free");
    yang_x265_param_apply_profile = (int32_t (*)(x265_param*, const char *profile)) m_lib.loadFunction("x265_param_apply_profile");
    yang_x265_encoder_headers = (int32_t (*)(x265_encoder*, x265_nal **pp_nal, int32_t *pi_nal)) m_lib.loadFunction("x265_encoder_headers");
    yang_x265_encoder_close = (void (*)(x265_encoder*)) m_lib.loadFunction("x265_encoder_close");
}

void YangH265EncoderMeta::unloadLib() {
    yang_x265_param_default_preset = nullptr;
    yang_x265_encoder_open = nullptr;
    yang_x265_param_alloc = nullptr;
    yang_x265_param_free = nullptr;
    yang_x265_param_default = nullptr;
    yang_x265_param_apply_profile = nullptr;
    yang_x265_encoder_headers = nullptr;
    yang_x265_encoder_close = nullptr;
}

void YangH265EncoderMeta::yang_getSpsPps(YangH2645Conf *p264, YangVideoInfo *p_yvp, YangVideoEncInfo *penc) {
    x265_encoder *p265Handle = nullptr;
    x265_param *param = yang_x265_param_alloc();
    yang_x265_param_default(param);

    if (penc->preset < 5) {
        yang_x265_param_default_preset(param, x265_preset_names[penc->preset], x265_tune_names[3]);
    }

    YangH265EncoderSoft::initX265Param(p_yvp, penc, param);
    yang_x265_param_apply_profile(param, x265_profile_names[0]);

    if ((p265Handle = yang_x265_encoder_open(param)) == nullptr) {
        printf("x265_encoder_open failed\n");
        return;
    }

    int32_t iNal = 0;
    x265_nal *p264Nal = nullptr;
    yang_x265_encoder_headers(p265Handle, &p264Nal, &iNal);

    for (int32_t i = 0; i < iNal; ++i) {
        switch (p264Nal[i].type) {
            case NAL_UNIT_VPS:
                p264->vpsLen = p264Nal[i].sizeBytes - 4;
                memcpy(p264->vps, p264Nal[i].payload + 4, p264->vpsLen);
                break;
            case NAL_UNIT_SPS:
                p264->spsLen = p264Nal[i].sizeBytes - 4;
                memcpy(p264->sps, p264Nal[i].payload + 4, p264->spsLen);
                break;
            case NAL_UNIT_PPS:
                p264->ppsLen = p264Nal[i].sizeBytes - 4;
                memcpy(p264->pps, p264Nal[i].payload + 4, p264->ppsLen);
                break;
            default:
                break;
        }
    }

    yang_x265_encoder_close(p265Handle);
    yang_x265_param_free(param);
}

void YangH265EncoderMeta::yang_initVmd(YangVideoMeta *p_vmd, YangVideoInfo *p_config, YangVideoEncInfo *penc) {
    if (!p_vmd->isInit) {
        yang_getSpsPps(&p_vmd->mp4Meta, p_config, penc);
        yang_getConfig_Flv_H265(&p_vmd->mp4Meta, p_vmd->livingMeta.buffer, &p_vmd->livingMeta.bufLen);
        p_vmd->isInit = 1;
    }
}

// By GST @Date