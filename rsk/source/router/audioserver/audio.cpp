// audio.cpp
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include "audio.hpp"
#include "AudioPlayUtils.hpp"
#include "MqttConfig.hpp"
#include "CustomAudio.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

// ... [保留之前的 initializeLogging 函数] ...

class AudioServer {
public:
    AudioServer() = default;
    ~AudioServer() = default;

    void start() {
        loadConfig();
        loadCustomAudioFiles();
        // Additional initialization can be placed here

        running = true;
        server_thread = std::thread(&AudioServer::run, this);
        spdlog::info("AudioServer started");
    }

    void stop() {
        running = false;
        if (server_thread.joinable()) {
            server_thread.join();
        }
        spdlog::info("AudioServer stopped");
    }

private:
    void loadConfig() {
        MqttConfig config;
        if (!config.load_file()) {
            spdlog::error("Failed to load MQTT config");
        } else {
            request_topic = config.get_request_topic();
            publish_topic = config.get_publish_topic();
            spdlog::info("MQTT config loaded. Request topic: {}, Publish topic: {}", 
                         request_topic, publish_topic);
        }
    }

    void loadCustomAudioFiles() {
        if (customAudioManager.loadFromFile("add_column_custom_audio_file.json")) {
            spdlog::info("Custom audio files loaded successfully");
        } else {
            spdlog::error("Failed to load custom audio files");
        }
    }

    void run() {
        while (running) {
            // Main server loop
            spdlog::debug("Server running...");
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    // 新增：处理自定义音频播放请求的方法
    void handleCustomAudioPlayRequest(const std::string& fileName) {
        const CustomAudio* audio = customAudioManager.findAudioByName(fileName);
        if (audio) {
            spdlog::info("Playing custom audio: {} (Type: {}, Size: {} bytes)", 
                         audio->fileName, audio->type, audio->size);
            // 这里添加实际的音频播放逻辑
        } else {
            spdlog::warn("Custom audio file '{}' not found", fileName);
        }
    }

    std::string request_topic;
    std::string publish_topic;
    std::thread server_thread;
    bool running = false;
    CustomAudioManager customAudioManager;
};

int main() {
    initializeLogging();
    spdlog::info("Application started");

    AudioServer server;
    server.start();

    spdlog::info("Server will run for 10 seconds");
    std::this_thread::sleep_for(std::chrono::seconds(10));  // Run for 10 seconds

    server.stop();
    spdlog::info("Application finished");
    return 0;
}

//By GST ARMV8 GCC13.2 audio.cpp