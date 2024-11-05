// Filename: CrossfeedBox.hpp
// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "crossfeed.hpp"

namespace ui::crossfeed_box {

G_BEGIN_DECLS

#define EE_TYPE_CROSSFEED_BOX (crossfeed_box_get_type())

G_DECLARE_FINAL_TYPE(CrossfeedBox, crossfeed_box, EE, CROSSFEED_BOX, GtkBox)

G_END_DECLS

auto create() -> CrossfeedBox*;

void setup(CrossfeedBox* self, std::shared_ptr<Crossfeed> crossfeed, const std::string& schema_path);

}  // namespace ui::crossfeed_box
// By GST @Date