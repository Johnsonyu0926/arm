// Filename: loudness_preset.hpp
#include "loudness_preset.hpp"
#include <gio/gio.h>
#include <glib.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

// By GST @2024/10/26
LoudnessPreset::LoudnessPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::loudness::id,
                       tags::schema::loudness::input_path,
                       tags::schema::loudness::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::loudness).append("#").append(util::to_string(index));
}

void LoudnessPreset::save(nlohmann::json& json) {
  const std::vector<std::pair<std::string, std::string>> settings_keys = {
      {"bypass", "bypass"},
      {"input-gain", "input-gain"},
      {"output-gain", "output-gain"},
      {"fft", "fft"},
      {"std", "std"},
      {"volume", "volume"},
      {"clipping", "clipping"},
      {"clipping-range", "clipping-range"}};

  for (const auto& [json_key, setting_key] : settings_keys) {
    if (json_key == "bypass" || json_key == "clipping") {
      json[section][instance_name][json_key] = g_settings_get_boolean(settings, setting_key.c_str()) != 0;
    } else if (json_key == "input-gain" || json_key == "output-gain" || json_key == "volume" ||
               json_key == "clipping-range") {
      json[section][instance_name][json_key] = g_settings_get_double(settings, setting_key.c_str());
    } else {
      json[section][instance_name][json_key] = util::gsettings_get_string(settings, setting_key.c_str());
    }
  }
}

void LoudnessPreset::load(const nlohmann::json& json) {
  const std::vector<std::pair<std::string, std::string>> settings_keys = {
      {"bypass", "bypass"},
      {"input-gain", "input-gain"},
      {"output-gain", "output-gain"},
      {"fft", "fft"},
      {"std", "std"},
      {"volume", "volume"},
      {"clipping", "clipping"},
      {"clipping-range", "clipping-range"}};

  for (const auto& [json_key, setting_key] : settings_keys) {
    if (json_key == "bypass" || json_key == "clipping") {
      update_key<bool>(json.at(section).at(instance_name), settings, json_key, setting_key);
    } else if (json_key == "input-gain" || json_key == "output-gain" || json_key == "volume" ||
               json_key == "clipping-range") {
      update_key<double>(json.at(section).at(instance_name), settings, json_key, setting_key);
    } else {
      update_key<gchar*>(json.at(section).at(instance_name), settings, json_key, setting_key);
    }
  }
}