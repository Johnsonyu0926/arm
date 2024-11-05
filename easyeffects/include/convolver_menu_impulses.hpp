// Filename: ConvolverMenuImpulses.hpp
// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkpopover.h>
#include <sndfile.hh>
#include <string>
#include "application.hpp"

namespace ui::convolver_menu_impulses {

G_BEGIN_DECLS

#define EE_TYPE_CONVOLVER_MENU_IMPULSES (convolver_menu_impulses_get_type())

G_DECLARE_FINAL_TYPE(ConvolverMenuImpulses, convolver_menu_impulses, EE, CONVOLVER_MENU_IMPULSES, GtkPopover)

G_END_DECLS

auto create() -> ConvolverMenuImpulses*;

void setup(ConvolverMenuImpulses* self,
           const std::string& schema_path,
           app::Application* application,
           std::shared_ptr<Convolver> convolver);

void append_to_string_list(ConvolverMenuImpulses* self, const std::string& irs_filename);

void remove_from_string_list(ConvolverMenuImpulses* self, const std::string& irs_filename);

}  // namespace ui::convolver_menu_impulses
// By GST @Date