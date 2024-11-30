#ifndef PTZOPERATE_H
#define PTZOPERATE_H

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
} PtzOperateResult;

typedef struct {
    char* cmd;
    char* operateCmd;
} PtzOperate;

PtzOperateResult* create_ptz_operate_result(void);
void destroy_ptz_operate_result(PtzOperateResult* result);
void ptz_operate_result_do_fail(PtzOperateResult* result, const char* str);
void ptz_operate_result_do_success(PtzOperateResult* result);

PtzOperate* create_ptz_operate(void);
void destroy_ptz_operate(PtzOperate* ptzOperate);
const char* ptz_operate_do_req(const PtzOperate* ptzOperate, CSocket* pClient);

#endif // PTZOPERATE_H