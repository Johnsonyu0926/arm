#ifndef MQTT_GPIOSET_H
#define MQTT_GPIOSET_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "gpioset.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    GpioSet gpioConfig;
    MqttClient* mqttClient;
} MqttGpioSet;

MqttGpioSet* create_mqtt_gpio_set(const char* clientId, const char* host, int port);
void destroy_mqtt_gpio_set(MqttGpioSet* mqttGpioSet);
bool load_gpio_set_config(MqttGpioSet* mqttGpioSet, const char* configPath);
bool save_gpio_set_config(const MqttGpioSet* mqttGpioSet, const char* configPath);
void initialize_mqtt_client(MqttGpioSet* mqttGpioSet);
void set_mqtt_callback(MqttGpioSet* mqttGpioSet, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttGpioSet* mqttGpioSet);
void disconnect_mqtt_client(MqttGpioSet* mqttGpioSet);
bool subscribe_mqtt_topic(MqttGpioSet* mqttGpioSet, const char* topic);
bool publish_mqtt_message(MqttGpioSet* mqttGpioSet, const char* topic, const char* message);
void mqtt_loop_forever(MqttGpioSet* mqttGpioSet);
json_t* handle_gpio_set_command(const MqttGpioSet* mqttGpioSet, const json_t* command);

#endif // MQTT_GPIOSET_H