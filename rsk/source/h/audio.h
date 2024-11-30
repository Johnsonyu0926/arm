#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "AudioPlayUtils.h"
#include "MqttConfig.h"
#include "CustomAudio.h"
#include "spdlog.h"

typedef struct {
    char* request_topic;
    char* publish_topic;
    pthread_t server_thread;
    bool running;
    CustomAudioManager custom_audio_manager;
} AudioServer;

void initialize_logging();
void audio_server_init(AudioServer* server);
void audio_server_start(AudioServer* server);
void audio_server_stop(AudioServer* server);
void audio_server_load_config(AudioServer* server);
void audio_server_load_custom_audio_files(AudioServer* server);
void* audio_server_run(void* arg);
void handle_custom_audio_play_request(AudioServer* server, const char* file_name);

#endif // AUDIO_H