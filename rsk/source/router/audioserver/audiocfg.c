#include "audiocfg.h"

static const char* CONFIG_FILE = "/mnt/cfg/audio.json";

// 初始化音频配置业务
void init_audio_cfg_business(CAudioCfgBusiness* cfg) {
    cfg->config_json = NULL;
}

// 释放音频配置业务
void free_audio_cfg_business(CAudioCfgBusiness* cfg) {
    if (cfg->config_json) {
        json_decref(cfg->config_json);
    }
}

// 加载音频配置业务
bool load_audio_cfg_business(CAudioCfgBusiness* cfg) {
    FILE* file = fopen(CONFIG_FILE, "r");
    if (!file) {
        fprintf(stderr, "Config file does not exist: %s\n", CONFIG_FILE);
        return false;
    }

    json_error_t error;
    cfg->config_json = json_loadf(file, 0, &error);
    fclose(file);
    if (!cfg->config_json) {
        fprintf(stderr, "JSON parse error: %s\n", error.text);
        return false;
    }

    return true;
}

// 获取音频文件路径
const char* get_audio_file_path(const CAudioCfgBusiness* cfg) {
    const char* audio_file_path = json_string_value(json_object_get(cfg->config_json, "audioFilePath"));
    return audio_file_path ? audio_file_path : "";
}