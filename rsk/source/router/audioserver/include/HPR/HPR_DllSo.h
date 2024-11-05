#ifndef __HPR_DLLSO_H__
#define __HPR_DLLSO_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

HPR_DECLARE HPR_HANDLE CALLBACK HPR_LoadDSo(const char* pPath);

#if defined(OS_POSIX)
HPR_DECLARE HPR_HANDLE CALLBACK HPR_LoadDSoEx(const char* pPath, HPR_INT32 iMode);
#endif

HPR_DECLARE HPR_INT32 CALLBACK HPR_UnloadDSo(HPR_HANDLE hModule);
HPR_DECLARE HPR_PROC CALLBACK HPR_GetDsoSym(HPR_HANDLE hModule, const char* pName);

#define HPR_MAX_DSO_PATH (260)

namespace hpr {

class HPR_DECLARE_CLASS hpr_dso {
public:
    hpr_dso();
    hpr_dso(const char* path);
    hpr_dso(hpr_handle module);
    hpr_dso(const hpr_dso&);
    virtual ~hpr_dso();

public:
    hpr_dso& operator=(const hpr_dso&);

    hpr_handle load(const char* path);
    hpr_int32 unload();
    hpr_handle get_symbol(const char* name);

private:
    hpr_int32 format_path(char* path);

private:
    hpr_handle m_hModule;
    char m_szPath[HPR_MAX_DSO_PATH];
};

}

#endif // __HPR_DLLSO_H__
