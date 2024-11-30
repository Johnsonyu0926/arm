#ifndef GETAUDIOLIST_H
#define GETAUDIOLIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    char* id;
    char* name;
    char* path;
} AudioFileInfo;

typedef struct {
    AudioFileInfo* files;
    size_t fileCount;
    char* configPath;
} AudioFileInfoManager;

AudioFileInfo* create_audio_file_info(const char* id, const char* name, const char* path);
void destroy_audio_file_info(AudioFileInfo* file);
json_t* audio_file_info_to_json(const AudioFileInfo* file);
AudioFileInfo* audio_file_info_from_json(const json_t* json);

AudioFileInfoManager* create_audio_file_info_manager(const char* configPath);
void destroy_audio_file_info_manager(AudioFileInfoManager* manager);
bool load_audio_file_info_manager(AudioFileInfoManager* manager);
bool save_audio_file_info_manager(const AudioFileInfoManager* manager);
void add_file_to_manager(AudioFileInfoManager* manager, AudioFileInfo* file);
void remove_file_from_manager(AudioFileInfoManager* manager, const char* id);
AudioFileInfo* find_file_in_manager(AudioFileInfoManager* manager, const char* id);

#endif // GETAUDIOLIST_H