// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "bass_loudness.hpp"

namespace ui::bass_loudness_box {

G_BEGIN_DECLS

#define EE_TYPE_BASS_LOUDNESS_BOX (bass_loudness_box_get_type())

G_DECLARE_FINAL_TYPE(BassLoudnessBox, bass_loudness_box, EE, BASS_LOUDNESS_BOX, GtkBox)

G_END_DECLS

auto create() -> BassLoudnessBox*;

void setup(BassLoudnessBox* self, std::shared_ptr<BassLoudness> bass_loudness, const std::string& schema_path);

}  // namespace ui::bass_loudness_box
// By GST @Date
