// Filename: DeesserBox.hpp
// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "deesser.hpp"

namespace ui::deesser_box {

G_BEGIN_DECLS

#define EE_TYPE_DEESSER_BOX (deesser_box_get_type())

G_DECLARE_FINAL_TYPE(DeesserBox, deesser_box, EE, DEESSER_BOX, GtkBox)

G_END_DECLS

auto create() -> DeesserBox*;

void setup(DeesserBox* self, std::shared_ptr<Deesser> deesser, const std::string& schema_path);

}  // namespace ui::deesser_box
// By GST @Date