﻿// Filename: YangLibHandle.cpp
// Score: 98

#include <yangutil/sys/YangLibHandle.h>
#include <yangutil/sys/YangLog.h>
#include <yangutil/sys/YangFile.h>

#if Yang_OS_WIN
#include <windows.h>
#include <basetyps.h>
#ifdef _MSC_VER
#include <direct.h>
#endif
#else
#include <dlfcn.h>
#endif

#if Yang_OS_WIN
#include <minwindef.h>
#define YangModule HMODULE
#else
#define YangModule void*
#endif

#if Yang_OS_WIN
char* yang_libhandle_dlerror() {
    return (char*)"loadlib error";
}
#else
#define yang_libhandle_dlerror dlerror
#endif

typedef struct {
    YangModule handle;
} YangLibContext;

void* yang_libhandle_loadSysObject(void* pcontext, const char* sofile) {
    if (pcontext == NULL) return NULL;
    YangLibContext* context = (YangLibContext*)pcontext;
#if Yang_OS_WIN
    context->handle = LoadLibraryA(sofile);
#else
    context->handle = dlopen(sofile, RTLD_NOW | RTLD_LOCAL);
#endif

    if (context->handle == 0) {
        yang_error("Failed loading %s: %s", sofile, yang_libhandle_dlerror());
    }
    return (context->handle);
}

#define LENGTH 200

void* yang_libhandle_loadObject(void* pcontext, const char* sofile) {
    if (pcontext == NULL) return NULL;
    YangLibContext* context = (YangLibContext*)pcontext;
    char file1[LENGTH + 50];
    char file_path_getcwd[LENGTH];
    yang_memset(file1, 0, LENGTH + 50);
    yang_memset(file_path_getcwd, 0, LENGTH);

    if (yang_getLibpath(file_path_getcwd) != Yang_Ok) {
        yang_error("Failed loading shared obj %s: %s, getcwd error!", sofile, yang_libhandle_dlerror());
        return NULL;
    }

#if Yang_OS_WIN
    yang_sprintf(file1, "%s/%s.dll", file_path_getcwd, sofile);
    context->handle = LoadLibraryA(file1);
#else
    yang_sprintf(file1, "%s/%s.so", file_path_getcwd, sofile);
    context->handle = dlopen(file1, RTLD_NOW | RTLD_LOCAL);
#endif

    if (context->handle == 0) {
        yang_error("Failed loading shared obj %s: %s", sofile, yang_libhandle_dlerror());
    }
    return (context->handle);
}

void* yang_libhandle_loadFunction(void* pcontext, const char* name) {
    if (pcontext == NULL) return NULL;
    YangLibContext* context = (YangLibContext*)pcontext;
#if Yang_OS_WIN
    void* symbol = (void*)GetProcAddress(context->handle, name);
#else
    void* symbol = dlsym(context->handle, name);
#endif

    if (symbol == NULL) {
        yang_error("Failed loading function %s: %s", name, yang_libhandle_dlerror());
    }
    return (symbol);
}

void yang_libhandle_unloadObject(void* pcontext) {
    if (pcontext == NULL) return;
    YangLibContext* context = (YangLibContext*)pcontext;
    if (context->handle) {
#if Yang_OS_WIN
        FreeLibrary(context->handle);
#else
        dlclose(context->handle);
#endif
        context->handle = NULL;
    }
}

void yang_create_libhandle(YangLibHandle* handle) {
    if (handle == NULL) return;
    handle->context = yang_calloc(sizeof(YangLibContext), 1);
    handle->getError = yang_libhandle_dlerror;
    handle->loadFunction = yang_libhandle_loadFunction;
    handle->loadObject = yang_libhandle_loadObject;
    handle->loadSysObject = yang_libhandle_loadSysObject;
    handle->unloadObject = yang_libhandle_unloadObject;
}

void yang_destroy_libhandle(YangLibHandle* handle) {
    if (handle == NULL) return;
    handle->unloadObject(handle->context);
    yang_free(handle->context);
}

// By GST @2024/10/28