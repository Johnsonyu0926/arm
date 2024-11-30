#ifndef VOLUME_ADAPT_SCHEMA_SET_H
#define VOLUME_ADAPT_SCHEMA_SET_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rs485_noise_manage.h"
#include "rs485_manage.h"
#include "acquisition_noise.h"
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

#endif // VOLUME_ADAPT_SCHEMA_SET_H