
#pragma once

#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"

class LimiterPreset : public PluginPresetBase {
 public:
  explicit LimiterPreset(PresetType preset_type, int index = 0);

  void save_to_file(const std::string& file_path);
  void load_from_file(const std::string& file_path);

 private:
  void save(nlohmann::json& json) override;
  void load(const nlohmann::json& json) override;
};

/*
 *  优化日期: 2023年10月27日

### 优化后的实现文件

#### `limiter_preset.cpp`
/*
 *  优化日期: 2023年10月27日
 */
/*
#include "limiter_preset.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

LimiterPreset::LimiterPreset(PresetType preset_type, int index)
    : PluginPresetBase(preset_type, index) {
    // Constructor implementation
}

void LimiterPreset::save(nlohmann::json& json) {
    json["preset_type"] = static_cast<int>(get_preset_type());
    json["index"] = get_index();
    // Additional saving logic for limiter settings
}

void LimiterPreset::load(const nlohmann::json& json) {
    if (json.contains("preset_type") && json.contains("index")) {
        set_preset_type(static_cast<PresetType>(json["preset_type"].get<int>()));
        set_index(json["index"].get<int>());
        // Additional loading logic for limiter settings
    } else {
        throw std::invalid_argument("Required JSON fields are missing");
    }
}

void LimiterPreset::save_to_file(const std::string& file_path) {
    nlohmann::json json;
    save(json);
    std::ofstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for writing: " + file_path);
    }
    file << json.dump(4); // 格式化输出为4个空格缩进
}

void LimiterPreset::load_from_file(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for reading: " + file_path);
    }
    nlohmann::json json;
    file >> json;
    load(json);
}