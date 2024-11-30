#ifndef RS485_MANAGE_H
#define RS485_MANAGE_H

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
} Rs485ManageResult;

typedef struct {
    char* cmd;
    char* rs485Config;
} Rs485Manage;

Rs485ManageResult* create_rs485_manage_result(void);
void destroy_rs485_manage_result(Rs485ManageResult* result);
void rs485_manage_result_do_fail(Rs485ManageResult* result, const char* str);
void rs485_manage_result_do_success(Rs485ManageResult* result);

Rs485Manage* create_rs485_manage(void);
void destroy_rs485_manage(Rs485Manage* rs485Manage);
int rs485_manage_do_req(const Rs485Manage* rs485Manage, CSocket* pClient);

#endif // RS485_MANAGE_H