#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "multiband_compressor.hpp"
#include "pipe_manager.hpp"

namespace ui::multiband_compressor_box {

G_BEGIN_DECLS

#define EE_TYPE_MULTIBAND_COMPRESSOR_BOX (multiband_compressor_box_get_type())

G_DECLARE_FINAL_TYPE(MultibandCompressorBox, multiband_compressor_box, EE, MULTIBAND_COMPRESSOR_BOX, GtkBox)

G_END_DECLS

auto create() -> std::unique_ptr<MultibandCompressorBox>;

void setup(MultibandCompressorBox* self,
           std::shared_ptr<MultibandCompressor> multiband_compressor,
           const std::string& schema_path,
           PipeManager* pm);

}  // namespace ui::multiband_compressor_box