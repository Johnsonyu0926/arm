#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

json json_parse(const char *str) {
    json j;
    j.size = strlen(str);
    j.data = (char *)malloc(j.size + 1);
    if (j.data != NULL) {
        strcpy(j.data, str);
    }
    return j;
}

char *json_dump(const json *j) {
    char *str = (char *)malloc(j->size + 1);
    if (str != NULL) {
        strcpy(str, j->data);
    }
    return str;
}
json json_parse(const char *str) {
    json j;
    j.size = strlen(str);
    j.data = (char *)malloc(j.size + 1);
    strcpy(j.data, str);
    return j;
}

char *json_dump(const json *j) {
    char *str = (char *)malloc(j->size + 1);
    strcpy(str, j->data);
    return str;
}
json json_parse(const char *str) {
    json j;
    j.size = strlen(str);
    j.data = (char *)malloc(j.size + 1);
    strcpy(j.data, str);
    return j;
}

char *json_dump(const json *j) {
    char *str = (char *)malloc(j->size + 1);
    strcpy(str, j->data);
    return str;
}