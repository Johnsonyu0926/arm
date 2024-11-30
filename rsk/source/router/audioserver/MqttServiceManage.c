#include "mqtt_servicemanage.h"

// 创建MqttServiceManage对象
MqttServiceManage* create_mqtt_service_manage(const char* clientId, const char* host, int port) {
    MqttServiceManage* mqttServiceManage = (MqttServiceManage*)malloc(sizeof(MqttServiceManage));
    if (!mqttServiceManage) return NULL;
    mqttServiceManage->clientId = strdup(clientId);
    mqttServiceManage->host = strdup(host);
    mqttServiceManage->port = port;
    mqttServiceManage->mqttClient = NULL;
    return mqttServiceManage;
}

// 销毁MqttServiceManage对象
void destroy_mqtt_service_manage(MqttServiceManage* mqttServiceManage) {
    if (mqttServiceManage) {
        free(mqttServiceManage->clientId);
        free(mqttServiceManage->host);
        free(mqttServiceManage);
    }
}

// 加载服务管理配置
bool load_service_manage_config(MqttServiceManage* mqttServiceManage, const char* configPath) {
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

    json_t* serviceManageConfigJson = json_object_get(json, "serviceManageConfig");
    mqttServiceManage->serviceManageConfig = *service_manage_from_json(serviceManageConfigJson);

    json_decref(json);
    return true;
}

// 保存服务管理配置
bool save_service_manage_config(const MqttServiceManage* mqttServiceManage, const char* configPath) {
    json_t* json = json_object();
    json_t* serviceManageConfigJson = service_manage_to_json(&mqttServiceManage->serviceManageConfig);
    json_object_set_new(json, "serviceManageConfig", serviceManageConfigJson);

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
void initialize_mqtt_client(MqttServiceManage* mqttServiceManage) {
    mqttServiceManage->mqttClient = create_mqtt_client(mqttServiceManage->clientId, mqttServiceManage->host, mqttServiceManage->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttServiceManage* mqttServiceManage, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttServiceManage->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttServiceManage* mqttServiceManage) {
    return connect_mqtt_client(mqttServiceManage->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttServiceManage* mqttServiceManage) {
    disconnect_mqtt_client(mqttServiceManage->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttServiceManage* mqttServiceManage, const char* topic) {
    return subscribe_mqtt_client(mqttServiceManage->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttServiceManage* mqttServiceManage, const char* topic, const char* message) {
    return publish_mqtt_client(mqttServiceManage->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttServiceManage* mqttServiceManage) {
    mqtt_client_loop_forever(mqttServiceManage->mqttClient);
}