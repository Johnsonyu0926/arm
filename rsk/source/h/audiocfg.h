#ifndef AUDIOCFG_H
#define AUDIOCFG_H

typedef struct {
    char serverPassword[64];
} CAudioCfgData;

typedef struct {
    CAudioCfgData *business;
    int business_count;
} CAudioCfgBusiness;

void CAudioCfgBusiness_load(CAudioCfgBusiness *self);
void CAudioCfgBusiness_saveToJson(CAudioCfgBusiness *self);


typedef struct {
    char deviceID[64];
    char server[64];
    int port;
    char password[64];
} CAudioCfgData;

typedef struct {
    CAudioCfgData *business;
    int business_count;
} CAudioCfgBusiness;

void CAudioCfgBusiness_load(CAudioCfgBusiness *self);
void CAudioCfgBusiness_saveToJson(CAudioCfgBusiness *self);

#endif // AUDIOCFG_H