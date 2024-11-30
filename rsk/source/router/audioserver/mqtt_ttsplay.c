#include "mqtt_ttsplay.h"

// 创建MqttTtsPlay对象
MqttTtsPlay* create_mqtt_tts_play(const char* clientId, const char* host, int port) {
    MqttTtsPlay* mqttTtsPlay = (MqttTtsPlay*)malloc(sizeof(MqttTtsPlay));
    if (!mqttTtsPlay) return NULL;
    mqttTtsPlay->clientId = strdup(clientId);
    mqttTtsPlay->host = strdup(host);
    mqttTtsPlay->port = port;
    mqttTtsPlay->mqttClient = NULL;
    return mqttTtsPlay;
}

// 销毁MqttTtsPlay对象
void destroy_mqtt_tts_play(MqttTtsPlay* mqttTtsPlay) {
    if (mqttTtsPlay) {
        free(mqttTtsPlay->clientId);
        free(mqttTtsPlay->host);
        free(mqttTtsPlay);
    }
}

// 加载TTS播放配置
bool load_tts_play_config(MqttTtsPlay* mqttTtsPlay, const char* configPath) {
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

    json_t* ttsConfigJson = json_object_get(json, "ttsConfig");
    mqttTtsPlay->ttsConfig = *tts_play_from_json(ttsConfigJson);

    json_decref(json);
    return true;
}

// 保存TTS播放配置
bool save_tts_play_config(const MqttTtsPlay* mqttTtsPlay, const char* configPath) {
    json_t* json = json_object();
    json_t* ttsConfigJson = tts_play_to_json(&mqttTtsPlay->ttsConfig);
    json_object_set_new(json, "ttsConfig", ttsConfigJson);

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
void initialize_mqtt_client(MqttTtsPlay* mqttTtsPlay) {
    mqttTtsPlay->mqttClient = create_mqtt_client(mqttTtsPlay->clientId, mqttTtsPlay->host, mqttTtsPlay->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttTtsPlay* mqttTtsPlay, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttTtsPlay->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttTtsPlay* mqttTtsPlay) {
    return connect_mqtt_client(mqttTtsPlay->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttTtsPlay* mqttTtsPlay) {
    disconnect_mqtt_client(mqttTtsPlay->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttTtsPlay* mqttTtsPlay, const char* topic) {
    return subscribe_mqtt_client(mqttTtsPlay->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttTtsPlay* mqttTtsPlay, const char* topic, const char* message) {
    return publish_mqtt_client(mqttTtsPlay->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttTtsPlay* mqttTtsPlay) {
    mqtt_client_loop_forever(mqttTtsPlay->mqttClient);
}

// 播放TTS
bool play_tts(MqttTtsPlay* mqttTtsPlay, const char* text) {
    return tts_play(mqttTtsPlay->ttsConfig, text);
}