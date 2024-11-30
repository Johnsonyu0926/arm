#ifndef HKVOLUME_H
#define HKVOLUME_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    char* id;
    int volume;
} HkVolume;

typedef struct {
    HkVolume volume;
    char* configPath;
} HkVolumeManager;

HkVolume* create_hk_volume(const char* id, int volume);
void destroy_hk_volume(HkVolume* volume);
json_t* hk_volume_to_json(const HkVolume* volume);
HkVolume* hk_volume_from_json(const json_t* json);

HkVolumeManager* create_hk_volume_manager(const char* configPath);
void destroy_hk_volume_manager(HkVolumeManager* manager);
bool load_hk_volume_manager(HkVolumeManager* manager);
bool save_hk_volume_manager(const HkVolumeManager* manager);
const HkVolume* get_hk_volume(const HkVolumeManager* manager);
void set_hk_volume(HkVolumeManager* manager, HkVolume* volume);

#endif // HKVOLUME_H