// Score: 95

#pragma once

#include <adwaita.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <glibconfig.h>

namespace ui::application_window {

G_BEGIN_DECLS

#define EE_TYPE_APPLICATION_WINDOW (application_window_get_type())

G_DECLARE_FINAL_TYPE(ApplicationWindow, application_window, EE, APP_WINDOW, AdwApplicationWindow)

G_END_DECLS

auto create(GApplication* gapp) -> ApplicationWindow*;

}  // namespace ui::application_window
// By GST @Date