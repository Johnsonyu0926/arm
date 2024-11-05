// Filename: stereo_tools_preset.cpp
#include "stereo_tools_preset.hpp"
#include <gio/gio.h>
#include <glib.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

StereoToolsPreset::StereoToolsPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::stereo_tools::id,
                       tags::schema::stereo_tools::input_path,
                       tags::schema::stereo_tools::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::stereo_tools).append("#").append(util::to_string(index));
}

void StereoToolsPreset::save(nlohmann::json& json) {
  auto& instance = json[section][instance_name];
  instance["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;
  instance["input-gain"] = g_settings_get_double(settings, "input-gain");
  instance["output-gain"] = g_settings_get_double(settings, "output-gain");
  instance["balance-in"] = g_settings_get_double(settings, "balance-in");
  instance["balance-out"] = g_settings_get_double(settings, "balance-out");
  instance["softclip"] = g_settings_get_boolean(settings, "softclip") != 0;
  instance["mutel"] = g_settings_get_boolean(settings, "mutel") != 0;
  instance["muter"] = g_settings_get_boolean(settings, "muter") != 0;
  instance["phasel"] = g_settings_get_boolean(settings, "phasel") != 0;
  instance["phaser"] = g_settings_get_boolean(settings, "phaser") != 0;
  instance["mode"] = util::gsettings_get_string(settings, "mode");
  instance["side-level"] = g_settings_get_double(settings, "slev");
  instance["side-balance"] = g_settings_get_double(settings, "sbal");
  instance["middle-level"] = g_settings_get_double(settings, "mlev");
  instance["middle-panorama"] = g_settings_get_double(settings, "mpan");
  instance["stereo-base"] = g_settings_get_double(settings, "stereo-base");
  instance["delay"] = g_settings_get_double(settings, "delay");
  instance["sc-level"] = g_settings_get_double(settings, "sc-level");
  instance["stereo-phase"] = g_settings_get_double(settings, "stereo-phase");
}

void StereoToolsPreset::load(const nlohmann::json& json) {
  const auto& instance = json.at(section).at(instance_name);
  update_key<bool>(instance, settings, "bypass", "bypass");
  update_key<double>(instance, settings, "input-gain", "input-gain");
  update_key<double>(instance, settings, "output-gain", "output-gain");
  update_key<double>(instance, settings, "balance-in", "balance-in");
  update_key<double>(instance, settings, "balance-out", "balance-out");
  update_key<bool>(instance, settings, "softclip", "softclip");
  update_key<bool>(instance, settings, "mutel", "mutel");
  update_key<bool>(instance, settings, "muter", "muter");
  update_key<bool>(instance, settings, "phasel", "phasel");
  update_key<bool>(instance, settings, "phaser", "phaser");
  update_key<gchar*>(instance, settings, "mode", "mode");
  update_key<double>(instance, settings, "slev", "side-level");
  update_key<double>(instance, settings, "sbal", "side-balance");
  update_key<double>(instance, settings, "mlev", "middle-level");
  update_key<double>(instance, settings, "mpan", "middle-panorama");
  update_key<double>(instance, settings, "stereo-base", "stereo-base");
  update_key<double>(instance, settings, "delay", "delay");
  update_key<double>(instance, settings, "sc-level", "sc-level");
  update_key<double>(instance, settings, "stereo-phase", "stereo-phase");
}

// By GST @2024/10/27