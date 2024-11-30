#ifndef __CUNSUPPORT_H__
#define __CUNSUPPORT_H__

#include "json.hpp"
#include "CSocket.h"

typedef struct {
    char cmd[32];
    int resultId;
    char msg[256];
} CUnsupport;

typedef struct {
    char cmd[32];
} CUnsupportBusiness;

void CUnsupport_init(CUnsupport *self);
void CUnsupport_do_success(CUnsupport *self, const char *reqcmd);
void CUnsupportBusiness_init(CUnsupportBusiness *self);
void CUnsupportBusiness_setCmd(CUnsupportBusiness *self, const char *unsupport_cmd);
int CUnsupportBusiness_do_req(CUnsupportBusiness *self, CSocket *pClient);

#endif // __CUNSUPPORT_H__