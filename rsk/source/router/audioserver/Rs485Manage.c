#include "rs485_manage.h"

// 创建Rs485ManageResult对象
Rs485ManageResult* create_rs485_manage_result(void) {
    Rs485ManageResult* result = (Rs485ManageResult*)malloc(sizeof(Rs485ManageResult));
    if (!result) return NULL;
    result->cmd = strdup("Rs485Manage");
    result->resultId = 0;
    result->msg = NULL;
    return result;
}

// 销毁Rs485ManageResult对象
void destroy_rs485_manage_result(Rs485ManageResult* result) {
    if (result) {
        free(result->cmd);
        free(result->msg);
        free(result);
    }
}

// 设置Rs485ManageResult失败状态
void rs485_manage_result_do_fail(Rs485ManageResult* result, const char* str) {
    result->resultId = 2;
    result->msg = strdup(str);
}

// 设置Rs485ManageResult成功状态
void rs485_manage_result_do_success(Rs485ManageResult* result) {
    result->resultId = 1;
    result->msg = strdup("Rs485Manage success");
}

// 创建Rs485Manage对象
Rs485Manage* create_rs485_manage(void) {
    Rs485Manage* rs485Manage = (Rs485Manage*)malloc(sizeof(Rs485Manage));
    if (!rs485Manage) return NULL;
    rs485Manage->cmd = NULL;
    rs485Manage->rs485Config = NULL;
    return rs485Manage;
}

// 销毁Rs485Manage对象
void destroy_rs485_manage(Rs485Manage* rs485Manage) {
    if (rs485Manage) {
        free(rs485Manage->cmd);
        free(rs485Manage->rs485Config);
        free(rs485Manage);
    }
}

// 处理Rs485Manage请求
int rs485_manage_do_req(const Rs485Manage* rs485Manage, CSocket* pClient) {
    Rs485ManageResult* res = create_rs485_manage_result();

    if (!rs485Manage->rs485Config || strlen(rs485Manage->rs485Config) == 0) {
        rs485_manage_result_do_fail(res, "rs485Config is empty");
    } else {
        char* str = utils_hex_to_string(rs485Manage->rs485Config);
        if (rs485_uart_work(str, strlen(str)) != 1) {
            rs485_manage_result_do_fail(res, "rs485Config write fail");
        } else {
            rs485_manage_result_do_success(res);
        }
        free(str);
    }

    json_t* json = json_pack("{s:s, s:i, s:s}", 
                             "cmd", res->cmd, 
                             "resultId", res->resultId, 
                             "msg", res->msg);
    char* s = json_dumps(json, 0);

    int sendResult = csocket_send(pClient, s, strlen(s));

    free(s);
    json_decref(json);
    destroy_rs485_manage_result(res);

    return sendResult;
}