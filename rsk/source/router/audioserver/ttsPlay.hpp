// tts.hpp
#pragma once

#include <string>
#include <fstream>
#include <functional>
#include <iostream>
#include <nlohmann/json.hpp>

class TtsPlay {
public:
    TtsPlay(std::string clientId, std::string host, int port)
        : clientId(std::move(clientId)), host(std::move(host)), port(port) {}

    [[nodiscard]] bool loadConfig(const std::string& configPath) {
        try {
            std::ifstream configFile(configPath);
            if (!configFile.is_open()) {
                std::cerr << "Failed to open config file: " << configPath << std::endl;
                return false;
            }

            nlohmann::json j = nlohmann::json::parse(configFile);
            ttsConfig = j.at("ttsConfig").get<std::string>();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading config file: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] bool saveConfig(const std::string& configPath) const {
        try {
            nlohmann::json j = {
                {"ttsConfig", ttsConfig}
            };

            std::ofstream configFile(configPath);
            if (!configFile.is_open()) {
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
        // Initialize MQTT client
    }

    void setCallback(std::function<void(std::string_view topic, std::string_view message)> callback) {
        this->callback = std::move(callback);
    }

    [[nodiscard]] bool connect() {
        // Connect to MQTT broker
        return true;
    }

    void disconnect() {
        // Disconnect from MQTT broker
    }

    [[nodiscard]] bool subscribe(std::string_view topic) {
        // Subscribe to MQTT topic
        return true;
    }

    [[nodiscard]] bool publish(std::string_view topic, std::string_view message) {
        // Publish message to MQTT topic
        return true;
    }

    void loopForever() {
        // Loop forever to process MQTT messages
    }

private:
    std::string clientId;
    std::string host;
    int port;
    std::string ttsConfig;
    std::function<void(std::string_view, std::string_view)> callback;
    // MQTT client instance
};

//BY GST ARMV8 GCC 13.2