#ifndef AUDIOCFG_H
#define AUDIOCFG_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    json_t* config_json;
} CAudioCfgBusiness;

void init_audio_cfg_business(CAudioCfgBusiness* cfg);
void free_audio_cfg_business(CAudioCfgBusiness* cfg);
bool load_audio_cfg_business(CAudioCfgBusiness* cfg);
const char* get_audio_file_path(const CAudioCfgBusiness* cfg);

#endif // AUDIOCFG_H