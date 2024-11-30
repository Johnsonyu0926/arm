#ifndef CUSTOM_AUDIO_H
#define CUSTOM_AUDIO_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    int type;
    char* fileName;
    int size;
} CustomAudio;

typedef struct {
    CustomAudio* audioFiles;
    size_t audioFileCount;
} CustomAudioManager;

CustomAudio* create_custom_audio(int type, const char* fileName, int size);
void destroy_custom_audio(CustomAudio* audio);
bool load_custom_audio_manager_from_file(CustomAudioManager* manager, const char* filename);
const CustomAudio* find_audio_by_name(const CustomAudioManager* manager, const char* name);
void init_custom_audio_manager(CustomAudioManager* manager);
void free_custom_audio_manager(CustomAudioManager* manager);

#endif // CUSTOM_AUDIO_H