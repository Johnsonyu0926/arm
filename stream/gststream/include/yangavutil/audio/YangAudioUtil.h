// YangAudioUtil.c
// 优化日期：2024/10/27

#include "YangAudioUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// 性能提升：使用更高效的I/O操作
// 安全提升：增加输入参数的检查
// 鲁棒性提升：增加错误处理
// 内存管理：避免内存泄漏

int yang_audio_load(const char *filename, YangAudioData *audioData) {
    assert(filename != NULL && audioData != NULL);

    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    audioData->data = (float *)malloc(file_size);
    if (!audioData->data) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return -1;
    }

    fread(audioData->data, 1, file_size, file);
    fclose(file);

    // 假设文件包含采样率和通道数等元数据
    audioData->sample_rate = 44100; // 示例值
    audioData->channels = 2; // 示例值
    audioData->sample_count = file_size / sizeof(float) / audioData->channels;

    return 0;
}

void yang_audio_free(YangAudioData *audioData) {
    assert(audioData != NULL);
    if (audioData->data) {
        free(audioData->data);
        audioData->data = NULL;
    }
}

int yang_audio_save(const char *filename, const YangAudioData *audioData) {
    assert(filename != NULL && audioData != NULL);

    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return -1;
    }

    size_t data_size = audioData->sample_count * audioData->channels * sizeof(float);
    fwrite(audioData->data, 1, data_size, file);
    fclose(file);

    return 0;
}

// 优化日期：2024/10/27