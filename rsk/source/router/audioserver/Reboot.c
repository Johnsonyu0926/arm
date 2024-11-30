#include "reboot.h"

// 创建RebootResult对象
RebootResult* create_reboot_result(void) {
    RebootResult* result = (RebootResult*)malloc(sizeof(RebootResult));
    if (!result) return NULL;
    result->cmd = strdup("Reboot");
    result->resultId = 0;
    result->msg = NULL;
    return result;
}

// 销毁RebootResult对象
void destroy_reboot_result(RebootResult* result) {
    if (result) {
        free(result->cmd);
        free(result->msg);
        free(result);
    }
}

// 设置RebootResult失败状态
void reboot_result_do_fail(RebootResult* result, const char* str) {
    result->resultId = 2;
    result->msg = strdup(str);
}

// 设置RebootResult成功状态
void reboot_result_do_success(RebootResult* result) {
    result->resultId = 1;
    result->msg = strdup("Reboot success");
}

// 创建Reboot对象
Reboot* create_reboot(void) {
    Reboot* reboot = (Reboot*)malloc(sizeof(Reboot));
    if (!reboot) return NULL;
    reboot->cmd = NULL;
    return reboot;
}

// 销毁Reboot对象
void destroy_reboot(Reboot* reboot) {
    if (reboot) {
        free(reboot->cmd);
        free(reboot);
    }
}

// 处理Reboot请求
int reboot_do_req(const Reboot* reboot, CSocket* pClient) {
    RebootResult* res = create_reboot_result();
    reboot_result_do_success(res);

    json_t* json = json_pack("{s:s, s:i, s:s}", 
                             "cmd", res->cmd, 
                             "resultId", res->resultId, 
                             "msg", res->msg);
    char* s = json_dumps(json, 0);

    int sendResult = csocket_send(pClient, s, strlen(s));

    if (sendResult > 0) {
        exec("reboot");
    }

    free(s);
    json_decref(json);
    destroy_reboot_result(res);

    return sendResult;
}