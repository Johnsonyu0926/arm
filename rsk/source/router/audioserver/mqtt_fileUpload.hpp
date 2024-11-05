// mqtt_fileupload.hpp
#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <iostream>
#include <memory>
#include <functional>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include "mqtt.hpp"

class MqttFileUpload {
public:
    MqttFileUpload(std::string clientId, std::string host, int port)
        : clientId(std::move(clientId)), host(std::move(host)), port(port) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    ~MqttFileUpload() {
        curl_global_cleanup();
    }

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

    [[nodiscard]] nlohmann::json handleFileUploadCommand(const nlohmann::json& command) {
        std::string downloadUrl = command["downloadUrl"];
        std::string fileName = command["fileName"];
        int audioUploadRecordId = command["audioUploadRecordId"];
        int storageType = command["storageType"];

        bool success = downloadFile(downloadUrl, fileName);

        nlohmann::json response = {
            {"result", success ? "文件上传成功" : "文件上传失败"},
            {"resultId", success ? 1 : 2},
            {"imei", command["imei"]},
            {"topic", "TaDiao/device/report/test/" + command["imei"].get<std::string>()},
            {"cmd", "fileUpload"},
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

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    bool downloadFile(const std::string& url, const std::string& fileName) {
        CURL* curl = curl_easy_init();
        if (!curl) {
            std::cerr << "Failed to initialize CURL" << std::endl;
            return false;
        }

        std::string readBuffer;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "Failed to download file: " << curl_easy_strerror(res) << std::endl;
            return false;
        }

        std::ofstream outFile(fileName, std::ios::binary);
        if (!outFile) {
            std::cerr << "Failed to open file for writing: " << fileName << std::endl;
            return false;
        }

        outFile.write(readBuffer.data(), readBuffer.size());
        return true;
    }
};

//BY GST ARMV8 GCC 13.2