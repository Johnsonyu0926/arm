// Filename: level_meter_preset.hpp
#include "level_meter_preset.hpp"
#include <gio/gio.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

// By GST @2024/10/26
LevelMeterPreset::LevelMeterPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::level_meter::id,
                       tags::schema::level_meter::input_path,
                       tags::schema::level_meter::output_path,
                       preset_type,
                       index) {
  instance_name = tags::plugin_name::level_meter + "#" + util::to_string(index);
}

void LevelMeterPreset::save(nlohmann::json& json) {
  json[section][instance_name]["bypass"] = g_settings_get_boolean(settings, "bypass");
}

void LevelMeterPreset::load(const nlohmann::json& json) {
  update_key<bool>(json.at(section).at(instance_name), settings, "bypass", "bypass");
}