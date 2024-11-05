// mqtt_heartbeat.hpp
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
#include "heartbeat.hpp"

class MqttHeartbeat {
public:
    MqttHeartbeat(std::string clientId, std::string host, int port)
        : clientId_(std::move(clientId)), host_(std::move(host)), port_(port) {}

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
            heartbeatConfig_ = j.at("heartbeatConfig").get<Heartbeat>();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading config file: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] bool saveConfig(const std::filesystem::path& configPath) const {
        try {
            nlohmann::json j = {
                {"heartbeatConfig", heartbeatConfig_}
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
        mqttClient_ = std::make_unique<MqttClient>(clientId_, host_, port_);
    }

    void setCallback(std::function<void(std::string_view topic, std::string_view message)> callback) {
        mqttClient_->setMessageCallback(std::move(callback));
    }

    [[nodiscard]] bool connect() {
        return mqttClient_->connect();
    }

    void disconnect() {
        mqttClient_->disconnect();
    }

    [[nodiscard]] bool subscribe(std::string_view topic) {
        return mqttClient_->subscribe(topic);
    }

    [[nodiscard]] bool publish(std::string_view topic, std::string_view message) {
        return mqttClient_->publish(topic, message);
    }

    void loopForever() {
        mqttClient_->loopForever();
    }

    [[nodiscard]] nlohmann::json generateHeartbeat() const {
        // 这里应该实现实际的心跳生成逻辑
        // 为了示例，我们返回一个简单的心跳JSON
        return nlohmann::json{
            {"cmd", "heartbeat"},
            {"timestamp", std::time(nullptr)},
            {"status", "ok"}
        };
    }

    void sendHeartbeat() {
        nlohmann::json heartbeat = generateHeartbeat();
        publish("heartbeat/topic", heartbeat.dump());
    }

private:
    std::string clientId_;
    std::string host_;
    int port_;
    Heartbeat heartbeatConfig_;
    std::unique_ptr<MqttClient> mqttClient_;
};

//BY GST ARMV8 GCC 13.2