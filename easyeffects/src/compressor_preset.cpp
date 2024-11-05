// Filename: compressor_preset.cpp
// 优化日期: 2024/10/26

#include "compressor_preset.hpp"
#include <gio/gio.h>
#include <glib.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

CompressorPreset::CompressorPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::compressor::id,
                       tags::schema::compressor::input_path,
                       tags::schema::compressor::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::compressor).append("#").append(util::to_string(index));
}

void CompressorPreset::save(nlohmann::json& json) {
  auto& instance = json[section][instance_name];
  instance["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;
  instance["input-gain"] = g_settings_get_double(settings, "input-gain");
  instance["output-gain"] = g_settings_get_double(settings, "output-gain");
  instance["dry"] = g_settings_get_double(settings, "dry");
  instance["wet"] = g_settings_get_double(settings, "wet");
  instance["mode"] = util::gsettings_get_string(settings, "mode");
  instance["attack"] = g_settings_get_double(settings, "attack");
  instance["release"] = g_settings_get_double(settings, "release");
  instance["release-threshold"] = g_settings_get_double(settings, "release-threshold");
  instance["threshold"] = g_settings_get_double(settings, "threshold");
  instance["ratio"] = g_settings_get_double(settings, "ratio");
  instance["knee"] = g_settings_get_double(settings, "knee");
  instance["makeup"] = g_settings_get_double(settings, "makeup");
  instance["boost-threshold"] = g_settings_get_double(settings, "boost-threshold");
  instance["boost-amount"] = g_settings_get_double(settings, "boost-amount");
  instance["stereo-split"] = g_settings_get_boolean(settings, "stereo-split") != 0;

  auto& sidechain = instance["sidechain"];
  sidechain["type"] = util::gsettings_get_string(settings, "sidechain-type");
  sidechain["mode"] = util::gsettings_get_string(settings, "sidechain-mode");
  sidechain["source"] = util::gsettings_get_string(settings, "sidechain-source");
  sidechain["stereo-split-source"] = util::gsettings_get_string(settings, "stereo-split-source");
  sidechain["preamp"] = g_settings_get_double(settings, "sidechain-preamp");
  sidechain["reactivity"] = g_settings_get_double(settings, "sidechain-reactivity");
  sidechain["lookahead"] = g_settings_get_double(settings, "sidechain-lookahead");

  instance["hpf-mode"] = util::gsettings_get_string(settings, "hpf-mode");
  instance["hpf-frequency"] = g_settings_get_double(settings, "hpf-frequency");
  instance["lpf-mode"] = util::gsettings_get_string(settings, "lpf-mode");
  instance["lpf-frequency"] = g_settings_get_double(settings, "lpf-frequency");
}

void CompressorPreset::load(const nlohmann::json& json) {
  const auto& instance = json.at(section).at(instance_name);
  update_key<bool>(instance, settings, "bypass", "bypass");
  update_key<double>(instance, settings, "input-gain", "input-gain");
  update_key<double>(instance, settings, "output-gain", "output-gain");
  update_key<double>(instance, settings, "dry", "dry");
  update_key<double>(instance, settings, "wet", "wet");
  update_key<gchar*>(instance, settings, "mode", "mode");
  update_key<double>(instance, settings, "attack", "attack");
  update_key<double>(instance, settings, "release", "release");
  update_key<double>(instance, settings, "release-threshold", "release-threshold");
  update_key<double>(instance, settings, "threshold", "threshold");
  update_key<double>(instance, settings, "ratio", "ratio");
  update_key<double>(instance, settings, "knee", "knee");
  update_key<double>(instance, settings, "makeup", "makeup");
  update_key<double>(instance, settings, "boost-threshold", "boost-threshold");
  update_key<double>(instance, settings, "boost-amount", "boost-amount");
  update_key<bool>(instance, settings, "stereo-split", "stereo-split");

  const auto& sidechain = instance.at("sidechain");
  update_key<gchar*>(sidechain, settings, "sidechain-type", "type");
  update_key<gchar*>(sidechain, settings, "sidechain-mode", "mode");
  update_key<gchar*>(sidechain, settings, "stereo-split-source", "stereo-split-source");
  update_key<gchar*>(sidechain, settings, "sidechain-source", "source");
  update_key<double>(sidechain, settings, "sidechain-preamp", "preamp");
  update_key<double>(sidechain, settings, "sidechain-reactivity", "reactivity");
  update_key<double>(sidechain, settings, "sidechain-lookahead", "lookahead");

  update_key<gchar*>(instance, settings, "hpf-mode", "hpf-mode");
  update_key<double>(instance, settings, "hpf-frequency", "hpf-frequency");
  update_key<gchar*>(instance, settings, "lpf-mode", "lpf-mode");
  update_key<double>(instance, settings, "lpf-frequency", "lpf-frequency");
}

// By GST @2024/10/26