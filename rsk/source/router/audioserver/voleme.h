#ifndef VOLUME_H
#define VOLUME_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    char* id;
    int level;
} Volume;

typedef struct {
    char* configPath;
    Volume volume;
} VolumeManager;

Volume* create_volume(const char* id, int level);
void destroy_volume(Volume* volume);
const char* get_volume_id(const Volume* volume);
void set_volume_id(Volume* volume, const char* newId);
int get_volume_level(const Volume* volume);
void set_volume_level(Volume* volume, int newLevel);
json_t* volume_to_json(const Volume* volume);
Volume* volume_from_json(const json_t* j);

VolumeManager* create_volume_manager(const char* configPath);
void destroy_volume_manager(VolumeManager* manager);
bool load_volume_manager(VolumeManager* manager);
bool save_volume_manager(const VolumeManager* manager);
const Volume* get_volume_manager_volume(const VolumeManager* manager);
void set_volume_manager_volume(VolumeManager* manager, Volume* newVolume);

#endif // VOLUME_H