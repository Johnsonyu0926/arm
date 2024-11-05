// Filename: presets_autoloading_holder.hpp
// Score: 100

#pragma once

#include <glib-object.h>
#include <glibconfig.h>
#include <string>

namespace ui::holders {

G_BEGIN_DECLS

#define EE_TYPE_PRESETS_AUTOLOADING_HOLDER (presets_autoloading_holder_get_type())

G_DECLARE_FINAL_TYPE(PresetsAutoloadingHolder, presets_autoloading_holder, EE, PRESETS_AUTOLOADING_HOLDER, GObject)

G_END_DECLS

struct PresetsAutoLoadingData {
 public:
  std::string device;
  std::string device_description;
  std::string device_profile;
  std::string preset_name;
};

struct _PresetsAutoloadingHolder {
  GObject parent_instance;

  PresetsAutoLoadingData* data;
};

auto create(const std::string& device,
            const std::string& device_description,
            const std::string& device_profile,
            const std::string& preset_name) -> PresetsAutoloadingHolder*;

}  // namespace ui::holders

// By GST @2024/10/27