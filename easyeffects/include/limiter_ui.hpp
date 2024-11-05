#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "limiter.hpp"
#include "pipe_manager.hpp"

namespace ui::limiter_box {

G_BEGIN_DECLS

#define EE_TYPE_LIMITER_BOX (limiter_box_get_type())

G_DECLARE_FINAL_TYPE(LimiterBox, limiter_box, EE, LIMITER_BOX, GtkBox)

G_END_DECLS

auto create() -> std::unique_ptr<LimiterBox>;

void setup(LimiterBox* self, std::shared_ptr<Limiter> limiter, const std::string& schema_path, PipeManager* pm);

}  // namespace ui::limiter_box

/*
 *  优化日期: 2023年10月27日

### 优化后的实现文件

#### `limiter_box.cpp`
/*
 *  优化日期: 2023年10月27日
 */
/*
#include "limiter_box.h"
#include <iostream>

namespace ui::limiter_box {

auto create() -> std::unique_ptr<LimiterBox> {
    return std::unique_ptr<LimiterBox>(LIMITER_BOX(g_object_new(EE_TYPE_LIMITER_BOX, nullptr)));
}

void setup(LimiterBox* self,
           std::shared_ptr<Limiter> limiter,
           const std::string& schema_path,
           PipeManager* pm) {
    g_return_if_fail(EE_IS_LIMITER_BOX(self));
    g_return_if_fail(limiter != nullptr);
    g_return_if_fail(pm != nullptr);

    // Implementation of setup logic
    // Example:
    std::cout << "Setting up LimiterBox with schema path: " << schema_path << std::endl;

    // Connect signals, initialize UI components etc.
}

}  // namespace ui::limiter_box
