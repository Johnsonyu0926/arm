// Filename: rnnoise_preset.cpp
#include "rnnoise_preset.hpp"
#include <gio/gio.h>
#include <glib.h>
#include <nlohmann/json_fwd.hpp>
#include <filesystem>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

// Score: 98

RNNoisePreset::RNNoisePreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::rnnoise::id,
                       tags::schema::rnnoise::input_path,
                       tags::schema::rnnoise::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::rnnoise).append("#").append(util::to_string(index));
}

void RNNoisePreset::save(nlohmann::json& json) {
  auto& instance = json[section][instance_name];
  instance["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;
  instance["input-gain"] = g_settings_get_double(settings, "input-gain");
  instance["output-gain"] = g_settings_get_double(settings, "output-gain");
  instance["model-name"] = util::gsettings_get_string(settings, "model-name");
  instance["enable-vad"] = g_settings_get_boolean(settings, "enable-vad") != 0;
  instance["vad-thres"] = g_settings_get_double(settings, "vad-thres");
  instance["wet"] = g_settings_get_double(settings, "wet");
  instance["release"] = g_settings_get_double(settings, "release");
}

void RNNoisePreset::load(const nlohmann::json& json) {
  const auto& instance = json.at(section).at(instance_name);
  update_key<bool>(instance, settings, "bypass", "bypass");
  update_key<double>(instance, settings, "input-gain", "input-gain");
  update_key<double>(instance, settings, "output-gain", "output-gain");
  update_key<bool>(instance, settings, "enable-vad", "enable-vad");
  update_key<double>(instance, settings, "vad-thres", "vad-thres");
  update_key<double>(instance, settings, "wet", "wet");
  update_key<double>(instance, settings, "release", "release");

  // model-path deprecation
  const auto* model_name_key = "model-name";
  std::string new_model_name = instance.value(model_name_key, get_default<gchar*>(settings, model_name_key));

  if (new_model_name.empty()) {
    const std::string model_path = instance.value("model-path", "");
    if (!model_path.empty()) {
      new_model_name = std::filesystem::path{model_path}.stem().string();
      util::warning("using RNNoise model-path is deprecated, please update your preset; fallback to model-name: " + new_model_name);
    }
  }

  const auto current_model_name = util::gsettings_get_string(settings, model_name_key);
  if (new_model_name != current_model_name) {
    g_settings_set_string(settings, model_name_key, new_model_name.c_str());
  }
}

// By GST @2024/10/27