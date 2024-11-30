#ifndef JSON_H
#define JSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *data;
    size_t size;
} json;

json json_parse(const char *str);
char *json_dump(const json *j);

#endif // JSON_H