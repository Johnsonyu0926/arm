#include "mqtt_operationcmd.h"

// 创建MqttPlanPlay对象
MqttPlanPlay* create_mqtt_plan_play(const char* clientId, const char* host, int port) {
    MqttPlanPlay* mqttPlanPlay = (MqttPlanPlay*)malloc(sizeof(MqttPlanPlay));
    if (!mqttPlanPlay) return NULL;
    mqttPlanPlay->clientId = strdup(clientId);
    mqttPlanPlay->host = strdup(host);
    mqttPlanPlay->port = port;
    mqttPlanPlay->mqttClient = NULL;
    return mqttPlanPlay;
}

// 销毁MqttPlanPlay对象
void destroy_mqtt_plan_play(MqttPlanPlay* mqttPlanPlay) {
    if (mqttPlanPlay) {
        free(mqttPlanPlay->clientId);
        free(mqttPlanPlay->host);
        free(mqttPlanPlay);
    }
}

// 加载计划播放配置
bool load_plan_play_config(MqttPlanPlay* mqttPlanPlay, const char* configPath) {
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

    json_t* planPlayConfigJson = json_object_get(json, "planPlayConfig");
    mqttPlanPlay->planPlayConfig = *plan_play_from_json(planPlayConfigJson);

    json_decref(json);
    return true;
}

// 保存计划播放配置
bool save_plan_play_config(const MqttPlanPlay* mqttPlanPlay, const char* configPath) {
    json_t* json = json_object();
    json_t* planPlayConfigJson = plan_play_to_json(&mqttPlanPlay->planPlayConfig);
    json_object_set_new(json, "planPlayConfig", planPlayConfigJson);

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
void initialize_mqtt_client(MqttPlanPlay* mqttPlanPlay) {
    mqttPlanPlay->mqttClient = create_mqtt_client(mqttPlanPlay->clientId, mqttPlanPlay->host, mqttPlanPlay->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttPlanPlay* mqttPlanPlay, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttPlanPlay->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttPlanPlay* mqttPlanPlay) {
    return connect_mqtt_client(mqttPlanPlay->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttPlanPlay* mqttPlanPlay) {
    disconnect_mqtt_client(mqttPlanPlay->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttPlanPlay* mqttPlanPlay, const char* topic) {
    return subscribe_mqtt_client(mqttPlanPlay->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttPlanPlay* mqttPlanPlay, const char* topic, const char* message) {
    return publish_mqtt_client(mqttPlanPlay->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttPlanPlay* mqttPlanPlay) {
    mqtt_client_loop_forever(mqttPlanPlay->mqttClient);
}

// 处理计划播放命令
json_t* handle_plan_play_command(const MqttPlanPlay* mqttPlanPlay, const json_t* command) {
    // 这里应该实现实际的计划播放逻辑
    // 为了示例，我们假设计划播放设置总是成功的
    return json_pack("{s:s, s:i, s:s, s:s, s:s, s:i, s:o}", 
                     "result", "计划播放设置成功", 
                     "resultId", 1, 
                     "imei", json_string_value(json_object_get(command, "imei")), 
                     "topic", "TaDiao/device/report/test/", 
                     "cmd", "planPlay", 
                     "publishId", json_integer_value(json_object_get(command, "publishId")), 
                     "data", json_object());
}