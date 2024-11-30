#include "audioplaystatus.h"

// 获取单例实例
PlayStatus* get_instance() {
    static PlayStatus instance;
    return &instance;
}

// 初始化播放状态
void init_play_status(PlayStatus* status) {
    atomic_store(&status->m_mqttPlayStatus, STOP_TASK_PLAYING);
    atomic_store(&status->m_playId, STOP_TASK_PLAYING);
    atomic_store(&status->m_priority, STOP_TASK_PLAYING);
    atomic_store(&status->m_pId, STOP_TASK_PLAYING);
    status->playContent = NULL;
}

// 获取播放状态
bool get_play_state(const PlayStatus* status) {
    return atomic_load(&status->m_playId) != STOP_TASK_PLAYING;
}

// 获取播放ID
int get_play_id(const PlayStatus* status) {
    return atomic_load(&status->m_playId);
}

// 设置播放ID
void set_play_id(PlayStatus* status, int id) {
    atomic_store(&status->m_playId, id);
}

// 获取优先级
int get_priority(const PlayStatus* status) {
    return atomic_load(&status->m_priority);
}

// 设置优先级
void set_priority(PlayStatus* status, int id) {
    atomic_store(&status->m_priority, id);
}

// 获取进程ID
pid_t get_process_id(const PlayStatus* status) {
    return atomic_load(&status->m_pId);
}

// 设置进程ID
void set_process_id(PlayStatus* status, pid_t id) {
    atomic_store(&status->m_pId, id);
}

// 获取播放内容
const char* get_play_content(const PlayStatus* status) {
    return status->playContent;
}

// 设置播放内容
void set_play_content(PlayStatus* status, const char* content) {
    free(status->playContent);
    status->playContent = strdup(content);
}

// 获取MQTT播放状态
int get_mqtt_play_status(const PlayStatus* status) {
    return atomic_load(&status->m_mqttPlayStatus);
}

// 设置MQTT播放状态
void set_mqtt_play_status(PlayStatus* status, int status) {
    atomic_store(&status->m_mqttPlayStatus, status);
}