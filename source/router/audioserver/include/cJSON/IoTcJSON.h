/*
  Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef IoTcJSON__h
#define IoTcJSON__h

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#define __WINDOWS__
#endif

#ifdef __WINDOWS__

/* When compiling for windows, we specify a specific calling convention to avoid issues where we are being called from a project with a different default calling convention.  For windows you have 3 define options:

CJSON_HIDE_SYMBOLS - Define this in the case where you don't want to ever dllexport symbols
CJSON_EXPORT_SYMBOLS - Define this on library build when you want to dllexport symbols (default)
CJSON_IMPORT_SYMBOLS - Define this if you want to dllimport symbol

For *nix builds that support visibility attribute, you can define similar behavior by

setting default visibility to hidden by adding
-fvisibility=hidden (for gcc)
or
-xldscope=hidden (for sun cc)
to CFLAGS

then using the CJSON_API_VISIBILITY flag to "export" the same symbols the way CJSON_EXPORT_SYMBOLS does

*/

#define IoTCJSON_CDECL __cdecl
#define IoTCJSON_STDCALL __stdcall

/* export symbols by default, this is necessary for copy pasting the C and header file */
#if !defined(IoTCJSON_HIDE_SYMBOLS) && !defined(IoTCJSON_IMPORT_SYMBOLS) && !defined(IoTCJSON_EXPORT_SYMBOLS)
#define IoTCJSON_EXPORT_SYMBOLS
#endif

#if defined(IoTCJSON_HIDE_SYMBOLS)
#define IoTCJSON_PUBLIC(type)   type IoTCJSON_STDCALL
#elif defined(IoTCJSON_EXPORT_SYMBOLS)
#define IoTCJSON_PUBLIC(type)   __declspec(dllexport) type IoTCJSON_STDCALL
#elif defined(IoTCJSON_IMPORT_SYMBOLS)
#define IoTCJSON_PUBLIC(type)   __declspec(dllimport) type IoTCJSON_STDCALL
#endif
#else /* !__WINDOWS__ */
#define IoTCJSON_CDECL
#define IoTCJSON_STDCALL

#if (defined(__GNUC__) || defined(__SUNPRO_CC) || defined (__SUNPRO_C)) && defined(CJSON_API_VISIBILITY)
#define IoTCJSON_PUBLIC(type)   __attribute__((visibility("default"))) type
#else
#define IoTCJSON_PUBLIC(type) type
#endif
#endif

/* project version */
#define IoTCJSON_VERSION_MAJOR 1
#define IoTCJSON_VERSION_MINOR 7
#define IoTCJSON_VERSION_PATCH 14

#include <stddef.h>

/* cJSON Types: */
#define IoTcJSON_Invalid (0)
#define IoTcJSON_False  (1 << 0)
#define IoTcJSON_True   (1 << 1)
#define IoTcJSON_NULL   (1 << 2)
#define IoTcJSON_Number (1 << 3)
#define IoTcJSON_String (1 << 4)
#define IoTcJSON_Array  (1 << 5)
#define IoTcJSON_Object (1 << 6)
#define IoTcJSON_Raw    (1 << 7) /* raw json */

#define IoTcJSON_IsReference 256
#define IoTcJSON_StringIsConst 512

/* The cJSON structure: */
typedef struct IoTcJSON
{
    /* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
    struct IoTcJSON *next;
    struct IoTcJSON *prev;
    /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */
    struct IoTcJSON *child;

    /* The type of the item, as above. */
    int type;

    /* The item's string, if type==cJSON_String  and type == cJSON_Raw */
    char *valuestring;
    /* writing to valueint is DEPRECATED, use cJSON_SetNumberValue instead */
    int valueint;
    /* The item's number, if type==cJSON_Number */
    double valuedouble;

    /* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
    char *string;
} IoTcJSON;

typedef struct IoTcJSON_Hooks
{
      /* malloc/free are CDECL on Windows regardless of the default calling convention of the compiler, so ensure the hooks allow passing those functions directly. */
      void *(IoTCJSON_CDECL *malloc_fn)(size_t sz);
      void (IoTCJSON_CDECL *free_fn)(void *ptr);
} IoTcJSON_Hooks;

