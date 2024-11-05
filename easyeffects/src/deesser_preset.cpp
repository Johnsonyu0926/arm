#include "deesser_preset.hpp"
#include <gio/gio.h>
#include <glib.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

// Filename: deesser_preset.cpp
// Score: 98

DeesserPreset::DeesserPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::deesser::id,
                       tags::schema::deesser::input_path,
                       tags::schema::deesser::output_path,
                       preset_type,
                       index) {
  instance_name.assign(tags::plugin_name::deesser).append("#").append(util::to_string(index));
}

void DeesserPreset::save(nlohmann::json& json) {
  json[section][instance_name]["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;
  json[section][instance_name]["input-gain"] = g_settings_get_double(settings, "input-gain");
  json[section][instance_name]["output-gain"] = g_settings_get_double(settings, "output-gain");
  json[section][instance_name]["detection"] = util::gsettings_get_string(settings, "detection");
  json[section][instance_name]["mode"] = util::gsettings_get_string(settings, "mode");
  json[section][instance_name]["threshold"] = g_settings_get_double(settings, "threshold");
  json[section][instance_name]["ratio"] = g_settings_get_double(settings, "ratio");
  json[section][instance_name]["laxity"] = g_settings_get_int(settings, "laxity");
  json[section][instance_name]["makeup"] = g_settings_get_double(settings, "makeup");
  json[section][instance_name]["f1-freq"] = g_settings_get_double(settings, "f1-freq");
  json[section][instance_name]["f2-freq"] = g_settings_get_double(settings, "f2-freq");
  json[section][instance_name]["f1-level"] = g_settings_get_double(settings, "f1-level");
  json[section][instance_name]["f2-level"] = g_settings_get_double(settings, "f2-level");
  json[section][instance_name]["f2-q"] = g_settings_get_double(settings, "f2-q");
  json[section][instance_name]["sc-listen"] = g_settings_get_boolean(settings, "sc-listen") != 0;
}

void DeesserPreset::load(const nlohmann::json& json) {
  update_key<bool>(json.at(section).at(instance_name), settings, "bypass", "bypass");
  update_key<double>(json.at(section).at(instance_name), settings, "input-gain", "input-gain");
  update_key<double>(json.at(section).at(instance_name), settings, "output-gain", "output-gain");
  update_key<gchar*>(json.at(section).at(instance_name), settings, "detection", "detection");
  update_key<gchar*>(json.at(section).at(instance_name), settings, "mode", "mode");
  update_key<double>(json.at(section).at(instance_name), settings, "threshold", "threshold");
  update_key<double>(json.at(section).at(instance_name), settings, "ratio", "ratio");
  update_key<int>(json.at(section).at(instance_name), settings, "laxity", "laxity");
  update_key<double>(json.at(section).at(instance_name), settings, "makeup", "makeup");
  update_key<double>(json.at(section).at(instance_name), settings, "f1-freq", "f1-freq");
  update_key<double>(json.at(section).at(instance_name), settings, "f2-freq", "f2-freq");
  update_key<double>(json.at(section).at(instance_name), settings, "f1-level", "f1-level");
  update_key<double>(json.at(section).at(instance_name), settings, "f2-level", "f2-level");
  update_key<double>(json.at(section).at(instance_name), settings, "f2-q", "f2-q");
  update_key<bool>(json.at(section).at(instance_name), settings, "sc-listen", "sc-listen");
}

// By GST @2024/10/26