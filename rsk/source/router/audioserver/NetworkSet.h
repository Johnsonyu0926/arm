#ifndef NETWORKSET_H
#define NETWORKSET_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "utils.h"
#include "csocket.h"

typedef struct {
    char* cmd;
    int resultId;
    char* msg;
} NetworkSetResult;

typedef struct {
    char* cmd;
    char* ipAddress;
    char* gateway;
    char* netMask;
} NetworkSet;

NetworkSetResult* create_network_set_result(void);
void destroy_network_set_result(NetworkSetResult* result);
void network_set_result_do_success(NetworkSetResult* result);

NetworkSet* create_network_set(void);
void destroy_network_set(NetworkSet* networkSet);
int network_set_do_req(const NetworkSet* networkSet, CSocket* pClient);

#endif // NETWORKSET_H