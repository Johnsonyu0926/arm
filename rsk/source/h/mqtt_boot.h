#ifndef MQTT_BOOT_H
#define MQTT_BOOT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "boot.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    Boot* bootConfig;
    size_t bootConfigSize;
    MqttClient* mqttClient;
} MqttBoot;

MqttBoot* create_mqtt_boot(const char* clientId, const char* host, int port);
void destroy_mqtt_boot(MqttBoot* mqttBoot);
bool load_mqtt_boot_config(MqttBoot* mqttBoot, const char* configPath);
bool save_mqtt_boot_config(const MqttBoot* mqttBoot, const char* configPath);
void initialize_mqtt_client(MqttBoot* mqttBoot);
void set_mqtt_callback(MqttBoot* mqttBoot, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttBoot* mqttBoot);
void disconnect_mqtt_client(MqttBoot* mqttBoot);
bool subscribe_mqtt_topic(MqttBoot* mqttBoot, const char* topic);
bool publish_mqtt_message(MqttBoot* mqttBoot, const char* topic, const char* message);
void mqtt_loop_forever(MqttBoot* mqttBoot);
json_t* generate_start_report(void);
void send_start_report(MqttBoot* mqttBoot);

#endif // MQTT_BOOT_H