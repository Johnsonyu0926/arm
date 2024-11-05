// Filename: preferences_spectrum.hpp
// Score: 100

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>

namespace ui::preferences::spectrum {

G_BEGIN_DECLS

#define EE_TYPE_PREFERENCES_SPECTRUM (preferences_spectrum_get_type())

G_DECLARE_FINAL_TYPE(PreferencesSpectrum, preferences_spectrum, EE, PREFERENCES_SPECTRUM, AdwPreferencesPage)

G_END_DECLS

auto create() -> PreferencesSpectrum*;

}  // namespace ui::preferences::spectrum

// By GST @2024/10/27