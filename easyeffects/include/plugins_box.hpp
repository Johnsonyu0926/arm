// Filename: plugins_box.hpp
// Score: 100

#pragma once

#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtk.h>
#include "application.hpp"
#include "pipeline_type.hpp"

namespace ui::plugins_box {

G_BEGIN_DECLS

#define EE_TYPE_PLUGINS_BOX (plugins_box_get_type())

G_DECLARE_FINAL_TYPE(PluginsBox, plugins_box, EE, PLUGINS_BOX, GtkBox)

G_END_DECLS

auto create() -> PluginsBox*;

void setup(PluginsBox* self, app::Application* application, PipelineType pipeline_type);

}  // namespace ui::plugins_box

// By GST @2024/10/27