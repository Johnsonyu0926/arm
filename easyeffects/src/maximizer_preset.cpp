// Filename: maximizer_preset.cpp
#include "maximizer_preset.hpp"
#include <gio/gio.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

// By GST @2024/10/26
MaximizerPreset::MaximizerPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::maximizer::id,
                       tags::schema::maximizer::input_path,
                       tags::schema::maximizer::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::maximizer).append("#").append(util::to_string(index));
}

void MaximizerPreset::save(nlohmann::json& json) {
  auto& instance_json = json[section][instance_name];
  instance_json["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;
  instance_json["input-gain"] = g_settings_get_double(settings, "input-gain");
  instance_json["output-gain"] = g_settings_get_double(settings, "output-gain");
  instance_json["release"] = g_settings_get_double(settings, "release");
  instance_json["threshold"] = g_settings_get_double(settings, "threshold");
}

void MaximizerPreset::load(const nlohmann::json& json) {
  const auto& instance_json = json.at(section).at(instance_name);
  update_key<bool>(instance_json, settings, "bypass", "bypass");
  update_key<double>(instance_json, settings, "input-gain", "input-gain");
  update_key<double>(instance_json, settings, "output-gain", "output-gain");
  update_key<double>(instance_json, settings, "release", "release");
  update_key<double>(instance_json, settings, "threshold", "threshold");
}