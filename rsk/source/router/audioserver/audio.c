#include "audio.h"
#include <unistd.h>

// 初始化日志记录
void initialize_logging() {
    // 初始化日志记录逻辑
    spdlog_init();
    spdlog_info("Logging initialized");
}

// 初始化音频服务器
void audio_server_init(AudioServer* server) {
    server->request_topic = NULL;
    server->publish_topic = NULL;
    server->running = false;
    custom_audio_manager_init(&server->custom_audio_manager);
}

// 启动音频服务器
void audio_server_start(AudioServer* server) {
    audio_server_load_config(server);
    audio_server_load_custom_audio_files(server);

    server->running = true;
    pthread_create(&server->server_thread, NULL, audio_server_run, server);
    spdlog_info("AudioServer started");
}

// 停止音频服务器
void audio_server_stop(AudioServer* server) {
    server->running = false;
    if (pthread_join(server->server_thread, NULL) == 0) {
        spdlog_info("AudioServer stopped");
    }
}

// 加载配置
void audio_server_load_config(AudioServer* server) {
    MqttConfig config;
    mqtt_config_init(&config);
    if (!mqtt_config_load_file(&config)) {
        spdlog_error("Failed to load MQTT config");
    } else {
        server->request_topic = strdup(config.request_topic);
        server->publish_topic = strdup(config.publish_topic);
        spdlog_info("MQTT config loaded. Request topic: %s, Publish topic: %s", 
                    server->request_topic, server->publish_topic);
    }
    mqtt_config_free(&config);
}

// 加载自定义音频文件
void audio_server_load_custom_audio_files(AudioServer* server) {
    if (custom_audio_manager_load_from_file(&server->custom_audio_manager, "add_column_custom_audio_file.json")) {
        spdlog_info("Custom audio files loaded successfully");
    } else {
        spdlog_error("Failed to load custom audio files");
    }
}

// 服务器运行逻辑
void* audio_server_run(void* arg) {
    AudioServer* server = (AudioServer*)arg;
    while (server->running) {
        spdlog_debug("Server running...");
        sleep(1);
    }
    return NULL;
}

// 处理自定义音频播放请求
void handle_custom_audio_play_request(AudioServer* server, const char* file_name) {
    const CustomAudio* audio = custom_audio_manager_find_audio_by_name(&server->custom_audio_manager, file_name);
    if (audio) {
        spdlog_info("Playing custom audio: %s (Type: %d, Size: %d bytes)", 
                    audio->file_name, audio->type, audio->size);
        // 这里添加实际的音频播放逻辑
    } else {
        spdlog_warn("Custom audio file '%s' not found", file_name);
    }
}

int main() {
    initialize_logging();
    spdlog_info("Application started");

    AudioServer server;
    audio_server_init(&server);
    audio_server_start(&server);

    spdlog_info("Server will run for 10 seconds");
    sleep(10);  // 运行10秒

    audio_server_stop(&server);
    spdlog_info("Application finished");
    return 0;
}