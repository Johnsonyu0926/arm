// Filename: preferences_general.hpp
// Score: 100

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>

#ifdef ENABLE_LIBPORTAL
#include "libportal.hpp"
#endif

namespace ui::preferences::general {

G_BEGIN_DECLS

#define EE_TYPE_PREFERENCES_GENERAL (preferences_general_get_type())

G_DECLARE_FINAL_TYPE(PreferencesGeneral, preferences_general, EE, PREFERENCES_GENERAL, AdwPreferencesPage)

G_END_DECLS

auto create() -> PreferencesGeneral*;

}  // namespace ui::preferences::general

// By GST @2024/10/27