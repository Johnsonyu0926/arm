#pragma once
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct IoTcJSON IoTcJSON;

typedef struct IoTcJSONHooks {
    void *(*malloc_fn)(size_t sz);
    void (*free_fn)(void *ptr);
} IoTcJSONHooks;

IoTcJSON *IoTcJSON_CreateObject(void);
IoTcJSON *IoTcJSON_CreateArray(void);
IoTcJSON *IoTcJSON_CreateBool(int b);
IoTcJSON *IoTcJSON_CreateNull(void);
IoTcJSON *IoTcJSON_CreateNumber(double num);
IoTcJSON *IoTcJSON_CreateString(const char *string);
IoTcJSON *IoTcJSON_CreateRaw(const char *raw);
IoTcJSON *IoTcJSON_CreateIntArray(const int *numbers, int count);
IoTcJSON *IoTcJSON_CreateFloatArray(const float *numbers, int count);
IoTcJSON *IoTcJSON_CreateDoubleArray(const double *numbers, int count);
IoTcJSON *IoTcJSON_CreateStringArray(const char **strings, int count);

void IoTcJSON_Delete(IoTcJSON *item);

int IoTcJSON_GetArraySize(const IoTcJSON *array);
IoTcJSON *IoTcJSON_GetArrayItem(const IoTcJSON *array, int index);
IoTcJSON *IoTcJSON_GetObjectItem(const IoTcJSON *object, const char *string);
const char *IoTcJSON_GetErrorPtr(void);

char *IoTcJSON_Print(const IoTcJSON *item);
char *IoTcJSON_PrintUnformatted(const IoTcJSON *item);
void IoTcJSON_DeleteItemFromArray(IoTcJSON *array, int which);
void IoTcJSON_DeleteItemFromObject(IoTcJSON *object, const char *string);
void IoTcJSON_InsertItemInArray(IoTcJSON *array, int which, IoTcJSON *newitem);
void IoTcJSON_ReplaceItemInArray(IoTcJSON *array, int which, IoTcJSON *newitem);
void IoTcJSON_ReplaceItemInObject(IoTcJSON *object,const char *string,IoTcJSON *newitem);

void IoTcJSON_Minify(char *json);
IoTcJSON *IoTcJSON_Parse(const char *value);
void IoTcJSON_InitHooks(IoTcJSONHooks* hooks);

#define IoTcJSON_AddNullToObject(object,name) IoTcJSON_AddItemToObject(object, name, IoTcJSON_CreateNull())
#define IoTcJSON_AddTrueToObject(object,name) IoTcJSON_AddItemToObject(object, name, IoTcJSON_CreateBool(1))
#define IoTcJSON_AddFalseToObject(object,name) IoTcJSON_AddItemToObject(object, name, IoTcJSON_CreateBool(0))
#define IoTcJSON_AddBoolToObject(object,name,b) IoTcJSON_AddItemToObject(object, name, IoTcJSON_CreateBool(b))
#define IoTcJSON_AddNumberToObject(object,name,n) IoTcJSON_AddItemToObject(object, name, IoTcJSON_CreateNumber(n))
#define IoTcJSON_AddStringToObject(object,name,s) IoTcJSON_AddItemToObject(object, name, IoTcJSON_CreateString(s))

#ifdef __cplusplus
}
#endif
