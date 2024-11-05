// Optimized on 2024/10/27

#include "plugin_preset_base.hpp"
#include <gio/gio.h>
#include <glib-object.h>
#include "preset_type.hpp"
#include "util.hpp"

PluginPresetBase::PluginPresetBase(const char* schema_id,
                                   const char* schema_path_input,
                                   const char* schema_path_output,
                                   PresetType preset_type,
                                   const int& index)
    : index(index), preset_type(preset_type) {
  const std::string schema_path = (preset_type == PresetType::input)
                                      ? (schema_path_input + util::to_string(index) + "/")
                                      : (schema_path_output + util::to_string(index) + "/");

  section = (preset_type == PresetType::input) ? "input" : "output";
  settings = g_settings_new_with_path(schema_id, schema_path.c_str());
}

PluginPresetBase::~PluginPresetBase() {
  g_object_unref(settings);
}

// Optimized on 2024/10/27