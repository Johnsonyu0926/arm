// Filename: reverb_preset.cpp
#include "reverb_preset.hpp"
#include <gio/gio.h>
#include <glib.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

// Score: 98

ReverbPreset::ReverbPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::reverb::id,
                       tags::schema::reverb::input_path,
                       tags::schema::reverb::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::reverb).append("#").append(util::to_string(index));
}

void ReverbPreset::save(nlohmann::json& json) {
  auto& instance = json[section][instance_name];
  instance["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;
  instance["input-gain"] = g_settings_get_double(settings, "input-gain");
  instance["output-gain"] = g_settings_get_double(settings, "output-gain");
  instance["room-size"] = util::gsettings_get_string(settings, "room-size");
  instance["decay-time"] = g_settings_get_double(settings, "decay-time");
  instance["hf-damp"] = g_settings_get_double(settings, "hf-damp");
  instance["diffusion"] = g_settings_get_double(settings, "diffusion");
  instance["amount"] = g_settings_get_double(settings, "amount");
  instance["dry"] = g_settings_get_double(settings, "dry");
  instance["predelay"] = g_settings_get_double(settings, "predelay");
  instance["bass-cut"] = g_settings_get_double(settings, "bass-cut");
  instance["treble-cut"] = g_settings_get_double(settings, "treble-cut");
}

void ReverbPreset::load(const nlohmann::json& json) {
  const auto& instance = json.at(section).at(instance_name);
  update_key<bool>(instance, settings, "bypass", "bypass");
  update_key<double>(instance, settings, "input-gain", "input-gain");
  update_key<double>(instance, settings, "output-gain", "output-gain");
  update_key<gchar*>(instance, settings, "room-size", "room-size");
  update_key<double>(instance, settings, "decay-time", "decay-time");
  update_key<double>(instance, settings, "hf-damp", "hf-damp");
  update_key<double>(instance, settings, "diffusion", "diffusion");
  update_key<double>(instance, settings, "amount", "amount");
  update_key<double>(instance, settings, "dry", "dry");
  update_key<double>(instance, settings, "predelay", "predelay");
  update_key<double>(instance, settings, "bass-cut", "bass-cut");
  update_key<double>(instance, settings, "treble-cut", "treble-cut");
}

// By GST @2024/10/27