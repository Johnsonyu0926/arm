// Filename: module_info_holder.cpp
#include "module_info_holder.h"
#include <glib-object.h>
#include <memory>
#include "pipe_objects.hpp"
#include "util.hpp"

// By GST @2024/10/26
namespace ui::holders {

enum { PROP_0, PROP_ID, PROP_NAME, PROP_DESCRIPTION, PROP_FILENAME };

// NOLINTNEXTLINE
G_DEFINE_TYPE(ModuleInfoHolder, module_info_holder, G_TYPE_OBJECT);

void module_info_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
  auto* self = MODULE_INFO_HOLDER(object);
  switch (prop_id) {
    case PROP_ID:
      self->info->id = g_value_get_uint(value);
      break;
    case PROP_NAME:
      self->info->name = g_value_get_string(value);
      break;
    case PROP_DESCRIPTION:
      self->info->description = g_value_get_string(value);
      break;
    case PROP_FILENAME:
      self->info->filename = g_value_get_string(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

void module_info_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
  auto* self = MODULE_INFO_HOLDER(object);
  switch (prop_id) {
    case PROP_ID:
      g_value_set_uint(value, self->info->id);
      break;
    case PROP_NAME:
      g_value_set_string(value, self->info->name.c_str());
      break;
    case PROP_DESCRIPTION:
      g_value_set_string(value, self->info->description.c_str());
      break;
    case PROP_FILENAME:
      g_value_set_string(value, self->info->filename.c_str());
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
      break;
  }
}

void module_info_holder_finalize(GObject* object) {
  auto* self = MODULE_INFO_HOLDER(object);
  self->info_updated.clear();
  util::debug(util::to_string(self->info->id) + ", " + self->info->name + " finalized");
  G_OBJECT_CLASS(module_info_holder_parent_class)->finalize(object);
}

void module_info_holder_class_init(ModuleInfoHolderClass* klass) {
  auto* object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = module_info_holder_finalize;
  object_class->set_property = module_info_set_property;
  object_class->get_property = module_info_get_property;

  g_object_class_install_property(
      object_class, PROP_ID,
      g_param_spec_uint("id", "Id", "Id", 0, G_MAXUINT, SPA_ID_INVALID, G_PARAM_READWRITE));

  g_object_class_install_property(object_class, PROP_NAME,
                                  g_param_spec_string("name", "Name", "Name", nullptr, G_PARAM_READWRITE));

  g_object_class_install_property(
      object_class, PROP_DESCRIPTION,
      g_param_spec_string("description", "Description", "Description", nullptr, G_PARAM_READWRITE));

  g_object_class_install_property(
      object_class, PROP_FILENAME,
      g_param_spec_string("file-name", "File Name", "File Name", nullptr, G_PARAM_READWRITE));
}

void module_info_holder_init(ModuleInfoHolder* self) {
  self->info = std::make_shared<ModuleInfo>();
}

auto create(const ModuleInfo& info) -> std::unique_ptr<ModuleInfoHolder> {
  auto holder = std::unique_ptr<ModuleInfoHolder>(MODULE_INFO_HOLDER(g_object_new(EE_TYPE_MODULE_INFO_HOLDER, nullptr)));
  holder->info->id = info.id;
  holder->info->name = info.name;
  holder->info->description = info.description;
  holder->info->filename = info.filename;
  return holder;
}

}  // namespace ui::holders
