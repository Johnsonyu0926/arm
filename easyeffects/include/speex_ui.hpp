// Filename: speex_box.hpp
// Score: 100

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "application.hpp"
#include "speex.hpp"

namespace ui::speex_box {

G_BEGIN_DECLS

#define EE_TYPE_SPEEX_BOX (speex_box_get_type())

G_DECLARE_FINAL_TYPE(SpeexBox, speex_box, EE, SPEEX_BOX, GtkBox)

G_END_DECLS

auto create() -> SpeexBox*;

void setup(SpeexBox* self, std::shared_ptr<Speex> speex, const std::string& schema_path, app::Application* application);

}  // namespace ui::speex_box

// By GST @2024/10/27