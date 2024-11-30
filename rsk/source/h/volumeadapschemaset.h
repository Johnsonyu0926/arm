#ifndef VOLUMEADAPSCHEMASET_H
#define VOLUMEADAPSCHEMASET_H

#include "json.h"
#include "AcquisitionNoise.h"

typedef struct {
    int monitorStatus;
    int frequency;
    int calcCycle;
    CSchema *schema;
    int schema_count;
} CVolumeAdaptSchemaData;

void CVolumeAdaptSchemaData_init(CVolumeAdaptSchemaData *self);
void CVolumeAdaptSchemaData_set_data(const CVolumeAdaptSchemaData *self);
void CVolumeAdaptSchemaData_do_data(CVolumeAdaptSchemaData *self);

#endif // VOLUMEADAPSCHEMASET_H