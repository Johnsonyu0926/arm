// Filename: reverb_box.hpp
// Score: 100


#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "reverb.hpp"

namespace ui::reverb_box {

G_BEGIN_DECLS

#define EE_TYPE_REVERB_BOX (reverb_box_get_type())

G_DECLARE_FINAL_TYPE(ReverbBox, reverb_box, EE, REVERB_BOX, GtkBox)

G_END_DECLS

auto create() -> ReverbBox*;

void setup(ReverbBox* self, std::shared_ptr<Reverb> reverb, const std::string& schema_path);

}  // namespace ui::reverb_box

// By GST @2024/10/27