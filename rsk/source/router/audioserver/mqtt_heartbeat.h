#ifndef MQTT_HEARTBEAT_H
#define MQTT_HEARTBEAT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "heartbeat.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    Heartbeat heartbeatConfig;
    MqttClient* mqttClient;
} MqttHeartbeat;

MqttHeartbeat* create_mqtt_heartbeat(const char* clientId, const char* host, int port);
void destroy_mqtt_heartbeat(MqttHeartbeat* mqttHeartbeat);
bool load_heartbeat_config(MqttHeartbeat* mqttHeartbeat, const char* configPath);
bool save_heartbeat_config(const MqttHeartbeat* mqttHeartbeat, const char* configPath);
void initialize_mqtt_client(MqttHeartbeat* mqttHeartbeat);
void set_mqtt_callback(MqttHeartbeat* mqttHeartbeat, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttHeartbeat* mqttHeartbeat);
void disconnect_mqtt_client(MqttHeartbeat* mqttHeartbeat);
bool subscribe_mqtt_topic(MqttHeartbeat* mqttHeartbeat, const char* topic);
bool publish_mqtt_message(MqttHeartbeat* mqttHeartbeat, const char* topic, const char* message);
void mqtt_loop_forever(MqttHeartbeat* mqttHeartbeat);
json_t* generate_heartbeat(const MqttHeartbeat* mqttHeartbeat);
void send_heartbeat(MqttHeartbeat* mqttHeartbeat);

#endif // MQTT_HEARTBEAT_H