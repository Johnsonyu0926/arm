#ifndef MICRECORD_H
#define MICRECORD_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    char* id;
    char* name;
    char* path;
} MicRecord;

typedef struct {
    MicRecord* records;
    size_t recordCount;
    char* configPath;
} MicRecordManager;

MicRecord* create_mic_record(const char* id, const char* name, const char* path);
void destroy_mic_record(MicRecord* record);
json_t* mic_record_to_json(const MicRecord* record);
MicRecord* mic_record_from_json(const json_t* json);

MicRecordManager* create_mic_record_manager(const char* configPath);
void destroy_mic_record_manager(MicRecordManager* manager);
bool load_mic_record_manager(MicRecordManager* manager);
bool save_mic_record_manager(const MicRecordManager* manager);
void add_record_to_manager(MicRecordManager* manager, MicRecord* record);
void remove_record_from_manager(MicRecordManager* manager, const char* id);
MicRecord* find_record_in_manager(MicRecordManager* manager, const char* id);

#endif // MICRECORD_H