#include "mqtt_recordplay.h"

// 创建MqttRecordPlay对象
MqttRecordPlay* create_mqtt_record_play(const char* clientId, const char* host, int port) {
    MqttRecordPlay* mqttRecordPlay = (MqttRecordPlay*)malloc(sizeof(MqttRecordPlay));
    if (!mqttRecordPlay) return NULL;
    mqttRecordPlay->clientId = strdup(clientId);
    mqttRecordPlay->host = strdup(host);
    mqttRecordPlay->port = port;
    mqttRecordPlay->mqttClient = NULL;
    return mqttRecordPlay;
}

// 销毁MqttRecordPlay对象
void destroy_mqtt_record_play(MqttRecordPlay* mqttRecordPlay) {
    if (mqttRecordPlay) {
        free(mqttRecordPlay->clientId);
        free(mqttRecordPlay->host);
        free(mqttRecordPlay);
    }
}

// 加载录音播放配置
bool load_record_play_config(MqttRecordPlay* mqttRecordPlay, const char* configPath) {
    FILE* file = fopen(configPath, "r");
    if (!file) {
        fprintf(stderr, "Config file does not exist: %s\n", configPath);
        return false;
    }

    json_error_t error;
    json_t* json = json_loadf(file, 0, &error);
    fclose(file);
    if (!json) {
        fprintf(stderr, "Error loading config file: %s\n", error.text);
        return false;
    }

    json_t* recordPlayConfigJson = json_object_get(json, "recordPlayConfig");
    mqttRecordPlay->recordPlayConfig = *record_play_from_json(recordPlayConfigJson);

    json_decref(json);
    return true;
}

// 保存录音播放配置
bool save_record_play_config(const MqttRecordPlay* mqttRecordPlay, const char* configPath) {
    json_t* json = json_object();
    json_t* recordPlayConfigJson = record_play_to_json(&mqttRecordPlay->recordPlayConfig);
    json_object_set_new(json, "recordPlayConfig", recordPlayConfigJson);

    FILE* file = fopen(configPath, "w");
    if (!file) {
        fprintf(stderr, "Failed to open config file for writing: %s\n", configPath);
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

// 初始化MQTT客户端
void initialize_mqtt_client(MqttRecordPlay* mqttRecordPlay) {
    mqttRecordPlay->mqttClient = create_mqtt_client(mqttRecordPlay->clientId, mqttRecordPlay->host, mqttRecordPlay->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttRecordPlay* mqttRecordPlay, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttRecordPlay->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttRecordPlay* mqttRecordPlay) {
    return connect_mqtt_client(mqttRecordPlay->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttRecordPlay* mqttRecordPlay) {
    disconnect_mqtt_client(mqttRecordPlay->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttRecordPlay* mqttRecordPlay, const char* topic) {
    return subscribe_mqtt_client(mqttRecordPlay->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttRecordPlay* mqttRecordPlay, const char* topic, const char* message) {
    return publish_mqtt_client(mqttRecordPlay->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttRecordPlay* mqttRecordPlay) {
    mqtt_client_loop_forever(mqttRecordPlay->mqttClient);
}