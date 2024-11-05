#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "level_meter.hpp"

namespace ui::level_meter_box {

G_BEGIN_DECLS

#define EE_TYPE_LEVEL_METER_BOX (level_meter_box_get_type())

G_DECLARE_FINAL_TYPE(LevelMeterBox, level_meter_box, EE, LEVEL_METER_BOX, GtkBox)

G_END_DECLS

// 使用智能指针来管理内存，确保资源安全释放
auto create() -> std::unique_ptr<LevelMeterBox>;

void setup(LevelMeterBox* self, std::shared_ptr<LevelMeter> level_meter, const std::string& schema_path);

}  // namespace ui::level_meter_box

/*
 *  优化日期: 2023年10月27日

### 优化后的实现文件

#### `level_meter_box.cpp`
/*
 *  优化日期: 2023年10月27日
 */
/*
#include "level_meter_box.h"
#include <iostream>

namespace ui::level_meter_box {

auto create() -> std::unique_ptr<LevelMeterBox> {
    return std::unique_ptr<LevelMeterBox>(LEVEL_METER_BOX(g_object_new(EE_TYPE_LEVEL_METER_BOX, nullptr)));
}

void setup(LevelMeterBox* self,
           std::shared_ptr<LevelMeter> level_meter,
           const std::string& schema_path) {
    g_return_if_fail(EE_IS_LEVEL_METER_BOX(self));
    g_return_if_fail(level_meter != nullptr);

    // 实现 setup 逻辑
    std::cout << "Setting up LevelMeterBox with schema path: " << schema_path << std::endl;

    // 连接信号，初始化 UI 组件等

    // 示例：将 level_meter 的信号连接到 UI 组件
    level_meter->results.connect([self](double momentary, double shortterm, double integrated,
                                        double relative, double range, double true_peak_L, double true_peak_R) {
        // 将结果更新到 UI 组件，假设有一些标签用于显示这些值
        std::cout << "Momentary: " << momentary << ", Shortterm: " << shortterm << ", Integrated: "
                  << integrated << ", Relative: " << relative << ", Range: " << range
                  << ", True Peak L: " << true_peak_L << ", True Peak R: " << true_peak_R << std::endl;
    });
}

}  // namespace ui::level_meter_box