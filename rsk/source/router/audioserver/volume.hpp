// volume.hpp
#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Volume {
public:
    Volume() = default;
    Volume(std::string id, int level)
        : id(std::move(id)), level(level) {}

    [[nodiscard]] const std::string& getId() const noexcept { return id; }
    void setId(std::string newId) { id = std::move(newId); }

    [[nodiscard]] int getLevel() const noexcept { return level; }
    void setLevel(int newLevel) noexcept { level = newLevel; }

    [[nodiscard]] json toJson() const {
        return json{
            {"id", id},
            {"level", level}
        };
    }

    static Volume fromJson(const json& j) {
        return Volume{
            j.at("id").get<std::string>(),
            j.at("level").get<int>()
        };
    }

private:
    std::string id;
    int level{0};
};

class VolumeManager {
public:
    explicit VolumeManager(std::string configPath) 
        : configPath(std::move(configPath)) {}

    [[nodiscard]] bool load() {
        try {
            std::ifstream configFile(configPath);
            if (!configFile.is_open()) {
                std::cerr << "Failed to open config file: " << configPath << std::endl;
                return false;
            }

            json j;
            configFile >> j;

            volume = Volume::fromJson(j);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading config file: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] bool save() const {
        try {
            const json j = volume.toJson();

            std::ofstream configFile(configPath);
            if (!configFile.is_open()) {
                std::cerr << "Failed to open config file for writing: " << configPath << std::endl;
                return false;
            }

            configFile << j.dump(4);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error saving config file: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] const Volume& getVolume() const noexcept { return volume; }
    void setVolume(Volume newVolume) { volume = std::move(newVolume); }

private:
    std::string configPath;
    Volume volume;
};

//BY GST ARMV8 GCC 13.2