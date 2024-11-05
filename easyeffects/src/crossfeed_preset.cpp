// Filename: crossfeed_preset.cpp 
// 优化日期: 2024/10/26 

#include "crossfeed_preset.hpp"
#include <gio/gio.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

CrossfeedPreset::CrossfeedPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::crossfeed::id,
                       tags::schema::crossfeed::input_path,
                       tags::schema::crossfeed::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::crossfeed).append("#").append(util::to_string(index));
}

void CrossfeedPreset::save(nlohmann::json& json) {
  if (settings) {
    json[section][instance_name]["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;
    json[section][instance_name]["input-gain"] = g_settings_get_double(settings, "input-gain");
    json[section][instance_name]["output-gain"] = g_settings_get_double(settings, "output-gain");
    json[section][instance_name]["fcut"] = g_settings_get_int(settings, "fcut");
    json[section][instance_name]["feed"] = g_settings_get_double(settings, "feed");
  }
}

void CrossfeedPreset::load(const nlohmann::json& json) {
  if (json.contains(section) && json.at(section).contains(instance_name)) {
    const auto& instance_json = json.at(section).at(instance_name);
    update_key<bool>(instance_json, settings, "bypass", "bypass");
    update_key<double>(instance_json, settings, "input-gain", "input-gain");
    update_key<double>(instance_json, settings, "output-gain", "output-gain");
    update_key<int>(instance_json, settings, "fcut", "fcut");
    update_key<double>(instance_json, settings, "feed", "feed");
  }
}

// By GST @2024/10/26