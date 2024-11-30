#include "server_set.h"

// 创建ServerSetResult对象
ServerSetResult* create_server_set_result(void) {
    ServerSetResult* result = (ServerSetResult*)malloc(sizeof(ServerSetResult));
    if (!result) return NULL;
    result->cmd = strdup("ServerSet");
    result->resultId = 0;
    result->msg = NULL;
    return result;
}

// 销毁ServerSetResult对象
void destroy_server_set_result(ServerSetResult* result) {
    if (result) {
        free(result->cmd);
        free(result->msg);
        free(result);
    }
}

// 设置ServerSetResult失败状态
void server_set_result_do_fail(ServerSetResult* result, const char* str) {
    result->resultId = 2;
    result->msg = strdup(str);
}

// 设置ServerSetResult成功状态
void server_set_result_do_success(ServerSetResult* result) {
    result->resultId = 1;
    result->msg = strdup("ServerSet success");
}

// 创建ServerSet对象
ServerSet* create_server_set(void) {
    ServerSet* serverSet = (ServerSet*)malloc(sizeof(ServerSet));
    if (!serverSet) return NULL;
    serverSet->cmd = NULL;
    serverSet->serverConfig = NULL;
    return serverSet;
}

// 销毁ServerSet对象
void destroy_server_set(ServerSet* serverSet) {
    if (serverSet) {
        free(serverSet->cmd);
        free(serverSet->serverConfig);
        free(serverSet);
    }
}

// 处理ServerSet请求
int server_set_do_req(const ServerSet* serverSet, CSocket* pClient) {
    ServerSetResult* res = create_server_set_result();

    if (!serverSet->serverConfig || strlen(serverSet->serverConfig) == 0) {
        server_set_result_do_fail(res, "serverConfig is empty");
    } else {
        char* str = utils_hex_to_string(serverSet->serverConfig);
        if (rs485_uart_work(str, strlen(str)) != 1) {
            server_set_result_do_fail(res, "serverConfig write fail");
        } else {
            server_set_result_do_success(res);
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
    destroy_server_set_result(res);

    return sendResult;
}