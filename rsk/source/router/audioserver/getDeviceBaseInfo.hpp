// getDeviceBaseInfo.hpp
#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

class DeviceBaseInfo {
public:
    DeviceBaseInfo() = default;
    DeviceBaseInfo(std::string deviceId, std::string deviceType, std::string firmwareVersion)
        : deviceId_(std::move(deviceId)), deviceType_(std::move(deviceType)), firmwareVersion_(std::move(firmwareVersion)) {}

    [[nodiscard]] std::string_view getDeviceId() const { return deviceId_; }
    void setDeviceId(std::string newDeviceId) { deviceId_ = std::move(newDeviceId); }

    [[nodiscard]] std::string_view getDeviceType() const { return deviceType_; }
    void setDeviceType(std::string newDeviceType) { deviceType_ = std::move(newDeviceType); }

    [[nodiscard]] std::string_view getFirmwareVersion() const { return firmwareVersion_; }
    void setFirmwareVersion(std::string newFirmwareVersion) { firmwareVersion_ = std::move(newFirmwareVersion); }

    [[nodiscard]] nlohmann::json toJson() const {
        return nlohmann::json{
            {"deviceId", deviceId_},
            {"deviceType", deviceType_},
            {"firmwareVersion", firmwareVersion_}
        };
    }

    static DeviceBaseInfo fromJson(const nlohmann::json& j) {
        return DeviceBaseInfo(
            j.at("deviceId").get<std::string>(),
            j.at("deviceType").get<std::string>(),
            j.at("firmwareVersion").get<std::string>()
        );
    }

private:
    std::string deviceId_;
    std::string deviceType_;
    std::string firmwareVersion_;
};

class DeviceBaseInfoManager {
public:
    explicit DeviceBaseInfoManager(std::string configPath) : configPath_(std::move(configPath)) {}

    [[nodiscard]] bool load() {
        try {
            std::ifstream configFile(configPath_);
            if (!configFile.is_open()) {
                std::cerr << "Failed to open config file: " << configPath_ << std::endl;
                return false;
            }

            nlohmann::json j = nlohmann::json::parse(configFile);
            deviceInfo_ = DeviceBaseInfo::fromJson(j);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading config file: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] bool save() const {
        try {
            nlohmann::json j = deviceInfo_.toJson();

            std::ofstream configFile(configPath_);
            if (!configFile.is_open()) {
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

    [[nodiscard]] const DeviceBaseInfo& getDeviceInfo() const { return deviceInfo_; }
    void setDeviceInfo(DeviceBaseInfo newDeviceInfo) { deviceInfo_ = std::move(newDeviceInfo); }

private:
    std::string configPath_;
    DeviceBaseInfo deviceInfo_;
};

//By GST ARMV8 GCC13.2 getDeviceBaseInfo.hpp