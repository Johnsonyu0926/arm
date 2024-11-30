#ifndef UPDATEPWD_H
#define UPDATEPWD_H

#include "json.hpp"
#include "audiocfg.h"
#include "utils.h"
#include "CSocket.h"

typedef struct {
    char cmd[32];
    int resultId;
    char msg[256];
} CUpdateResult;

typedef struct {
    char cmd[32];
    char password[64];
    char oldPassword[64];
} CUpdate;

void CUpdateResult_init(CUpdateResult *self);
void CUpdateResult_do_success(CUpdateResult *self);
void CUpdateResult_do_fail(CUpdateResult *self);
void CUpdate_init(CUpdate *self);
int CUpdate_do_req(CUpdate *self, CSocket *pClient);

#endif // UPDATEPWD_H