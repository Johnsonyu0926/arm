// micrecord.hpp
#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <nlohmann/json.hpp>

class MicRecord {
public:
    MicRecord() = default;
    MicRecord(std::string id, std::string name, std::string path)
        : id_(std::move(id)), name_(std::move(name)), path_(std::move(path)) {}

    [[nodiscard]] std::string_view getId() const { return id_; }
    void setId(std::string newId) { id_ = std::move(newId); }

    [[nodiscard]] std::string_view getName() const { return name_; }
    void setName(std::string newName) { name_ = std::move(newName); }

    [[nodiscard]] std::string_view getPath() const { return path_; }
    void setPath(std::string newPath) { path_ = std::move(newPath); }

    [[nodiscard]] nlohmann::json toJson() const {
        return nlohmann::json{
            {"id", id_},
            {"name", name_},
            {"path", path_}
        };
    }

    static MicRecord fromJson(const nlohmann::json& j) {
        return MicRecord(
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

class MicRecordManager {
public:
    explicit MicRecordManager(std::filesystem::path configPath) 
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
            records_.clear();
            records_.reserve(j.size());
            for (const auto& item : j) {
                records_.push_back(MicRecord::fromJson(item));
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
            for (const auto& record : records_) {
                j.push_back(record.toJson());
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

    void addRecord(MicRecord record) {
        records_.push_back(std::move(record));
    }

    void removeRecord(std::string_view id) {
        records_.erase(
            std::remove_if(records_.begin(), records_.end(),
                [&id](const MicRecord& record) { return record.getId() == id; }),
            records_.end()
        );
    }

    [[nodiscard]] MicRecord* findRecord(std::string_view id) {
        auto it = std::find_if(records_.begin(), records_.end(),
            [&id](const MicRecord& record) { return record.getId() == id; });
        return (it != records_.end()) ? &(*it) : nullptr;
    }

private:
    std::filesystem::path configPath_;
    std::vector<MicRecord> records_;
};

//BY GST ARMV8 GCC 13.2