#include "add_column_custom_audio_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CAddColumnCustomAudioFileData_setName(CAddColumnCustomAudioFileData *self, const char *name) {
    strncpy(self->name, name, sizeof(self->name) - 1);
    self->name[sizeof(self->name) - 1] = '\0';
}

bool CAddColumnCustomAudioFileBusiness_exist(CAddColumnCustomAudioFileBusiness *self, const char *name) {
    for (int i = 0; i < self->business_count; ++i) {
        if (strcmp(self->business[i].name, name) == 0) {
            return true;
        }
    }
    return false;
}

void CAddColumnCustomAudioFileBusiness_push_back(CAddColumnCustomAudioFileBusiness *self, CAddColumnCustomAudioFileData *data) {
    self->business = realloc(self->business, (self->business_count + 1) * sizeof(CAddColumnCustomAudioFileData));
    self->business[self->business_count] = *data;
    self->business_count++;
}

void CAddColumnCustomAudioFileBusiness_saveJson(CAddColumnCustomAudioFileBusiness *self) {
    // 实现保存业务到JSON的函数
}

void CAddColumnCustomAudioFileBusiness_updateSize(CAddColumnCustomAudioFileBusiness *self, const char *name, int size) {
    for (int i = 0; i < self->business_count; ++i) {
        if (strcmp(self->business[i].name, name) == 0) {
            self->business[i].size = size;
            break;
        }
    }
}