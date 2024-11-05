// Filename: multiband_gate_preset.hpp
// Score: 100

#pragma once

#include <sys/types.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_multiband_gate.hpp"

class MultibandGatePreset : public PluginPresetBase {
 public:
  explicit MultibandGatePreset(PresetType preset_type, const int& index = 0);

 private:
  static constexpr uint n_bands = tags::multiband_gate::n_bands;

  void save(nlohmann::json& json) override;
  void load(const nlohmann::json& json) override;
};

// By GST @2024/10/27