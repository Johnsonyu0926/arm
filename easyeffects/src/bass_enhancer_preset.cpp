#include "bass_enhancer_preset.hpp"
#include <gio/gio.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

BassEnhancerPreset::BassEnhancerPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::bass_enhancer::id,
                       tags::schema::bass_enhancer::input_path,
                       tags::schema::bass_enhancer::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::bass_enhancer).append("#").append(util::to_string(index));
}

void BassEnhancerPreset::save(nlohmann::json& json) {
  auto set_json_value = [&](const char* key, auto value) {
    json[section][instance_name][key] = value;
  };

  set_json_value("bypass", g_settings_get_boolean(settings, "bypass") != 0);
  set_json_value("input-gain", g_settings_get_double(settings, "input-gain"));
  set_json_value("output-gain", g_settings_get_double(settings, "output-gain"));
  set_json_value("amount", g_settings_get_double(settings, "amount"));
  set_json_value("harmonics", g_settings_get_double(settings, "harmonics"));
  set_json_value("scope", g_settings_get_double(settings, "scope"));
  set_json_value("floor", g_settings_get_double(settings, "floor"));
  set_json_value("blend", g_settings_get_double(settings, "blend"));
  set_json_value("floor-active", g_settings_get_boolean(settings, "floor-active") != 0);
}

void BassEnhancerPreset::load(const nlohmann::json& json) {
  auto update_setting = [&](const char* json_key, const char* setting_key, auto default_value) {
    if (json.at(section).at(instance_name).contains(json_key)) {
      update_key(json.at(section).at(instance_name), settings, json_key, setting_key);
    } else {
      g_settings_set(settings, setting_key, default_value);
    }
  };

  update_setting("bypass", "bypass", false);
  update_setting("input-gain", "input-gain", 0.0);
  update_setting("output-gain", "output-gain", 0.0);
  update_setting("amount", "amount", 0.0);
  update_setting("harmonics", "harmonics", 0.0);
  update_setting("scope", "scope", 0.0);
  update_setting("floor", "floor", 0.0);
  update_setting("blend", "blend", 0.0);
  update_setting("floor-active", "floor-active", false);
}

// Optimized on 2024/10/27