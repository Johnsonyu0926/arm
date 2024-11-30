#include "custom_audio.h"
#include <jansson.h>
#include <string.h>

// 创建自定义音频
CustomAudio* create_custom_audio(int type, const char* fileName, int size) {
    CustomAudio* audio = (CustomAudio*)malloc(sizeof(CustomAudio));
    if (!audio) return NULL;
    audio->type = type;
    audio->fileName = strdup(fileName);
    audio->size = size;
    return audio;
}

// 销毁自定义音频
void destroy_custom_audio(CustomAudio* audio) {
    if (audio) {
        free(audio->fileName);
        free(audio);
    }
}

// 初始化自定义音频管理器
void init_custom_audio_manager(CustomAudioManager* manager) {
    manager->audioFiles = NULL;
    manager->audioFileCount = 0;
}

// 释放自定义音频管理器
void free_custom_audio_manager(CustomAudioManager* manager) {
    for (size_t i = 0; i < manager->audioFileCount; ++i) {
        destroy_custom_audio(&manager->audioFiles[i]);
    }
    free(manager->audioFiles);
}

// 从文件加载自定义音频管理器
bool load_custom_audio_manager_from_file(CustomAudioManager* manager, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open custom audio file: %s\n", filename);
        return false;
    }

    json_error_t error;
    json_t* json = json_loadf(file, 0, &error);
    fclose(file);
    if (!json) {
        fprintf(stderr, "Error loading custom audio file: %s\n", error.text);
        return false;
    }

    size_t index;
    json_t* value;
    json_array_foreach(json, index, value) {
        int type = json_integer_value(json_object_get(value, "type"));
        const char* fileName = json_string_value(json_object_get(value, "fileName"));
        int size = json_integer_value(json_object_get(value, "size"));
        CustomAudio* audio = create_custom_audio(type, fileName, size);
        manager->audioFiles = (CustomAudio*)realloc(manager->audioFiles, (manager->audioFileCount + 1) * sizeof(CustomAudio));
        manager->audioFiles[manager->audioFileCount++] = *audio;
        free(audio);
    }

    json_decref(json);
    return true;
}

// 根据名称查找音频
const CustomAudio* find_audio_by_name(const CustomAudioManager* manager, const char* name) {
    for (size_t i = 0; i < manager->audioFileCount; ++i) {
        if (strcmp(manager->audioFiles[i].fileName, name) == 0) {
            return &manager->audioFiles[i];
        }
    }
    return NULL;
}