typedef int IoTcJSON_bool;

/* Limits how deeply nested arrays/objects can be before cJSON rejects to parse them.
 * This is to prevent stack overflows. */
#ifndef IoTIoTCJSON_NESTING_LIMIT
#define IoTIoTCJSON_NESTING_LIMIT 1000
#endif

/* returns the version of cJSON as a string */
IoTCJSON_PUBLIC(const char*) IoTcJSON_Version(void);

/* Supply malloc, realloc and free functions to cJSON */
IoTCJSON_PUBLIC(void) IoTcJSON_InitHooks(IoTcJSON_Hooks* hooks);

/* Memory Management: the caller is always responsible to free the results from all variants of cJSON_Parse (with cJSON_Delete) and cJSON_Print (with stdlib free, cJSON_Hooks.free_fn, or cJSON_free as appropriate). The exception is cJSON_PrintPreallocated, where the caller has full responsibility of the buffer. */
/* Supply a block of JSON, and this returns a cJSON object you can interrogate. */
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_Parse(const char *value);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_ParseWithLength(const char *value, size_t buffer_length);
/* ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve the pointer to the final byte parsed. */
/* If you supply a ptr in return_parse_end and parsing fails, then return_parse_end will contain a pointer to the error so will match cJSON_GetErrorPtr(). */
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_ParseWithOpts(const char *value, const char **return_parse_end, IoTcJSON_bool require_null_terminated);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_ParseWithLengthOpts(const char *value, size_t buffer_length, const char **return_parse_end, IoTcJSON_bool require_null_terminated);

/* Render a cJSON entity to text for transfer/storage. */
IoTCJSON_PUBLIC(char *) IoTcJSON_Print(const IoTcJSON *item);
/* Render a cJSON entity to text for transfer/storage without any formatting. */
IoTCJSON_PUBLIC(char *) IoTcJSON_PrintUnformatted(const IoTcJSON *item);
/* Render a cJSON entity to text using a buffered strategy. prebuffer is a guess at the final size. guessing well reduces reallocation. fmt=0 gives unformatted, =1 gives formatted */
IoTCJSON_PUBLIC(char *) IoTcJSON_PrintBuffered(const IoTcJSON *item, int prebuffer, IoTcJSON_bool fmt);
/* Render a cJSON entity to text using a buffer already allocated in memory with given length. Returns 1 on success and 0 on failure. */
/* NOTE: cJSON is not always 100% accurate in estimating how much memory it will use, so to be safe allocate 5 bytes more than you actually need */
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_PrintPreallocated(IoTcJSON *item, char *buffer, const int length, const IoTcJSON_bool format);
/* Delete a cJSON entity and all subentities. */
IoTCJSON_PUBLIC(void) IoTcJSON_Delete(IoTcJSON *item);

/* Returns the number of items in an array (or object). */
IoTCJSON_PUBLIC(int) IoTcJSON_GetArraySize(const IoTcJSON *array);
/* Retrieve item number "index" from array "array". Returns NULL if unsuccessful. */
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_GetArrayItem(const IoTcJSON *array, int index);
/* Get item "string" from object. Case insensitive. */
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_GetObjectItem(const IoTcJSON * const object, const char * const string);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_GetObjectItemCaseSensitive(const IoTcJSON * const object, const char * const string);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_HasObjectItem(const IoTcJSON *object, const char *string);
/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when cJSON_Parse() returns 0. 0 when cJSON_Parse() succeeds. */
IoTCJSON_PUBLIC(const char *) IoTcJSON_GetErrorPtr(void);

