#include "hkTts.h"

// 创建TTS配置
TtsConfig* create_tts_config(const char* apiKey, const char* url, const char* voice) {
    TtsConfig* config = (TtsConfig*)malloc(sizeof(TtsConfig));
    if (!config) return NULL;
    config->apiKey = strdup(apiKey);
    config->url = strdup(url);
    config->voice = strdup(voice);
    return config;
}

// 销毁TTS配置
void destroy_tts_config(TtsConfig* config) {
    if (config) {
        free(config->apiKey);
        free(config->url);
        free(config->voice);
        free(config);
    }
}

// 获取API密钥
const char* get_api_key(const TtsConfig* config) {
    return config->apiKey;
}

// 设置API密钥
void set_api_key(TtsConfig* config, const char* newApiKey) {
    free(config->apiKey);
    config->apiKey = strdup(newApiKey);
}

// 获取URL
const char* get_url(const TtsConfig* config) {
    return config->url;
}

// 设置URL
void set_url(TtsConfig* config, const char* newUrl) {
    free(config->url);
    config->url = strdup(newUrl);
}

// 获取语音
const char* get_voice(const TtsConfig* config) {
    return config->voice;
}

// 设置语音
void set_voice(TtsConfig* config, const char* newVoice) {
    free(config->voice);
    config->voice = strdup(newVoice);
}

// 将TTS配置转换为JSON对象
json_t* tts_config_to_json(const TtsConfig* config) {
    json_t* json = json_object();
    json_object_set_new(json, "apiKey", json_string(config->apiKey));
    json_object_set_new(json, "url", json_string(config->url));
    json_object_set_new(json, "voice", json_string(config->voice));
    return json;
}

// 从JSON对象创建TTS配置
TtsConfig* tts_config_from_json(const json_t* json) {
    const char* apiKey = json_string_value(json_object_get(json, "apiKey"));
    const char* url = json_string_value(json_object_get(json, "url"));
    const char* voice = json_string_value(json_object_get(json, "voice"));
    return create_tts_config(apiKey, url, voice);
}

// 创建TTS配置管理器
TtsConfigManager* create_tts_config_manager(const char* configPath) {
    TtsConfigManager* manager = (TtsConfigManager*)malloc(sizeof(TtsConfigManager));
    if (!manager) return NULL;
    manager->configPath = strdup(configPath);
    manager->config.apiKey = NULL;
    manager->config.url = NULL;
    manager->config.voice = NULL;
    return manager;
}

// 销毁TTS配置管理器
void destroy_tts_config_manager(TtsConfigManager* manager) {
    if (manager) {
        destroy_tts_config(&manager->config);
        free(manager->configPath);
        free(manager);
    }
}

// 加载TTS配置管理器
bool load_tts_config_manager(TtsConfigManager* manager) {
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

    TtsConfig* config = tts_config_from_json(json);
    manager->config = *config;
    free(config);

    json_decref(json);
    return true;
}

// 保存TTS配置管理器
bool save_tts_config_manager(const TtsConfigManager* manager) {
    json_t* json = tts_config_to_json(&manager->config);

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

// 获取TTS配置
const TtsConfig* get_tts_config(const TtsConfigManager* manager) {
    return &manager->config;
}

// 设置TTS配置
void set_tts_config(TtsConfigManager* manager, TtsConfig* newConfig) {
    destroy_tts_config(&manager->config);
    manager->config = *newConfig;
    free(newConfig);
}