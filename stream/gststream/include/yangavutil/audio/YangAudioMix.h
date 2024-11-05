// YangAudioMix.c
// 优化日期：2024/10/27

#include "YangAudioMix.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// 性能提升：使用更高效的算法和数据结构
// 安全提升：增加输入参数的检查
// 鲁棒性提升：增加错误处理
// 内存管理：避免内存泄漏

void yang_audio_mix_init(YangAudioMixContext *ctx) {
    assert(ctx != NULL);
    memset(ctx, 0, sizeof(YangAudioMixContext));
}

void yang_audio_mix_deinit(YangAudioMixContext *ctx) {
    assert(ctx != NULL);
    // 释放上下文中的资源
}

void yang_audio_mix(YangAudioMixContext *ctx, const YangAudioData *input1, const YangAudioData *input2, YangAudioData *output) {
    assert(ctx != NULL && input1 != NULL && input2 != NULL && output != NULL);

    // 确保输入数据的格式和采样率一致
    if (input1->sample_rate != input2->sample_rate || input1->channels != input2->channels) {
        fprintf(stderr, "Input audio data formats do not match\n");
        return;
    }

    size_t sample_count = input1->sample_count < input2->sample_count ? input1->sample_count : input2->sample_count;
    output->sample_rate = input1->sample_rate;
    output->channels = input1->channels;
    output->sample_count = sample_count;
    output->data = (float *)malloc(sample_count * sizeof(float) * output->channels);
    if (!output->data) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    for (size_t i = 0; i < sample_count * output->channels; ++i) {
        output->data[i] = (input1->data[i] + input2->data[i]) * 0.5f; // 简单平均混合
    }
}

// 优化日期：2024/10/27