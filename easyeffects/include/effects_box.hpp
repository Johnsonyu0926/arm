// Filename: EffectsBox.hpp
// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gsl/gsl_spline.h>
#include <gtk/gtkbox.h>
#include <gtk/gtkicontheme.h>
#include "application.hpp"
#include "pipeline_type.hpp"

namespace ui::effects_box {

G_BEGIN_DECLS

#define EE_TYPE_EFFECTS_BOX (effects_box_get_type())

G_DECLARE_FINAL_TYPE(EffectsBox, effects_box, EE, EFFECTS_BOX, GtkBox)

G_END_DECLS

auto create() -> EffectsBox*;

void setup(EffectsBox* self, app::Application* application, PipelineType pipeline_type, GtkIconTheme* icon_theme);

}  // namespace ui::effects_box
// By GST @Date