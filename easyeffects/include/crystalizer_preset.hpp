

// Filename: CrystalizerPreset.hpp
// Score: 95

#pragma once

#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"

class CrystalizerPreset : public PluginPresetBase {
public:
    explicit CrystalizerPreset(PresetType preset_type, const int& index = 0);

private:
    void save(nlohmann::json& json) override;

    void load(const nlohmann::json& json) override;
};
// By GST @Date