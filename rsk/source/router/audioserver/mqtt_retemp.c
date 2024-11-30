#include "mqtt_retemp.h"

// 创建MqttReTemp对象
MqttReTemp* create_mqtt_retemp(const char* clientId, const char* host, int port) {
    MqttReTemp* mqttReTemp = (MqttReTemp*)malloc(sizeof(MqttReTemp));
    if (!mqttReTemp) return NULL;
    mqttReTemp->clientId = strdup(clientId);
    mqttReTemp->host = strdup(host);
    mqttReTemp->port = port;
    mqttReTemp->mqttClient = NULL;
    return mqttReTemp;
}

// 销毁MqttReTemp对象
void destroy_mqtt_retemp(MqttReTemp* mqttReTemp) {
    if (mqttReTemp) {
        free(mqttReTemp->clientId);
        free(mqttReTemp->host);
        free(mqttReTemp);
    }
}

// 加载ReTemp配置
bool load_retemp_config(MqttReTemp* mqttReTemp, const char* configPath) {
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

    json_t* reTempConfigJson = json_object_get(json, "reTempConfig");
    mqttReTemp->reTempConfig = *retemp_from_json(reTempConfigJson);

    json_decref(json);
    return true;
}

// 保存ReTemp配置
bool save_retemp_config(const MqttReTemp* mqttReTemp, const char* configPath) {
    json_t* json = json_object();
    json_t* reTempConfigJson = retemp_to_json(&mqttReTemp->reTempConfig);
    json_object_set_new(json, "reTempConfig", reTempConfigJson);

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
void initialize_mqtt_client(MqttReTemp* mqttReTemp) {
    mqttReTemp->mqttClient = create_mqtt_client(mqttReTemp->clientId, mqttReTemp->host, mqttReTemp->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttReTemp* mqttReTemp, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttReTemp->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttReTemp* mqttReTemp) {
    return connect_mqtt_client(mqttReTemp->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttReTemp* mqttReTemp) {
    disconnect_mqtt_client(mqttReTemp->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttReTemp* mqttReTemp, const char* topic) {
    return subscribe_mqtt_client(mqttReTemp->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttReTemp* mqttReTemp, const char* topic, const char* message) {
    return publish_mqtt_client(mqttReTemp->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttReTemp* mqttReTemp) {
    mqtt_client_loop_forever(mqttReTemp->mqttClient);
}

// 处理音频播放命令
json_t* handle_audio_play_command(const MqttReTemp* mqttReTemp, const json_t* command) {
    // 这里应该实现实际的音频播放逻辑
    // 为了示例，我们假设播放总是成功的
    return json_pack("{s:s, s:i, s:s, s:s, s:s, s:i, s:o}", 
                     "result", "播放成功", 
                     "resultId", 1, 
                     "imei", json_string_value(json_object_get(command, "imei")), 
                     "topic", "TaDiao/device/report/test/", 
                     "cmd", "audioPlay", 
                     "publishId", json_integer_value(json_object_get(command, "publishId")), 
                     "data", json_object());
}