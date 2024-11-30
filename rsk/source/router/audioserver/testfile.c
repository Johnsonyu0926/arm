#include "testfile.h"
#include "json.hpp"
#include "audiocfg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void TestFile_init(TestFile *self) {
    strcpy(self->ffmpegCmd, "ffplay -fflags nobuffer -nodisp -analyzeduration 1 %s &");
}

void TestFileBusiness_init(TestFileBusiness *self) {
    CAudioCfgBusiness business;
    CAudioCfgBusiness_load(&business);
    snprintf(self->filePath, sizeof(self->filePath), "%s/cfg/test.json", business.business[0].savePrefix);
    TestFileBusiness_load(self);
}

void TestFileBusiness_load(TestFileBusiness *self) {
    FILE *file = fopen(self->filePath, "r");
    if (!file) {
        printf("open test.json fail\n");
        return;
    }
    char buffer[1024];
    fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    json js = json_parse(buffer);
    TestFile *testFile = &self->testFile;
    strcpy(testFile->ffmpegCmd, js.data);
}

const char *TestFileBusiness_getFfmpegCmd(const TestFileBusiness *self) {
    return self->testFile.ffmpegCmd;
}