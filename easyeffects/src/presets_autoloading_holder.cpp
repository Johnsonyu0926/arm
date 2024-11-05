// Optimized on 2024/10/27

#include "presets_autoloading_holder.hpp"
#include <glib-object.h>
#include <gobject/gobject.h>
#include <string>
#include "util.hpp"

namespace ui::holders {

// NOLINTNEXTLINE
G_DEFINE_TYPE(PresetsAutoloadingHolder, presets_autoloading_holder, G_TYPE_OBJECT);

void presets_autoloading_holder_finalize(GObject* object) {
  auto* self = EE_PRESETS_AUTOLOADING_HOLDER(object);

  util::debug(self->data->device + ", " + self->data->preset_name + " finalized");

  delete self->data;

  G_OBJECT_CLASS(presets_autoloading_holder_parent_class)->finalize(object);
}

void presets_autoloading_holder_class_init(PresetsAutoloadingHolderClass* klass) {
  auto* object_class = G_OBJECT_CLASS(klass);

  object_class->finalize = presets_autoloading_holder_finalize;
}

void presets_autoloading_holder_init(PresetsAutoloadingHolder* self) {
  self->data = new PresetsAutoLoadingData();
}

auto create(const std::string& device,
            const std::string& device_description,
            const std::string& device_profile,
            const std::string& preset_name) -> PresetsAutoloadingHolder* {
  auto* holder = static_cast<PresetsAutoloadingHolder*>(g_object_new(EE_TYPE_PRESETS_AUTOLOADING_HOLDER, nullptr));

  holder->data->device = device;
  holder->data->device_description = device_description;
  holder->data->device_profile = device_profile;
  holder->data->preset_name = preset_name;

  return holder;
}

}  // namespace ui::holders

// Optimized on 2024/10/27