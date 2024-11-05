

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "echo_canceller.hpp"

namespace ui::echo_canceller_box {

G_BEGIN_DECLS

#define EE_TYPE_ECHO_CANCELLER_BOX (echo_canceller_box_get_type())

G_DECLARE_FINAL_TYPE(EchoCancellerBox, echo_canceller_box, EE, ECHO_CANCELLER_BOX, GtkBox)

G_END_DECLS

auto create() -> EchoCancellerBox*;

void setup(EchoCancellerBox* self, std::shared_ptr<EchoCanceller> echo_canceller, const std::string& schema_path);

}  // namespace ui::echo_canceller_box
