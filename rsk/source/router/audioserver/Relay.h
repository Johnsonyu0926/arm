#ifndef RELAY_H
#define RELAY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

typedef struct {
    char* relay_topic;
} RelayConfig;

RelayConfig* create_relay_config(void);
void destroy_relay_config(RelayConfig* config);
const char* get_relay_topic(const RelayConfig* config);
void set_relay_topic(RelayConfig* config, const char* new_relay_topic);
bool load_relay_config_file(RelayConfig* config);
bool update_relay_config_file(const RelayConfig* config);

#endif // RELAY_CONFIG_H