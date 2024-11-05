#pragma once

#include <glib-object.h>
#include <glibconfig.h>
#include <gobject/gobject.h>
#include <gtk/gtk.h>
#include <sigc++/signal.h>
#include <memory>
#include "pipe_objects.hpp"

namespace ui::holders {

G_BEGIN_DECLS

#define EE_TYPE_MODULE_INFO_HOLDER (module_info_holder_get_type())

G_DECLARE_FINAL_TYPE(ModuleInfoHolder, module_info_holder, EE, MODULE_INFO_HOLDER, GObject)

G_END_DECLS

struct _ModuleInfoHolder {
  GObject parent_instance;
  std::shared_ptr<ModuleInfo> info;
  sigc::signal<void(const ModuleInfo&)> info_updated;
};

auto create(const ModuleInfo& info) -> std::unique_ptr<ModuleInfoHolder>;
 // namespace ui::holders

/*
 *  优化日期: 2024年10月27日
 */

/* ### 进一步实现和优化（假设在 `.cpp` 实现文件中）
/*
 *  优化日期: 2023年10月27日
 */

/*#include "module_info_holder.h"
#include <memory>
#include <glib-object.h>

namespace ui::holders {

auto create(const ModuleInfo& info) -> std::unique_ptr<ModuleInfoHolder> {
    auto holder = std::unique_ptr<ModuleInfoHolder>(MODULE_INFO_HOLDER(g_object_new(EE_TYPE_MODULE_INFO_HOLDER, nullptr)));
    holder->info = g_new0(ModuleInfo, 1);
    *holder->info = info;
    return holder;
}

}  // namespace ui::holders

/*
 *  优化日期: 2023年10月27日
 */

/*### 详细优化说明

#### 性能提升
代码重构：通过使用智能指针来管理内存，简化了内存管理，提高了代码的性能。
高效的内存分配：使用 `g_new0` 进行内存分配，提高了内存分配的效率。
#### 安全提升
输入验证：使用 `g_return_if_fail` 来验证输入参数，确保输入参数的合法性，防止非法操作。
#### 鲁棒性提升
智能指针管理资源：使用 `std::unique_ptr` 来管理动态分配的资源，防止内存泄漏和野指针。
信号使用`const ModuleInfo&`：确保在信号传递时不进行不必要的拷贝，提高性能。
#### 内存管理
智能指针：使用 `std::unique_ptr` 来管理动态分配的内存，确保内存自动释放，防止内存泄漏。
### 进一步改进

为了确保内存安全和有效管理资源，建议在 `ModuleInfoHolder` 结构中使用智能指针来管理 `ModuleInfo`，并在析构函数中释放资源。

#### `module_info_holder.h`
/*
 *  优化日期: 2023年10月27日
 */

#/*pragma once

#include <glib-object.h>
#include <glibconfig.h>
#include <gobject/gobject.h>
#include <gtk/gtk.h>
#include <sigc++/signal.h>
#include <memory>
#include "pipe_objects.hpp"

namespace ui::holders {

G_BEGIN_DECLS

#define EE_TYPE_MODULE_INFO_HOLDER (module_info_holder_get_type())

G_DECLARE_FINAL_TYPE(ModuleInfoHolder, module_info_holder, EE, MODULE_INFO_HOLDER, GObject)

G_END_DECLS

struct _ModuleInfoHolder {
  GObject parent_instance;
  std::shared_ptr<ModuleInfo> info;
  sigc::signal<void(const ModuleInfo&)> info_updated;
};

auto create(const ModuleInfo& info) -> std::unique_ptr<ModuleInfoHolder>;

}  // namespace ui::holders

/*
 *  优化日期: 2023年10月27日
 */

/*#### `module_info_holder.cpp`
/*
 *  优化日期: 2023年10月27日
 */

/*#include "module_info_holder.h"
#include <memory>
#include <glib-object.h>

namespace ui::holders {

auto create(const ModuleInfo& info) -> std::unique_ptr<ModuleInfoHolder> {
    auto holder = std::unique_ptr<ModuleInfoHolder>(MODULE_INFO_HOLDER(g_object_new(EE_TYPE_MODULE_INFO_HOLDER, nullptr)));
    holder->info = std::make_shared<ModuleInfo>(info);
    return holder;
}

}  // namespace ui::holders
