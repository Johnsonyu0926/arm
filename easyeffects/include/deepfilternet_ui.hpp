// Filename: DeepFilterNetBox.hpp
// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "deepfilternet.hpp"

namespace ui::deepfilternet_box {

G_BEGIN_DECLS

#define EE_TYPE_DEEPFILTERNET_BOX (deepfilternet_box_get_type())

G_DECLARE_FINAL_TYPE(DeepFilterNetBox, deepfilternet_box, EE, DEEPFILTERNET_BOX, GtkBox)

G_END_DECLS

auto create() -> DeepFilterNetBox*;

void setup(DeepFilterNetBox* self, std::shared_ptr<DeepFilterNet> deepfilternet, const std::string& schema_path);

}  // namespace ui::deepfilternet_box
// By GST @Date