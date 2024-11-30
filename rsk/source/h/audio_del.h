#ifndef AUDIO_DEL_H
#define AUDIO_DEL_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "add_custom_audio_file.h"
#include "add_column_custom_audio_file.h"
#include "audiocfg.h"
#include "utils.h"
#include "getaudiolist.h"

typedef struct {
    char* cmd;
    int resultId;
    char* msg;
    CGetAudioData* data;
    size_t data_count;
} CDeleteAudioResult;

void init_delete_audio_result(CDeleteAudioResult* result);
void free_delete_audio_result(CDeleteAudioResult* result);
void do_success(CDeleteAudioResult* result);

typedef struct {
    char* cmd;
    char* deleteName;
    int storageType;
} CDeleteAudio;

void init_delete_audio(CDeleteAudio* del);
void free_delete_audio(CDeleteAudio* del);
int do_del(const char* name, int type);
int do_req(CDeleteAudio* del, CSocket* pClient);

#endif // AUDIO_DEL_H