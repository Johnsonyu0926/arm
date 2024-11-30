#ifndef AUDIOSTOP_H
#define AUDIOSTOP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "utils.h"
#include "socket.h"

typedef struct {
    char* cmd;
    int resultId;
    char* msg;
} CAudioStopResult;

void init_audio_stop_result(CAudioStopResult* result);
void free_audio_stop_result(CAudioStopResult* result);
void do_success(CAudioStopResult* result);

typedef struct {
    char* cmd;
} CAudioStop;

void init_audio_stop(CAudioStop* stop);
void free_audio_stop(CAudioStop* stop);
int do_req(const CAudioStop* stop, CSocket* pClient);

#endif // AUDIOSTOP_H