#ifndef REBOOT_H
#define REBOOT_H

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
} RebootResult;

typedef struct {
    char* cmd;
} Reboot;

RebootResult* create_reboot_result(void);
void destroy_reboot_result(RebootResult* result);
void reboot_result_do_fail(RebootResult* result, const char* str);
void reboot_result_do_success(RebootResult* result);

Reboot* create_reboot(void);
void destroy_reboot(Reboot* reboot);
int reboot_do_req(const Reboot* reboot, CSocket* pClient);

#endif // REBOOT_H