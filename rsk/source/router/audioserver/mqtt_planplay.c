#ifndef MQTT_PLANPLAY_H
#define MQTT_PLANPLAY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "planPlay.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    PlanPlay planPlayConfig;
    MqttClient* mqttClient;
} MqttPlanPlay;

MqttPlanPlay* create_mqtt_planplay(const char* clientId, const char* host, int port);
void destroy_mqtt_planplay(MqttPlanPlay* mqttPlanPlay);
bool load_mqtt_planplay_config(MqttPlanPlay* mqttPlanPlay, const char* configPath);
bool save_mqtt_planplay_config(const MqttPlanPlay* mqttPlanPlay, const char* configPath);
void initialize_mqtt_client(MqttPlanPlay* mqttPlanPlay);
void set_mqtt_callback(MqttPlanPlay* mqttPlanPlay, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttPlanPlay* mqttPlanPlay);
void disconnect_mqtt_client(MqttPlanPlay* mqttPlanPlay);
bool subscribe_mqtt_topic(MqttPlanPlay* mqttPlanPlay, const char* topic);
bool publish_mqtt_message(MqttPlanPlay* mqttPlanPlay, const char* topic, const char* message);
void mqtt_loop_forever(MqttPlanPlay* mqttPlanPlay);
json_t* handle_planplay_command(const json_t* command);

#endif // MQTT_PLANPLAY_H