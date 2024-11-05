// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkpopover.h>
#include "application.hpp"
#include "pipeline_type.hpp"

namespace ui::blocklist_menu {

G_BEGIN_DECLS

#define EE_TYPE_BLOCKLIST_MENU (blocklist_menu_get_type())

G_DECLARE_FINAL_TYPE(BlocklistMenu, blocklist_menu, EE, BLOCKLIST_MENU, GtkPopover)

G_END_DECLS

auto create() -> BlocklistMenu*;

void setup(BlocklistMenu* self, app::Application* application, PipelineType pipeline_type);

}  // namespace ui::blocklist_menu
// By GST @Date