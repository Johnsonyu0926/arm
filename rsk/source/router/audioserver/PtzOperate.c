#include "ptzoperate.h"

// 创建PtzOperateResult对象
PtzOperateResult* create_ptz_operate_result(void) {
    PtzOperateResult* result = (PtzOperateResult*)malloc(sizeof(PtzOperateResult));
    if (!result) return NULL;
    result->cmd = strdup("PtzOperate");
    result->resultId = 0;
    result->msg = NULL;
    return result;
}

// 销毁PtzOperateResult对象
void destroy_ptz_operate_result(PtzOperateResult* result) {
    if (result) {
        free(result->cmd);
        free(result->msg);
        free(result);
    }
}

// 设置PtzOperateResult失败状态
void ptz_operate_result_do_fail(PtzOperateResult* result, const char* str) {
    result->resultId = 2;
    result->msg = strdup(str);
}

// 设置PtzOperateResult成功状态
void ptz_operate_result_do_success(PtzOperateResult* result) {
    result->resultId = 1;
    result->msg = strdup("PtzOperate success");
}

// 创建PtzOperate对象
PtzOperate* create_ptz_operate(void) {
    PtzOperate* ptzOperate = (PtzOperate*)malloc(sizeof(PtzOperate));
    if (!ptzOperate) return NULL;
    ptzOperate->cmd = NULL;
    ptzOperate->operateCmd = NULL;
    return ptzOperate;
}

// 销毁PtzOperate对象
void destroy_ptz_operate(PtzOperate* ptzOperate) {
    if (ptzOperate) {
        free(ptzOperate->cmd);
        free(ptzOperate->operateCmd);
        free(ptzOperate);
    }
}

// 处理PtzOperate请求
const char* ptz_operate_do_req(const PtzOperate* ptzOperate, CSocket* pClient) {
    PtzOperateResult* res = create_ptz_operate_result();

    if (!ptzOperate->operateCmd || strlen(ptzOperate->operateCmd) == 0) {
        ptz_operate_result_do_fail(res, "operateCmd is empty");
    } else {
        char* str = utils_hex_to_string(ptzOperate->operateCmd);
        if (rs485_uart_work(str, strlen(str)) != 1) {
            ptz_operate_result_do_fail(res, "operateCmd write fail");
        } else {
            ptz_operate_result_do_success(res);
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
    destroy_ptz_operate_result(res);

    return sendResult > 0 ? "Success" : "Fail";
}