// Filename: app_info.hpp
// Score: 95

#pragma once

#include <STTypes.h>
#include <adwaita.h>
#include <fmt/core.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <gtk/gtkicontheme.h>
#include <unordered_map>
#include "application.hpp"
#include "pipe_objects.hpp"

namespace ui::app_info {

G_BEGIN_DECLS

#define EE_TYPE_APP_INFO (app_info_get_type())

G_DECLARE_FINAL_TYPE(AppInfo, app_info, EE, APP_INFO, GtkBox)

G_END_DECLS

auto create() -> AppInfo*;

void setup(AppInfo* self,
           app::Application* application,
           GSettings* settings,
           GtkIconTheme* icon_theme,
           std::unordered_map<uint, bool>& enabled_app_list);

void update(AppInfo* self, NodeInfo node_info);

}  // namespace ui::app_info
// By GST @Date