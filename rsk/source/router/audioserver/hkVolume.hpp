// hkvolume.hpp
#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

class HkVolume {
public:
    HkVolume() = default;
    HkVolume(std::string id, int volume) : id_(std::move(id)), volume_(volume) {}

    [[nodiscard]] std::string_view getId() const { return id_; }
    void setId(std::string newId) { id_ = std::move(newId); }

    [[nodiscard]] int getVolume() const { return volume_; }
    void setVolume(int newVolume) { volume_ = newVolume; }

    [[nodiscard]] nlohmann::json toJson() const {
        return nlohmann::json{
            {"id", id_},
            {"volume", volume_}
        };
    }

    static HkVolume fromJson(const nlohmann::json& j) {
        return HkVolume(
            j.at("id").get<std::string>(),
            j.at("volume").get<int>()
        );
    }

private:
    std::string id_;
    int volume_{0};
};

class HkVolumeManager {
public:
    explicit HkVolumeManager(std::filesystem::path configPath) 
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
            volume_ = HkVolume::fromJson(j);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading config file: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] bool save() const {
        try {
            nlohmann::json j = volume_.toJson();

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

    [[nodiscard]] const HkVolume& getVolume() const { return volume_; }
    void setVolume(HkVolume newVolume) { volume_ = std::move(newVolume); }

private:
    std::filesystem::path configPath_;
    HkVolume volume_;
};

//BY GST ARMV8 GCC 13.2