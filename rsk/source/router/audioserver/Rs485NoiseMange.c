#include "rs485_noise_manage.h"

static const char* NOISECONFIG = "/mnt/cfg/noise.json";

// 创建Rs485NoiseManage对象
Rs485NoiseManage* create_rs485_noise_manage(void) {
    Rs485NoiseManage* manage = (Rs485NoiseManage*)malloc(sizeof(Rs485NoiseManage));
    if (!manage) return NULL;
    manage->noise_topic = strdup("IOT/intranet/client/noise/");
    return manage;
}

// 销毁Rs485NoiseManage对象
void destroy_rs485_noise_manage(Rs485NoiseManage* manage) {
    if (manage) {
        free(manage->noise_topic);
        free(manage);
    }
}

// 获取噪声主题
const char* get_noise_topic(const Rs485NoiseManage* manage) {
    return manage->noise_topic;
}

// 设置噪声主题
void set_noise_topic(Rs485NoiseManage* manage, const char* new_noise_topic) {
    free(manage->noise_topic);
    manage->noise_topic = strdup(new_noise_topic);
}

// 加载配置文件
bool load_noise_config_file(Rs485NoiseManage* manage) {
    FILE* file = fopen(NOISECONFIG, "r");
    if (!file) {
        fprintf(stderr, "Config file does not exist: %s\n", NOISECONFIG);
        return false;
    }

    json_error_t error;
    json_t* json = json_loadf(file, 0, &error);
    fclose(file);
    if (!json) {
        fprintf(stderr, "Error loading config file: %s\n", error.text);
        return false;
    }

    json_t* noise_topic_json = json_object_get(json, "noise_topic");

    set_noise_topic(manage, json_string_value(noise_topic_json));

    json_decref(json);
    return true;
}

// 更新配置文件
bool update_noise_config_file(const Rs485NoiseManage* manage) {
    json_t* json = json_object();
    json_object_set_new(json, "noise_topic", json_string(manage->noise_topic));

    FILE* file = fopen(NOISECONFIG, "w");
    if (!file) {
        fprintf(stderr, "Failed to open config file for writing: %s\n", NOISECONFIG);
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