/* Check item type and return its value */
IoTCJSON_PUBLIC(char *) IoTcJSON_GetStringValue(const IoTcJSON * const item);
IoTCJSON_PUBLIC(double) IoTcJSON_GetNumberValue(const IoTcJSON * const item);

/* These functions check the type of an item */
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_IsInvalid(const IoTcJSON * const item);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_IsFalse(const IoTcJSON * const item);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_IsTrue(const IoTcJSON * const item);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_IsBool(const IoTcJSON * const item);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_IsNull(const IoTcJSON * const item);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_IsNumber(const IoTcJSON * const item);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_IsString(const IoTcJSON * const item);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_IsArray(const IoTcJSON * const item);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_IsObject(const IoTcJSON * const item);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_IsRaw(const IoTcJSON * const item);

/* These calls create a cJSON item of the appropriate type. */
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateNull(void);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateTrue(void);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateFalse(void);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateBool(IoTcJSON_bool boolean);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateNumber(double num);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateString(const char *string);
/* raw json */
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateRaw(const char *raw);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateArray(void);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateObject(void);

/* Create a string where valuestring references a string so
 * it will not be freed by cJSON_Delete */
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateStringReference(const char *string);
/* Create an object/array that only references it's elements so
 * they will not be freed by cJSON_Delete */
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateObjectReference(const IoTcJSON *child);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateArrayReference(const IoTcJSON *child);

/* These utilities create an Array of count items.
 * The parameter count cannot be greater than the number of elements in the number array, otherwise array access will be out of bounds.*/
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateIntArray(const int *numbers, int count);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateFloatArray(const float *numbers, int count);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateDoubleArray(const double *numbers, int count);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_CreateStringArray(const char *const *strings, int count);

/* Append item to the specified array/object. */
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_AddItemToArray(IoTcJSON *array, IoTcJSON *item);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_AddItemToObject(IoTcJSON *object, const char *string, IoTcJSON *item);
/* Use this when string is definitely const (i.e. a literal, or as good as), and will definitely survive the cJSON object.
 * WARNING: When this function was used, make sure to always check that (item->type & cJSON_StringIsConst) is zero before
 * writing to `item->string` */
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_AddItemToObjectCS(IoTcJSON *object, const char *string, IoTcJSON *item);
/* Append reference to item to the specified array/object. Use this when you want to add an existing cJSON to a new cJSON, but don't want to corrupt your existing cJSON. */
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_AddItemReferenceToArray(IoTcJSON *array, IoTcJSON *item);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_AddItemReferenceToObject(IoTcJSON *object, const char *string, IoTcJSON *item);

/* Remove/Detach items from Arrays/Objects. */
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_DetachItemViaPointer(IoTcJSON *parent, IoTcJSON * const item);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_DetachItemFromArray(IoTcJSON *array, int which);
IoTCJSON_PUBLIC(void) IoTcJSON_DeleteItemFromArray(IoTcJSON *array, int which);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_DetachItemFromObject(IoTcJSON *object, const char *string);
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_DetachItemFromObjectCaseSensitive(IoTcJSON *object, const char *string);
IoTCJSON_PUBLIC(void) IoTcJSON_DeleteItemFromObject(IoTcJSON *object, const char *string);
IoTCJSON_PUBLIC(void) IoTcJSON_DeleteItemFromObjectCaseSensitive(IoTcJSON *object, const char *string);

/* Update array items. */
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_InsertItemInArray(IoTcJSON *array, int which, IoTcJSON *newitem); /* Shifts pre-existing items to the right. */
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_ReplaceItemViaPointer(IoTcJSON * const parent, IoTcJSON * const item, IoTcJSON * replacement);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_ReplaceItemInArray(IoTcJSON *array, int which, IoTcJSON *newitem);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_ReplaceItemInObject(IoTcJSON *object,const char *string,IoTcJSON *newitem);
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_ReplaceItemInObjectCaseSensitive(IoTcJSON *object,const char *string,IoTcJSON *newitem);

