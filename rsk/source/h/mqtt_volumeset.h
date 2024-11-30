#ifndef MQTT_VOLUMESET_H
#define MQTT_VOLUMESET_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "volumeset.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    VolumeSet volumeConfig;
    MqttClient* mqttClient;
} MqttVolumeSet;

MqttVolumeSet* create_mqtt_volume_set(const char* clientId, const char* host, int port);
void destroy_mqtt_volume_set(MqttVolumeSet* mqttVolumeSet);
bool load_volume_set_config(MqttVolumeSet* mqttVolumeSet, const char* configPath);
bool save_volume_set_config(const MqttVolumeSet* mqttVolumeSet, const char* configPath);
void initialize_mqtt_client(MqttVolumeSet* mqttVolumeSet);
void set_mqtt_callback(MqttVolumeSet* mqttVolumeSet, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttVolumeSet* mqttVolumeSet);
void disconnect_mqtt_client(MqttVolumeSet* mqttVolumeSet);
bool subscribe_mqtt_topic(MqttVolumeSet* mqttVolumeSet, const char* topic);
bool publish_mqtt_message(MqttVolumeSet* mqttVolumeSet, const char* topic, const char* message);
void mqtt_loop_forever(MqttVolumeSet* mqttVolumeSet);

#endif // MQTT_VOLUMESET_H