// Filename: stereo_tools_box.hpp
// Score: 100

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "stereo_tools.hpp"

namespace ui::stereo_tools_box {

G_BEGIN_DECLS

#define EE_TYPE_STEREO_TOOLS_BOX (stereo_tools_box_get_type())

G_DECLARE_FINAL_TYPE(StereoToolsBox, stereo_tools_box, EE, STEREO_TOOLS_BOX, GtkBox)

G_END_DECLS

auto create() -> StereoToolsBox*;

void setup(StereoToolsBox* self, std::shared_ptr<StereoTools> stereo_tools, const std::string& schema_path);

}  // namespace ui::stereo_tools_box

// By GST @2024/10/27