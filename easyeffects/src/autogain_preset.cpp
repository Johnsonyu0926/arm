#include "autogain_preset.hpp"
#include <gio/gio.h>
#include <glib.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

AutoGainPreset::AutoGainPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::autogain::id,
                       tags::schema::autogain::input_path,
                       tags::schema::autogain::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::autogain).append("#").append(util::to_string(index));
}

void AutoGainPreset::save(nlohmann::json& json) {
  auto set_json_value = [&](const char* key, auto value) {
    json[section][instance_name][key] = value;
  };

  set_json_value("bypass", g_settings_get_boolean(settings, "bypass") != 0);
  set_json_value("input-gain", g_settings_get_double(settings, "input-gain"));
  set_json_value("output-gain", g_settings_get_double(settings, "output-gain"));
  set_json_value("target", g_settings_get_double(settings, "target"));
  set_json_value("silence-threshold", g_settings_get_double(settings, "silence-threshold"));
  set_json_value("maximum-history", g_settings_get_int(settings, "maximum-history"));
  set_json_value("reference", util::gsettings_get_string(settings, "reference"));
}

void AutoGainPreset::load(const nlohmann::json& json) {
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
  update_setting("target", "target", 0.0);
  update_setting("silence-threshold", "silence-threshold", 0.0);
  update_setting("maximum-history", "maximum-history", 0);
  update_setting("reference", "reference", "");
}

// Optimized on 2024/10/27