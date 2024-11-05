// CustomAudio.hpp
#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

class CustomAudio {
public:
    int type;
    std::string fileName;
    int size;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CustomAudio, type, fileName, size)
};

class CustomAudioManager {
public:
    CustomAudioManager() = default;

    bool loadFromFile(const std::string& filename) {
        try {
            std::ifstream file(filename);
            if (!file.is_open()) {
                spdlog::error("Failed to open custom audio file: {}", filename);
                return false;
            }

            nlohmann::json j;
            file >> j;

            audioFiles = j.get<std::vector<CustomAudio>>();
            spdlog::info("Loaded {} custom audio files", audioFiles.size());
            return true;
        } catch (const std::exception& e) {
            spdlog::error("Error loading custom audio file: {}", e.what());
            return false;
        }
    }

    const CustomAudio* findAudioByName(const std::string& name) const {
        auto it = std::find_if(audioFiles.begin(), audioFiles.end(),
                               [&name](const CustomAudio& audio) { return audio.fileName == name; });
        return it != audioFiles.end() ? &(*it) : nullptr;
    }

private:
    std::vector<CustomAudio> audioFiles;
};