// mqtt_boot.hpp
#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>
#include <functional>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "mqtt.hpp"
#include "boot.hpp"

class MqttBoot {
public:
    MqttBoot(std::string clientId, std::string host, int port)
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
            bootConfig = j.at("bootConfig").get<std::vector<Boot>>();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading config file: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] bool saveConfig(const std::filesystem::path& configPath) const {
        try {
            nlohmann::json j = {
                {"bootConfig", bootConfig}
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

    [[nodiscard]] nlohmann::json generateStartReport() const {
        nlohmann::json report = {
            {"sdcardSpace", 16225000},
            {"lng", "120.2168731"},
            {"flashSpace", 16225000},
            {"project", "0Tr1N2kbP5sKUDglAIOUxHdwp4VkLucz"},
            {"iotVersion", "COMMON"},
            {"version", "2.0.30"},
            {"volume", 4},
            {"iccId", "898604B41022C0341497"},
            {"v12", 0},
            {"volt", 971},
            {"v24", 0},
            {"storageType", 1},
            {"imei", "869298057401606"},
            {"topic", "TaDiao/device/report/test/869298057401606"},
            {"cmd", "start"},
            {"lat", "030.2099819"},
            {"v5", 0}
        };
        return report;
    }

    void sendStartReport() {
        nlohmann::json report = generateStartReport();
        std::string message = report.dump();
        publish(report["topic"], message);
    }

private:
    std::string clientId;
    std::string host;
    int port;
    std::vector<Boot> bootConfig;
    std::unique_ptr<MqttClient> mqttClient;
};

//BY GST ARMV8 GCC 13.2