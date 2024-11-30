#ifndef MQTT_RECORDPLAY_H
#define MQTT_RECORDPLAY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "recordplay.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    RecordPlay recordPlayConfig;
    MqttClient* mqttClient;
} MqttRecordPlay;

MqttRecordPlay* create_mqtt_record_play(const char* clientId, const char* host, int port);
void destroy_mqtt_record_play(MqttRecordPlay* mqttRecordPlay);
bool load_record_play_config(MqttRecordPlay* mqttRecordPlay, const char* configPath);
bool save_record_play_config(const MqttRecordPlay* mqttRecordPlay, const char* configPath);
void initialize_mqtt_client(MqttRecordPlay* mqttRecordPlay);
void set_mqtt_callback(MqttRecordPlay* mqttRecordPlay, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttRecordPlay* mqttRecordPlay);
void disconnect_mqtt_client(MqttRecordPlay* mqttRecordPlay);
bool subscribe_mqtt_topic(MqttRecordPlay* mqttRecordPlay, const char* topic);
bool publish_mqtt_message(MqttRecordPlay* mqttRecordPlay, const char* topic, const char* message);
void mqtt_loop_forever(MqttRecordPlay* mqttRecordPlay);

#endif // MQTT_RECORDPLAY_H