/* Duplicate a cJSON item */
IoTCJSON_PUBLIC(IoTcJSON *) IoTcJSON_Duplicate(const IoTcJSON *item, IoTcJSON_bool recurse);
/* Duplicate will create a new, identical cJSON item to the one you pass, in new memory that will
 * need to be released. With recurse!=0, it will duplicate any children connected to the item.
 * The item->next and ->prev pointers are always zero on return from Duplicate. */
/* Recursively compare two cJSON items for equality. If either a or b is NULL or invalid, they will be considered unequal.
 * case_sensitive determines if object keys are treated case sensitive (1) or case insensitive (0) */
IoTCJSON_PUBLIC(IoTcJSON_bool) IoTcJSON_Compare(const IoTcJSON * const a, const IoTcJSON * const b, const IoTcJSON_bool case_sensitive);

/* Minify a strings, remove blank characters(such as ' ', '\t', '\r', '\n') from strings.
 * The input pointer json cannot point to a read-only address area, such as a string constant, 
 * but should point to a readable and writable adress area. */
IoTCJSON_PUBLIC(void) IoTcJSON_Minify(char *json);

/* Helper functions for creating and adding items to an object at the same time.
 * They return the added item or NULL on failure. */
IoTCJSON_PUBLIC(IoTcJSON*) IoTcJSON_AddNullToObject(IoTcJSON * const object, const char * const name);
IoTCJSON_PUBLIC(IoTcJSON*) IoTcJSON_AddTrueToObject(IoTcJSON * const object, const char * const name);
IoTCJSON_PUBLIC(IoTcJSON*) IoTcJSON_AddFalseToObject(IoTcJSON * const object, const char * const name);
IoTCJSON_PUBLIC(IoTcJSON*) IoTcJSON_AddBoolToObject(IoTcJSON * const object, const char * const name, const IoTcJSON_bool boolean);
IoTCJSON_PUBLIC(IoTcJSON*) IoTcJSON_AddNumberToObject(IoTcJSON * const object, const char * const name, const double number);
IoTCJSON_PUBLIC(IoTcJSON*) IoTcJSON_AddStringToObject(IoTcJSON * const object, const char * const name, const char * const string);
IoTCJSON_PUBLIC(IoTcJSON*) IoTcJSON_AddRawToObject(IoTcJSON * const object, const char * const name, const char * const raw);
IoTCJSON_PUBLIC(IoTcJSON*) IoTcJSON_AddObjectToObject(IoTcJSON * const object, const char * const name);
IoTCJSON_PUBLIC(IoTcJSON*) IoTcJSON_AddArrayToObject(IoTcJSON * const object, const char * const name);

/* When assigning an integer value, it needs to be propagated to valuedouble too. */
#define IoTcJSON_SetIntValue(object, number) ((object) ? (object)->valueint = (object)->valuedouble = (number) : (number))
/* helper for the cJSON_SetNumberValue macro */
IoTCJSON_PUBLIC(double) IoTcJSON_SetNumberHelper(IoTcJSON *object, double number);
#define IoTcJSON_SetNumberValue(object, number) ((object != NULL) ? IoTcJSON_SetNumberHelper(object, (double)number) : (number))
/* Change the valuestring of a cJSON_String object, only takes effect when type of object is cJSON_String */
IoTCJSON_PUBLIC(char*) IoTcJSON_SetValuestring(IoTcJSON *object, const char *valuestring);

/* Macro for iterating over an array or object */
#define IoTcJSON_ArrayForEach(element, array) for(element = (array != NULL) ? (array)->child : NULL; element != NULL; element = element->next)

/* malloc/free objects using the malloc/free functions that have been set with cJSON_InitHooks */
IoTCJSON_PUBLIC(void *) IoTcJSON_malloc(size_t size);
IoTCJSON_PUBLIC(void) IoTcJSON_free(void *object);

#ifdef __cplusplus
}
#endif

#endif
