#ifndef MQTT_TTSPLAY_H
#define MQTT_TTSPLAY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "mqtt.h"
#include "ttsplay.h"

typedef struct {
    char* clientId;
    char* host;
    int port;
    TtsPlay ttsConfig;
    MqttClient* mqttClient;
} MqttTtsPlay;

MqttTtsPlay* create_mqtt_tts_play(const char* clientId, const char* host, int port);
void destroy_mqtt_tts_play(MqttTtsPlay* mqttTtsPlay);
bool load_tts_play_config(MqttTtsPlay* mqttTtsPlay, const char* configPath);
bool save_tts_play_config(const MqttTtsPlay* mqttTtsPlay, const char* configPath);
void initialize_mqtt_client(MqttTtsPlay* mqttTtsPlay);
void set_mqtt_callback(MqttTtsPlay* mqttTtsPlay, void (*callback)(const char* topic, const char* message));
bool connect_mqtt_client(MqttTtsPlay* mqttTtsPlay);
void disconnect_mqtt_client(MqttTtsPlay* mqttTtsPlay);
bool subscribe_mqtt_topic(MqttTtsPlay* mqttTtsPlay, const char* topic);
bool publish_mqtt_message(MqttTtsPlay* mqttTtsPlay, const char* topic, const char* message);
void mqtt_loop_forever(MqttTtsPlay* mqttTtsPlay);
bool play_tts(MqttTtsPlay* mqttTtsPlay, const char* text);

#endif // MQTT_TTSPLAY_H