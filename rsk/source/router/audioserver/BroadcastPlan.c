#include "broadcast_plan.h"

// 创建广播计划
BroadcastPlan* create_broadcast_plan(int id, const char* name, char** audioFiles, size_t audioFileCount) {
    BroadcastPlan* plan = (BroadcastPlan*)malloc(sizeof(BroadcastPlan));
    if (!plan) return NULL;
    plan->id = id;
    plan->name = strdup(name);
    plan->audioFiles = (char**)malloc(audioFileCount * sizeof(char*));
    for (size_t i = 0; i < audioFileCount; ++i) {
        plan->audioFiles[i] = strdup(audioFiles[i]);
    }
    plan->audioFileCount = audioFileCount;
    return plan;
}

// 销毁广播计划
void destroy_broadcast_plan(BroadcastPlan* plan) {
    if (plan) {
        free(plan->name);
        for (size_t i = 0; i < plan->audioFileCount; ++i) {
            free(plan->audioFiles[i]);
        }
        free(plan->audioFiles);
        free(plan);
    }
}

// 将广播计划转换为JSON对象
json_t* broadcast_plan_to_json(const BroadcastPlan* plan) {
    json_t* json = json_object();
    json_object_set_new(json, "id", json_integer(plan->id));
    json_object_set_new(json, "name", json_string(plan->name));
    json_t* audioFilesArray = json_array();
    for (size_t i = 0; i < plan->audioFileCount; ++i) {
        json_array_append_new(audioFilesArray, json_string(plan->audioFiles[i]));
    }
    json_object_set_new(json, "audioFiles", audioFilesArray);
    return json;
}

// 从JSON对象创建广播计划
BroadcastPlan* broadcast_plan_from_json(const json_t* json) {
    int id = json_integer_value(json_object_get(json, "id"));
    const char* name = json_string_value(json_object_get(json, "name"));
    json_t* audioFilesArray = json_object_get(json, "audioFiles");
    size_t audioFileCount = json_array_size(audioFilesArray);
    char** audioFiles = (char**)malloc(audioFileCount * sizeof(char*));
    for (size_t i = 0; i < audioFileCount; ++i) {
        audioFiles[i] = strdup(json_string_value(json_array_get(audioFilesArray, i)));
    }
    return create_broadcast_plan(id, name, audioFiles, audioFileCount);
}

// 创建广播计划管理器
BroadcastPlanManager* create_broadcast_plan_manager(const char* configPath) {
    BroadcastPlanManager* manager = (BroadcastPlanManager*)malloc(sizeof(BroadcastPlanManager));
    if (!manager) return NULL;
    manager->plans = NULL;
    manager->planCount = 0;
    manager->configPath = strdup(configPath);
    return manager;
}

// 销毁广播计划管理器
void destroy_broadcast_plan_manager(BroadcastPlanManager* manager) {
    if (manager) {
        for (size_t i = 0; i < manager->planCount; ++i) {
            destroy_broadcast_plan(&manager->plans[i]);
        }
        free(manager->plans);
        free(manager->configPath);
        free(manager);
    }
}

// 加载广播计划管理器
bool load_broadcast_plan_manager(BroadcastPlanManager* manager) {
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
        BroadcastPlan* plan = broadcast_plan_from_json(value);
        manager->plans = (BroadcastPlan*)realloc(manager->plans, (manager->planCount + 1) * sizeof(BroadcastPlan));
        manager->plans[manager->planCount++] = *plan;
        free(plan);
    }

    json_decref(json);
    return true;
}

// 保存广播计划管理器
bool save_broadcast_plan_manager(const BroadcastPlanManager* manager) {
    json_t* json = json_array();
    for (size_t i = 0; i < manager->planCount; ++i) {
        json_t* planJson = broadcast_plan_to_json(&manager->plans[i]);
        json_array_append_new(json, planJson);
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

// 添加计划到管理器
void add_plan_to_manager(BroadcastPlanManager* manager, BroadcastPlan* plan) {
    manager->plans = (BroadcastPlan*)realloc(manager->plans, (manager->planCount + 1) * sizeof(BroadcastPlan));
    manager->plans[manager->planCount++] = *plan;
}

// 从管理器中移除计划
void remove_plan_from_manager(BroadcastPlanManager* manager, int id) {
    for (size_t i = 0; i < manager->planCount; ++i) {
        if (manager->plans[i].id == id) {
            destroy_broadcast_plan(&manager->plans[i]);
            for (size_t j = i; j < manager->planCount - 1; ++j) {
                manager->plans[j] = manager->plans[j + 1];
            }
            --manager->planCount;
            manager->plans = (BroadcastPlan*)realloc(manager->plans, manager->planCount * sizeof(BroadcastPlan));
            break;
        }
    }
}

// 在管理器中查找计划
BroadcastPlan* find_plan_in_manager(BroadcastPlanManager* manager, int id) {
    for (size_t i = 0; i < manager->planCount; ++i) {
        if (manager->plans[i].id == id) {
            return &manager->plans[i];
        }
    }
    return NULL;
}