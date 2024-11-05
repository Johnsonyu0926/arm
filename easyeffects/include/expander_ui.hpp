// Filename: ExpanderBox.hpp
// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "expander.hpp"
#include "pipe_manager.hpp"

namespace ui::expander_box {

G_BEGIN_DECLS

#define EE_TYPE_EXPANDER_BOX (expander_box_get_type())

G_DECLARE_FINAL_TYPE(ExpanderBox, expander_box, EE, EXPANDER_BOX, GtkBox)

G_END_DECLS

auto create() -> ExpanderBox*;

void setup(ExpanderBox* self, std::shared_ptr<Expander> expander, const std::string& schema_path, PipeManager* pm);

}  // namespace ui::expander_box
// By GST @Date