#ifndef AUDIOPLAY_H
#define AUDIOPLAY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "audiocfg.h"
#include "utils.h"
#include "add_custom_audio_file.h"
#include "socket.h"

typedef struct {
    char* cmd;
    int resultId;
    char* msg;
} CAudioPlayResult;

void init_audio_play_result(CAudioPlayResult* result);
void free_audio_play_result(CAudioPlayResult* result);
void do_success(CAudioPlayResult* result);
void do_fail(CAudioPlayResult* result, const char* str);

typedef struct {
    char* cmd;
    char* audioName;
    int playType;
    int duration;
} CAudioPlay;

void init_audio_play(CAudioPlay* play);
void free_audio_play(CAudioPlay* play);
int do_req(CAudioPlay* play, CSocket* pClient);

#endif // AUDIOPLAY_H