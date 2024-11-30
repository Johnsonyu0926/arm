#ifndef MQTT_FILEUPLOAD_H
#define MQTT_FILEUPLOAD_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <curl/curl.h>
#include "mqtt.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    MqttClient* mqttClient;
} MqttFileUpload;

MqttFileUpload* create_mqtt_file_upload(const char* clientId, const char* host, int port);
void destroy_mqtt_file_upload(MqttFileUpload* mqttFileUpload);
void initialize_mqtt_client(MqttFileUpload* mqttFileUpload);
void set_mqtt_callback(MqttFileUpload* mqttFileUpload, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttFileUpload* mqttFileUpload);
void disconnect_mqtt_client(MqttFileUpload* mqttFileUpload);
bool subscribe_mqtt_topic(MqttFileUpload* mqttFileUpload, const char* topic);
bool publish_mqtt_message(MqttFileUpload* mqttFileUpload, const char* topic, const char* message);
void mqtt_loop_forever(MqttFileUpload* mqttFileUpload);
json_t* handle_file_upload_command(const json_t* command);

#endif // MQTT_FILEUPLOAD_H