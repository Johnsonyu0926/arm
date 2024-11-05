// accaf.hpp
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <nlohmann/json.hpp>

class CustomAudioFile {
public:
    CustomAudioFile(std::string id, std::string name, std::string path)
        : id_(std::move(id)), name_(std::move(name)), path_(std::move(path)) {}

    [[nodiscard]] const std::string& getId() const { return id_; }
    [[nodiscard]] const std::string& getName() const { return name_; }
    [[nodiscard]] const std::string& getPath() const { return path_; }

    void setId(std::string newId) { id_ = std::move(newId); }
    void setName(std::string newName) { name_ = std::move(newName); }
    void setPath(std::string newPath) { path_ = std::move(newPath); }

    [[nodiscard]] nlohmann::json toJson() const {
        return nlohmann::json{
            {"id", id_},
            {"name", name_},
            {"path", path_}
        };
    }

    static CustomAudioFile fromJson(const nlohmann::json& j) {
        return CustomAudioFile(
            j.at("id").get<std::string>(),
            j.at("name").get<std::string>(),
            j.at("path").get<std::string>()
        );
    }

private:
    std::string id_;
    std::string name_;
    std::string path_;
};

class CustomAudioFileManager {
public:
    explicit CustomAudioFileManager(std::filesystem::path configPath) 
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
            files_.clear();
            files_.reserve(j.size());
            for (const auto& item : j) {
                files_.emplace_back(CustomAudioFile::fromJson(item));
            }
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading config file: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] bool save() const {
        try {
            nlohmann::json j = nlohmann::json::array();
            for (const auto& file : files_) {
                j.push_back(file.toJson());
            }

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

    void addFile(CustomAudioFile file) {
        files_.emplace_back(std::move(file));
    }

    void removeFile(const std::string& id) {
        files_.erase(
            std::remove_if(files_.begin(), files_.end(),
                [&id](const CustomAudioFile& file) { return file.getId() == id; }),
            files_.end()
        );
    }

    [[nodiscard]] CustomAudioFile* findFile(const std::string& id) {
        auto it = std::find_if(files_.begin(), files_.end(),
            [&id](const CustomAudioFile& file) { return file.getId() == id; });
        return (it != files_.end()) ? &(*it) : nullptr;
    }

private:
    std::filesystem::path configPath_;
    std::vector<CustomAudioFile> files_;
};

//By GST ARMV8 GCC13.2 accaf.hpp