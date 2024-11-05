// Score: 95

#pragma once

#include <adwaita.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkbox.h>
#include <memory>
#include <string>
#include "filter.hpp"

namespace ui::filter_box {

G_BEGIN_DECLS

#define EE_TYPE_FILTER_BOX (filter_box_get_type())

G_DECLARE_FINAL_TYPE(FilterBox, filter_box, EE, FILTER_BOX, GtkBox)

G_END_DECLS

auto create() -> FilterBox*;

void setup(FilterBox* self, std::shared_ptr<Filter> filter, const std::string& schema_path);

}  // namespace ui::filter_box
// By GST @Date