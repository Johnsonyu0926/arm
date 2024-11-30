#include "mqtt_reboot.h"

// 创建MqttReboot对象
MqttReboot* create_mqtt_reboot(const char* clientId, const char* host, int port) {
    MqttReboot* mqttReboot = (MqttReboot*)malloc(sizeof(MqttReboot));
    if (!mqttReboot) return NULL;
    mqttReboot->clientId = strdup(clientId);
    mqttReboot->host = strdup(host);
    mqttReboot->port = port;
    mqttReboot->mqttClient = NULL;
    return mqttReboot;
}

// 销毁MqttReboot对象
void destroy_mqtt_reboot(MqttReboot* mqttReboot) {
    if (mqttReboot) {
        free(mqttReboot->clientId);
        free(mqttReboot->host);
        free(mqttReboot);
    }
}

// 加载重启配置
bool load_reboot_config(MqttReboot* mqttReboot, const char* configPath) {
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

    json_t* rebootConfigJson = json_object_get(json, "rebootConfig");
    mqttReboot->rebootConfig = *reboot_from_json(rebootConfigJson);

    json_decref(json);
    return true;
}

// 保存重启配置
bool save_reboot_config(const MqttReboot* mqttReboot, const char* configPath) {
    json_t* json = json_object();
    json_t* rebootConfigJson = reboot_to_json(&mqttReboot->rebootConfig);
    json_object_set_new(json, "rebootConfig", rebootConfigJson);

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
void initialize_mqtt_client(MqttReboot* mqttReboot) {
    mqttReboot->mqttClient = create_mqtt_client(mqttReboot->clientId, mqttReboot->host, mqttReboot->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttReboot* mqttReboot, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttReboot->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttReboot* mqttReboot) {
    return connect_mqtt_client(mqttReboot->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttReboot* mqttReboot) {
    disconnect_mqtt_client(mqttReboot->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttReboot* mqttReboot, const char* topic) {
    return subscribe_mqtt_client(mqttReboot->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttReboot* mqttReboot, const char* topic, const char* message) {
    return publish_mqtt_client(mqttReboot->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttReboot* mqttReboot) {
    mqtt_client_loop_forever(mqttReboot->mqttClient);
}

// 处理重启命令
json_t* handle_reboot_command(const MqttReboot* mqttReboot, const json_t* command) {
    // 这里应该实现实际的重启逻辑
    // 为了示例，我们假设重启总是成功的
    return json_pack("{s:s, s:i, s:s, s:s, s:s, s:i, s:o}", 
                     "result", "重启成功", 
                     "resultId", 1, 
                     "imei", json_string_value(json_object_get(command, "imei")), 
                     "topic", "TaDiao/device/report/test/", 
                     "cmd", "reboot", 
                     "publishId", json_integer_value(json_object_get(command, "publishId")), 
                     "data", json_object());
}