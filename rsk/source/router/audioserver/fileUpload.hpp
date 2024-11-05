// fileupload.hpp
#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

class FileUpload {
public:
    FileUpload() = default;
    explicit FileUpload(std::string fileName) : fileName_(std::move(fileName)) {}

    [[nodiscard]] std::string_view getFileName() const { return fileName_; }
    void setFileName(std::string newFileName) { fileName_ = std::move(newFileName); }

    [[nodiscard]] bool uploadFile(const std::filesystem::path& sourcePath, const std::filesystem::path& destinationPath) const {
        try {
            if (!std::filesystem::exists(sourcePath)) {
                std::cerr << "Source file does not exist: " << sourcePath << std::endl;
                return false;
            }

            std::filesystem::create_directories(destinationPath.parent_path());

            std::ifstream source(sourcePath, std::ios::binary);
            if (!source) {
                std::cerr << "Failed to open source file: " << sourcePath << std::endl;
                return false;
            }

            std::ofstream dest(destinationPath, std::ios::binary);
            if (!dest) {
                std::cerr << "Failed to open destination file: " << destinationPath << std::endl;
                return false;
            }

            dest << source.rdbuf();

            return dest.good() && source.good();
        } catch (const std::exception& e) {
            std::cerr << "Exception during file upload: " << e.what() << std::endl;
            return false;
        }
    }

    [[nodiscard]] nlohmann::json toJson() const {
        return nlohmann::json{{"fileName", fileName_}};
    }

    static FileUpload fromJson(const nlohmann::json& j) {
        return FileUpload(j.at("fileName").get<std::string>());
    }

private:
    std::string fileName_;
};

//By GST ARMV8 GCC13.2 fileupload.hpp