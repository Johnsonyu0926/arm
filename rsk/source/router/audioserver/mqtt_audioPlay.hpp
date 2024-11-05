// mqtt_audioplay.hpp
#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>
#include <functional>
#include <filesystem>
#include <algorithm>
#include <nlohmann/json.hpp>
#include "mqtt.hpp"
#include "audioPlay.hpp"
#include "audioFile.hpp"

class MqttAudioPlay {
public:
    MqttAudioPlay(std::string clientId, std::string host, int port)
        : clientId(std::move(clientId)), host(std::move(host)), port(port) {}

    // ... (保留之前的方法)

    [[nodiscard]] bool loadAudioFileList(const std::filesystem::path& filePath) {
        if (!std::filesystem::exists(filePath)) {
            std::cerr << "Audio file list does not exist: " << filePath << std::endl;
            return false;
        }

        try {
            std::ifstream file(filePath);
            if (!file) {
                std::cerr << "Failed to open audio file list: " << filePath << std::endl;
                return false;
            }

            nlohmann::json j = nlohmann::json::parse(file);
            audioFileList = j.get<std::vector<AudioFile>>();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading audio file list: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] nlohmann::json handleAudioFilePlayCommand(const nlohmann::json& command) {
        std::string fileName = command["fileName"].get<std::string>();
        auto it = std::find_if(audioFileList.begin(), audioFileList.end(),
                               [&fileName](const AudioFile& audio) {
                                   return audio.fileName == fileName;
                               });

        if (it == audioFileList.end()) {
            return createErrorResponse(command, "Audio file not found");
        }

        // 这里应该实现实际的音频文件播放逻辑
        // 为了示例，我们假设播放总是成功的
        nlohmann::json response = {
            {"result", "音频文件播放成功"},
            {"resultId", 1},
            {"imei", command["imei"]},
            {"topic", "TaDiao/device/report/test/" + command["imei"].get<std::string>()},
            {"cmd", "audioFilePlay"},
            {"publishId", command["publishId"]},
            {"data", {
                {"fileName", it->fileName},
                {"size", it->size},
                {"type", it->type}
            }}
        };
        return response;
    }

private:
    std::string clientId;
    std::string host;
    int port;
    std::vector<AudioPlay> audioConfig;
    std::vector<AudioFile> audioFileList;
    std::unique_ptr<MqttClient> mqttClient;

    nlohmann::json createErrorResponse(const nlohmann::json& command, const std::string& errorMessage) {
        return {
            {"result", errorMessage},
            {"resultId", 2},
            {"imei", command["imei"]},
            {"topic", "TaDiao/device/report/test/" + command["imei"].get<std::string>()},
            {"cmd", "audioFilePlay"},
            {"publishId", command["publishId"]},
            {"data", {}}
        };
    }
};

//BY GST ARMV8 GCC 13.2
/*
使用这个更新后的类，您可以加载音频文件列表并处理音频文件播放命令。例如：
MqttAudioPlay audioPlayer("clientId", "host", 1883);
audioPlayer.loadAudioFileList("/path/to/audio_file_list.json");

// 在某个地方处理收到的 MQTT 消息
/*
/*
nlohmann::json command = /* 解析收到的 MQTT 消息 */;
/*if (command["cmd"] == "audioFilePlay") {
    nlohmann::json response = audioPlayer.handleAudioFilePlayCommand(command);
    // 发送响应
}
/*
这个实现假设音频文件已经在设备上可用，并且播放逻辑已经实现。在实际应用中，您可能需要添加更多的错误检查和处理逻辑，例如：
检查文件是否真的存在于设备上。
验证文件大小是否与列表中的大小匹配。
处理播放过程中可能出现的错误。
添加播放进度报告功能。
实现播放队列管理，以处理连续播放多个文件的情况。