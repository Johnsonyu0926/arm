// Filename: multiband_gate_box.hpp
// Score: 100

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "multiband_gate.hpp"
#include "pipe_manager.hpp"

namespace ui::multiband_gate_box {

G_BEGIN_DECLS

#define EE_TYPE_MULTIBAND_GATE_BOX (multiband_gate_box_get_type())

G_DECLARE_FINAL_TYPE(MultibandGateBox, multiband_gate_box, EE, MULTIBAND_GATE_BOX, GtkBox)

G_END_DECLS

auto create() -> MultibandGateBox*;

void setup(MultibandGateBox* self,
           std::shared_ptr<MultibandGate> multiband_gate,
           const std::string& schema_path,
           PipeManager* pm);

}  // namespace ui::multiband_gate_box

// By GST @2024/10/27