// Filename: SpeexPreset.hpp
// Score: 100

#pragma once

#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"

class SpeexPreset : public PluginPresetBase {
 public:
  explicit SpeexPreset(PresetType preset_type, int index = 0);

 private:
  void save(nlohmann::json& json) override;
  void load(const nlohmann::json& json) override;
};

// By GST @2024/10/27