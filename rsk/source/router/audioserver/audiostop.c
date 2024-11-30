#include "audiostop.h"

// 初始化音频停止结果
void init_audio_stop_result(CAudioStopResult* result) {
    result->cmd = NULL;
    result->resultId = 0;
    result->msg = NULL;
}

// 释放音频停止结果
void free_audio_stop_result(CAudioStopResult* result) {
    free(result->cmd);
    free(result->msg);
}

// 设置成功结果
void do_success(CAudioStopResult* result) {
    result->cmd = strdup("AudioStop");
    result->resultId = 1;
    result->msg = strdup("stop play success");
}

// 初始化音频停止
void init_audio_stop(CAudioStop* stop) {
    stop->cmd = NULL;
}

// 释放音频停止
void free_audio_stop(CAudioStop* stop) {
    free(stop->cmd);
}

// 发送响应
static int send_response(CSocket* pClient, const CAudioStopResult* audioStopResult) {
    json_t* js = json_object();
    json_object_set_new(js, "cmd", json_string(audioStopResult->cmd));
    json_object_set_new(js, "resultId", json_integer(audioStopResult->resultId));
    json_object_set_new(js, "msg", json_string(audioStopResult->msg));
    char* str = json_dumps(js, JSON_INDENT(4));
    int result = send_socket(pClient, str, strlen(str));
    free(str);
    json_decref(js);
    return result;
}

// 处理请求
int do_req(const CAudioStop* stop, CSocket* pClient) {
    audio_stop();
    CAudioStopResult audioStopResult;
    init_audio_stop_result(&audioStopResult);
    do_success(&audioStopResult);
    int result = send_response(pClient, &audioStopResult);
    free_audio_stop_result(&audioStopResult);
    return result;
}