#include "mqttManage.h"

// 创建MqttManage对象
MqttManage* create_mqtt_manage() {
    MqttManage* mqttManage = (MqttManage*)malloc(sizeof(MqttManage));
    if (!mqttManage) return NULL;
    mqttManage->clientId = NULL;
    mqttManage->host = NULL;
    mqttManage->port = 0;
    mqttManage->mqttClient = NULL;
    return mqttManage;
}

// 销毁MqttManage对象
void destroy_mqtt_manage(MqttManage* mqttManage) {
    if (mqttManage) {
        free(mqttManage->clientId);
        free(mqttManage->host);
        free(mqttManage);
    }
}

// 加载MqttManage配置
bool load_mqtt_manage_config(MqttManage* mqttManage, const char* configPath) {
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

    json_t* clientIdJson = json_object_get(json, "clientId");
    json_t* hostJson = json_object_get(json, "host");
    json_t* portJson = json_object_get(json, "port");

    mqttManage->clientId = strdup(json_string_value(clientIdJson));
    mqttManage->host = strdup(json_string_value(hostJson));
    mqttManage->port = json_integer_value(portJson);

    json_decref(json);
    return true;
}

// 保存MqttManage配置
bool save_mqtt_manage_config(const MqttManage* mqttManage, const char* configPath) {
    json_t* json = json_object();
    json_object_set_new(json, "clientId", json_string(mqttManage->clientId));
    json_object_set_new(json, "host", json_string(mqttManage->host));
    json_object_set_new(json, "port", json_integer(mqttManage->port));

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
void initialize_mqtt_client(MqttManage* mqttManage) {
    mqttManage->mqttClient = create_mqtt_client(mqttManage->clientId, mqttManage->host, mqttManage->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttManage* mqttManage, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttManage->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttManage* mqttManage) {
    return connect_mqtt_client(mqttManage->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttManage* mqttManage) {
    disconnect_mqtt_client(mqttManage->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttManage* mqttManage, const char* topic) {
    return subscribe_mqtt_client(mqttManage->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttManage* mqttManage, const char* topic, const char* message) {
    return publish_mqtt_client(mqttManage->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttManage* mqttManage) {
    mqtt_client_loop_forever(mqttManage->mqttClient);
}