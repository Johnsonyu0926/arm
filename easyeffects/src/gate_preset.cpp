// Filename: gate_preset.hpp
#include "gate_preset.hpp"
#include <gio/gio.h>
#include <glib.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

// By GST @2024/10/26
GatePreset::GatePreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::gate::id,
                       tags::schema::gate::input_path,
                       tags::schema::gate::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::gate).append("#").append(util::to_string(index));
}

void GatePreset::save(nlohmann::json& json) {
  auto& instance = json[section][instance_name];
  instance["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;
  instance["input-gain"] = g_settings_get_double(settings, "input-gain");
  instance["output-gain"] = g_settings_get_double(settings, "output-gain");
  instance["dry"] = g_settings_get_double(settings, "dry");
  instance["wet"] = g_settings_get_double(settings, "wet");
  instance["attack"] = g_settings_get_double(settings, "attack");
  instance["release"] = g_settings_get_double(settings, "release");
  instance["curve-threshold"] = g_settings_get_double(settings, "curve-threshold");
  instance["curve-zone"] = g_settings_get_double(settings, "curve-zone");
  instance["hysteresis"] = g_settings_get_boolean(settings, "hysteresis") != 0;
  instance["hysteresis-threshold"] = g_settings_get_double(settings, "hysteresis-threshold");
  instance["hysteresis-zone"] = g_settings_get_double(settings, "hysteresis-zone");
  instance["reduction"] = g_settings_get_double(settings, "reduction");
  instance["makeup"] = g_settings_get_double(settings, "makeup");
  instance["stereo-split"] = g_settings_get_boolean(settings, "stereo-split") != 0;
  instance["sidechain"]["input"] = util::gsettings_get_string(settings, "sidechain-input");
  instance["sidechain"]["mode"] = util::gsettings_get_string(settings, "sidechain-mode");
  instance["sidechain"]["source"] = util::gsettings_get_string(settings, "sidechain-source");
  instance["sidechain"]["stereo-split-source"] = util::gsettings_get_string(settings, "stereo-split-source");
  instance["sidechain"]["preamp"] = g_settings_get_double(settings, "sidechain-preamp");
  instance["sidechain"]["reactivity"] = g_settings_get_double(settings, "sidechain-reactivity");
  instance["sidechain"]["lookahead"] = g_settings_get_double(settings, "sidechain-lookahead");
  instance["hpf-mode"] = util::gsettings_get_string(settings, "hpf-mode");
  instance["hpf-frequency"] = g_settings_get_double(settings, "hpf-frequency");
  instance["lpf-mode"] = util::gsettings_get_string(settings, "lpf-mode");
  instance["lpf-frequency"] = g_settings_get_double(settings, "lpf-frequency");
}

void GatePreset::load(const nlohmann::json& json) {
  const auto& instance = json.at(section).at(instance_name);
  update_key<bool>(instance, settings, "bypass", "bypass");
  update_key<double>(instance, settings, "input-gain", "input-gain");
  update_key<double>(instance, settings, "output-gain", "output-gain");
  update_key<double>(instance, settings, "dry", "dry");
  update_key<double>(instance, settings, "wet", "wet");
  update_key<double>(instance, settings, "attack", "attack");
  update_key<double>(instance, settings, "release", "release");
  update_key<double>(instance, settings, "curve-threshold", "curve-threshold");
  update_key<double>(instance, settings, "curve-zone", "curve-zone");
  update_key<bool>(instance, settings, "hysteresis", "hysteresis");
  update_key<double>(instance, settings, "hysteresis-threshold", "hysteresis-threshold");
  update_key<double>(instance, settings, "hysteresis-zone", "hysteresis-zone");
  update_key<double>(instance, settings, "reduction", "reduction");
  update_key<double>(instance, settings, "makeup", "makeup");
  update_key<bool>(instance, settings, "stereo-split", "stereo-split");
  update_key<gchar*>(instance.at("sidechain"), settings, "sidechain-input", "input");
  update_key<gchar*>(instance.at("sidechain"), settings, "sidechain-mode", "mode");
  update_key<gchar*>(instance.at("sidechain"), settings, "stereo-split-source", "stereo-split-source");
  update_key<gchar*>(instance.at("sidechain"), settings, "sidechain-source", "source");
  update_key<double>(instance.at("sidechain"), settings, "sidechain-preamp", "preamp");
  update_key<double>(instance.at("sidechain"), settings, "sidechain-reactivity", "reactivity");
  update_key<double>(instance.at("sidechain"), settings, "sidechain-lookahead", "lookahead");
  update_key<gchar*>(instance, settings, "hpf-mode", "hpf-mode");
  update_key<double>(instance, settings, "hpf-frequency", "hpf-frequency");
  update_key<gchar*>(instance, settings, "lpf-mode", "lpf-mode");
  update_key<double>(instance, settings, "lpf-frequency", "lpf-frequency");
}