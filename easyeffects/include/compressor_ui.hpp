// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "compressor.hpp"
#include "pipe_manager.hpp"

namespace ui::compressor_box {

G_BEGIN_DECLS

#define EE_TYPE_COMPRESSOR_BOX (compressor_box_get_type())

G_DECLARE_FINAL_TYPE(CompressorBox, compressor_box, EE, COMPRESSOR_BOX, GtkBox)

G_END_DECLS

auto create() -> CompressorBox*;

void setup(CompressorBox* self,
           std::shared_ptr<Compressor> compressor,
           const std::string& schema_path,
           PipeManager* pm);

}  // namespace ui::compressor_box
// By GST @Date