#ifndef MQTT_SERVICEMANAGE_H
#define MQTT_SERVICEMANAGE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "servicemanage.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    ServiceManage serviceManageConfig;
    MqttClient* mqttClient;
} MqttServiceManage;

MqttServiceManage* create_mqtt_service_manage(const char* clientId, const char* host, int port);
void destroy_mqtt_service_manage(MqttServiceManage* mqttServiceManage);
bool load_service_manage_config(MqttServiceManage* mqttServiceManage, const char* configPath);
bool save_service_manage_config(const MqttServiceManage* mqttServiceManage, const char* configPath);
void initialize_mqtt_client(MqttServiceManage* mqttServiceManage);
void set_mqtt_callback(MqttServiceManage* mqttServiceManage, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttServiceManage* mqttServiceManage);
void disconnect_mqtt_client(MqttServiceManage* mqttServiceManage);
bool subscribe_mqtt_topic(MqttServiceManage* mqttServiceManage, const char* topic);
bool publish_mqtt_message(MqttServiceManage* mqttServiceManage, const char* topic, const char* message);
void mqtt_loop_forever(MqttServiceManage* mqttServiceManage);

#endif // MQTT_SERVICEMANAGE_H