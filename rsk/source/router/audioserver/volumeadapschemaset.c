#include "volumeadapschemaset.h"
#include "json.h"
#include "AcquisitionNoise.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CVolumeAdaptSchemaData_init(CVolumeAdaptSchemaData *self) {
    self->monitorStatus = 0;
    self->frequency = 0;
    self->calcCycle = 0;
    self->schema = NULL;
    self->schema_count = 0;
}

void CVolumeAdaptSchemaData_set_data(const CVolumeAdaptSchemaData *self) {
    AcquisitionNoise *instance = AcquisitionNoise_getInstance();
    AcquisitionNoise_setMonitorStatus(instance, self->monitorStatus);
    AcquisitionNoise_setFrequency(instance, self->frequency);
    AcquisitionNoise_setCalcCycle(instance, self->calcCycle);
    if (self->calcCycle < AcquisitionNoise_noiseDeque_size(instance)) {
        size_t n = AcquisitionNoise_noiseDeque_size(instance) - self->calcCycle;
        for (size_t i = 0; i < n; ++i) {
            AcquisitionNoise_noiseDeque_pop_back(instance);
        }
    }
    AcquisitionNoise_schema_assign(instance, self->schema, self->schema_count);
    AcquisitionNoise_file_update(instance);
}

void CVolumeAdaptSchemaData_do_data(CVolumeAdaptSchemaData *self) {
    AcquisitionNoise *instance = AcquisitionNoise_getInstance();
    self->monitorStatus = AcquisitionNoise_getMonitorStatus(instance);
    self->frequency = AcquisitionNoise_getFrequency(instance);
    self->calcCycle = AcquisitionNoise_getCalcCycle(instance);
    AcquisitionNoise_schema_get(instance, &self->schema, &self->schema_count);
}