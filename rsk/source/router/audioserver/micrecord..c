#include "micrecord.h"

// 创建麦克风记录
MicRecord* create_mic_record(const char* id, const char* name, const char* path) {
    MicRecord* record = (MicRecord*)malloc(sizeof(MicRecord));
    if (!record) return NULL;
    record->id = strdup(id);
    record->name = strdup(name);
    record->path = strdup(path);
    return record;
}

// 销毁麦克风记录
void destroy_mic_record(MicRecord* record) {
    if (record) {
        free(record->id);
        free(record->name);
        free(record->path);
        free(record);
    }
}

// 将麦克风记录转换为JSON对象
json_t* mic_record_to_json(const MicRecord* record) {
    json_t* json = json_object();
    json_object_set_new(json, "id", json_string(record->id));
    json_object_set_new(json, "name", json_string(record->name));
    json_object_set_new(json, "path", json_string(record->path));
    return json;
}

// 从JSON对象创建麦克风记录
MicRecord* mic_record_from_json(const json_t* json) {
    const char* id = json_string_value(json_object_get(json, "id"));
    const char* name = json_string_value(json_object_get(json, "name"));
    const char* path = json_string_value(json_object_get(json, "path"));
    return create_mic_record(id, name, path);
}

// 创建麦克风记录管理器
MicRecordManager* create_mic_record_manager(const char* configPath) {
    MicRecordManager* manager = (MicRecordManager*)malloc(sizeof(MicRecordManager));
    if (!manager) return NULL;
    manager->records = NULL;
    manager->recordCount = 0;
    manager->configPath = strdup(configPath);
    return manager;
}

// 销毁麦克风记录管理器
void destroy_mic_record_manager(MicRecordManager* manager) {
    if (manager) {
        for (size_t i = 0; i < manager->recordCount; ++i) {
            destroy_mic_record(&manager->records[i]);
        }
        free(manager->records);
        free(manager->configPath);
        free(manager);
    }
}

// 加载麦克风记录管理器
bool load_mic_record_manager(MicRecordManager* manager) {
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
        MicRecord* record = mic_record_from_json(value);
        manager->records = (MicRecord*)realloc(manager->records, (manager->recordCount + 1) * sizeof(MicRecord));
        manager->records[manager->recordCount++] = *record;
        free(record);
    }

    json_decref(json);
    return true;
}

// 保存麦克风记录管理器
bool save_mic_record_manager(const MicRecordManager* manager) {
    json_t* json = json_array();
    for (size_t i = 0; i < manager->recordCount; ++i) {
        json_t* recordJson = mic_record_to_json(&manager->records[i]);
        json_array_append_new(json, recordJson);
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

// 添加记录到管理器
void add_record_to_manager(MicRecordManager* manager, MicRecord* record) {
    manager->records = (MicRecord*)realloc(manager->records, (manager->recordCount + 1) * sizeof(MicRecord));
    manager->records[manager->recordCount++] = *record;
}

// 从管理器中移除记录
void remove_record_from_manager(MicRecordManager* manager, const char* id) {
    for (size_t i = 0; i < manager->recordCount; ++i) {
        if (strcmp(manager->records[i].id, id) == 0) {
            destroy_mic_record(&manager->records[i]);
            for (size_t j = i; j < manager->recordCount - 1; ++j) {
                manager->records[j] = manager->records[j + 1];
            }
            --manager->recordCount;
            manager->records = (MicRecord*)realloc(manager->records, manager->recordCount * sizeof(MicRecord));
            break;
        }
    }
}

// 在管理器中查找记录
MicRecord* find_record_in_manager(MicRecordManager* manager, const char* id) {
    for (size_t i = 0; i < manager->recordCount; ++i) {
        if (strcmp(manager->records[i].id, id) == 0) {
            return &manager->records[i];
        }
    }
    return NULL;
}