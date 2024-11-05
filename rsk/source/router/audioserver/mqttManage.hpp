// MqttManage.hpp
#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <functional>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "mqtt.hpp"

class MqttManage {
public:
    MqttManage() = default;
    ~MqttManage() = default;

    MqttManage(const MqttManage&) = delete;
    MqttManage& operator=(const MqttManage&) = delete;
    MqttManage(MqttManage&&) noexcept = default;
    MqttManage& operator=(MqttManage&&) noexcept = default;

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
            clientId = j.at("clientId").get<std::string>();
            host = j.at("host").get<std::string>();
            port = j.at("port").get<int>();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading config file: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] bool saveConfig(const std::filesystem::path& configPath) const {
        try {
            nlohmann::json j = {
                {"clientId", clientId},
                {"host", host},
                {"port", port}
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

private:
    std::string clientId;
    std::string host;
    int port{};
    std::unique_ptr<MqttClient> mqttClient;
};

//BY GST ARMV8 GCC 13.2