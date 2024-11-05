// Filename: pipe_manager_box.hpp
// Score: 100



#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include "application.hpp"

namespace ui::pipe_manager_box {

G_BEGIN_DECLS

#define EE_TYPE_PIPE_MANAGER_BOX (pipe_manager_box_get_type())

G_DECLARE_FINAL_TYPE(PipeManagerBox, pipe_manager_box, EE, PIPE_MANAGER_BOX, GtkBox)

G_END_DECLS

auto create() -> PipeManagerBox*;

void setup(PipeManagerBox* self, app::Application* application);

}  // namespace ui::pipe_manager_box

// By GST @2024/10/27