#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    char* request_topic;
    char* publish_topic;
} MqttConfig;

MqttConfig* create_mqtt_config(void);
void destroy_mqtt_config(MqttConfig* config);
const char* get_request_topic(const MqttConfig* config);
void set_request_topic(MqttConfig* config, const char* new_request_topic);
const char* get_publish_topic(const MqttConfig* config);
void set_publish_topic(MqttConfig* config, const char* new_publish_topic);
bool load_mqtt_config_file(MqttConfig* config);
bool update_mqtt_config_file(const MqttConfig* config);

#endif // MQTT_CONFIG_H