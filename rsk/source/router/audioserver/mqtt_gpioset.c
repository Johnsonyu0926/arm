#include "mqtt_gpioset.h"

// 创建MqttGpioSet对象
MqttGpioSet* create_mqtt_gpio_set(const char* clientId, const char* host, int port) {
    MqttGpioSet* mqttGpioSet = (MqttGpioSet*)malloc(sizeof(MqttGpioSet));
    if (!mqttGpioSet) return NULL;
    mqttGpioSet->clientId = strdup(clientId);
    mqttGpioSet->host = strdup(host);
    mqttGpioSet->port = port;
    mqttGpioSet->mqttClient = NULL;
    return mqttGpioSet;
}

// 销毁MqttGpioSet对象
void destroy_mqtt_gpio_set(MqttGpioSet* mqttGpioSet) {
    if (mqttGpioSet) {
        free(mqttGpioSet->clientId);
        free(mqttGpioSet->host);
        free(mqttGpioSet);
    }
}

// 加载GPIO设置配置
bool load_gpio_set_config(MqttGpioSet* mqttGpioSet, const char* configPath) {
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

    json_t* gpioConfigJson = json_object_get(json, "gpioConfig");
    mqttGpioSet->gpioConfig = *gpio_set_from_json(gpioConfigJson);

    json_decref(json);
    return true;
}

// 保存GPIO设置配置
bool save_gpio_set_config(const MqttGpioSet* mqttGpioSet, const char* configPath) {
    json_t* json = json_object();
    json_t* gpioConfigJson = gpio_set_to_json(&mqttGpioSet->gpioConfig);
    json_object_set_new(json, "gpioConfig", gpioConfigJson);

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
void initialize_mqtt_client(MqttGpioSet* mqttGpioSet) {
    mqttGpioSet->mqttClient = create_mqtt_client(mqttGpioSet->clientId, mqttGpioSet->host, mqttGpioSet->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttGpioSet* mqttGpioSet, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttGpioSet->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttGpioSet* mqttGpioSet) {
    return connect_mqtt_client(mqttGpioSet->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttGpioSet* mqttGpioSet) {
    disconnect_mqtt_client(mqttGpioSet->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttGpioSet* mqttGpioSet, const char* topic) {
    return subscribe_mqtt_client(mqttGpioSet->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttGpioSet* mqttGpioSet, const char* topic, const char* message) {
    return publish_mqtt_client(mqttGpioSet->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttGpioSet* mqttGpioSet) {
    mqtt_client_loop_forever(mqttGpioSet->mqttClient);
}

// 处理GPIO设置命令
json_t* handle_gpio_set_command(const MqttGpioSet* mqttGpioSet, const json_t* command) {
    // 这里应该实现实际的GPIO设置逻辑
    // 为了示例，我们假设设置总是成功的
    return json_pack("{s:s, s:i, s:s, s:s, s:s, s:i, s:o}", 
                     "result", "GPIO设置成功", 
                     "resultId", 1, 
                     "imei", json_string_value(json_object_get(command, "imei")), 
                     "topic", "TaDiao/device/report/test/", 
                     "cmd", "gpioSet", 
                     "publishId", json_integer_value(json_object_get(command, "publishId")), 
                     "data", json_object());
}