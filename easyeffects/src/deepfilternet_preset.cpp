#include "deepfilternet_preset.hpp"
#include <gio/gio.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

// Filename: deepfilternet_preset.cpp
// Score: 98

DeepFilterNetPreset::DeepFilterNetPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::deepfilternet::id,
                       tags::schema::deepfilternet::input_path,
                       tags::schema::deepfilternet::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::deepfilternet).append("#").append(util::to_string(index));
}

void DeepFilterNetPreset::save(nlohmann::json& json) {
  json[section][instance_name]["attenuation-limit"] = g_settings_get_double(settings, "attenuation-limit");
  json[section][instance_name]["min-processing-threshold"] = g_settings_get_double(settings, "min-processing-threshold");
  json[section][instance_name]["max-erb-processing-threshold"] = g_settings_get_double(settings, "max-erb-processing-threshold");
  json[section][instance_name]["max-df-processing-threshold"] = g_settings_get_double(settings, "max-df-processing-threshold");
  json[section][instance_name]["min-processing-buffer"] = g_settings_get_int(settings, "min-processing-buffer");
  json[section][instance_name]["post-filter-beta"] = g_settings_get_double(settings, "post-filter-beta");
}

void DeepFilterNetPreset::load(const nlohmann::json& json) {
  update_key<double>(json.at(section).at(instance_name), settings, "attenuation-limit", "attenuation-limit");
  update_key<double>(json.at(section).at(instance_name), settings, "min-processing-threshold", "min-processing-threshold");
  update_key<double>(json.at(section).at(instance_name), settings, "max-erb-processing-threshold", "max-erb-processing-threshold");
  update_key<double>(json.at(section).at(instance_name), settings, "max-df-processing-threshold", "max-df-processing-threshold");
  update_key<int>(json.at(section).at(instance_name), settings, "min-processing-buffer", "min-processing-buffer");
  update_key<double>(json.at(section).at(instance_name), settings, "post-filter-beta", "post-filter-beta");
}

// By GST @2024/10/26