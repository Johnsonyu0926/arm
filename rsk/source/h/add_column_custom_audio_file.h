#ifndef ADD_COLUMN_CUSTOM_AUDIO_FILE_H
#define ADD_COLUMN_CUSTOM_AUDIO_FILE_H

#include <stdbool.h>

typedef struct {
    int type;
    char name[256];
    int size;
} CAddColumnCustomAudioFileData;

typedef struct {
    CAddColumnCustomAudioFileData *business;
    int business_count;
} CAddColumnCustomAudioFileBusiness;

void CAddColumnCustomAudioFileData_setName(CAddColumnCustomAudioFileData *self, const char *name);
bool CAddColumnCustomAudioFileBusiness_exist(CAddColumnCustomAudioFileBusiness *self, const char *name);
void CAddColumnCustomAudioFileBusiness_push_back(CAddColumnCustomAudioFileBusiness *self, CAddColumnCustomAudioFileData *data);
void CAddColumnCustomAudioFileBusiness_saveJson(CAddColumnCustomAudioFileBusiness *self);
void CAddColumnCustomAudioFileBusiness_updateSize(CAddColumnCustomAudioFileBusiness *self, const char *name, int size);

#endif // ADD_COLUMN_CUSTOM_AUDIO_FILE_H