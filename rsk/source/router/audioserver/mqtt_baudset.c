#include "mqtt_baudset.h"

// 创建MqttBaudSet对象
MqttBaudSet* create_mqtt_baud_set(const char* clientId, const char* host, int port) {
    MqttBaudSet* mqttBaudSet = (MqttBaudSet*)malloc(sizeof(MqttBaudSet));
    if (!mqttBaudSet) return NULL;
    mqttBaudSet->clientId = strdup(clientId);
    mqttBaudSet->host = strdup(host);
    mqttBaudSet->port = port;
    mqttBaudSet->baudConfig = NULL;
    mqttBaudSet->baudConfigCount = 0;
    mqttBaudSet->mqttClient = NULL;
    return mqttBaudSet;
}

// 销毁MqttBaudSet对象
void destroy_mqtt_baud_set(MqttBaudSet* mqttBaudSet) {
    if (mqttBaudSet) {
        free(mqttBaudSet->clientId);
        free(mqttBaudSet->host);
        free(mqttBaudSet->baudConfig);
        free(mqttBaudSet);
    }
}

// 加载波特率设置配置
bool load_baud_set_config(MqttBaudSet* mqttBaudSet, const char* configPath) {
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

    size_t index;
    json_t* value;
    json_array_foreach(json, index, value) {
        BaudSet* baudSet = baud_set_from_json(value);
        mqttBaudSet->baudConfig = (BaudSet*)realloc(mqttBaudSet->baudConfig, (mqttBaudSet->baudConfigCount + 1) * sizeof(BaudSet));
        mqttBaudSet->baudConfig[mqttBaudSet->baudConfigCount++] = *baudSet;
        free(baudSet);
    }

    json_decref(json);
    return true;
}

// 保存波特率设置配置
bool save_baud_set_config(const MqttBaudSet* mqttBaudSet, const char* configPath) {
    json_t* json = json_array();
    for (size_t i = 0; i < mqttBaudSet->baudConfigCount; ++i) {
        json_t* baudSetJson = baud_set_to_json(&mqttBaudSet->baudConfig[i]);
        json_array_append_new(json, baudSetJson);
    }

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
void initialize_mqtt_client(MqttBaudSet* mqttBaudSet) {
    mqttBaudSet->mqttClient = create_mqtt_client(mqttBaudSet->clientId, mqttBaudSet->host, mqttBaudSet->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttBaudSet* mqttBaudSet, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttBaudSet->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttBaudSet* mqttBaudSet) {
    return connect_mqtt_client(mqttBaudSet->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttBaudSet* mqttBaudSet) {
    disconnect_mqtt_client(mqttBaudSet->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttBaudSet* mqttBaudSet, const char* topic) {
    return subscribe_mqtt_client(mqttBaudSet->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttBaudSet* mqttBaudSet, const char* topic, const char* message) {
    return publish_mqtt_client(mqttBaudSet->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttBaudSet* mqttBaudSet) {
    mqtt_client_loop_forever(mqttBaudSet->mqttClient);
}

// 处理波特率设置命令
json_t* handle_baud_set_command(const MqttBaudSet* mqttBaudSet, const json_t* command) {
    // 这里应该实现实际的波特率设置逻辑
    // 为了示例，我们假设设置总是成功的
    return json_pack("{s:s, s:i, s:s, s:s, s:s, s:i, s:o}", 
                     "result", "波特率设置成功", 
                     "resultId", 1, 
                     "imei", json_string_value(json_object_get(command, "imei")), 
                     "topic", "TaDiao/device/report/test/", 
                     "cmd", "baudSet", 
                     "publishId", json_integer_value(json_object_get(command, "publishId")), 
                     "data", json_object());
}