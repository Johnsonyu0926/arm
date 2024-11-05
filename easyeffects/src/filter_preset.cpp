// Filename: filter_preset.cpp
// Optimized on: 2024/10/27

#include "filter_preset.hpp"
#include <gio/gio.h>
#include <glib.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

FilterPreset::FilterPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::filter::id,
                       tags::schema::filter::input_path,
                       tags::schema::filter::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::filter).append("#").append(util::to_string(index));
}

void FilterPreset::save(nlohmann::json& json) {
  auto& instance = json[section][instance_name];
  instance["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;
  instance["input-gain"] = g_settings_get_double(settings, "input-gain");
  instance["output-gain"] = g_settings_get_double(settings, "output-gain");
  instance["frequency"] = g_settings_get_double(settings, "frequency");
  instance["width"] = g_settings_get_double(settings, "width");
  instance["gain"] = g_settings_get_double(settings, "gain");
  instance["quality"] = g_settings_get_double(settings, "quality");
  instance["balance"] = g_settings_get_double(settings, "balance");
  instance["type"] = util::gsettings_get_string(settings, "type");
  instance["mode"] = util::gsettings_get_string(settings, "mode");
  instance["equal-mode"] = util::gsettings_get_string(settings, "equal-mode");
  instance["slope"] = util::gsettings_get_string(settings, "slope");
}

void FilterPreset::load(const nlohmann::json& json) {
  const auto& instance = json.at(section).at(instance_name);
  update_key<bool>(instance, settings, "bypass", "bypass");
  update_key<double>(instance, settings, "input-gain", "input-gain");
  update_key<double>(instance, settings, "output-gain", "output-gain");
  update_key<double>(instance, settings, "frequency", "frequency");
  update_key<double>(instance, settings, "width", "width");
  update_key<double>(instance, settings, "gain", "gain");
  update_key<double>(instance, settings, "quality", "quality");
  update_key<double>(instance, settings, "balance", "balance");
  update_key<gchar*>(instance, settings, "type", "type");
  update_key<gchar*>(instance, settings, "mode", "mode");
  update_key<gchar*>(instance, settings, "equal-mode", "equal-mode");
  update_key<gchar*>(instance, settings, "slope", "slope");
}

// Optimized on: 2024/10/27