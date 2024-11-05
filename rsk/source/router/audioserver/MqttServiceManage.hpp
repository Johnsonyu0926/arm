// MqttServiceManage.hpp
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
#include "serviceManage.hpp"

class MqttServiceManage {
public:
    MqttServiceManage(std::string clientId, std::string host, int port)
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
            serviceManageConfig = j.at("serviceManageConfig").get<ServiceManage>();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading config file: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] bool saveConfig(const std::filesystem::path& configPath) const {
        try {
            nlohmann::json j = {
                {"serviceManageConfig", serviceManageConfig}
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
    int port;
    ServiceManage serviceManageConfig;
    std::unique_ptr<MqttClient> mqttClient;
};

//BY GST ARMV8 GCC 13.2