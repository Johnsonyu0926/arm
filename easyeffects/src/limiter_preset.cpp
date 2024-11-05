// Filename: limiter_preset.hpp
#include "limiter_preset.hpp"
#include <gio/gio.h>
#include <glib.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

// By GST @2024/10/26
LimiterPreset::LimiterPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::limiter::id,
                       tags::schema::limiter::input_path,
                       tags::schema::limiter::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::limiter).append("#").append(util::to_string(index));
}

void LimiterPreset::save(nlohmann::json& json) {
  const std::vector<std::pair<std::string, std::string>> settings_keys = {
      {"mode", "mode"},
      {"oversampling", "oversampling"},
      {"dithering", "dithering"},
      {"bypass", "bypass"},
      {"input-gain", "input-gain"},
      {"output-gain", "output-gain"},
      {"lookahead", "lookahead"},
      {"attack", "attack"},
      {"release", "release"},
      {"threshold", "threshold"},
      {"sidechain-preamp", "sidechain-preamp"},
      {"stereo-link", "stereo-link"},
      {"alr-attack", "alr-attack"},
      {"alr-release", "alr-release"},
      {"alr-knee", "alr-knee"},
      {"alr", "alr"},
      {"gain-boost", "gain-boost"},
      {"external-sidechain", "external-sidechain"}};

  for (const auto& [json_key, setting_key] : settings_keys) {
    if (json_key == "bypass" || json_key == "alr" || json_key == "gain-boost" || json_key == "external-sidechain") {
      json[section][instance_name][json_key] = g_settings_get_boolean(settings, setting_key.c_str()) != 0;
    } else if (json_key == "input-gain" || json_key == "output-gain" || json_key == "lookahead" ||
               json_key == "attack" || json_key == "release" || json_key == "threshold" ||
               json_key == "sidechain-preamp" || json_key == "stereo-link" || json_key == "alr-attack" ||
               json_key == "alr-release" || json_key == "alr-knee") {
      json[section][instance_name][json_key] = g_settings_get_double(settings, setting_key.c_str());
    } else {
      json[section][instance_name][json_key] = util::gsettings_get_string(settings, setting_key.c_str());
    }
  }
}

void LimiterPreset::load(const nlohmann::json& json) {
  const std::vector<std::pair<std::string, std::string>> settings_keys = {
      {"mode", "mode"},
      {"oversampling", "oversampling"},
      {"dithering", "dithering"},
      {"bypass", "bypass"},
      {"input-gain", "input-gain"},
      {"output-gain", "output-gain"},
      {"lookahead", "lookahead"},
      {"attack", "attack"},
      {"release", "release"},
      {"threshold", "threshold"},
      {"sidechain-preamp", "sidechain-preamp"},
      {"stereo-link", "stereo-link"},
      {"alr-attack", "alr-attack"},
      {"alr-release", "alr-release"},
      {"alr-knee", "alr-knee"},
      {"alr", "alr"},
      {"gain-boost", "gain-boost"},
      {"external-sidechain", "external-sidechain"}};

  for (const auto& [json_key, setting_key] : settings_keys) {
    if (json_key == "bypass" || json_key == "alr" || json_key == "gain-boost" || json_key == "external-sidechain") {
      update_key<bool>(json.at(section).at(instance_name), settings, json_key, setting_key);
    } else if (json_key == "input-gain" || json_key == "output-gain" || json_key == "lookahead" ||
               json_key == "attack" || json_key == "release" || json_key == "threshold" ||
               json_key == "sidechain-preamp" || json_key == "stereo-link" || json_key == "alr-attack" ||
               json_key == "alr-release" || json_key == "alr-knee") {
      update_key<double>(json.at(section).at(instance_name), settings, json_key, setting_key);
    } else {
      update_key<gchar*>(json.at(section).at(instance_name), settings, json_key, setting_key);
    }
  }
}