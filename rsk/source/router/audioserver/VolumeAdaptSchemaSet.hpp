// VolumeAdaptSchemaSet.h
#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

class VolumeAdaptSchemaSet {
public:
    VolumeAdaptSchemaSet() = default;
    ~VolumeAdaptSchemaSet() = default;

    VolumeAdaptSchemaSet(const VolumeAdaptSchemaSet&) = delete;
    VolumeAdaptSchemaSet& operator=(const VolumeAdaptSchemaSet&) = delete;
    VolumeAdaptSchemaSet(VolumeAdaptSchemaSet&&) noexcept = default;
    VolumeAdaptSchemaSet& operator=(VolumeAdaptSchemaSet&&) noexcept = default;

    [[nodiscard]] std::string_view get_schema() const noexcept { return schema; }
    void set_schema(std::string_view new_schema) { schema = new_schema; }

    [[nodiscard]] bool load_file() {
        if (!std::filesystem::exists(SCHEMACONFIG)) {
            std::cerr << "Config file does not exist: " << SCHEMACONFIG << std::endl;
            return false;
        }

        std::ifstream i(SCHEMACONFIG);
        if (!i) {
            std::cerr << "Failed to open config file: " << SCHEMACONFIG << std::endl;
            return false;
        }

        try {
            nlohmann::json js = nlohmann::json::parse(i);
            schema = js.at("schema").get<std::string>();
        } catch (const nlohmann::json::exception& ex) {
            std::cerr << "JSON parse error: " << ex.what() << std::endl;
            return false;
        }

        return true;
    }

    [[nodiscard]] bool file_update() const {
        std::ofstream o(SCHEMACONFIG);
        if (!o) {
            std::cerr << "Failed to open config file for writing: " << SCHEMACONFIG << std::endl;
            return false;
        }

        nlohmann::json js;
        js["schema"] = schema;

        o << js.dump(4) << std::endl;
        return o.good();
    }

private:
    static constexpr std::string_view SCHEMACONFIG = "/mnt/cfg/schema.json";
    std::string schema = "default";
};

//BY GST ARMV8 GCC 13.2