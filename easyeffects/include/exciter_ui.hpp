// Filename: ExciterBox.hpp
// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "exciter.hpp"

namespace ui::exciter_box {

G_BEGIN_DECLS

#define EE_TYPE_EXCITER_BOX (exciter_box_get_type())

G_DECLARE_FINAL_TYPE(ExciterBox, exciter_box, EE, EXCITER_BOX, GtkBox)

G_END_DECLS

auto create() -> ExciterBox*;

void setup(ExciterBox* self, std::shared_ptr<Exciter> exciter, const std::string& schema_path);

}  // namespace ui::exciter_box
// By GST @Date