#ifndef add_custom_audio_file_H
#define add_custom_audio_file_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    char* id;
    char* name;
    char* path;
} CustomAudioFile;

CustomAudioFile* create_custom_audio_file(const char* id, const char* name, const char* path);
void destroy_custom_audio_file(CustomAudioFile* file);
const char* get_id(const CustomAudioFile* file);
void set_id(CustomAudioFile* file, const char* new_id);
const char* get_name(const CustomAudioFile* file);
void set_name(CustomAudioFile* file, const char* new_name);
const char* get_path(const CustomAudioFile* file);
void set_path(CustomAudioFile* file, const char* new_path);
json_t* custom_audio_file_to_json(const CustomAudioFile* file);
CustomAudioFile* custom_audio_file_from_json(const json_t* json);

typedef struct {
    char* config_path;
    CustomAudioFile** files;
    size_t file_count;
} CustomAudioFileManager;

CustomAudioFileManager* create_custom_audio_file_manager(const char* config_path);
void destroy_custom_audio_file_manager(CustomAudioFileManager* manager);
bool load_custom_audio_file_manager(CustomAudioFileManager* manager);
bool save_custom_audio_file_manager(const CustomAudioFileManager* manager);
void add_file_to_manager(CustomAudioFileManager* manager, CustomAudioFile* file);
void remove_file_from_manager(CustomAudioFileManager* manager, const char* id);
CustomAudioFile* find_file_in_manager(const CustomAudioFileManager* manager, const char* id);

#endif // ACA_H