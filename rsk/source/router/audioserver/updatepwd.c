#include "updatepwd.h"
#include "json.hpp"
#include "audiocfg.h"
#include "utils.h"
#include "CSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CUpdateResult_init(CUpdateResult *self) {
    memset(self->cmd, 0, sizeof(self->cmd));
    self->resultId = 0;
    memset(self->msg, 0, sizeof(self->msg));
}

void CUpdateResult_do_success(CUpdateResult *self) {
    strcpy(self->cmd, "UpdatePwd");
    self->resultId = 1;
    strcpy(self->msg, "update password success");
}

void CUpdateResult_do_fail(CUpdateResult *self) {
    strcpy(self->cmd, "UpdatePwd");
    self->resultId = 2;
    strcpy(self->msg, "update password fail");
}

void CUpdate_init(CUpdate *self) {
    memset(self->cmd, 0, sizeof(self->cmd));
    memset(self->password, 0, sizeof(self->password));
    memset(self->oldPassword, 0, sizeof(self->oldPassword));
}

int CUpdate_do_req(CUpdate *self, CSocket *pClient) {
    CAudioCfgBusiness cfg;
    CAudioCfgBusiness_load(&cfg);
    if (strcmp(self->oldPassword, cfg.business[0].serverPassword) != 0) {
        CUpdateResult updateResult;
        CUpdateResult_init(&updateResult);
        CUpdateResult_do_fail(&updateResult);
        json resp = json_parse((const char *)&updateResult);
        char *str = json_dump(&resp);
        int result = CSocket_Send(pClient, str, strlen(str));
        free(str);
        return 0;
    } else {
        CUtils utils;
        CUtils_change_password(&utils, self->password);

        strcpy(cfg.business[0].serverPassword, self->password);
        CAudioCfgBusiness_saveToJson(&cfg);
        CUpdateResult updateResult;
        CUpdateResult_init(&updateResult);
        CUpdateResult_do_success(&updateResult);
        json resp = json_parse((const char *)&updateResult);
        char *str = json_dump(&resp);
        int result = CSocket_Send(pClient, str, strlen(str));
        free(str);
        return 1;
    }
}