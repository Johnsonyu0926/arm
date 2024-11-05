// Relay.hpp
#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

class RelayConfig {
public:
    RelayConfig() = default;
    ~RelayConfig() = default;

    RelayConfig(const RelayConfig&) = delete;
    RelayConfig& operator=(const RelayConfig&) = delete;
    RelayConfig(RelayConfig&&) noexcept = default;
    RelayConfig& operator=(RelayConfig&&) noexcept = default;

    [[nodiscard]] std::string_view get_relay_topic() const noexcept { return relay_topic; }
    void set_relay_topic(std::string_view new_relay_topic) { relay_topic = new_relay_topic; }

    [[nodiscard]] bool load_file() {
        if (!std::filesystem::exists(RELAYCONFIG)) {
            std::cerr << "Config file does not exist: " << RELAYCONFIG << std::endl;
            return false;
        }

        std::ifstream i(RELAYCONFIG);
        if (!i) {
            std::cerr << "Failed to open config file: " << RELAYCONFIG << std::endl;
            return false;
        }

        try {
            nlohmann::json js = nlohmann::json::parse(i);
            relay_topic = js.at("relay_topic").get<std::string>();
        } catch (const nlohmann::json::exception& ex) {
            std::cerr << "JSON parse error: " << ex.what() << std::endl;
            return false;
        }

        return true;
    }

    [[nodiscard]] bool file_update() const {
        std::ofstream o(RELAYCONFIG);
        if (!o) {
            std::cerr << "Failed to open config file for writing: " << RELAYCONFIG << std::endl;
            return false;
        }

        nlohmann::json js;
        js["relay_topic"] = relay_topic;

        o << js.dump(4) << std::endl;
        return o.good();
    }

private:
    static constexpr std::string_view RELAYCONFIG = "/mnt/cfg/relay.json";
    std::string relay_topic = "IOT/intranet/client/relay/";
};

//BY GST ARMV8 GCC 13.2