#ifndef RELAY_SET_H
#define RELAY_SET_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "utils.h"
#include "rs485.h"
#include "csocket.h"

typedef struct {
    char* cmd;
    int resultId;
    char* msg;
} RelaySetResult;

typedef struct {
    char* cmd;
    char* relayState;
} RelaySet;

RelaySetResult* create_relay_set_result(void);
void destroy_relay_set_result(RelaySetResult* result);
void relay_set_result_do_fail(RelaySetResult* result, const char* str);
void relay_set_result_do_success(RelaySetResult* result);

RelaySet* create_relay_set(void);
void destroy_relay_set(RelaySet* relaySet);
int relay_set_do_req(const RelaySet* relaySet, CSocket* pClient);

#endif // RELAY_SET_H