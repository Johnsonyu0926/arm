// Filename: presets_menu.hpp
// Score: 100

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkpopover.h>
#include "application.hpp"
#include "preset_type.hpp"

namespace ui::presets_menu {

G_BEGIN_DECLS

#define EE_TYPE_PRESETS_MENU (presets_menu_get_type())

G_DECLARE_FINAL_TYPE(PresetsMenu, presets_menu, EE, PRESETS_MENU, GtkPopover)

G_END_DECLS

auto create() -> PresetsMenu*;

void setup(PresetsMenu* self, app::Application* application, PresetType preset_type);

}  // namespace ui::presets_menu

// By GST @2024/10/27