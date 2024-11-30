#ifndef SERVER_SET_H
#define SERVER_SET_H

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
} ServerSetResult;

typedef struct {
    char* cmd;
    char* serverConfig;
} ServerSet;

ServerSetResult* create_server_set_result(void);
void destroy_server_set_result(ServerSetResult* result);
void server_set_result_do_fail(ServerSetResult* result, const char* str);
void server_set_result_do_success(ServerSetResult* result);

ServerSet* create_server_set(void);
void destroy_server_set(ServerSet* serverSet);
int server_set_do_req(const ServerSet* serverSet, CSocket* pClient);

#endif // SERVER_SET_H