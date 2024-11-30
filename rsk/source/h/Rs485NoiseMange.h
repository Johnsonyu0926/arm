#ifndef RS485_NOISE_MANAGE_H
#define RS485_NOISE_MANAGE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    char* noise_topic;
} Rs485NoiseManage;

Rs485NoiseManage* create_rs485_noise_manage(void);
void destroy_rs485_noise_manage(Rs485NoiseManage* manage);
const char* get_noise_topic(const Rs485NoiseManage* manage);
void set_noise_topic(Rs485NoiseManage* manage, const char* new_noise_topic);
bool load_noise_config_file(Rs485NoiseManage* manage);
bool update_noise_config_file(const Rs485NoiseManage* manage);
