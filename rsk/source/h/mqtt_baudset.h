#ifndef MQTT_BAUDSET_H
#define MQTT_BAUDSET_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "baudset.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    BaudSet* baudConfig;
    size_t baudConfigCount;
    MqttClient* mqttClient;
} MqttBaudSet;

MqttBaudSet* create_mqtt_baud_set(const char* clientId, const char* host, int port);
void destroy_mqtt_baud_set(MqttBaudSet* mqttBaudSet);
bool load_baud_set_config(MqttBaudSet* mqttBaudSet, const char* configPath);
bool save_baud_set_config(const MqttBaudSet* mqttBaudSet, const char* configPath);
void initialize_mqtt_client(MqttBaudSet* mqttBaudSet);
void set_mqtt_callback(MqttBaudSet* mqttBaudSet, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttBaudSet* mqttBaudSet);
void disconnect_mqtt_client(MqttBaudSet* mqttBaudSet);
bool subscribe_mqtt_topic(MqttBaudSet* mqttBaudSet, const char* topic);
bool publish_mqtt_message(MqttBaudSet* mqttBaudSet, const char* topic, const char* message);
void mqtt_loop_forever(MqttBaudSet* mqttBaudSet);
json_t* handle_baud_set_command(const MqttBaudSet* mqttBaudSet, const json_t* command);

#endif // MQTT_BAUDSET_H