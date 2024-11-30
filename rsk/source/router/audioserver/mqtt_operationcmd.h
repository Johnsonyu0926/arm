//mqtt_operationcmd
#ifndef MQTT_OPERATIONCMD_H
#define MQTT_OPERATIONCMD_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "planplay.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    PlanPlay planPlayConfig;
    MqttClient* mqttClient;
} MqttPlanPlay;

MqttPlanPlay* create_mqtt_plan_play(const char* clientId, const char* host, int port);
void destroy_mqtt_plan_play(MqttPlanPlay* mqttPlanPlay);
bool load_plan_play_config(MqttPlanPlay* mqttPlanPlay, const char* configPath);
bool save_plan_play_config(const MqttPlanPlay* mqttPlanPlay, const char* configPath);
void initialize_mqtt_client(MqttPlanPlay* mqttPlanPlay);
void set_mqtt_callback(MqttPlanPlay* mqttPlanPlay, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttPlanPlay* mqttPlanPlay);
void disconnect_mqtt_client(MqttPlanPlay* mqttPlanPlay);
bool subscribe_mqtt_topic(MqttPlanPlay* mqttPlanPlay, const char* topic);
bool publish_mqtt_message(MqttPlanPlay* mqttPlanPlay, const char* topic, const char* message);
void mqtt_loop_forever(MqttPlanPlay* mqttPlanPlay);
json_t* handle_plan_play_command(const MqttPlanPlay* mqttPlanPlay, const json_t* command);

#endif // MQTT_PLANPLAY_H