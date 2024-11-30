#ifndef MQTT_AUDIOSTOP_H
#define MQTT_AUDIOSTOP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "audiostop.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    AudioStop* audioConfig;
    size_t audioConfigCount;
    MqttClient* mqttClient;
} MqttAudioStop;

MqttAudioStop* create_mqtt_audio_stop(const char* clientId, const char* host, int port);
void destroy_mqtt_audio_stop(MqttAudioStop* mqttAudioStop);
bool load_config(MqttAudioStop* mqttAudioStop, const char* configPath);
bool save_config(const MqttAudioStop* mqttAudioStop, const char* configPath);
void initialize_client(MqttAudioStop* mqttAudioStop);
void set_callback(MqttAudioStop* mqttAudioStop, void (*callback)(const char* topic, const char* message));
bool connect_mqtt(MqttAudioStop* mqttAudioStop);
void disconnect_mqtt(MqttAudioStop* mqttAudioStop);
bool subscribe_mqtt(MqttAudioStop* mqttAudioStop, const char* topic);
bool publish_mqtt(MqttAudioStop* mqttAudioStop, const char* topic, const char* message);
void loop_forever(MqttAudioStop* mqttAudioStop);
json_t* handle_audio_stop_command(const json_t* command);

#endif // MQTT_AUDIOSTOP_H