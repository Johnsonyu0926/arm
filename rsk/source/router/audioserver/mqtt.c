#include "mqtt.h"

// 创建MQTT对象
MQTT* create_mqtt(const char* clientId, const char* host, int port) {
    MQTT* mqtt = (MQTT*)malloc(sizeof(MQTT));
    if (!mqtt) return NULL;
    mqtt->clientId = strdup(clientId);
    mqtt->host = strdup(host);
    mqtt->port = port;
    mqtt->mosq = mosquitto_new(clientId, true, mqtt);
    mqtt->request_topic = strdup("IOT/intranet/client/request/");
    mqtt->publish_topic = strdup("IOT/intranet/server/report/");
    return mqtt;
}

// 销毁MQTT对象
void destroy_mqtt(MQTT* mqtt) {
    if (mqtt) {
        free(mqtt->clientId);
        free(mqtt->host);
        free(mqtt->request_topic);
        free(mqtt->publish_topic);
        mosquitto_destroy(mqtt->mosq);
        free(mqtt);
    }
}

// 连接MQTT客户端
bool connect_mqtt(MQTT* mqtt) {
    int rc = mosquitto_connect(mqtt->mosq, mqtt->host, mqtt->port, 60);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Failed to connect to MQTT broker: %s\n", mosquitto_strerror(rc));
        return false;
    }
    return true;
}

// 断开MQTT客户端
void disconnect_mqtt(MQTT* mqtt) {
    mosquitto_disconnect(mqtt->mosq);
}

// 订阅MQTT主题
bool subscribe_mqtt(MQTT* mqtt, const char* topic) {
    int rc = mosquitto_subscribe(mqtt->mosq, NULL, topic, 0);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Failed to subscribe to topic: %s\n", mosquitto_strerror(rc));
        return false;
    }
    return true;
}

// 发布MQTT消息
bool publish_mqtt(MQTT* mqtt, const char* topic, const char* message) {
    int rc = mosquitto_publish(mqtt->mosq, NULL, topic, strlen(message), message, 0, false);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Failed to publish message: %s\n", mosquitto_strerror(rc));
        return false;
    }
    return true;
}

// MQTT循环
void mqtt_loop_forever(MQTT* mqtt) {
    mosquitto_loop_forever(mqtt->mosq, -1, 1);
}

// 设置MQTT回调
void mqtt_set_callback(MQTT* mqtt, void (*callback)(const char* topic, const char* message)) {
    mosquitto_message_callback_set(mqtt->mosq, callback);
}

// 心跳函数
void mqtt_heartbeat(MQTT* mqtt) {
    CUtils utils;
    utils.async_wait(0, asns::MQTT_HEART_BEAT_TIME, asns::MQTT_HEART_BEAT_TIME, [mqtt, &utils] {
        std::string reStr = ServiceManage::instance().heartBeat();
        publish_mqtt(mqtt, mqtt->publish_topic, reStr.c_str());
        utils.heart_beat("/tmp/audio_server_mqtt_heartbeat.txt");
    });
}