// Filename: plugins_menu.hpp
// Score: 100

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkpopover.h>
#include "application.hpp"
#include "pipeline_type.hpp"

namespace ui::plugins_menu {

G_BEGIN_DECLS

#define EE_TYPE_PLUGINS_MENU (plugins_menu_get_type())

G_DECLARE_FINAL_TYPE(PluginsMenu, plugins_menu, EE, PLUGINS_MENU, GtkPopover)

G_END_DECLS

auto create() -> PluginsMenu*;

void setup(PluginsMenu* self, app::Application* application, PipelineType pipeline_type);

}  // namespace ui::plugins_menu

// By GST @2024/10/27