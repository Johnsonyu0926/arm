#include "mqttConfig.h"

static const char* MQTTCONFIG = "/mnt/cfg/mqtt.json";

// 创建MqttConfig对象
MqttConfig* create_mqtt_config(void) {
    MqttConfig* config = (MqttConfig*)malloc(sizeof(MqttConfig));
    if (!config) return NULL;
    config->request_topic = strdup("IOT/intranet/client/request/");
    config->publish_topic = strdup("IOT/intranet/server/report/");
    return config;
}

// 销毁MqttConfig对象
void destroy_mqtt_config(MqttConfig* config) {
    if (config) {
        free(config->request_topic);
        free(config->publish_topic);
        free(config);
    }
}

// 获取请求主题
const char* get_request_topic(const MqttConfig* config) {
    return config->request_topic;
}

// 设置请求主题
void set_request_topic(MqttConfig* config, const char* new_request_topic) {
    free(config->request_topic);
    config->request_topic = strdup(new_request_topic);
}

// 获取发布主题
const char* get_publish_topic(const MqttConfig* config) {
    return config->publish_topic;
}

// 设置发布主题
void set_publish_topic(MqttConfig* config, const char* new_publish_topic) {
    free(config->publish_topic);
    config->publish_topic = strdup(new_publish_topic);
}

// 加载配置文件
bool load_mqtt_config_file(MqttConfig* config) {
    FILE* file = fopen(MQTTCONFIG, "r");
    if (!file) {
        fprintf(stderr, "Config file does not exist: %s\n", MQTTCONFIG);
        return false;
    }

    json_error_t error;
    json_t* json = json_loadf(file, 0, &error);
    fclose(file);
    if (!json) {
        fprintf(stderr, "Error loading config file: %s\n", error.text);
        return false;
    }

    json_t* request_topic_json = json_object_get(json, "request_topic");
    json_t* publish_topic_json = json_object_get(json, "publish_topic");

    set_request_topic(config, json_string_value(request_topic_json));
    set_publish_topic(config, json_string_value(publish_topic_json));

    json_decref(json);
    return true;
}

// 更新配置文件
bool update_mqtt_config_file(const MqttConfig* config) {
    json_t* json = json_object();
    json_object_set_new(json, "request_topic", json_string(config->request_topic));
    json_object_set_new(json, "publish_topic", json_string(config->publish_topic));

    FILE* file = fopen(MQTTCONFIG, "w");
    if (!file) {
        fprintf(stderr, "Failed to open config file for writing: %s\n", MQTTCONFIG);
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