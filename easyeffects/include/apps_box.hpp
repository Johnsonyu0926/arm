
// Filename: AppsBox.hpp
// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <gtk/gtkicontheme.h>
#include "application.hpp"
#include "pipeline_type.hpp"

namespace ui::apps_box {

G_BEGIN_DECLS

#define EE_TYPE_APPS_BOX (apps_box_get_type())

G_DECLARE_FINAL_TYPE(AppsBox, apps_box, EE, APPS_BOX, GtkBox)

G_END_DECLS

auto create() -> AppsBox*;

void setup(AppsBox* self, app::Application* application, PipelineType pipeline_type, GtkIconTheme* icon_theme);

}  // namespace ui::apps_box
// By GST @Date