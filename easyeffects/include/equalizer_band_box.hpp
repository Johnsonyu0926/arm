// Filename: EqualizerBandBox.hpp
// Score: 95

#pragma once

#include <adwaita.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>

namespace ui::equalizer_band_box {

G_BEGIN_DECLS

#define EE_TYPE_EQUALIZER_BAND_BOX (equalizer_band_box_get_type())

G_DECLARE_FINAL_TYPE(EqualizerBandBox, equalizer_band_box, EE, EQUALIZER_BAND_BOX, GtkBox)

G_END_DECLS

auto create() -> EqualizerBandBox*;

void setup(EqualizerBandBox* self, GSettings* settings);

void bind(EqualizerBandBox* self, int index);

}  // namespace ui::equalizer_band_box
// By GST @Date