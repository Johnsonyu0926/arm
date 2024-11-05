// Filename: speex_preset.cpp
#include "speex_preset.hpp"
#include <gio/gio.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

SpeexPreset::SpeexPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::speex::id,
                       tags::schema::speex::input_path,
                       tags::schema::speex::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::speex).append("#").append(util::to_string(index));
}

void SpeexPreset::save(nlohmann::json& json) {
  auto& instance = json[section][instance_name];
  instance["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;
  instance["input-gain"] = g_settings_get_double(settings, "input-gain");
  instance["output-gain"] = g_settings_get_double(settings, "output-gain");
  instance["enable-denoise"] = g_settings_get_boolean(settings, "enable-denoise") != 0;
  instance["noise-suppression"] = g_settings_get_int(settings, "noise-suppression");
  instance["enable-agc"] = g_settings_get_boolean(settings, "enable-agc") != 0;
  instance["vad"]["enable"] = g_settings_get_boolean(settings, "enable-vad") != 0;
  instance["vad"]["probability-start"] = g_settings_get_int(settings, "vad-probability-start");
  instance["vad"]["probability-continue"] = g_settings_get_int(settings, "vad-probability-continue");
  instance["enable-dereverb"] = g_settings_get_boolean(settings, "enable-dereverb") != 0;
}

void SpeexPreset::load(const nlohmann::json& json) {
  const auto& instance = json.at(section).at(instance_name);
  update_key<bool>(instance, settings, "bypass", "bypass");
  update_key<double>(instance, settings, "input-gain", "input-gain");
  update_key<double>(instance, settings, "output-gain", "output-gain");
  update_key<bool>(instance, settings, "enable-denoise", "enable-denoise");
  update_key<int>(instance, settings, "noise-suppression", "noise-suppression");
  update_key<bool>(instance, settings, "enable-agc", "enable-agc");
  const auto& vad = instance.at("vad");
  update_key<bool>(vad, settings, "enable-vad", "enable");
  update_key<int>(vad, settings, "vad-probability-start", "probability-start");
  update_key<int>(vad, settings, "vad-probability-continue", "probability-continue");
  update_key<bool>(instance, settings, "enable-dereverb", "enable-dereverb");
}

// By GST @2024/10/27