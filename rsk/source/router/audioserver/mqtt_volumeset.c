#include "mqtt_volumeset.h"

// 创建MqttVolumeSet对象
MqttVolumeSet* create_mqtt_volume_set(const char* clientId, const char* host, int port) {
    MqttVolumeSet* mqttVolumeSet = (MqttVolumeSet*)malloc(sizeof(MqttVolumeSet));
    if (!mqttVolumeSet) return NULL;
    mqttVolumeSet->clientId = strdup(clientId);
    mqttVolumeSet->host = strdup(host);
    mqttVolumeSet->port = port;
    mqttVolumeSet->mqttClient = NULL;
    return mqttVolumeSet;
}

// 销毁MqttVolumeSet对象
void destroy_mqtt_volume_set(MqttVolumeSet* mqttVolumeSet) {
    if (mqttVolumeSet) {
        free(mqttVolumeSet->clientId);
        free(mqttVolumeSet->host);
        free(mqttVolumeSet);
    }
}

// 加载音量设置配置
bool load_volume_set_config(MqttVolumeSet* mqttVolumeSet, const char* configPath) {
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

    json_t* volumeConfigJson = json_object_get(json, "volumeConfig");
    mqttVolumeSet->volumeConfig = *volume_set_from_json(volumeConfigJson);

    json_decref(json);
    return true;
}

// 保存音量设置配置
bool save_volume_set_config(const MqttVolumeSet* mqttVolumeSet, const char* configPath) {
    json_t* json = json_object();
    json_t* volumeConfigJson = volume_set_to_json(&mqttVolumeSet->volumeConfig);
    json_object_set_new(json, "volumeConfig", volumeConfigJson);

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
void initialize_mqtt_client(MqttVolumeSet* mqttVolumeSet) {
    mqttVolumeSet->mqttClient = create_mqtt_client(mqttVolumeSet->clientId, mqttVolumeSet->host, mqttVolumeSet->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttVolumeSet* mqttVolumeSet, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttVolumeSet->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttVolumeSet* mqttVolumeSet) {
    return connect_mqtt_client(mqttVolumeSet->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttVolumeSet* mqttVolumeSet) {
    disconnect_mqtt_client(mqttVolumeSet->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttVolumeSet* mqttVolumeSet, const char* topic) {
    return subscribe_mqtt_client(mqttVolumeSet->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttVolumeSet* mqttVolumeSet, const char* topic, const char* message) {
    return publish_mqtt_client(mqttVolumeSet->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttVolumeSet* mqttVolumeSet) {
    mqtt_client_loop_forever(mqttVolumeSet->mqttClient);
}