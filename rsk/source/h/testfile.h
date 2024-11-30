#ifndef TESTFILE_H
#define TESTFILE_H

#include "json.hpp"
#include "audiocfg.h"

typedef struct {
    char ffmpegCmd[256];
} TestFile;

typedef struct {
    TestFile testFile;
    char filePath[256];
} TestFileBusiness;

void TestFile_init(TestFile *self);
void TestFileBusiness_init(TestFileBusiness *self);
void TestFileBusiness_load(TestFileBusiness *self);
const char *TestFileBusiness_getFfmpegCmd(const TestFileBusiness *self);

#endif // TESTFILE_H