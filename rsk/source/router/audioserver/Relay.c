#include "relay.h"

static const char* RELAYCONFIG = "/mnt/cfg/relay.json";

// 创建RelayConfig对象
RelayConfig* create_relay_config(void) {
    RelayConfig* config = (RelayConfig*)malloc(sizeof(RelayConfig));
    if (!config) return NULL;
    config->relay_topic = strdup("IOT/intranet/client/relay/");
    return config;
}

// 销毁RelayConfig对象
void destroy_relay_config(RelayConfig* config) {
    if (config) {
        free(config->relay_topic);
        free(config);
    }
}

// 获取继电器主题
const char* get_relay_topic(const RelayConfig* config) {
    return config->relay_topic;
}

// 设置继电器主题
void set_relay_topic(RelayConfig* config, const char* new_relay_topic) {
    free(config->relay_topic);
    config->relay_topic = strdup(new_relay_topic);
}

// 加载配置文件
bool load_relay_config_file(RelayConfig* config) {
    FILE* file = fopen(RELAYCONFIG, "r");
    if (!file) {
        fprintf(stderr, "Config file does not exist: %s\n", RELAYCONFIG);
        return false;
    }

    json_error_t error;
    json_t* json = json_loadf(file, 0, &error);
    fclose(file);
    if (!json) {
        fprintf(stderr, "Error loading config file: %s\n", error.text);
        return false;
    }

    json_t* relay_topic_json = json_object_get(json, "relay_topic");

    set_relay_topic(config, json_string_value(relay_topic_json));

    json_decref(json);
    return true;
}

// 更新配置文件
bool update_relay_config_file(const RelayConfig* config) {
    json_t* json = json_object();
    json_object_set_new(json, "relay_topic", json_string(config->relay_topic));

    FILE* file = fopen(RELAYCONFIG, "w");
    if (!file) {
        fprintf(stderr, "Failed to open config file for writing: %s\n", RELAYCONFIG);
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