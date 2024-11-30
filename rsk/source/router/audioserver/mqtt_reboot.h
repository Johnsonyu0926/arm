#ifndef MQTT_REBOOT_H
#define MQTT_REBOOT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "reboot.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    Reboot rebootConfig;
    MqttClient* mqttClient;
} MqttReboot;

MqttReboot* create_mqtt_reboot(const char* clientId, const char* host, int port);
void destroy_mqtt_reboot(MqttReboot* mqttReboot);
bool load_reboot_config(MqttReboot* mqttReboot, const char* configPath);
bool save_reboot_config(const MqttReboot* mqttReboot, const char* configPath);
void initialize_mqtt_client(MqttReboot* mqttReboot);
void set_mqtt_callback(MqttReboot* mqttReboot, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttReboot* mqttReboot);
void disconnect_mqtt_client(MqttReboot* mqttReboot);
bool subscribe_mqtt_topic(MqttReboot* mqttReboot, const char* topic);
bool publish_mqtt_message(MqttReboot* mqttReboot, const char* topic, const char* message);
void mqtt_loop_forever(MqttReboot* mqttReboot);
json_t* handle_reboot_command(const MqttReboot* mqttReboot, const json_t* command);

#endif // MQTT_REBOOT_H