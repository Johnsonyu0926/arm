// mqtt_filedelete.hpp
#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <iostream>
#include <memory>
#include <functional>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "mqtt.hpp"

class MqttFileDelete {
public:
    MqttFileDelete(std::string clientId, std::string host, int port)
        : clientId(std::move(clientId)), host(std::move(host)), port(port) {}

    void initializeClient() {
        mqttClient = std::make_unique<MqttClient>(clientId, host, port);
    }

    void setCallback(std::function<void(std::string_view topic, std::string_view message)> callback) {
        mqttClient->setMessageCallback(std::move(callback));
    }

    [[nodiscard]] bool connect() {
        return mqttClient->connect();
    }

    void disconnect() {
        mqttClient->disconnect();
    }

    [[nodiscard]] bool subscribe(std::string_view topic) {
        return mqttClient->subscribe(topic);
    }

    [[nodiscard]] bool publish(std::string_view topic, std::string_view message) {
        return mqttClient->publish(topic, message);
    }

    void loopForever() {
        mqttClient->loopForever();
    }

    [[nodiscard]] nlohmann::json handleFileDeleteCommand(const nlohmann::json& command) {
        std::string fileName = command["fileName"];
        int audioUploadRecordId = command["audioUploadRecordId"];
        int storageType = command["storageType"];

        bool success = deleteFile(fileName, storageType);

        nlohmann::json response = {
            {"result", success ? "文件删除成功" : "文件删除失败"},
            {"resultId", success ? 1 : 2},
            {"imei", command["imei"]},
            {"topic", "TaDiao/device/report/test/" + command["imei"].get<std::string>()},
            {"cmd", "fileDelete"},
            {"publishId", command["publishId"]},
            {"data", {{"audioUploadRecordId", audioUploadRecordId}}}
        };
        return response;
    }

private:
    std::string clientId;
    std::string host;
    int port;
    std::unique_ptr<MqttClient> mqttClient;

    bool deleteFile(const std::string& fileName, int storageType) {
        std::filesystem::path filePath;
        switch (storageType) {
            case 1:
                filePath = "/path/to/storage1/" + fileName;
                break;
            case 2:
                filePath = "/path/to/storage2/" + fileName;
                break;
            // Add more cases if needed
            default:
                std::cerr << "Unknown storage type: " << storageType << std::endl;
                return false;
        }

        try {
            if (std::filesystem::remove(filePath)) {
                std::cout << "File deleted successfully: " << filePath << std::endl;
                return true;
            } else {
                std::cerr << "File does not exist: " << filePath << std::endl;
                return false;
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error deleting file: " << e.what() << std::endl;
            return false;
        }
    }
};

//BY GST ARMV8 GCC 13.2