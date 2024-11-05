// Filename: bass_loudness_preset.cpp
// 优化日期: 2024/10/26

#include "bass_loudness_preset.hpp"
#include <gio/gio.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

BassLoudnessPreset::BassLoudnessPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::bass_loudness::id,
                       tags::schema::bass_loudness::input_path,
                       tags::schema::bass_loudness::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::bass_loudness).append("#").append(util::to_string(index));
}

void BassLoudnessPreset::save(nlohmann::json& json) {
  try {
    json[section][instance_name]["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;
    json[section][instance_name]["input-gain"] = g_settings_get_double(settings, "input-gain");
    json[section][instance_name]["output-gain"] = g_settings_get_double(settings, "output-gain");
    json[section][instance_name]["loudness"] = g_settings_get_double(settings, "loudness");
    json[section][instance_name]["output"] = g_settings_get_double(settings, "output");
    json[section][instance_name]["link"] = g_settings_get_double(settings, "link");
  } catch (const std::exception& e) {
    util::debug("Error saving BassLoudnessPreset: " + std::string(e.what()));
  }
}

void BassLoudnessPreset::load(const nlohmann::json& json) {
  try {
    update_key<bool>(json.at(section).at(instance_name), settings, "bypass", "bypass");
    update_key<double>(json.at(section).at(instance_name), settings, "input-gain", "input-gain");
    update_key<double>(json.at(section).at(instance_name), settings, "output-gain", "output-gain");
    update_key<double>(json.at(section).at(instance_name), settings, "loudness", "loudness");
    update_key<double>(json.at(section).at(instance_name), settings, "output", "output");
    update_key<double>(json.at(section).at(instance_name), settings, "link", "link");
  } catch (const std::exception& e) {
    util::debug("Error loading BassLoudnessPreset: " + std::string(e.what()));
  }
}

// By GST @2024/10/26