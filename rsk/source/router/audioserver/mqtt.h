#ifndef MQTT_H
#define MQTT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <jansson.h>
#include "mqtt_servicemanage.h"
#include "audiocfg.h"
#include "public.h"
#include "utils.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    struct mosquitto* mosq;
    char* request_topic;
    char* publish_topic;
} MQTT;

MQTT* create_mqtt(const char* clientId, const char* host, int port);
void destroy_mqtt(MQTT* mqtt);
bool connect_mqtt(MQTT* mqtt);
void disconnect_mqtt(MQTT* mqtt);
bool subscribe_mqtt(MQTT* mqtt, const char* topic);
bool publish_mqtt(MQTT* mqtt, const char* topic, const char* message);
void mqtt_loop_forever(MQTT* mqtt);
void mqtt_set_callback(MQTT* mqtt, void (*callback)(const char* topic, const char* message));
void mqtt_heartbeat(MQTT* mqtt);

#endif // MQTT_H