#ifndef MQTT_RETEMP_H
#define MQTT_RETEMP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "retemp.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    ReTemp reTempConfig;
    MqttClient* mqttClient;
} MqttReTemp;

MqttReTemp* create_mqtt_retemp(const char* clientId, const char* host, int port);
void destroy_mqtt_retemp(MqttReTemp* mqttReTemp);
bool load_retemp_config(MqttReTemp* mqttReTemp, const char* configPath);
bool save_retemp_config(const MqttReTemp* mqttReTemp, const char* configPath);
void initialize_mqtt_client(MqttReTemp* mqttReTemp);
void set_mqtt_callback(MqttReTemp* mqttReTemp, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttReTemp* mqttReTemp);
void disconnect_mqtt_client(MqttReTemp* mqttReTemp);
bool subscribe_mqtt_topic(MqttReTemp* mqttReTemp, const char* topic);
bool publish_mqtt_message(MqttReTemp* mqttReTemp, const char* topic, const char* message);
void mqtt_loop_forever(MqttReTemp* mqttReTemp);
json_t* handle_audio_play_command(const MqttReTemp* mqttReTemp, const json_t* command);

#endif // MQTT_RETEMP_H