// Filename: Chart.hpp
// Score: 95

#pragma once

#include <adwaita.h>
#include <gdk/gdk.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <gtk/gtkshortcut.h>
#include <string>
#include <vector>

namespace ui::chart {

G_BEGIN_DECLS

#define EE_TYPE_CHART (chart_get_type())

G_DECLARE_FINAL_TYPE(Chart, chart, EE, CHART, GtkWidget)

G_END_DECLS

enum class ChartType { bar, line, dots };

enum class ChartScale { linear, logarithmic };

void set_chart_type(Chart* self, const ChartType& value);

void set_chart_scale(Chart* self, const ChartScale& value);

void set_x_data(Chart* self, const std::vector<double>& x);

void set_y_data(Chart* self, const std::vector<double>& y);

void set_background_color(Chart* self, GdkRGBA color);

void set_color(Chart* self, GdkRGBA color);

void set_axis_labels_color(Chart* self, GdkRGBA color);

void set_line_width(Chart* self, const float& value);

void set_draw_bar_border(Chart* self, const bool& v);

void set_rounded_corners(Chart* self, const bool& v);

void set_fill_bars(Chart* self, const bool& v);

void set_n_x_decimals(Chart* self, const int& v);

void set_n_y_decimals(Chart* self, const int& v);

void set_x_unit(Chart* self, const std::string& value);

void set_y_unit(Chart* self, const std::string& value);

void set_margin(Chart* self, const float& v);

auto get_is_visible(Chart* self) -> bool;

void set_dynamic_y_scale(Chart* self, const bool& v);

auto create() -> Chart*;

}  // namespace ui::chart
// By GST @Date