// Filename: pitch_box.hpp
// Score: 100


#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "pitch.hpp"

namespace ui::pitch_box {

G_BEGIN_DECLS

#define EE_TYPE_PITCH_BOX (pitch_box_get_type())

G_DECLARE_FINAL_TYPE(PitchBox, pitch_box, EE, PITCH_BOX, GtkBox)

G_END_DECLS

auto create() -> PitchBox*;

void setup(PitchBox* self, std::shared_ptr<Pitch> pitch, const std::string& schema_path);

}  // namespace ui::pitch_box

// By GST @2024/10/27