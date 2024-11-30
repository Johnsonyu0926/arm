#ifndef MQTT_LEDSHOWSET_H
#define MQTT_LEDSHOWSET_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "ledshowset.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    LedShowSet ledShowConfig;
    MqttClient* mqttClient;
} MqttLedShowSet;

MqttLedShowSet* create_mqtt_led_show_set(const char* clientId, const char* host, int port);
void destroy_mqtt_led_show_set(MqttLedShowSet* mqttLedShowSet);
bool load_led_show_set_config(MqttLedShowSet* mqttLedShowSet, const char* configPath);
bool save_led_show_set_config(const MqttLedShowSet* mqttLedShowSet, const char* configPath);
void initialize_mqtt_client(MqttLedShowSet* mqttLedShowSet);
void set_mqtt_callback(MqttLedShowSet* mqttLedShowSet, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttLedShowSet* mqttLedShowSet);
void disconnect_mqtt_client(MqttLedShowSet* mqttLedShowSet);
bool subscribe_mqtt_topic(MqttLedShowSet* mqttLedShowSet, const char* topic);
bool publish_mqtt_message(MqttLedShowSet* mqttLedShowSet, const char* topic, const char* message);
void mqtt_loop_forever(MqttLedShowSet* mqttLedShowSet);
json_t* handle_led_show_set_command(const MqttLedShowSet* mqttLedShowSet, const json_t* command);

#endif // MQTT_LEDSHOWSET_H