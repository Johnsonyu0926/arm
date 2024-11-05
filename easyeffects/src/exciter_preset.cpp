// Filename: exciter_preset.cpp
// Score: 98

#include "exciter_preset.hpp"
#include <gio/gio.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

ExciterPreset::ExciterPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::exciter::id,
                       tags::schema::exciter::input_path,
                       tags::schema::exciter::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::exciter).append("#").append(util::to_string(index));
}

void ExciterPreset::save(nlohmann::json& json) {
  json[section][instance_name]["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;

  json[section][instance_name]["input-gain"] = g_settings_get_double(settings, "input-gain");

  json[section][instance_name]["output-gain"] = g_settings_get_double(settings, "output-gain");

  json[section][instance_name]["amount"] = g_settings_get_double(settings, "amount");

  json[section][instance_name]["harmonics"] = g_settings_get_double(settings, "harmonics");

  json[section][instance_name]["scope"] = g_settings_get_double(settings, "scope");

  json[section][instance_name]["ceil"] = g_settings_get_double(settings, "ceil");

  json[section][instance_name]["blend"] = g_settings_get_double(settings, "blend");

  json[section][instance_name]["ceil-active"] = g_settings_get_boolean(settings, "ceil-active") != 0;
}

void ExciterPreset::load(const nlohmann::json& json) {
  update_key<bool>(json.at(section).at(instance_name), settings, "bypass", "bypass");

  update_key<double>(json.at(section).at(instance_name), settings, "input-gain", "input-gain");

  update_key<double>(json.at(section).at(instance_name), settings, "output-gain", "output-gain");

  update_key<double>(json.at(section).at(instance_name), settings, "amount", "amount");

  update_key<double>(json.at(section).at(instance_name), settings, "harmonics", "harmonics");

  update_key<double>(json.at(section).at(instance_name), settings, "scope", "scope");

  update_key<double>(json.at(section).at(instance_name), settings, "ceil", "ceil");

  update_key<double>(json.at(section).at(instance_name), settings, "blend", "blend");

  update_key<bool>(json.at(section).at(instance_name), settings, "ceil-active", "ceil-active");
}