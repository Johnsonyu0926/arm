#pragma once

#include <sys/types.h>
#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"
#include "tags_multiband_compressor.hpp"

class MultibandCompressorPreset : public PluginPresetBase {
 public:
  explicit MultibandCompressorPreset(PresetType preset_type, int index = 0);

 private:
  static constexpr uint n_bands = tags::multiband_compressor::n_bands;

  void save(nlohmann::json& json) override;
  void load(const nlohmann::json& json) override;
};