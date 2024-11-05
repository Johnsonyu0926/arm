// MqttConfig.hpp
#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

class MqttConfig {
public:
    MqttConfig() = default;
    ~MqttConfig() = default;

    MqttConfig(const MqttConfig&) = delete;
    MqttConfig& operator=(const MqttConfig&) = delete;
    MqttConfig(MqttConfig&&) noexcept = default;
    MqttConfig& operator=(MqttConfig&&) noexcept = default;

    [[nodiscard]] std::string_view get_request_topic() const noexcept { return request_topic; }
    void set_request_topic(std::string_view new_request_topic) { request_topic = new_request_topic; }

    [[nodiscard]] std::string_view get_publish_topic() const noexcept { return publish_topic; }
    void set_publish_topic(std::string_view new_publish_topic) { publish_topic = new_publish_topic; }

    [[nodiscard]] bool load_file() {
        if (!std::filesystem::exists(MQTTCONFIG)) {
            std::cerr << "Config file does not exist: " << MQTTCONFIG << std::endl;
            return false;
        }

        std::ifstream i(MQTTCONFIG);
        if (!i) {
            std::cerr << "Failed to open config file: " << MQTTCONFIG << std::endl;
            return false;
        }

        try {
            nlohmann::json js = nlohmann::json::parse(i);
            request_topic = js.at("request_topic").get<std::string>();
            publish_topic = js.at("publish_topic").get<std::string>();
        } catch (const nlohmann::json::exception& ex) {
            std::cerr << "JSON parse error: " << ex.what() << std::endl;
            return false;
        }

        return true;
    }

    [[nodiscard]] bool file_update() const {
        std::ofstream o(MQTTCONFIG);
        if (!o) {
            std::cerr << "Failed to open config file for writing: " << MQTTCONFIG << std::endl;
            return false;
        }

        nlohmann::json js;
        js["request_topic"] = request_topic;
        js["publish_topic"] = publish_topic;

        o << js.dump(4) << std::endl;
        return o.good();
    }

private:
    static constexpr std::string_view MQTTCONFIG = "/mnt/cfg/mqtt.json";
    std::string request_topic = "IOT/intranet/client/request/";
    std::string publish_topic = "IOT/intranet/server/report/";
};

//BY GST ARMV8 GCC 13.2