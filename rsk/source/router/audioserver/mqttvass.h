#ifndef MQTTVASS_H
#define MQTTVASS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rs485noisemange.h"
#include "rs485manage.h"
#include "acquisitionnoise.h"
#include "cutils.h"
#include "rs485.h"

typedef struct {
    bool monitorStatus;
} VolumeAdaptSchemaSet;

typedef struct {
    int resultId;
    char* result;
} Result;

typedef struct {
    VolumeAdaptSchemaSet data;
} Request;

void start_async_task(void (*task)(void));
int do_success(const Request* request, Result* result);

#endif // MQTTVASS_H