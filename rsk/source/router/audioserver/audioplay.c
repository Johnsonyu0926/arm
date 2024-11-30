#include "audioplay.h"

// 初始化音频播放结果
void init_audio_play_result(CAudioPlayResult* result) {
    result->cmd = NULL;
    result->resultId = 0;
    result->msg = NULL;
}

// 释放音频播放结果
void free_audio_play_result(CAudioPlayResult* result) {
    free(result->cmd);
    free(result->msg);
}

// 设置成功结果
void do_success(CAudioPlayResult* result) {
    result->cmd = strdup("AudioPlay");
    result->resultId = 1;
    result->msg = strdup("play success");
}

// 设置失败结果
void do_fail(CAudioPlayResult* result, const char* str) {
    result->cmd = strdup("AudioPlay");
    result->resultId = 2;
    result->msg = strdup(str);
}

// 初始化音频播放
void init_audio_play(CAudioPlay* play) {
    play->cmd = NULL;
    play->audioName = NULL;
    play->playType = 0;
    play->duration = 0;
}

// 释放音频播放
void free_audio_play(CAudioPlay* play) {
    free(play->cmd);
    free(play->audioName);
}

// 检查文件是否存在
bool file_exists(const CAddCustomAudioFileBusiness* business, const CAudioCfgBusiness* cfg, const char* audioName) {
    return (exist_add_custom_audio_file_business(business, audioName) && find_dir_file_exists(cfg->config_json, audioName)) ||
           (!exist_add_custom_audio_file_business(business, audioName) && find_dir_file_exists(cfg->config_json, audioName));
}

// 发送响应
int send_response(CSocket* pClient, const CAudioPlayResult* audioPlayResult) {
    json_t* js = json_object();
    json_object_set_new(js, "cmd", json_string(audioPlayResult->cmd));
    json_object_set_new(js, "resultId", json_integer(audioPlayResult->resultId));
    json_object_set_new(js, "msg", json_string(audioPlayResult->msg));
    char* str = json_dumps(js, JSON_INDENT(4));
    int result = send_socket(pClient, str, strlen(str));
    free(str);
    json_decref(js);
    return result;
}

// 处理请求
int do_req(CAudioPlay* play, CSocket* pClient) {
    CAudioCfgBusiness cfg;
    init_audio_cfg_business(&cfg);
    load_audio_cfg_business(&cfg);

    CAudioPlayResult audioPlayResult;
    init_audio_play_result(&audioPlayResult);
    do_success(&audioPlayResult);

    if (get_process_status("madplay")) {
        do_fail(&audioPlayResult, "Existing playback task");
        return send_response(pClient, &audioPlayResult);
    }

    CAddCustomAudioFileBusiness business;
    init_add_custom_audio_file_business(&business);

    if (!file_exists(&business, &cfg, play->audioName)) {
        do_fail(&audioPlayResult, "no file");
    } else {
        if (get_process_status("ffplay")) {
            audio_stop();
        }
        switch (play->playType) {
            case 0:
                audio_loop_play(get_audio_file_path(&cfg), play->audioName, ASYNC_START);
                break;
            case 1:
            case 2:
                if (play->duration < 1) {
                    do_fail(&audioPlayResult, "parameter cannot be less than 1");
                } else {
                    if (play->playType == 1) {
                        audio_num_play(play->duration, get_audio_file_path(&cfg), play->audioName, ASYNC_START);
                    } else {
                        audio_time_play(play->duration, get_audio_file_path(&cfg), play->audioName, ASYNC_START);
                    }
                }
                break;
            default:
                do_fail(&audioPlayResult, "Invalid play type");
        }
    }

    free_add_custom_audio_file_business(&business);
    free_audio_cfg_business(&cfg);
    return send_response(pClient, &audioPlayResult);
}