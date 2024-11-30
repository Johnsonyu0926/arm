#ifndef MQTT_AUDIOSTREAMSTART_H
#define MQTT_AUDIOSTREAMSTART_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "audiostreamstart.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    AudioStreamStart* audioConfig;
    size_t audioConfigCount;
    MqttClient* mqttClient;
} MqttAudioStreamStart;

MqttAudioStreamStart* create_mqtt_audio_stream_start(const char* clientId, const char* host, int port);
void destroy_mqtt_audio_stream_start(MqttAudioStreamStart* mqttAudioStreamStart);
bool load_audio_stream_start_config(MqttAudioStreamStart* mqttAudioStreamStart, const char* configPath);
bool save_audio_stream_start_config(const MqttAudioStreamStart* mqttAudioStreamStart, const char* configPath);
void initialize_mqtt_client(MqttAudioStreamStart* mqttAudioStreamStart);
void set_mqtt_callback(MqttAudioStreamStart* mqttAudioStreamStart, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttAudioStreamStart* mqttAudioStreamStart);
void disconnect_mqtt_client(MqttAudioStreamStart* mqttAudioStreamStart);
bool subscribe_mqtt_topic(MqttAudioStreamStart* mqttAudioStreamStart, const char* topic);
bool publish_mqtt_message(MqttAudioStreamStart* mqttAudioStreamStart, const char* topic, const char* message);
void mqtt_loop_forever(MqttAudioStreamStart* mqttAudioStreamStart);
json_t* handle_audio_stream_start_command(const MqttAudioStreamStart* mqttAudioStreamStart, const json_t* command);

#endif // MQTT_AUDIOSTREAMSTART_H