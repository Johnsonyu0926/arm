#include "timerestart.h"
#include "json.hpp"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>

#define TIMED_RESTART_FILE_NAME "/mnt/cfg/timedRestart.json"

int TimedRestart_file_load(TimedRestart *self) {
    FILE *file = fopen(TIMED_RESTART_FILE_NAME, "r");
    if (!file) {
        fprintf(stderr, "%s file load error!\n", TIMED_RESTART_FILE_NAME);
        return 0;
    }

    char buffer[1024];
    fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    json js = json_parse(buffer);
    self->timeList = NULL;
    self->timeList_count = 0;

    json timeList = json_parse(js.data);
    for (int i = 0; i < timeList.size; ++i) {
        self->timeList = realloc(self->timeList, (self->timeList_count + 1) * sizeof(char *));
        self->timeList[self->timeList_count] = strdup(timeList.data + i);
        self->timeList_count++;
    }

    return 1;
}

void TimedRestart_testJson(TimedRestart *self) {
    json js;
    self->timeList = realloc(self->timeList, (self->timeList_count + 1) * sizeof(char *));
    self->timeList[self->timeList_count] = strdup("01:00");
    self->timeList_count++;

    js.data = malloc(1024);
    snprintf(js.data, 1024, "{\"rebootTask\":[\"%s\"]}", self->timeList[0]);

    FILE *file = fopen(TIMED_RESTART_FILE_NAME, "w");
    if (file) {
        fwrite(js.data, 1, strlen(js.data), file);
        fclose(file);
    }
    free(js.data);
}

const char *TimedRestart_getCurrTime() {
    static char buffer[6];
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    strftime(buffer, sizeof(buffer), "%H:%M", lt);
    return buffer;
}

void *TimedRestart_loop_thread(void *arg) {
    TimedRestart *self = (TimedRestart *)arg;
    while (true) {
        for (int i = 0; i < self->timeList_count; ++i) {
            printf("%s %s\n", self->timeList[i], TimedRestart_getCurrTime());
            if (strcmp(TimedRestart_getCurrTime(), self->timeList[i]) == 0) {
                CUtils_reboot();
            }
        }
        sleep(30);
    }
    return NULL;
}

void TimedRestart_loop(TimedRestart *self) {
    pthread_t thread;
    pthread_create(&thread, NULL, TimedRestart_loop_thread, self);
    pthread_detach(thread);
}