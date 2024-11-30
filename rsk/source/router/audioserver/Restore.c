#include "restore.h"

// 创建RestoreResult对象
RestoreResult* create_restore_result(void) {
    RestoreResult* result = (RestoreResult*)malloc(sizeof(RestoreResult));
    if (!result) return NULL;
    result->cmd = strdup("Restore");
    result->resultId = 0;
    result->msg = NULL;
    return result;
}

// 销毁RestoreResult对象
void destroy_restore_result(RestoreResult* result) {
    if (result) {
        free(result->cmd);
        free(result->msg);
        free(result);
    }
}

// 设置RestoreResult失败状态
void restore_result_do_fail(RestoreResult* result, const char* str) {
    result->resultId = 2;
    result->msg = strdup(str);
}

// 设置RestoreResult成功状态
void restore_result_do_success(RestoreResult* result) {
    result->resultId = 1;
    result->msg = strdup("Restore success");
}

// 创建Restore对象
Restore* create_restore(void) {
    Restore* restore = (Restore*)malloc(sizeof(Restore));
    if (!restore) return NULL;
    restore->cmd = NULL;
    return restore;
}

// 销毁Restore对象
void destroy_restore(Restore* restore) {
    if (restore) {
        free(restore->cmd);
        free(restore);
    }
}

// 处理Restore请求
int restore_do_req(const Restore* restore, CSocket* pClient) {
    RestoreResult* res = create_restore_result();
    restore_result_do_success(res);

    json_t* json = json_pack("{s:s, s:i, s:s}", 
                             "cmd", res->cmd, 
                             "resultId", res->resultId, 
                             "msg", res->msg);
    char* s = json_dumps(json, 0);

    int sendResult = csocket_send(pClient, s, strlen(s));

    if (sendResult > 0) {
        exec("restore factory settings");
    }

    free(s);
    json_decref(json);
    destroy_restore_result(res);

    return sendResult;
}