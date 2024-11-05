// audiocfg.hpp
#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

class CAudioCfgBusiness {
public:
    CAudioCfgBusiness() = default;
    ~CAudioCfgBusiness() = default;

    CAudioCfgBusiness(const CAudioCfgBusiness&) = delete;
    CAudioCfgBusiness& operator=(const CAudioCfgBusiness&) = delete;
    CAudioCfgBusiness(CAudioCfgBusiness&&) = delete;
    CAudioCfgBusiness& operator=(CAudioCfgBusiness&&) = delete;

    [[nodiscard]] bool load() {
        std::filesystem::path config_path(CONFIG_FILE);
        if (!std::filesystem::exists(config_path)) {
            std::cerr << "Config file does not exist: " << CONFIG_FILE << std::endl;
            return false;
        }

        std::ifstream config_file(config_path);
        if (!config_file) {
            std::cerr << "Failed to open config file: " << CONFIG_FILE << std::endl;
            return false;
        }

        try {
            config_file >> config_json;
            return true;
        } catch (const nlohmann::json::exception& ex) {
            std::cerr << "JSON parse error: " << ex.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] std::string_view getAudioFilePath() const {
        static const std::string default_path;
        return config_json.value("audioFilePath", default_path);
    }

    // Add other getter methods as needed

private:
    static constexpr const char* CONFIG_FILE = "/mnt/cfg/audio.json";
    nlohmann::json config_json;
};

//By GST ARMV8 GCC13.2 audiocfg.hpp