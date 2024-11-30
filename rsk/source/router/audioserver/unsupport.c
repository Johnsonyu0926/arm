#include "unsupport.h"
#include "json.hpp"
#include "CSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CUnsupport_init(CUnsupport *self) {
    memset(self->cmd, 0, sizeof(self->cmd));
    self->resultId = 0;
    memset(self->msg, 0, sizeof(self->msg));
}

void CUnsupport_do_success(CUnsupport *self, const char *reqcmd) {
    strncpy(self->cmd, reqcmd, sizeof(self->cmd) - 1);
    self->resultId = 2;
    strcpy(self->msg, "unsupport operation");
}

void CUnsupportBusiness_init(CUnsupportBusiness *self) {
    memset(self->cmd, 0, sizeof(self->cmd));
}

void CUnsupportBusiness_setCmd(CUnsupportBusiness *self, const char *unsupport_cmd) {
    strncpy(self->cmd, unsupport_cmd, sizeof(self->cmd) - 1);
}

int CUnsupportBusiness_do_req(CUnsupportBusiness *self, CSocket *pClient) {
    printf("unsupport cmd: %s\n", self->cmd);
    CUnsupport res;
    CUnsupport_init(&res);
    CUnsupport_do_success(&res, self->cmd);
    json j = json_parse((const char *)&res);
    char *s = json_dump(&j);
    int result = CSocket_Send(pClient, s, strlen(s));
    free(s);
    return result;
}