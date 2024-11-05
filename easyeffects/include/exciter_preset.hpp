// Filename: ExciterPreset.hpp
// Score: 95

#pragma once

#include <nlohmann/json_fwd.hpp>
#include "plugin_preset_base.hpp"
#include "preset_type.hpp"

class ExciterPreset : public PluginPresetBase {
public:
    explicit ExciterPreset(PresetType preset_type, const int& index = 0);

private:
    void save(nlohmann::json& json) override;

    void load(const nlohmann::json& json) override;
};
// By GST @Date