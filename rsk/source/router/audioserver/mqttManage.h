#ifndef MQTT_MANAGE_H
#define MQTT_MANAGE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    MqttClient* mqttClient;
} MqttManage;

MqttManage* create_mqtt_manage();
void destroy_mqtt_manage(MqttManage* mqttManage);
bool load_mqtt_manage_config(MqttManage* mqttManage, const char* configPath);
bool save_mqtt_manage_config(const MqttManage* mqttManage, const char* configPath);
void initialize_mqtt_client(MqttManage* mqttManage);
void set_mqtt_callback(MqttManage* mqttManage, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttManage* mqttManage);
void disconnect_mqtt_client(MqttManage* mqttManage);
bool subscribe_mqtt_topic(MqttManage* mqttManage, const char* topic);
bool publish_mqtt_message(MqttManage* mqttManage, const char* topic, const char* message);
void mqtt_loop_forever(MqttManage* mqttManage);

#endif // MQTT_MANAGE_H