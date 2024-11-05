// Filename: ConvolverMenuCombine.hpp
// Score: 95

#pragma once

#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtk.h>
#include <string>

namespace ui::convolver_menu_combine {

G_BEGIN_DECLS

#define EE_TYPE_CONVOLVER_MENU_COMBINE (convolver_menu_combine_get_type())

G_DECLARE_FINAL_TYPE(ConvolverMenuCombine, convolver_menu_combine, EE, CONVOLVER_MENU_COMBINE, GtkPopover)

G_END_DECLS

auto create() -> ConvolverMenuCombine*;

void append_to_string_list(ConvolverMenuCombine* self, const std::string& irs_filename);

void remove_from_string_list(ConvolverMenuCombine* self, const std::string& irs_filename);

}  // namespace ui::convolver_menu_combine
// By GST @Date