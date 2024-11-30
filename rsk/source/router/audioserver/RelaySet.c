#include "relay_set.h"

// 创建RelaySetResult对象
RelaySetResult* create_relay_set_result(void) {
    RelaySetResult* result = (RelaySetResult*)malloc(sizeof(RelaySetResult));
    if (!result) return NULL;
    result->cmd = strdup("RelaySet");
    result->resultId = 0;
    result->msg = NULL;
    return result;
}

// 销毁RelaySetResult对象
void destroy_relay_set_result(RelaySetResult* result) {
    if (result) {
        free(result->cmd);
        free(result->msg);
        free(result);
    }
}

// 设置RelaySetResult失败状态
void relay_set_result_do_fail(RelaySetResult* result, const char* str) {
    result->resultId = 2;
    result->msg = strdup(str);
}

// 设置RelaySetResult成功状态
void relay_set_result_do_success(RelaySetResult* result) {
    result->resultId = 1;
    result->msg = strdup("RelaySet success");
}

// 创建RelaySet对象
RelaySet* create_relay_set(void) {
    RelaySet* relaySet = (RelaySet*)malloc(sizeof(RelaySet));
    if (!relaySet) return NULL;
    relaySet->cmd = NULL;
    relaySet->relayState = NULL;
    return relaySet;
}

// 销毁RelaySet对象
void destroy_relay_set(RelaySet* relaySet) {
    if (relaySet) {
        free(relaySet->cmd);
        free(relaySet->relayState);
        free(relaySet);
    }
}

// 处理RelaySet请求
int relay_set_do_req(const RelaySet* relaySet, CSocket* pClient) {
    RelaySetResult* res = create_relay_set_result();

    if (!relaySet->relayState || strlen(relaySet->relayState) == 0) {
        relay_set_result_do_fail(res, "relayState is empty");
    } else {
        char* str = utils_hex_to_string(relaySet->relayState);
        if (rs485_uart_work(str, strlen(str)) != 1) {
            relay_set_result_do_fail(res, "relayState write fail");
        } else {
            relay_set_result_do_success(res);
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
    destroy_relay_set_result(res);

    return sendResult;
}