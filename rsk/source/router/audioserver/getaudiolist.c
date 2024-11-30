#include "getaudiolist.h"

// 创建音频文件信息
AudioFileInfo* create_audio_file_info(const char* id, const char* name, const char* path) {
    AudioFileInfo* file = (AudioFileInfo*)malloc(sizeof(AudioFileInfo));
    if (!file) return NULL;
    file->id = strdup(id);
    file->name = strdup(name);
    file->path = strdup(path);
    return file;
}

// 销毁音频文件信息
void destroy_audio_file_info(AudioFileInfo* file) {
    if (file) {
        free(file->id);
        free(file->name);
        free(file->path);
        free(file);
    }
}

// 将音频文件信息转换为JSON对象
json_t* audio_file_info_to_json(const AudioFileInfo* file) {
    json_t* json = json_object();
    json_object_set_new(json, "id", json_string(file->id));
    json_object_set_new(json, "name", json_string(file->name));
    json_object_set_new(json, "path", json_string(file->path));
    return json;
}

// 从JSON对象创建音频文件信息
AudioFileInfo* audio_file_info_from_json(const json_t* json) {
    const char* id = json_string_value(json_object_get(json, "id"));
    const char* name = json_string_value(json_object_get(json, "name"));
    const char* path = json_string_value(json_object_get(json, "path"));
    return create_audio_file_info(id, name, path);
}

// 创建音频文件信息管理器
AudioFileInfoManager* create_audio_file_info_manager(const char* configPath) {
    AudioFileInfoManager* manager = (AudioFileInfoManager*)malloc(sizeof(AudioFileInfoManager));
    if (!manager) return NULL;
    manager->files = NULL;
    manager->fileCount = 0;
    manager->configPath = strdup(configPath);
    return manager;
}

// 销毁音频文件信息管理器
void destroy_audio_file_info_manager(AudioFileInfoManager* manager) {
    if (manager) {
        for (size_t i = 0; i < manager->fileCount; ++i) {
            destroy_audio_file_info(&manager->files[i]);
        }
        free(manager->files);
        free(manager->configPath);
        free(manager);
    }
}

// 加载音频文件信息管理器
bool load_audio_file_info_manager(AudioFileInfoManager* manager) {
    FILE* file = fopen(manager->configPath, "r");
    if (!file) {
        fprintf(stderr, "Config file does not exist: %s\n", manager->configPath);
        return false;
    }

    json_error_t error;
    json_t* json = json_loadf(file, 0, &error);
    fclose(file);
    if (!json) {
        fprintf(stderr, "Error loading config file: %s\n", error.text);
        return false;
    }

    size_t index;
    json_t* value;
    json_array_foreach(json, index, value) {
        AudioFileInfo* file = audio_file_info_from_json(value);
        manager->files = (AudioFileInfo*)realloc(manager->files, (manager->fileCount + 1) * sizeof(AudioFileInfo));
        manager->files[manager->fileCount++] = *file;
        free(file);
    }

    json_decref(json);
    return true;
}

// 保存音频文件信息管理器
bool save_audio_file_info_manager(const AudioFileInfoManager* manager) {
    json_t* json = json_array();
    for (size_t i = 0; i < manager->fileCount; ++i) {
        json_t* fileJson = audio_file_info_to_json(&manager->files[i]);
        json_array_append_new(json, fileJson);
    }

    FILE* file = fopen(manager->configPath, "w");
    if (!file) {
        fprintf(stderr, "Failed to open config file for writing: %s\n", manager->configPath);
        json_decref(json);
        return false;
    }

    if (json_dumpf(json, file, JSON_INDENT(4)) != 0) {
        fprintf(stderr, "Error saving config file\n");
        fclose(file);
        json_decref(json);
        return false;
    }

    fclose(file);
    json_decref(json);
    return true;
}

// 添加文件到管理器
void add_file_to_manager(AudioFileInfoManager* manager, AudioFileInfo* file) {
    manager->files = (AudioFileInfo*)realloc(manager->files, (manager->fileCount + 1) * sizeof(AudioFileInfo));
    manager->files[manager->fileCount++] = *file;
}

// 从管理器中移除文件
void remove_file_from_manager(AudioFileInfoManager* manager, const char* id) {
    for (size_t i = 0; i < manager->fileCount; ++i) {
        if (strcmp(manager->files[i].id, id) == 0) {
            destroy_audio_file_info(&manager->files[i]);
            for (size_t j = i; j < manager->fileCount - 1; ++j) {
                manager->files[j] = manager->files[j + 1];
            }
            --manager->fileCount;
            manager->files = (AudioFileInfo*)realloc(manager->files, manager->fileCount * sizeof(AudioFileInfo));
            break;
        }
    }
}

// 在管理器中查找文件
AudioFileInfo* find_file_in_manager(AudioFileInfoManager* manager, const char* id) {
    for (size_t i = 0; i < manager->fileCount; ++i) {
        if (strcmp(manager->files[i].id, id) == 0) {
            return &manager->files[i];
        }
    }
    return NULL;
}