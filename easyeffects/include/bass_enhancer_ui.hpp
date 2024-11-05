// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "bass_enhancer.hpp"

namespace ui::bass_enhancer_box {

G_BEGIN_DECLS

#define EE_TYPE_BASS_ENHANCER_BOX (bass_enhancer_box_get_type())

G_DECLARE_FINAL_TYPE(BassEnhancerBox, bass_enhancer_box, EE, BASS_ENHANCER_BOX, GtkBox)

G_END_DECLS

auto create() -> BassEnhancerBox*;

void setup(BassEnhancerBox* self, std::shared_ptr<BassEnhancer> bass_enhancer, const std::string& schema_path);

}  // namespace ui::bass_enhancer_box
// By GST @Date