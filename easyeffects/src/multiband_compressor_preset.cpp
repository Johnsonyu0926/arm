// Filename: multiband_compressor_preset.cpp
#include "multiband_compressor_preset.hpp"
#include <gio/gio.h>
#include <glib.h>
#include <sys/types.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

// By GST @2024/10/26
MultibandCompressorPreset::MultibandCompressorPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::multiband_compressor::id,
                       tags::schema::multiband_compressor::input_path,
                       tags::schema::multiband_compressor::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::multiband_compressor).append("#").append(util::to_string(index));
}

void MultibandCompressorPreset::save(nlohmann::json& json) {
  auto& instance = json[section][instance_name];

  instance["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;
  instance["input-gain"] = g_settings_get_double(settings, "input-gain");
  instance["output-gain"] = g_settings_get_double(settings, "output-gain");
  instance["dry"] = g_settings_get_double(settings, "dry");
  instance["wet"] = g_settings_get_double(settings, "wet");
  instance["compressor-mode"] = util::gsettings_get_string(settings, "compressor-mode");
  instance["envelope-boost"] = util::gsettings_get_string(settings, "envelope-boost");
  instance["stereo-split"] = g_settings_get_boolean(settings, "stereo-split") != 0;

  for (uint n = 0U; n < n_bands; n++) {
    const auto nstr = util::to_string(n);
    const auto bandn = "band" + nstr;
    auto& band_instance = instance[bandn];

    if (n > 0U) {
      band_instance["enable-band"] = g_settings_get_boolean(settings, ("enable-band" + nstr).c_str()) != 0;
      band_instance["split-frequency"] = g_settings_get_double(settings, ("split-frequency" + nstr).c_str());
    }

    band_instance["compressor-enable"] = g_settings_get_boolean(settings, ("compressor-enable" + nstr).c_str()) != 0;
    band_instance["solo"] = g_settings_get_boolean(settings, ("solo" + nstr).c_str()) != 0;
    band_instance["mute"] = g_settings_get_boolean(settings, ("mute" + nstr).c_str()) != 0;
    band_instance["attack-threshold"] = g_settings_get_double(settings, ("attack-threshold" + nstr).c_str());
    band_instance["attack-time"] = g_settings_get_double(settings, ("attack-time" + nstr).c_str());
    band_instance["release-threshold"] = g_settings_get_double(settings, ("release-threshold" + nstr).c_str());
    band_instance["release-time"] = g_settings_get_double(settings, ("release-time" + nstr).c_str());
    band_instance["ratio"] = g_settings_get_double(settings, ("ratio" + nstr).c_str());
    band_instance["knee"] = g_settings_get_double(settings, ("knee" + nstr).c_str());
    band_instance["makeup"] = g_settings_get_double(settings, ("makeup" + nstr).c_str());
    band_instance["compression-mode"] = util::gsettings_get_string(settings, ("compression-mode" + nstr).c_str());
    band_instance["external-sidechain"] = g_settings_get_boolean(settings, ("external-sidechain" + nstr).c_str()) != 0;
    band_instance["sidechain-mode"] = util::gsettings_get_string(settings, ("sidechain-mode" + nstr).c_str());
    band_instance["sidechain-source"] = util::gsettings_get_string(settings, ("sidechain-source" + nstr).c_str());
    band_instance["stereo-split-source"] = util::gsettings_get_string(settings, ("stereo-split-source" + nstr).c_str());
    band_instance["sidechain-lookahead"] = g_settings_get_double(settings, ("sidechain-lookahead" + nstr).c_str());
    band_instance["sidechain-reactivity"] = g_settings_get_double(settings, ("sidechain-reactivity" + nstr).c_str());
    band_instance["sidechain-preamp"] = g_settings_get_double(settings, ("sidechain-preamp" + nstr).c_str());
    band_instance["sidechain-custom-lowcut-filter"] = g_settings_get_boolean(settings, ("sidechain-custom-lowcut-filter" + nstr).c_str()) != 0;
    band_instance["sidechain-custom-highcut-filter"] = g_settings_get_boolean(settings, ("sidechain-custom-highcut-filter" + nstr).c_str()) != 0;
    band_instance["sidechain-lowcut-frequency"] = g_settings_get_double(settings, ("sidechain-lowcut-frequency" + nstr).c_str());
    band_instance["sidechain-highcut-frequency"] = g_settings_get_double(settings, ("sidechain-highcut-frequency" + nstr).c_str());
    band_instance["boost-threshold"] = g_settings_get_double(settings, ("boost-threshold" + nstr).c_str());
    band_instance["boost-amount"] = g_settings_get_double(settings, ("boost-amount" + nstr).c_str());
  }
}

void MultibandCompressorPreset::load(const nlohmann::json& json) {
  const auto& instance = json.at(section).at(instance_name);

  update_key<bool>(instance, settings, "bypass", "bypass");
  update_key<double>(instance, settings, "input-gain", "input-gain");
  update_key<double>(instance, settings, "output-gain", "output-gain");
  update_key<double>(instance, settings, "dry", "dry");
  update_key<double>(instance, settings, "wet", "wet");
  update_key<gchar*>(instance, settings, "compressor-mode", "compressor-mode");
  update_key<gchar*>(instance, settings, "envelope-boost", "envelope-boost");
  update_key<bool>(instance, settings, "stereo-split", "stereo-split");

  for (uint n = 0U; n < n_bands; n++) {
    const auto nstr = util::to_string(n);
    const auto bandn = "band" + nstr;
    const auto& band_instance = instance.at(bandn);

    if (n > 0U) {
      update_key<bool>(band_instance, settings, "enable-band" + nstr, "enable-band");
      update_key<double>(band_instance, settings, "split-frequency" + nstr, "split-frequency");
    }

    update_key<bool>(band_instance, settings, "compressor-enable" + nstr, "compressor-enable");
    update_key<bool>(band_instance, settings, "solo" + nstr, "solo");
    update_key<bool>(band_instance, settings, "mute" + nstr, "mute");
    update_key<double>(band_instance, settings, "attack-threshold" + nstr, "attack-threshold");
    update_key<double>(band_instance, settings, "attack-time" + nstr, "attack-time");
    update_key<double>(band_instance, settings, "release-threshold" + nstr, "release-threshold");
    update_key<double>(band_instance, settings, "release-time" + nstr, "release-time");
    update_key<double>(band_instance, settings, "ratio" + nstr, "ratio");
    update_key<double>(band_instance, settings, "knee" + nstr, "knee");
    update_key<double>(band_instance, settings, "makeup" + nstr, "makeup");
    update_key<gchar*>(band_instance, settings, "compression-mode" + nstr, "compression-mode");
    update_key<bool>(band_instance, settings, "external-sidechain" + nstr, "external-sidechain");
    update_key<gchar*>(band_instance, settings, "sidechain-mode" + nstr, "sidechain-mode");
    update_key<gchar*>(band_instance, settings, "sidechain-source" + nstr, "sidechain-source");
    update_key<gchar*>(band_instance, settings, "stereo-split-source" + nstr, "stereo-split-source");
    update_key<double>(band_instance, settings, "sidechain-lookahead" + nstr, "sidechain-lookahead");
    update_key<double>(band_instance, settings, "sidechain-reactivity" + nstr, "sidechain-reactivity");
    update_key<double>(band_instance, settings, "sidechain-preamp" + nstr, "sidechain-preamp");
    update_key<bool>(band_instance, settings, "sidechain-custom-lowcut-filter" + nstr, "sidechain-custom-lowcut-filter");
    update_key<bool>(band_instance, settings, "sidechain-custom-highcut-filter" + nstr, "sidechain-custom-highcut-filter");
    update_key<double>(band_instance, settings, "sidechain-lowcut-frequency" + nstr, "sidechain-lowcut-frequency");
    update_key<double>(band_instance, settings, "sidechain-highcut-frequency" + nstr, "sidechain-highcut-frequency");
    update_key<double>(band_instance, settings, "boost-threshold" + nstr, "boost-threshold");
    update_key<double>(band_instance, settings, "boost-amount" + nstr, "boost-amount");
  }
}