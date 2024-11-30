#include "volume.h"

// 创建Volume对象
Volume* create_volume(const char* id, int level) {
    Volume* volume = (Volume*)malloc(sizeof(Volume));
    if (!volume) return NULL;
    volume->id = strdup(id);
    volume->level = level;
    return volume;
}

// 销毁Volume对象
void destroy_volume(Volume* volume) {
    if (volume) {
        free(volume->id);
        free(volume);
    }
}

// 获取Volume的ID
const char* get_volume_id(const Volume* volume) {
    return volume->id;
}

// 设置Volume的ID
void set_volume_id(Volume* volume, const char* newId) {
    free(volume->id);
    volume->id = strdup(newId);
}

// 获取Volume的级别
int get_volume_level(const Volume* volume) {
    return volume->level;
}

// 设置Volume的级别
void set_volume_level(Volume* volume, int newLevel) {
    volume->level = newLevel;
}

// 将Volume对象转换为JSON
json_t* volume_to_json(const Volume* volume) {
    json_t* j = json_object();
    json_object_set_new(j, "id", json_string(volume->id));
    json_object_set_new(j, "level", json_integer(volume->level));
    return j;
}

// 从JSON创建Volume对象
Volume* volume_from_json(const json_t* j) {
    const char* id = json_string_value(json_object_get(j, "id"));
    int level = json_integer_value(json_object_get(j, "level"));
    return create_volume(id, level);
}

// 创建VolumeManager对象
VolumeManager* create_volume_manager(const char* configPath) {
    VolumeManager* manager = (VolumeManager*)malloc(sizeof(VolumeManager));
    if (!manager) return NULL;
    manager->configPath = strdup(configPath);
    manager->volume.id = NULL;
    manager->volume.level = 0;
    return manager;
}

// 销毁VolumeManager对象
void destroy_volume_manager(VolumeManager* manager) {
    if (manager) {
        free(manager->configPath);
        destroy_volume(&manager->volume);
        free(manager);
    }
}

// 加载VolumeManager配置
bool load_volume_manager(VolumeManager* manager) {
    FILE* file = fopen(manager->configPath, "r");
    if (!file) {
        fprintf(stderr, "Failed to open config file: %s\n", manager->configPath);
        return false;
    }

    json_error_t error;
    json_t* j = json_loadf(file, 0, &error);
    fclose(file);
    if (!j) {
        fprintf(stderr, "Error loading config file: %s\n", error.text);
        return false;
    }

    Volume* volume = volume_from_json(j);
    set_volume_manager_volume(manager, volume);
    json_decref(j);
    return true;
}

// 保存VolumeManager配置
bool save_volume_manager(const VolumeManager* manager) {
    json_t* j = volume_to_json(&manager->volume);

    FILE* file = fopen(manager->configPath, "w");
    if (!file) {
        fprintf(stderr, "Failed to open config file for writing: %s\n", manager->configPath);
        json_decref(j);
        return false;
    }

    if (json_dumpf(j, file, JSON_INDENT(4)) != 0) {
        fprintf(stderr, "Error saving config file\n");
        fclose(file);
        json_decref(j);
        return false;
    }

    fclose(file);
    json_decref(j);
    return true;
}

// 获取VolumeManager的Volume
const Volume* get_volume_manager_volume(const VolumeManager* manager) {
    return &manager->volume;
}

// 设置VolumeManager的Volume
void set_volume_manager_volume(VolumeManager* manager, Volume* newVolume) {
    destroy_volume(&manager->volume);
    manager->volume = *newVolume;
    free(newVolume);
}