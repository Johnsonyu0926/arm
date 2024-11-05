// mqtt_micrecordupload.hpp
#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <iostream>
#include <memory>
#include <functional>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "mqtt.hpp"
#include "micRecordUpload.hpp"

class MqttMicRecordUpload {
public:
    MqttMicRecordUpload(std::string clientId, std::string host, int port)
        : clientId(std::move(clientId)), host(std::move(host)), port(port) {}

    [[nodiscard]] bool loadConfig(const std::filesystem::path& configPath) {
        if (!std::filesystem::exists(configPath)) {
            std::cerr << "Config file does not exist: " << configPath << std::endl;
            return false;
        }

        try {
            std::ifstream configFile(configPath);
            if (!configFile) {
                std::cerr << "Failed to open config file: " << configPath << std::endl;
                return false;
            }

            nlohmann::json j = nlohmann::json::parse(configFile);
            micRecordConfig = j.at("micRecordConfig").get<MicRecordUpload>();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading config file: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] bool saveConfig(const std::filesystem::path& configPath) const {
        try {
            nlohmann::json j = {
                {"micRecordConfig", micRecordConfig}
            };

            std::ofstream configFile(configPath);
            if (!configFile) {
                std::cerr << "Failed to open config file for writing: " << configPath << std::endl;
                return false;
            }

            configFile << j.dump(4);
            return configFile.good();
        } catch (const std::exception& e) {
            std::cerr << "Error saving config file: " << e.what() << std::endl;
            return false;
        }
    }

    void initializeClient() {
        mqttClient = std::make_unique<MqttClient>(clientId, host, port);
    }

    void setCallback(std::function<void(std::string_view topic, std::string_view message)> callback) {
        mqttClient->setMessageCallback(std::move(callback));
    }

    [[nodiscard]] bool connect() {
        return mqttClient->connect();
    }

    void disconnect() {
        mqttClient->disconnect();
    }

    [[nodiscard]] bool subscribe(std::string_view topic) {
        return mqttClient->subscribe(topic);
    }

    [[nodiscard]] bool publish(std::string_view topic, std::string_view message) {
        return mqttClient->publish(topic, message);
    }

    void loopForever() {
        mqttClient->loopForever();
    }

    [[nodiscard]] nlohmann::json handleMicRecordUploadCommand(const nlohmann::json& command) {
        // 这里应该实现实际的麦克风录音上传逻辑
        // 为了示例，我们假设上传总是成功的
        nlohmann::json response = {
            {"result", "麦克风录音上传成功"},
            {"resultId", 1},
            {"imei", command["imei"]},
            {"topic", "TaDiao/device/report/test/" + command["imei"].get<std::string>()},
            {"cmd", "micRecordUpload"},
            {"publishId", command["publishId"]},
            {"data", ""}
        };
        return response;
    }

private:
    std::string clientId;
    std::string host;
    int port;
    MicRecordUpload micRecordConfig;
    std::unique_ptr<MqttClient> mqttClient;
};

//BY GST ARMV8 GCC 13.2