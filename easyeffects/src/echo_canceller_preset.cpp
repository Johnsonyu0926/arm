#include "echo_canceller_preset.hpp"
#include <gio/gio.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

// Filename: echo_canceller_preset.cpp
// Score: 98

EchoCancellerPreset::EchoCancellerPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::echo_canceller::id,
                       tags::schema::echo_canceller::input_path,
                       tags::schema::echo_canceller::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::echo_canceller).append("#").append(util::to_string(index));
}

void EchoCancellerPreset::save(nlohmann::json& json) {
  json[section][instance_name]["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;
  json[section][instance_name]["input-gain"] = g_settings_get_double(settings, "input-gain");
  json[section][instance_name]["output-gain"] = g_settings_get_double(settings, "output-gain");
  json[section][instance_name]["filter-length"] = g_settings_get_int(settings, "filter-length");
  json[section][instance_name]["residual-echo-suppression"] = g_settings_get_int(settings, "residual-echo-suppression");
  json[section][instance_name]["near-end-suppression"] = g_settings_get_int(settings, "near-end-suppression");
}

void EchoCancellerPreset::load(const nlohmann::json& json) {
  update_key<bool>(json.at(section).at(instance_name), settings, "bypass", "bypass");
  update_key<double>(json.at(section).at(instance_name), settings, "input-gain", "input-gain");
  update_key<double>(json.at(section).at(instance_name), settings, "output-gain", "output-gain");
  update_key<int>(json.at(section).at(instance_name), settings, "filter-length", "filter-length");
  update_key<int>(json.at(section).at(instance_name), settings, "residual-echo-suppression", "residual-echo-suppression");
  update_key<int>(json.at(section).at(instance_name), settings, "near-end-suppression", "near-end-suppression");
}

// By GST @2024/10/26