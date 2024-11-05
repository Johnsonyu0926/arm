// Filename: ConvolverBox.hpp
// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "application.hpp"
#include "convolver.hpp"

namespace ui::convolver_box {

G_BEGIN_DECLS

#define EE_TYPE_CONVOLVER_BOX (convolver_box_get_type())

G_DECLARE_FINAL_TYPE(ConvolverBox, convolver_box, EE, CONVOLVER_BOX, GtkBox)

G_END_DECLS

auto create() -> ConvolverBox*;

void setup(ConvolverBox* self,
           std::shared_ptr<Convolver> convolver,
           const std::string& schema_path,
           app::Application* application);

}  // namespace ui::convolver_box
// By GST @Date