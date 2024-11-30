#ifndef ADD_MQTT_CUSTOM_AUDIO_FILE_H
#define ADD_MQTT_CUSTOM_AUDIO_FILE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    char* id;
    char* name;
    char* path;
} MqttCustomAudioFile;

MqttCustomAudioFile* create_mqtt_custom_audio_file(const char* id, const char* name, const char* path);
void destroy_mqtt_custom_audio_file(MqttCustomAudioFile* file);
const char* get_id(const MqttCustomAudioFile* file);
void set_id(MqttCustomAudioFile* file, const char* new_id);
const char* get_name(const MqttCustomAudioFile* file);
void set_name(MqttCustomAudioFile* file, const char* new_name);
const char* get_path(const MqttCustomAudioFile* file);
void set_path(MqttCustomAudioFile* file, const char* new_path);
json_t* mqtt_custom_audio_file_to_json(const MqttCustomAudioFile* file);
MqttCustomAudioFile* mqtt_custom_audio_file_from_json(const json_t* json);

typedef struct {
    char* config_path;
    MqttCustomAudioFile** files;
    size_t file_count;
} MqttCustomAudioFileManager;

MqttCustomAudioFileManager* create_mqtt_custom_audio_file_manager(const char* config_path);
void destroy_mqtt_custom_audio_file_manager(MqttCustomAudioFileManager* manager);
bool load_mqtt_custom_audio_file_manager(MqttCustomAudioFileManager* manager);
bool save_mqtt_custom_audio_file_manager(const MqttCustomAudioFileManager* manager);
void add_file_to_manager(MqttCustomAudioFileManager* manager, MqttCustomAudioFile* file);
void remove_file_from_manager(MqttCustomAudioFileManager* manager, const char* id);
MqttCustomAudioFile* find_file_in_manager(const MqttCustomAudioFileManager* manager, const char* id);

#endif // MQTT_CUSTOM_AUDIO_FILE_H