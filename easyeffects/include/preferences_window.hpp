// Filename: preferences_window.hpp
// Score: 100

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>

namespace ui::preferences::window {

G_BEGIN_DECLS

#define EE_TYPE_PREFERENCES_WINDOW (preferences_window_get_type())

G_DECLARE_FINAL_TYPE(PreferencesWindow, preferences_window, EE, PREFERENCES_WINDOW, AdwPreferencesDialog)

G_END_DECLS

auto create() -> PreferencesWindow*;

}  // namespace ui::preferences::window

// By GST @2024/10/27