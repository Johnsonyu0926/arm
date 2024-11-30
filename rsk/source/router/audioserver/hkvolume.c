#include "hkvolume.h"

// 创建音量对象
HkVolume* create_hk_volume(const char* id, int volume) {
    HkVolume* vol = (HkVolume*)malloc(sizeof(HkVolume));
    if (!vol) return NULL;
    vol->id = strdup(id);
    vol->volume = volume;
    return vol;
}

// 销毁音量对象
void destroy_hk_volume(HkVolume* volume) {
    if (volume) {
        free(volume->id);
        free(volume);
    }
}

// 将音量对象转换为JSON对象
json_t* hk_volume_to_json(const HkVolume* volume) {
    json_t* json = json_object();
    json_object_set_new(json, "id", json_string(volume->id));
    json_object_set_new(json, "volume", json_integer(volume->volume));
    return json;
}

// 从JSON对象创建音量对象
HkVolume* hk_volume_from_json(const json_t* json) {
    const char* id = json_string_value(json_object_get(json, "id"));
    int volume = json_integer_value(json_object_get(json, "volume"));
    return create_hk_volume(id, volume);
}

// 创建音量管理器
HkVolumeManager* create_hk_volume_manager(const char* configPath) {
    HkVolumeManager* manager = (HkVolumeManager*)malloc(sizeof(HkVolumeManager));
    if (!manager) return NULL;
    manager->configPath = strdup(configPath);
    return manager;
}

// 销毁音量管理器
void destroy_hk_volume_manager(HkVolumeManager* manager) {
    if (manager) {
        destroy_hk_volume(&manager->volume);
        free(manager->configPath);
        free(manager);
    }
}

// 加载音量管理器
bool load_hk_volume_manager(HkVolumeManager* manager) {
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

    HkVolume* volume = hk_volume_from_json(json);
    manager->volume = *volume;
    free(volume);
    json_decref(json);
    return true;
}

// 保存音量管理器
bool save_hk_volume_manager(const HkVolumeManager* manager) {
    json_t* json = hk_volume_to_json(&manager->volume);

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

// 获取音量
const HkVolume* get_hk_volume(const HkVolumeManager* manager) {
    return &manager->volume;
}

// 设置音量
void set_hk_volume(HkVolumeManager* manager, HkVolume* volume) {
    manager->volume = *volume;
}