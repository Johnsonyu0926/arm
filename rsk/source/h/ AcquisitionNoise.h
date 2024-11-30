#ifndef ACQUISITIONNOISE_H
#define ACQUISITIONNOISE_H

#include <stdbool.h>
#include <deque>

typedef struct {
    int monitorStatus;
    int frequency;
    int calcCycle;
    std::deque<double> noiseDeque;
    std::vector<CSchema> schema;
    double decibel;
} AcquisitionNoise;

AcquisitionNoise *AcquisitionNoise_getInstance();
int AcquisitionNoise_getMonitorStatus(AcquisitionNoise *self);
void AcquisitionNoise_setMonitorStatus(AcquisitionNoise *self, int monitorStatus);
int AcquisitionNoise_getFrequency(AcquisitionNoise *self);
void AcquisitionNoise_setFrequency(AcquisitionNoise *self, int frequency);
int AcquisitionNoise_getCalcCycle(AcquisitionNoise *self);
void AcquisitionNoise_setCalcCycle(AcquisitionNoise *self, int calcCycle);
size_t AcquisitionNoise_noiseDeque_size(AcquisitionNoise *self);
void AcquisitionNoise_noiseDeque_pop_back(AcquisitionNoise *self);
void AcquisitionNoise_schema_assign(AcquisitionNoise *self, CSchema *schema, int schema_count);
void AcquisitionNoise_schema_get(AcquisitionNoise *self, CSchema **schema, int *schema_count);
void AcquisitionNoise_file_update(AcquisitionNoise *self);

#endif // ACQUISITIONNOISE_H