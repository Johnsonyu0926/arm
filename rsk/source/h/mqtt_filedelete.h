#ifndef MQTT_FILEDELETE_H
#define MQTT_FILEDELETE_H

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
} MqttFileDelete;

MqttFileDelete* create_mqtt_file_delete(const char* clientId, const char* host, int port);
void destroy_mqtt_file_delete(MqttFileDelete* mqttFileDelete);
void initialize_mqtt_client(MqttFileDelete* mqttFileDelete);
void set_mqtt_callback(MqttFileDelete* mqttFileDelete, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttFileDelete* mqttFileDelete);
void disconnect_mqtt_client(MqttFileDelete* mqttFileDelete);
bool subscribe_mqtt_topic(MqttFileDelete* mqttFileDelete, const char* topic);
bool publish_mqtt_message(MqttFileDelete* mqttFileDelete, const char* topic, const char* message);
void mqtt_loop_forever(MqttFileDelete* mqttFileDelete);
json_t* handle_file_delete_command(const json_t* command);

#endif // MQTT_FILEDELETE_H