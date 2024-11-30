#include "mqtt_heartbeat.h"
#include <time.h>

// 创建MqttHeartbeat对象
MqttHeartbeat* create_mqtt_heartbeat(const char* clientId, const char* host, int port) {
    MqttHeartbeat* mqttHeartbeat = (MqttHeartbeat*)malloc(sizeof(MqttHeartbeat));
    if (!mqttHeartbeat) return NULL;
    mqttHeartbeat->clientId = strdup(clientId);
    mqttHeartbeat->host = strdup(host);
    mqttHeartbeat->port = port;
    mqttHeartbeat->mqttClient = NULL;
    return mqttHeartbeat;
}

// 销毁MqttHeartbeat对象
void destroy_mqtt_heartbeat(MqttHeartbeat* mqttHeartbeat) {
    if (mqttHeartbeat) {
        free(mqttHeartbeat->clientId);
        free(mqttHeartbeat->host);
        free(mqttHeartbeat);
    }
}

// 加载心跳配置
bool load_heartbeat_config(MqttHeartbeat* mqttHeartbeat, const char* configPath) {
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

    json_t* heartbeatConfigJson = json_object_get(json, "heartbeatConfig");
    mqttHeartbeat->heartbeatConfig = *heartbeat_from_json(heartbeatConfigJson);

    json_decref(json);
    return true;
}

// 保存心跳配置
bool save_heartbeat_config(const MqttHeartbeat* mqttHeartbeat, const char* configPath) {
    json_t* json = json_object();
    json_t* heartbeatConfigJson = heartbeat_to_json(&mqttHeartbeat->heartbeatConfig);
    json_object_set_new(json, "heartbeatConfig", heartbeatConfigJson);

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
void initialize_mqtt_client(MqttHeartbeat* mqttHeartbeat) {
    mqttHeartbeat->mqttClient = create_mqtt_client(mqttHeartbeat->clientId, mqttHeartbeat->host, mqttHeartbeat->port);
}

// 设置MQTT回调
void set_mqtt_callback(MqttHeartbeat* mqttHeartbeat, void (*callback)(const char* topic, const char* message)) {
    set_mqtt_client_callback(mqttHeartbeat->mqttClient, callback);
}

// 连接MQTT客户端
bool connect_mqtt_client(MqttHeartbeat* mqttHeartbeat) {
    return connect_mqtt_client(mqttHeartbeat->mqttClient);
}

// 断开MQTT客户端
void disconnect_mqtt_client(MqttHeartbeat* mqttHeartbeat) {
    disconnect_mqtt_client(mqttHeartbeat->mqttClient);
}

// 订阅MQTT主题
bool subscribe_mqtt_topic(MqttHeartbeat* mqttHeartbeat, const char* topic) {
    return subscribe_mqtt_client(mqttHeartbeat->mqttClient, topic);
}

// 发布MQTT消息
bool publish_mqtt_message(MqttHeartbeat* mqttHeartbeat, const char* topic, const char* message) {
    return publish_mqtt_client(mqttHeartbeat->mqttClient, topic, message);
}

// MQTT循环
void mqtt_loop_forever(MqttHeartbeat* mqttHeartbeat) {
    mqtt_client_loop_forever(mqttHeartbeat->mqttClient);
}

// 生成心跳
json_t* generate_heartbeat(const MqttHeartbeat* mqttHeartbeat) {
    // 这里应该实现实际的心跳生成逻辑
    // 为了示例，我们返回一个简单的心跳JSON
    return json_pack("{s:s, s:i, s:s}", 
                     "cmd", "heartbeat", 
                     "timestamp", (int)time(NULL), 
                     "status", "ok");
}

// 发送心跳
void send_heartbeat(MqttHeartbeat* mqttHeartbeat) {
    json_t* heartbeat = generate_heartbeat(mqttHeartbeat);
    char* heartbeatStr = json_dumps(heartbeat, 0);
    publish_mqtt_message(mqttHeartbeat, "heartbeat/topic", heartbeatStr);
    free(heartbeatStr);
    json_decref(heartbeat);
}