// BroadcastPlan.hpp
#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <nlohmann/json.hpp>

class BroadcastPlan {
public:
    BroadcastPlan() = default;
    BroadcastPlan(int id, std::string name, std::vector<std::string> audioFiles)
        : id_(id), name_(std::move(name)), audioFiles_(std::move(audioFiles)) {}

    [[nodiscard]] int getId() const { return id_; }
    void setId(int newId) { id_ = newId; }

    [[nodiscard]] const std::string& getName() const { return name_; }
    void setName(std::string newName) { name_ = std::move(newName); }

    [[nodiscard]] const std::vector<std::string>& getAudioFiles() const { return audioFiles_; }
    void setAudioFiles(std::vector<std::string> newAudioFiles) { audioFiles_ = std::move(newAudioFiles); }

    [[nodiscard]] nlohmann::json toJson() const {
        return nlohmann::json{
            {"id", id_},
            {"name", name_},
            {"audioFiles", audioFiles_}
        };
    }

    static BroadcastPlan fromJson(const nlohmann::json& j) {
        return BroadcastPlan(
            j.at("id").get<int>(),
            j.at("name").get<std::string>(),
            j.at("audioFiles").get<std::vector<std::string>>()
        );
    }

private:
    int id_;
    std::string name_;
    std::vector<std::string> audioFiles_;
};

class BroadcastPlanManager {
public:
    explicit BroadcastPlanManager(std::filesystem::path configPath) 
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
            plans_.clear();
            plans_.reserve(j.size());
            for (const auto& item : j) {
                plans_.push_back(BroadcastPlan::fromJson(item));
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
            for (const auto& plan : plans_) {
                j.push_back(plan.toJson());
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

    void addPlan(BroadcastPlan plan) {
        plans_.push_back(std::move(plan));
    }

    void removePlan(int id) {
        plans_.erase(
            std::remove_if(plans_.begin(), plans_.end(),
                [id](const BroadcastPlan& plan) { return plan.getId() == id; }),
            plans_.end()
        );
    }

    [[nodiscard]] BroadcastPlan* findPlan(int id) {
        auto it = std::find_if(plans_.begin(), plans_.end(),
            [id](const BroadcastPlan& plan) { return plan.getId() == id; });
        return (it != plans_.end()) ? &(*it) : nullptr;
    }

private:
    std::filesystem::path configPath_;
    std::vector<BroadcastPlan> plans_;
};

//By GST ARMV8 GCC13.2 BroadcastPlan.hpp