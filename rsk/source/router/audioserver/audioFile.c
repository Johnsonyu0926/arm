#include "audioFile.h"
#include <stdlib.h>
#include <string.h>

// 创建音频文件
AudioFile* create_audio_file(int type, const char* fileName, size_t size) {
    AudioFile* file = (AudioFile*)malloc(sizeof(AudioFile));
    if (!file) return NULL;
    file->type = type;
    file->fileName = strdup(fileName);
    file->size = size;
    return file;
}

// 销毁音频文件
void destroy_audio_file(AudioFile* file) {
    if (file) {
        free(file->fileName);
        free(file);
    }
}

// 将音频文件转换为JSON对象
json_t* audio_file_to_json(const AudioFile* file) {
    json_t* json = json_object();
    json_object_set_new(json, "type", json_integer(file->type));
    json_object_set_new(json, "fileName", json_string(file->fileName));
    json_object_set_new(json, "size", json_integer(file->size));
    return json;
}

// 从JSON对象创建音频文件
AudioFile* audio_file_from_json(const json_t* json) {
    int type = json_integer_value(json_object_get(json, "type"));
    const char* fileName = json_string_value(json_object_get(json, "fileName"));
    size_t size = json_integer_value(json_object_get(json, "size"));
    return create_audio_file(type, fileName, size);
}