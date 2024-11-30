#ifndef MQTT_AUDIOPLAY_H
#define MQTT_AUDIOPLAY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include "public.h"

typedef struct {
    atomic_int m_mqttPlayStatus;
    atomic_int m_playId;
    atomic_int m_priority;
    atomic_int m_pId;
    char* playContent;
} PlayStatus;

PlayStatus* get_instance();
void init_play_status(PlayStatus* status);
bool get_play_state(const PlayStatus* status);
int get_play_id(const PlayStatus* status);
void set_play_id(PlayStatus* status, int id);
int get_priority(const PlayStatus* status);
void set_priority(PlayStatus* status, int id);
pid_t get_process_id(const PlayStatus* status);
void set_process_id(PlayStatus* status, pid_t id);
const char* get_play_content(const PlayStatus* status);
void set_play_content(PlayStatus* status, const char* content);
int get_mqtt_play_status(const PlayStatus* status);
void set_mqtt_play_status(PlayStatus* status, int status);

#endif // AUDIOPLAYSTATUS_H