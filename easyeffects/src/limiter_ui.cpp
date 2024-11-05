// Filename: limiter_ui.cpp
#include "limiter_ui.hpp"
#include <STTypes.h>
#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <gio/gio.h>
#include <gio/gliststore.h>
#include <glib-object.h>
#include <glib.h>
#include <glibconfig.h>
#include <gobject/gobject.h>
#include <gtk/gtk.h>
#include <gtk/gtkdropdown.h>
#include <gtk/gtksingleselection.h>
#include <sigc++/connection.h>
#include <memory>
#include <string>
#include <vector>
#include "limiter.hpp"
#include "node_info_holder.hpp"
#include "pipe_manager.hpp"
#include "pipe_objects.hpp"
#include "tags_pipewire.hpp"
#include "tags_resources.hpp"
#include "tags_schema.hpp"
#include "ui_helpers.hpp"
#include "util.hpp"

// By GST @2024/10/26
namespace ui::limiter_box {

struct Data {
 public:
  ~Data() { util::debug("data struct destroyed"); }

  uint serial = 0U;
  std::shared_ptr<Limiter> limiter;
  std::vector<sigc::connection> connections;
  std::vector<gulong> gconnections;
};

struct _LimiterBox {
  GtkBox parent_instance;

  GtkScale *input_gain, *output_gain;
  GtkLevelBar *input_level_left, *input_level_right, *output_level_left, *output_level_right;
  GtkLabel *input_level_left_label, *input_level_right_label, *output_level_left_label, *output_level_right_label,
      *plugin_credit;
  GtkDropDown *mode, *oversampling, *dither, *dropdown_input_devices;
  GtkLabel *gain_left, *gain_right, *sidechain_left, *sidechain_right;
  GtkSpinButton *sc_preamp, *lookahead, *attack, *release, *threshold, *stereo_link, *alr_attack, *alr_release,
      *alr_knee;
  GtkCheckButton* gain_boost;
  GtkToggleButton *alr, *external_sidechain, *show_native_ui;
  GListStore* input_devices_model;
  GSettings* settings;
  Data* data;
};

// NOLINTNEXTLINE
G_DEFINE_TYPE(LimiterBox, limiter_box, GTK_TYPE_BOX)

void on_reset(LimiterBox* self, GtkButton* btn) {
  util::reset_all_keys_except(self->settings);
}

void on_show_native_window(LimiterBox* self, GtkToggleButton* btn) {
  if (gtk_toggle_button_get_active(btn)) {
    self->data->limiter->show_native_ui();
  } else {
    self->data->limiter->close_native_ui();
  }
}

void setup_dropdown_input_device(LimiterBox* self) {
  auto* selection = gtk_single_selection_new(G_LIST_MODEL(self->input_devices_model));

  g_signal_connect(self->dropdown_input_devices, "notify::selected-item",
                   G_CALLBACK(+[](GtkDropDown* dropdown, GParamSpec* pspec, LimiterBox* self) {
                     if (auto selected_item = gtk_drop_down_get_selected_item(dropdown); selected_item != nullptr) {
                       auto* holder = static_cast<ui::holders::NodeInfoHolder*>(selected_item);
                       g_settings_set_string(self->settings, "sidechain-input-device", holder->info->name.c_str());
                     }
                   }),
                   self);

  gtk_drop_down_set_model(self->dropdown_input_devices, G_LIST_MODEL(self->input_devices_model));
  g_object_unref(selection);
}

void setup(LimiterBox* self, std::shared_ptr<Limiter> limiter, const std::string& schema_path, PipeManager* pm) {
  self->data->limiter = limiter;
  auto serial = get_new_filter_serial();
  self->data->serial = serial;
  g_object_set_data(G_OBJECT(self), "serial", GUINT_TO_POINTER(serial));
  set_ignore_filter_idle_add(serial, false);
  self->settings = g_settings_new_with_path(tags::schema::limiter::id, schema_path.c_str());
  limiter->set_post_messages(true);
  setup_dropdown_input_device(self);

  for (const auto& [serial, node] : pm->node_map) {
    if (node.name == tags::pipewire::ee_sink_name || node.name == tags::pipewire::ee_source_name) {
      continue;
    }

    if (node.media_class == tags::pipewire::media_class::source ||
        node.media_class == tags::pipewire::media_class::virtual_source ||
        node.media_role == tags::pipewire::media_role::dsp) {
      auto* holder = ui::holders::create(node);
      g_list_store_append(self->input_devices_model, holder);
      g_object_unref(holder);
    }
  }

  auto connect_level_signal = [&](auto& signal, auto* level_bar, auto* label) {
    self->data->connections.push_back(signal.connect([=](const float left, const float right) {
      g_object_ref(self);
      util::idle_add(
          [=]() {
            if (get_ignore_filter_idle_add(serial)) {
              return;
            }
            update_level(level_bar, label, level_bar, label, left, right);
          },
          [=]() { g_object_unref(self); });
    }));
  };

  connect_level_signal(limiter->input_level, self->input_level_left, self->input_level_left_label);
  connect_level_signal(limiter->output_level, self->output_level_left, self->output_level_left_label);

  auto connect_gain_signal = [&](auto& signal, auto* label) {
    self->data->connections.push_back(signal.connect([=](const float value) {
      g_object_ref(self);
      util::idle_add(
          [=]() {
            if (get_ignore_filter_idle_add(serial)) {
              return;
            }
            if (!GTK_IS_LABEL(label)) {
              return;
            }
            gtk_label_set_text(label, fmt::format("{0:.0f}", util::linear_to_db(value)).c_str());
          },
          [=]() { g_object_unref(self); });
    }));
  };

  connect_gain_signal(limiter->gain_left, self->gain_left);
  connect_gain_signal(limiter->gain_right, self->gain_right);
  connect_gain_signal(limiter->sidechain_left, self->sidechain_left);
  connect_gain_signal(limiter->sidechain_right, self->sidechain_right);

  self->data->connections.push_back(pm->source_added.connect([=](const NodeInfo info) {
    for (guint n = 0U; n < g_list_model_get_n_items(G_LIST_MODEL(self->input_devices_model)); n++) {
      auto* holder =
          static_cast<ui::holders::NodeInfoHolder*>(g_list_model_get_item(G_LIST_MODEL(self->input_devices_model), n));

      if (holder->info->id == info.id) {
        g_object_unref(holder);
        return;
      }

      g_object_unref(holder);
    }

    auto* holder = ui::holders::create(info);
    g_list_store_append(self->input_devices_model, holder);
    g_object_unref(holder);
  }));

  self->data->connections.push_back(pm->source_removed.connect([=](const NodeInfo info) {
    for (guint n = 0U; n < g_list_model_get_n_items(G_LIST_MODEL(self->input_devices_model)); n++) {
      auto* holder =
          static_cast<ui::holders::NodeInfoHolder*>(g_list_model_get_item(G_LIST_MODEL(self->input_devices_model), n));

      if (holder->info->id == info.id) {
        g_list_store_remove(self->input_devices_model, n);
        g_object_unref(holder);
        return;
      }

      g_object_unref(holder);
    }
  }));

  gtk_label_set_text(self->plugin_credit, ui::get_plugin_credit_translated(self->data->limiter->package).c_str());

  gsettings_bind_widgets<"input-gain", "output-gain">(self->settings, self->input_gain, self->output_gain);

  auto bind_spin_button = [&](const char* key, GtkSpinButton* spin_button) {
    g_settings_bind(self->settings, key, gtk_spin_button_get_adjustment(spin_button), "value", G_SETTINGS_BIND_DEFAULT);
  };

  bind_spin_button("sidechain-preamp", self->sc_preamp);
  bind_spin_button("lookahead", self->lookahead);
  bind_spin_button("attack", self->attack);
  bind_spin_button("release", self->release);
  bind_spin_button("threshold", self->threshold);
  bind_spin_button("stereo-link", self->stereo_link);
  bind_spin_button("alr-attack", self->alr_attack);
  bind_spin_button("alr-release", self->alr_release);
  bind_spin_button("alr-knee", self->alr_knee);

  g_settings_bind(self->settings, "gain-boost", self->gain_boost, "active", G_SETTINGS_BIND_DEFAULT);
  g_settings_bind(self->settings, "external-sidechain", self->external_sidechain, "active", G_SETTINGS_BIND_DEFAULT);
  g_settings_bind(self->settings, "alr", self->alr, "active", G_SETTINGS_BIND_DEFAULT);
  g_settings_bind(ui::get_global_app_settings(), "show-native-plugin-ui", self->show_native_ui, "visible",
                  G_SETTINGS_BIND_DEFAULT);

  ui::gsettings_bind_enum_to_combo_widget(self->settings, "mode", self->mode);
  ui::gsettings_bind_enum_to_combo_widget(self->settings, "oversampling", self->oversampling);
  ui::gsettings_bind_enum_to_combo_widget(self->settings, "dithering", self->dither);
}

void dispose(GObject* object) {
  auto* self = EE_LIMITER_BOX(object);

  self->data->limiter->close_native_ui();
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

  G_OBJECT_CLASS(limiter_box_parent_class)->dispose(object);
}

void finalize(GObject* object) {
  auto* self = EE_LIMITER_BOX(object);

  delete self->data;

  util::debug("finalized");

  G_OBJECT_CLASS(limiter_box_parent_class)->finalize(object);
}

void limiter_box_class_init(LimiterBoxClass* klass) {
  auto* object_class = G_OBJECT_CLASS(klass);
  auto* widget_class = GTK_WIDGET_CLASS(klass);

  object_class->dispose = dispose;
  object_class->finalize = finalize;

  gtk_widget_class_set_template_from_resource(widget_class, tags::resources::limiter_ui);

  gtk_widget_class_bind_template_child(widget_class, LimiterBox, input_gain);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, output_gain);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, input_level_left);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, input_level_right);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, output_level_left);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, output_level_right);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, input_level_left_label);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, input_level_right_label);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, output_level_left_label);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, output_level_right_label);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, plugin_credit);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, mode);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, oversampling);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, dither);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, sc_preamp);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, lookahead);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, attack);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, release);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, threshold);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, stereo_link);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, alr_attack);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, alr_release);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, alr_knee);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, alr);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, external_sidechain);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, gain_boost);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, gain_left);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, gain_right);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, sidechain_left);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, sidechain_right);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, dropdown_input_devices);
  gtk_widget_class_bind_template_child(widget_class, LimiterBox, show_native_ui);

  gtk_widget_class_bind_template_callback(widget_class, on_reset);
  gtk_widget_class_bind_template_callback(widget_class, on_show_native_window);
}

void limiter_box_init(LimiterBox* self) {
  gtk_widget_init_template(GTK_WIDGET(self));

  self->data = new Data();
  self->input_devices_model = g_list_store_new(ui::holders::node_info_holder_get_type());

  prepare_scales<"dB">(self->input_gain, self->output_gain);
  prepare_spinbuttons<"dB">(self->sc_preamp, self->threshold, self->alr_knee);
  prepare_spinbuttons<"ms">(self->lookahead, self->attack, self->release, self->alr_attack, self->alr_release);
  prepare_spinbuttons<"%">(self->stereo_link);
}

auto create() -> LimiterBox* {
  return static_cast<LimiterBox*>(g_object_new(EE_TYPE_LIMITER_BOX, nullptr));
}

}  // namespace ui::limiter_box