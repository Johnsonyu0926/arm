#ifndef RESTORE_H
#define RESTORE_H

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
} RestoreResult;

typedef struct {
    char* cmd;
} Restore;

RestoreResult* create_restore_result(void);
void destroy_restore_result(RestoreResult* result);
void restore_result_do_fail(RestoreResult* result, const char* str);
void restore_result_do_success(RestoreResult* result);

Restore* create_restore(void);
void destroy_restore(Restore* restore);
int restore_do_req(const Restore* restore, CSocket* pClient);

#endif // RESTORE_H