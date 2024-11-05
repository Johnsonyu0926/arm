// Filename: AutogainBox.hpp
// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "autogain.hpp"

namespace ui::autogain_box {

G_BEGIN_DECLS

#define EE_TYPE_AUTOGAIN_BOX (autogain_box_get_type())

G_DECLARE_FINAL_TYPE(AutogainBox, autogain_box, EE, AUTOGAIN_BOX, GtkBox)

G_END_DECLS

auto create() -> AutogainBox*;

void setup(AutogainBox* self, std::shared_ptr<AutoGain> autogain, const std::string& schema_path);

}  // namespace ui::autogain_box
// By GST @Date