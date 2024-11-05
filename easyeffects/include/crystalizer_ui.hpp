// Filename: CrystalizerBox.hpp
// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "crystalizer.hpp"

namespace ui::crystalizer_box {

G_BEGIN_DECLS

#define EE_TYPE_CRYSTALIZER_BOX (crystalizer_box_get_type())

G_DECLARE_FINAL_TYPE(CrystalizerBox, crystalizer_box, EE, CRYSTALIZER_BOX, GtkBox)

G_END_DECLS

auto create() -> CrystalizerBox*;

void setup(CrystalizerBox* self, std::shared_ptr<Crystalizer> crystalizer, const std::string& schema_path);

}  // namespace ui::crystalizer_box
// By GST @Date