// Filename: autogain_ui.cpp
// 优化日期: 2024/10/26

#include "autogain_ui.hpp"
#include <STTypes.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
#include <glibconfig.h>
#include <gobject/gobject.h>
#include <gtk/gtk.h>
#include <sigc++/connection.h>
#include <memory>
#include <string>
#include <vector>
#include "autogain.hpp"
#include "tags_resources.hpp"
#include "tags_schema.hpp"
#include "ui_helpers.hpp"
#include "util.hpp"

namespace ui::autogain_box {

struct Data {
 public:
  ~Data() { util::debug("data struct destroyed"); }

  uint serial = 0U;
  std::shared_ptr<AutoGain> autogain;
  std::vector<sigc::connection> connections;
  std::vector<gulong> gconnections;
};

struct _AutoGainBox {
  GtkBox parent_instance;
  GtkScale *input_gain, *output_gain;
  GtkLevelBar *input_level_left, *input_level_right, *output_level_left, *output_level_right;
  GtkLabel *input_level_left_label, *input_level_right_label, *output_level_left_label, *output_level_right_label, *plugin_credit;
  GtkSpinButton *gain, *output, *link;
  GSettings* settings;
  Data* data;
};

// NOLINTNEXTLINE
G_DEFINE_TYPE(AutoGainBox, autogain_box, GTK_TYPE_BOX)

void on_reset(AutoGainBox* self, GtkButton* btn) {
  util::reset_all_keys_except(self->settings);
}

void setup(AutoGainBox* self, std::shared_ptr<AutoGain> autogain, const std::string& schema_path) {
  auto serial = get_new_filter_serial();
  self->data->serial = serial;
  g_object_set_data(G_OBJECT(self), "serial", GUINT_TO_POINTER(serial));
  set_ignore_filter_idle_add(serial, false);
  self->data->autogain = autogain;
  self->settings = g_settings_new_with_path(tags::schema::autogain::id, schema_path.c_str());
  autogain->set_post_messages(true);

  self->data->connections.push_back(autogain->input_level.connect([=](const float left, const float right) {
    g_object_ref(self);
    util::idle_add(
        [=]() {
          if (get_ignore_filter_idle_add(serial)) {
            return;
          }
          update_level(self->input_level_left, self->input_level_left_label, self->input_level_right, self->input_level_right_label, left, right);
        },
        [=]() { g_object_unref(self); });
  }));

  self->data->connections.push_back(autogain->output_level.connect([=](const float left, const float right) {
    g_object_ref(self);
    util::idle_add(
        [=]() {
          if (get_ignore_filter_idle_add(serial)) {
            return;
          }
          update_level(self->output_level_left, self->output_level_left_label, self->output_level_right, self->output_level_right_label, left, right);
        },
        [=]() { g_object_unref(self); });
  }));

  gtk_label_set_text(self->plugin_credit, ui::get_plugin_credit_translated(self->data->autogain->package).c_str());

  gsettings_bind_widgets<"input-gain", "output-gain">(self->settings, self->input_gain, self->output_gain);
  gsettings_bind_widgets<"gain", "output", "link">(self->settings, self->gain, self->output, self->link);
}

void dispose(GObject* object) {
  auto* self = EE_AUTOGAIN_BOX(object);
  set_ignore_filter_idle_add(self->data->serial, true);

  for (auto& c : self->data->connections) {
    c.disconnect();
  }

  for (auto& handler_id : self->data->gconnections) {
    g_signal_handler_disconnect(self->settings, handler_id);
  }

  self->data->connections.clear();
  self->data->gconnections.clear();
  g_object_unref(self->settings);

  util::debug("disposed");
  G_OBJECT_CLASS(autogain_box_parent_class)->dispose(object);
}

void finalize(GObject* object) {
  auto* self = EE_AUTOGAIN_BOX(object);
  delete self->data;
  util::debug("finalize");
  G_OBJECT_CLASS(autogain_box_parent_class)->finalize(object);
}

void autogain_box_class_init(AutoGainBoxClass* klass) {
  auto* object_class = G_OBJECT_CLASS(klass);
  auto* widget_class = GTK_WIDGET_CLASS(klass);

  object_class->dispose = dispose;
  object_class->finalize = finalize;

  gtk_widget_class_set_template_from_resource(widget_class, tags::resources::autogain_ui);

  gtk_widget_class_bind_template_child(widget_class, AutoGainBox, input_gain);
  gtk_widget_class_bind_template_child(widget_class, AutoGainBox, output_gain);
  gtk_widget_class_bind_template_child(widget_class, AutoGainBox, input_level_left);
  gtk_widget_class_bind_template_child(widget_class, AutoGainBox, input_level_right);
  gtk_widget_class_bind_template_child(widget_class, AutoGainBox, output_level_left);
  gtk_widget_class_bind_template_child(widget_class, AutoGainBox, output_level_right);
  gtk_widget_class_bind_template_child(widget_class, AutoGainBox, input_level_left_label);
  gtk_widget_class_bind_template_child(widget_class, AutoGainBox, input_level_right_label);
  gtk_widget_class_bind_template_child(widget_class, AutoGainBox, output_level_left_label);
  gtk_widget_class_bind_template_child(widget_class, AutoGainBox, output_level_right_label);
  gtk_widget_class_bind_template_child(widget_class, AutoGainBox, plugin_credit);
  gtk_widget_class_bind_template_child(widget_class, AutoGainBox, gain);
  gtk_widget_class_bind_template_child(widget_class, AutoGainBox, output);
  gtk_widget_class_bind_template_child(widget_class, AutoGainBox, link);
  gtk_widget_class_bind_template_callback(widget_class, on_reset);
}

void autogain_box_init(AutoGainBox* self) {
  gtk_widget_init_template(GTK_WIDGET(self));
  self->data = new Data();
  prepare_spinbuttons<"dB">(self->gain, self->output, self->link);
  prepare_scales<"dB">(self->input_gain, self->output_gain);
}

auto create() -> AutoGainBox* {
  return static_cast<AutoGainBox*>(g_object_new(EE_TYPE_AUTOGAIN_BOX, nullptr));
}

}  // namespace ui::autogain_box

// By GST @2024/10/26