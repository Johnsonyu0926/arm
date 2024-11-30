#include "mqtt_ledshowset.h"

// 创建MqttLedShowSet对象
MqttLedShowSet* create_mqtt_led_show_set(const char* clientId, const char* host, int port) {
    MqttLedShowSet* mqttLedShowSet = (MqttLedShowSet*)malloc(sizeof(MqttLedShowSet));
    if (!mqttLedShowSet) return NULL;
    mqttLedShowSet->clientId = strdup(clientId);
    mqttLedShowSet->host = strdup(host);
    mqttLedShowSet->port = port;
    mqttLedShowSet->mqttClient = NULL;
    return mqttLedShowSet;
}

// 销毁MqttLedShowSet对象
void destroy_mqtt_led_show_set(MqttLedShowSet* mqttLedShowSet) {
    if (mqttLedShowSet) {
        free(mqttLedShowSet->clientId);
        free(mqttLedShowSet->host);
        free(mqttLedShowSet);
    }
}

// 加载LED显示设置配置
bool load_led_show_set_config(MqttLedShowSet* mqttLedShowSet, const char* configPath) {
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

    json_t* ledShowConfigJson = json_object_get(json, "ledShowConfig");
    mqttLedShowSet->ledShowConfig = *led_show_set_from_json(ledShowConfigJson);

    json_decref(json);
    return true;
}

// 保存LED显示设置配置
bool save_led_show_set_config(const MqttLedShowSet* mqttLedShowSet, const char* configPath) {
    json_t* json = json_object();
    json_t* ledShowConfigJson = led_show_set_to_json(&mqttLedShowSet->ledShowConfig);
    json_object_set_new(json, "ledShowConfig", ledShowConfigJson);

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
void initialize_mqtt_client(MqttLedShowSet* mqttLedShowSet) {
    mqttLedShowSet->mqttClient = create_mqtt_client(mqttLedShowSet->clientId, mqttLedShowSet->host, mqttLedShowSet->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttLedShowSet* mqttLedShowSet, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttLedShowSet->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttLedShowSet* mqttLedShowSet) {
    return connect_mqtt_client(mqttLedShowSet->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttLedShowSet* mqttLedShowSet) {
    disconnect_mqtt_client(mqttLedShowSet->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttLedShowSet* mqttLedShowSet, const char* topic) {
    return subscribe_mqtt_client(mqttLedShowSet->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttLedShowSet* mqttLedShowSet, const char* topic, const char* message) {
    return publish_mqtt_client(mqttLedShowSet->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttLedShowSet* mqttLedShowSet) {
    mqtt_client_loop_forever(mqttLedShowSet->mqttClient);
}

// 处理LED显示设置命令
json_t* handle_led_show_set_command(const MqttLedShowSet* mqttLedShowSet, const json_t* command) {
    // 这里应该实现实际的LED显示设置逻辑
    // 为了示例，我们假设设置总是成功的
    return json_pack("{s:s, s:i, s:s, s:s, s:s, s:i, s:o}", 
                     "result", "LED 显示设置成功", 
                     "resultId", 1, 
                     "imei", json_string_value(json_object_get(command, "imei")), 
                     "topic", "TaDiao/device/report/test/", 
                     "cmd", "ledShowSet", 
                     "publishId", json_integer_value(json_object_get(command, "publishId")), 
                     "data", json_object());
}