#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "gate.hpp"
#include "pipe_manager.hpp"

namespace ui::gate_box {

G_BEGIN_DECLS

#define EE_TYPE_GATE_BOX (gate_box_get_type())

G_DECLARE_FINAL_TYPE(GateBox, gate_box, EE, GATE_BOX, GtkBox)

G_END_DECLS

auto create() -> std::unique_ptr<GateBox>;

void setup(GateBox* self, std::shared_ptr<Gate> gate, const std::string& schema_path, PipeManager* pm);

}  // namespace ui::gate_box

/*
 *  优化日期: 2023年10月27日

### 优化后的实现文件

#### `gate_box.cpp`
/*
 *  优化日期: 2023年10月27日
 */
/*
#include "gate_box.h"
#include <iostream>

namespace ui::gate_box {

auto create() -> std::unique_ptr<GateBox> {
    return std::unique_ptr<GateBox>(GATE_BOX(g_object_new(EE_TYPE_GATE_BOX, nullptr)));
}

void setup(GateBox* self,
           std::shared_ptr<Gate> gate,
           const std::string& schema_path,
           PipeManager* pm) {
    g_return_if_fail(EE_IS_GATE_BOX(self));
    g_return_if_fail(gate != nullptr);
    g_return_if_fail(pm != nullptr);

    // 实现 setup 逻辑
    std::cout << "Setting up GateBox with schema path: " << schema_path << std::endl;

    // 连接信号，初始化 UI 组件等

    // 示例：将 gate 的信号连接到 UI 组件
    gate->attack_zone_start.connect([self](float value) {
        // 将结果更新到 UI 组件，假设有一些标签用于显示这些值
        std::cout << "Attack Zone Start: " << value << std::endl;
    });

    gate->attack_threshold.connect([self](float value) {
        std::cout << "Attack Threshold: " << value << std::endl;
    });

    gate->release_zone_start.connect([self](float value) {
        std::cout << "Release Zone Start: " << value << std::endl;
    });

    gate->release_threshold.connect([self](float value) {
        std::cout << "Release Threshold: " << value << std::endl;
    });

    gate->reduction.connect([self](float value) {
        std::cout << "Reduction: " << value << std::endl;
    });

    gate->sidechain.connect([self](float value) {
        std::cout << "Sidechain: " << value << std::endl;
    });

    gate->curve.connect([self](float value) {
        std::cout << "Curve: " << value << std::endl;
    });

    gate->envelope.connect([self](float value) {
        std::cout << "Envelope: " << value << std::endl;
    });
}

}  // namespace ui::gate_box