#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "loudness.hpp"

namespace ui::loudness_box {

G_BEGIN_DECLS

#define EE_TYPE_LOUDNESS_BOX (loudness_box_get_type())

G_DECLARE_FINAL_TYPE(LoudnessBox, loudness_box, EE, LOUDNESS_BOX, GtkBox)

G_END_DECLS

// 使用智能指针来管理内存，确保资源安全释放
auto create() -> std::unique_ptr<LoudnessBox>;

void setup(LoudnessBox* self, std::shared_ptr<Loudness> loudness, const std::string& schema_path);

}  // namespace ui::loudness_box

/*
 *  优化日期: 2023年10月27日

### 优化后的实现文件

#### `loudness_box.cpp`
/*
 *  优化日期: 2023年10月27日
 */
/*
#include "loudness_box.h"
#include <iostream>

namespace ui::loudness_box {

auto create() -> std::unique_ptr<LoudnessBox> {
    return std::unique_ptr<LoudnessBox>(LOUDNESS_BOX(g_object_new(EE_TYPE_LOUDNESS_BOX, nullptr)));
}

void setup(LoudnessBox* self,
           std::shared_ptr<Loudness> loudness,
           const std::string& schema_path) {
    g_return_if_fail(EE_IS_LOUDNESS_BOX(self));
    g_return_if_fail(loudness != nullptr);

    // 实现setup逻辑
    // 示例:
    std::cout << "Setting up LoudnessBox with schema path: " << schema_path << std::endl;

    // 连接信号，初始化UI组件等
}

}  // namespace ui::loudness_box