#include "clientthread.h"
#include <jansson.h>
#include <string.h>

// 初始化实例
bool client_thread_init_instance(CClientThread* thread) {
    // Initialization code.
    return true;
}

// 退出实例
bool client_thread_exit_instance(CClientThread* thread) {
    // Cleanup code.
    return true;
}

// 检查缓冲区
bool client_thread_check(const CClientThread* thread, const unsigned char* szBuf) {
    // Check the buffer.
    return true;
}

// 生成缓冲区数据
bool client_thread_gen(const CClientThread* thread, unsigned char* szBuf) {
    // Generate data for the buffer.
    return true;
}

// 处理请求
int client_thread_do_req(CClientThread* thread, const char* buf, CSocket* pClient) {
    json_error_t error;
    json_t* j = json_loads(buf, 0, &error);
    if (!j) {
        fprintf(stderr, "JSON parsing error: %s\n", error.text);
        return -1;
    }

    const char* cmd = json_string_value(json_object_get(j, "cmd"));
    if (!cmd) {
        json_decref(j);
        return -1;
    }

    if (strcmp(cmd, "AudioPlay") == 0) {
        CAudioPlay play;
        init_audio_play(&play);
        json_unpack(j, "{s:s, s:s, s:i, s:i}", "cmd", &play.cmd, "audioName", &play.audioName, "playType", &play.playType, "duration", &play.duration);
        int result = do_req(&play, pClient);
        free_audio_play(&play);
        json_decref(j);
        return result;
    } else if (strcmp(cmd, "AudioStop") == 0) {
        CAudioStop stop;
        init_audio_stop(&stop);
        json_unpack(j, "{s:s}", "cmd", &stop.cmd);
        int result = do_req(&stop, pClient);
        free_audio_stop(&stop);
        json_decref(j);
        return result;
    } else if (strcmp(cmd, "AudioDelete") == 0) {
        CDeleteAudio del;
        init_delete_audio(&del);
        json_unpack(j, "{s:s, s:s, s:i}", "cmd", &del.cmd, "deleteName", &del.deleteName, "storageType", &del.storageType);
        int result = do_req(&del, pClient);
        free_delete_audio(&del);
        json_decref(j);
        return result;
    } else if (strcmp(cmd, "BroadcastPlan") == 0) {
        BroadcastPlan plan;
        init_broadcast_plan(&plan);
        json_unpack(j, "{s:s, s:i, s:s, s:o}", "cmd", &plan.cmd, "id", &plan.id, "name", &plan.name, "audioFiles", &plan.audioFiles);
        int result = do_req(&plan, pClient);
        free_broadcast_plan(&plan);
        json_decref(j);
        return result;
    }

    json_decref(j);
    return -1;
}

// 验证请求
int client_thread_do_verify(const CClientThread* thread, const char* buf) {
    // Verification code.
    return 0;
}

// 处理字符串请求
int client_thread_do_str_req(CClientThread* thread, CSocket* pClient) {
    // String request handling code.
    return 0;
}

// 验证字符串请求
int client_thread_do_str_verify(const CClientThread* thread, const char* buf, CSocket* pClient) {
    // String verification code.
    return 0;
}