// hkTts.hpp
#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

class TtsConfig {
public:
    TtsConfig() = default;
    TtsConfig(std::string apiKey, std::string url, std::string voice)
        : apiKey_(std::move(apiKey)), url_(std::move(url)), voice_(std::move(voice)) {}

    [[nodiscard]] std::string_view getApiKey() const { return apiKey_; }
    void setApiKey(std::string newApiKey) { apiKey_ = std::move(newApiKey); }

    [[nodiscard]] std::string_view getUrl() const { return url_; }
    void setUrl(std::string newUrl) { url_ = std::move(newUrl); }

    [[nodiscard]] std::string_view getVoice() const { return voice_; }
    void setVoice(std::string newVoice) { voice_ = std::move(newVoice); }

    [[nodiscard]] nlohmann::json toJson() const {
        return nlohmann::json{
            {"apiKey", apiKey_},
            {"url", url_},
            {"voice", voice_}
        };
    }

    static TtsConfig fromJson(const nlohmann::json& j) {
        return TtsConfig(
            j.at("apiKey").get<std::string>(),
            j.at("url").get<std::string>(),
            j.at("voice").get<std::string>()
        );
    }

private:
    std::string apiKey_;
    std::string url_;
    std::string voice_;
};

class TtsConfigManager {
public:
    explicit TtsConfigManager(std::filesystem::path configPath) 
        : configPath_(std::move(configPath)) {}

    [[nodiscard]] bool load() {
        if (!std::filesystem::exists(configPath_)) {
            std::cerr << "Config file does not exist: " << configPath_ << std::endl;
            return false;
        }

        try {
            std::ifstream configFile(configPath_);
            if (!configFile) {
                std::cerr << "Failed to open config file: " << configPath_ << std::endl;
                return false;
            }

            nlohmann::json j = nlohmann::json::parse(configFile);
            config_ = TtsConfig::fromJson(j);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading config file: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] bool save() const {
        try {
            nlohmann::json j = config_.toJson();

            std::ofstream configFile(configPath_);
            if (!configFile) {
                std::cerr << "Failed to open config file for writing: " << configPath_ << std::endl;
                return false;
            }

            configFile << j.dump(4);
            return configFile.good();
        } catch (const std::exception& e) {
            std::cerr << "Error saving config file: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] const TtsConfig& getConfig() const { return config_; }
    void setConfig(TtsConfig newConfig) { config_ = std::move(newConfig); }

private:
    std::filesystem::path configPath_;
    TtsConfig config_;
};

//By GST ARMV8 GCC13.2 hkTts.hpp