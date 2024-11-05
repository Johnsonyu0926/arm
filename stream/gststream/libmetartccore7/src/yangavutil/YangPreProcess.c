﻿#include <yangavutil/audio/YangPreProcess.h>
#include <speex/speex_echo.h>
#include <speex/speex_preprocess.h>

typedef struct {
    int32_t channel;
    int32_t frameSize;
    int32_t sampleRate;
    int32_t echoPath;
    SpeexPreprocessState *preprocState;
} YangPreProcessContext;

void yang_preprocess_init(void* pcontext, int32_t pFrameSize, int32_t sampleRate, int32_t pchannel) {
    if (pcontext == NULL) return;
    YangPreProcessContext* context = (YangPreProcessContext*)pcontext;
    context->frameSize = pFrameSize;
    context->sampleRate = sampleRate;
    context->channel = pchannel;
    int32_t frame_size = context->frameSize;
    if (context->preprocState == NULL) {
        context->preprocState = speex_preprocess_state_init(frame_size, context->sampleRate);
    }

    int32_t noiseSuppress = -25;
    int32_t i = 1;
    speex_preprocess_ctl(context->preprocState, SPEEX_PREPROCESS_SET_DENOISE, &i);
    speex_preprocess_ctl(context->preprocState, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noiseSuppress);
    i = 0;
    speex_preprocess_ctl(context->preprocState, SPEEX_PREPROCESS_SET_AGC, &i);
    i = sampleRate;
    speex_preprocess_ctl(context->preprocState, SPEEX_PREPROCESS_SET_AGC_LEVEL, &i);
    speex_preprocess_ctl(context->preprocState, SPEEX_PREPROCESS_SET_DEREVERB, &i);
    float f = .0;
    speex_preprocess_ctl(context->preprocState, SPEEX_PREPROCESS_SET_DEREVERB_DECAY, &f);
    speex_preprocess_ctl(context->preprocState, SPEEX_PREPROCESS_SET_DEREVERB_LEVEL, &f);
}

void yang_preprocess_closePre(void* pcontext) {
    if (pcontext == NULL) return;
    YangPreProcessContext* context = (YangPreProcessContext*)pcontext;
    if (context->preprocState) {
        speex_preprocess_state_destroy(context->preprocState);
    }
    context->preprocState = NULL;
}

void yang_preprocess_preprocess_run(void* pcontext, short *pcm) {
    if (pcontext == NULL) return;
    YangPreProcessContext* context = (YangPreProcessContext*)pcontext;
    if (context->preprocState) {
        speex_preprocess_run(context->preprocState, pcm);
    }
}

void yang_preprocess_state_reset(void* pcontext) {
    if (pcontext == NULL) return;
    // No implementation needed as per original code
}

void yang_create_preProcess(YangPreProcess* pre) {
    if (pre == NULL) return;
    pre->context = (YangPreProcessContext*)yang_calloc(sizeof(YangPreProcessContext), 1);
    pre->init = yang_preprocess_init;
    pre->closePreprocess = yang_preprocess_closePre;
    pre->preprocess_run = yang_preprocess_preprocess_run;
    pre->state_reset = yang_preprocess_state_reset;
}

void yang_destroy_preProcess(YangPreProcess* pre) {
    if (pre == NULL || pre->context == NULL) return;
    YangPreProcessContext* context = (YangPreProcessContext*)pre->context;
    if (context && context->preprocState) {
        speex_preprocess_state_destroy(context->preprocState);
    }
    yang_free(pre->context);
}

// By GST @Date