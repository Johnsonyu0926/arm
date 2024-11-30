#include "networkset.h"

// 创建NetworkSetResult对象
NetworkSetResult* create_network_set_result(void) {
    NetworkSetResult* result = (NetworkSetResult*)malloc(sizeof(NetworkSetResult));
    if (!result) return NULL;
    result->cmd = strdup("NetworkSet");
    result->resultId = 1;
    result->msg = strdup("NetworkSet handle success");
    return result;
}

// 销毁NetworkSetResult对象
void destroy_network_set_result(NetworkSetResult* result) {
    if (result) {
        free(result->cmd);
        free(result->msg);
        free(result);
    }
}

// 创建NetworkSet对象
NetworkSet* create_network_set(void) {
    NetworkSet* networkSet = (NetworkSet*)malloc(sizeof(NetworkSet));
    if (!networkSet) return NULL;
    networkSet->cmd = NULL;
    networkSet->ipAddress = NULL;
    networkSet->gateway = NULL;
    networkSet->netMask = NULL;
    return networkSet;
}

// 销毁NetworkSet对象
void destroy_network_set(NetworkSet* networkSet) {
    if (networkSet) {
        free(networkSet->cmd);
        free(networkSet->ipAddress);
        free(networkSet->gateway);
        free(networkSet->netMask);
        free(networkSet);
    }
}

// 设置NetworkSetResult成功状态
void network_set_result_do_success(NetworkSetResult* result) {
    result->cmd = strdup("NetworkSet");
    result->resultId = 1;
    result->msg = strdup("NetworkSet handle success");
}

// 处理NetworkSet请求
int network_set_do_req(const NetworkSet* networkSet, CSocket* pClient) {
    CUtils utils;
    NetworkSetResult* res = create_network_set_result();
    network_set_result_do_success(res);

    json_t* json = json_pack("{s:s, s:i, s:s}", 
                             "cmd", res->cmd, 
                             "resultId", res->resultId, 
                             "msg", res->msg);
    char* s = json_dumps(json, 0);

    int sendResult = csocket_send(pClient, s, strlen(s));

    if (sendResult > 0) {
        utils_network_set(networkSet->gateway, networkSet->ipAddress, networkSet->netMask);
    }

    free(s);
    json_decref(json);
    destroy_network_set_result(res);

    return sendResult;
}