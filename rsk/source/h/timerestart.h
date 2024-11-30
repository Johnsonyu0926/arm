#ifndef TIMEDRESTART_H
#define TIMEDRESTART_H

#include <stdbool.h>
#include <time.h>

typedef struct {
    char **timeList;
    int timeList_count;
} TimedRestart;

int TimedRestart_file_load(TimedRestart *self);
void TimedRestart_testJson(TimedRestart *self);
const char *TimedRestart_getCurrTime();
void TimedRestart_loop(TimedRestart *self);

#endif // TIMEDRESTART_H