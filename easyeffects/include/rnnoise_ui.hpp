// Filename: rnnoise_box.hpp
// Score: 100



#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "application.hpp"
#include "rnnoise.hpp"

namespace ui::rnnoise_box {

G_BEGIN_DECLS

#define EE_TYPE_RNNOISE_BOX (rnnoise_box_get_type())

G_DECLARE_FINAL_TYPE(RNNoiseBox, rnnoise_box, EE, RNNOISE_BOX, GtkBox)

G_END_DECLS

auto create() -> RNNoiseBox*;

void setup(RNNoiseBox* self,
           std::shared_ptr<RNNoise> rnnoise,
           const std::string& schema_path,
           app::Application* application);

}  // namespace ui::rnnoise_box

// By GST @2024/10/27