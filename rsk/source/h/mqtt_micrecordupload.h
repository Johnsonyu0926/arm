#ifndef MQTT_MICRECORDUPLOAD_H
#define MQTT_MICRECORDUPLOAD_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "micrecordupload.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    MicRecordUpload micRecordConfig;
    MqttClient* mqttClient;
} MqttMicRecordUpload;

MqttMicRecordUpload* create_mqtt_mic_record_upload(const char* clientId, const char* host, int port);
void destroy_mqtt_mic_record_upload(MqttMicRecordUpload* mqttMicRecordUpload);
bool load_mic_record_upload_config(MqttMicRecordUpload* mqttMicRecordUpload, const char* configPath);
bool save_mic_record_upload_config(const MqttMicRecordUpload* mqttMicRecordUpload, const char* configPath);
void initialize_mqtt_client(MqttMicRecordUpload* mqttMicRecordUpload);
void set_mqtt_callback(MqttMicRecordUpload* mqttMicRecordUpload, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttMicRecordUpload* mqttMicRecordUpload);
void disconnect_mqtt_client(MqttMicRecordUpload* mqttMicRecordUpload);
bool subscribe_mqtt_topic(MqttMicRecordUpload* mqttMicRecordUpload, const char* topic);
bool publish_mqtt_message(MqttMicRecordUpload* mqttMicRecordUpload, const char* topic, const char* message);
void mqtt_loop_forever(MqttMicRecordUpload* mqttMicRecordUpload);
json_t* handle_mic_record_upload_command(const MqttMicRecordUpload* mqttMicRecordUpload, const json_t* command);

#endif // MQTT_MICRECORDUPLOAD_H