#include "AcquisitionNoise.h"
#include <stdlib.h>
#include <string.h>

static AcquisitionNoise instance = {0};

AcquisitionNoise *AcquisitionNoise_getInstance() {
    return &instance;
}

int AcquisitionNoise_getMonitorStatus(AcquisitionNoise *self) {
    return self->monitorStatus;
}

void AcquisitionNoise_setMonitorStatus(AcquisitionNoise *self, int monitorStatus) {
    self->monitorStatus = monitorStatus;
}

int AcquisitionNoise_getFrequency(AcquisitionNoise *self) {
    return self->frequency;
}

void AcquisitionNoise_setFrequency(AcquisitionNoise *self, int frequency) {
    self->frequency = frequency;
}

int AcquisitionNoise_getCalcCycle(AcquisitionNoise *self) {
    return self->calcCycle;
}

void AcquisitionNoise_setCalcCycle(AcquisitionNoise *self, int calcCycle) {
    self->calcCycle = calcCycle;
}

size_t AcquisitionNoise_noiseDeque_size(AcquisitionNoise *self) {
    return self->noiseDeque.size();
}

void AcquisitionNoise_noiseDeque_pop_back(AcquisitionNoise *self) {
    self->noiseDeque.pop_back();
}

void AcquisitionNoise_schema_assign(AcquisitionNoise *self, CSchema *schema, int schema_count) {
    self->schema.assign(schema, schema + schema_count);
}

void AcquisitionNoise_schema_get(AcquisitionNoise *self, CSchema **schema, int *schema_count) {
    *schema_count = self->schema.size();
    *schema = (CSchema *)malloc(*schema_count * sizeof(CSchema));
    memcpy(*schema, self->schema.data(), *schema_count * sizeof(CSchema));
}

void AcquisitionNoise_file_update(AcquisitionNoise *self) {
    // 实现文件更新的函数
}