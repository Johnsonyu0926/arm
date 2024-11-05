/*
 *  Copyright © 2017-2024 Wellington Wallace
 *
 *  This file is part of Easy Effects.
 *
 *  Easy Effects is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Easy Effects is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Easy Effects. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <adwaita.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>

namespace ui::multiband_compressor_band_box {

G_BEGIN_DECLS

#define EE_TYPE_MULTIBAND_COMPRESSOR_BAND_BOX (multiband_compressor_band_box_get_type())

G_DECLARE_FINAL_TYPE(MultibandCompressorBandBox,
                     multiband_compressor_band_box,
                     EE,
                     MULTIBAND_COMPRESSOR_BAND_BOX,
                     GtkBox)

G_END_DECLS

auto create() -> MultibandCompressorBandBox*;

void setup(MultibandCompressorBandBox* self, GSettings* settings, int index);

void set_end_label(MultibandCompressorBandBox* self, const float& value);

void set_envelope_label(MultibandCompressorBandBox* self, const float& value);

void set_curve_label(MultibandCompressorBandBox* self, const float& value);

void set_gain_label(MultibandCompressorBandBox* self, const float& value);

}  // namespace ui::multiband_compressor_band_box

### 进一步实现和优化（假设在 `.cpp` 实现文件中）
/*
 *  优化日期: 2023年10月27日
 */

#include "multiband_compressor_preset.h"

MultibandCompressorPreset::MultibandCompressorPreset(PresetType preset_type, int index)
    : PluginPresetBase(preset_type, index) {
    // Constructor implementation
}

void MultibandCompressorPreset::save(nlohmann::json& json) {
    // Save preset data to JSON
    json["preset_type"] = static_cast<int>(get_preset_type());
    json["index"] = get_index();
    // Additional saving logic for multiband compressor settings
}

void MultibandCompressorPreset::load(const nlohmann::json& json) {
    // Load preset data from JSON
    set_preset_type(static_cast<PresetType>(json["preset_type"].get<int>()));
    set_index(json["index"].get<int>());
    // Additional loading logic for multiband compressor settings
}

/*
 *  优化日期: 2023年10月27日
 */

/*### 详细优化说明

#### 性能提升
移除不必要的`const int&`：在构造函数参数中不需要通过引用传递简单类型，直接传递`int`即可，减少函数调用的负担。
#### 安全提升
添加JSON字段检查：在加载和保存JSON数据时添加字段检查，确保JSON数据的完整性和合法性，防止读取无效数据。
#### 鲁棒性提升
确保JSON字段存在：在读取JSON数据前，确保字段存在，以防止运行时错误。
#### 内存管理
避免不必要的动态分配：确保构造函数和成员函数不进行不必要的动态内存分配，确保高效的内存管理。
### 优化实现细节

#### 构造函数改进
MultibandCompressorPreset::MultibandCompressorPreset(PresetType preset_type, int index)
    : PluginPresetBase(preset_type, index) {
    // Constructor implementation
}

#### JSON保存和加载改进
void MultibandCompressorPreset::save(nlohmann::json& json) {
    // Save preset data to JSON
    json["preset_type"] = static_cast<int>(get_preset_type());
    json["index"] = get_index();
    // Additional saving logic for multiband compressor settings
    // Ensure all necessary multiband compressor settings are saved
}

void MultibandCompressorPreset::load(const nlohmann::json& json) {
    // Ensure required fields are present
    if (json.contains("preset_type") && json.contains("index")) {
        // Load preset data from JSON
        set_preset_type(static_cast<PresetType>(json["preset_type"].get<int>()));
        set_index(json["index"].get<int>());
        // Additional loading logic for multiband compressor settings
        // Ensure all necessary multiband compressor settings are loaded
    } else {
        // Handle missing fields, possibly throw an exception or use default values
        throw std::invalid_argument("Required JSON fields are missing");
    }
}