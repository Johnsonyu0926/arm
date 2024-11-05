#include "apps_box.hpp"
#include <STTypes.h>
#include <adwaita.h>
#include <gio/gio.h>
#include <gio/gliststore.h>
#include <glib-object.h>
#include <glib.h>
#include <gobject/gobject.h>
#include <gtk/gtk.h>
#include <gtk/gtknoselection.h>
#include <sigc++/connection.h>
#include <algorithm>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "app_info.hpp"
#include "application.hpp"
#include "node_info_holder.hpp"
#include "pipe_objects.hpp"
#include "pipeline_type.hpp"
#include "tags_app.hpp"
#include "tags_pipewire.hpp"
#include "tags_resources.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

namespace ui::apps_box {

struct Data {
 public:
  ~Data() { util::debug("data struct destroyed"); }

  app::Application* application;
  std::unordered_map<uint, bool> enabled_app_list;
  std::vector<sigc::connection> connections;
  std::vector<gulong> gconnections;
};

struct _AppsBox {
  GtkBox parent_instance;
  GtkOverlay* overlay;
  AdwStatusPage* overlay_empty_list;
  GtkListView* listview;
  GtkIconTheme* icon_theme;
  GListStore *apps_model, *all_apps_model;
  GSettings *settings, *app_settings;
  Data* data;
};

// NOLINTNEXTLINE
G_DEFINE_TYPE(AppsBox, apps_box, GTK_TYPE_BOX)

auto app_is_blocklisted(AppsBox* self, const std::string& name) -> bool {
  const auto list = util::gchar_array_to_vector(g_settings_get_strv(self->settings, "blocklist"));
  return std::ranges::find(list, name) != list.end();
}

void update_empty_list_overlay(AppsBox* self) {
  gtk_widget_set_visible(GTK_WIDGET(self->overlay_empty_list),
                         (g_list_model_get_n_items(G_LIST_MODEL(self->apps_model)) == 0U) ? 1 : 0);
}

void on_app_added(AppsBox* self, const NodeInfo& node_info) {
  // Avoid adding the same stream twice
  for (guint n = 0U; n < g_list_model_get_n_items(G_LIST_MODEL(self->all_apps_model)); n++) {
    auto* holder = static_cast<ui::holders::NodeInfoHolder*>(g_list_model_get_item(G_LIST_MODEL(self->all_apps_model), n));
    if (holder->info->serial == node_info.serial) {
      g_object_unref(holder);
      return;
    }
    g_object_unref(holder);
  }

  auto* holder = ui::holders::create(node_info);
  g_list_store_append(self->all_apps_model, holder);

  if (g_settings_get_boolean(self->settings, "show-blocklisted-apps") != 0 || !app_is_blocklisted(self, node_info.name)) {
    g_list_store_append(self->apps_model, holder);
  }

  // Adjust reference count
  g_object_unref(holder);
  update_empty_list_overlay(self);
}

void on_app_removed(AppsBox* self, const uint64_t serial) {
  auto remove_app = [&](GListStore* model) {
    for (guint n = 0U; n < g_list_model_get_n_items(G_LIST_MODEL(model)); n++) {
      auto* holder = static_cast<ui::holders::NodeInfoHolder*>(g_list_model_get_item(G_LIST_MODEL(model), n));
      if (holder->info->serial == serial) {
        holder->info_updated.clear();
        g_list_store_remove(model, n);
        g_object_unref(holder);
        break;
      }
      g_object_unref(holder);
    }
  };

  remove_app(self->all_apps_model);
  remove_app(self->apps_model);
  update_empty_list_overlay(self);
}

void on_app_changed(AppsBox* self, const NodeInfo node_info) {
  for (guint n = 0U; n < g_list_model_get_n_items(G_LIST_MODEL(self->apps_model)); n++) {
    auto* holder = static_cast<ui::holders::NodeInfoHolder*>(g_list_model_get_item(G_LIST_MODEL(self->apps_model), n));
    if (holder->info->serial == node_info.serial) {
      holder->info_updated.emit(node_info);
      g_object_unref(holder);
      return;
    }
    g_object_unref(holder);
  }
}

void connect_stream(AppsBox* self, const uint& id, const std::string& media_class) {
  if (media_class == tags::pipewire::media_class::output_stream) {
    self->data->application->pm->connect_stream_output(id);
  } else if (media_class == tags::pipewire::media_class::input_stream) {
    self->data->application->pm->connect_stream_input(id);
  }
}

void disconnect_stream(AppsBox* self, const uint& id, const std::string& media_class) {
  if (media_class == tags::pipewire::media_class::output_stream || media_class == tags::pipewire::media_class::input_stream) {
    self->data->application->pm->disconnect_stream(id);
  }
}

void setup_listview(AppsBox* self) {
  auto* factory = gtk_signal_list_item_factory_new();

  g_signal_connect(factory, "setup",
                   G_CALLBACK(+[](GtkSignalListItemFactory* factory, GtkListItem* item, AppsBox* self) {
                     auto app_info = ui::app_info::create();
                     ui::app_info::setup(app_info, self->data->application, self->settings, self->icon_theme,
                                         self->data->enabled_app_list);
                     gtk_list_item_set_activatable(item, 0);
                     gtk_list_item_set_child(item, GTK_WIDGET(app_info));
                     g_object_set_data(G_OBJECT(item), "app-info", app_info);
                   }),
                   self);

  g_signal_connect(factory, "bind",
                   G_CALLBACK(+[](GtkSignalListItemFactory* factory, GtkListItem* item, AppsBox* self) {
                     auto* app_info = static_cast<ui::app_info::AppInfo*>(g_object_get_data(G_OBJECT(item), "app-info"));
                     auto* holder = static_cast<ui::holders::NodeInfoHolder*>(gtk_list_item_get_item(item));

                     if (const auto node_it = self->data->application->pm->node_map.find(holder->info->serial);
                         node_it != self->data->application->pm->node_map.end()) {
                       ui::app_info::update(app_info, node_it->second);
                     }

                     holder->info_updated.connect([=](const NodeInfo node_info) { ui::app_info::update(app_info, node_info); });
                   }),
                   self);

  g_signal_connect(factory, "unbind",
                   G_CALLBACK(+[](GtkSignalListItemFactory* self, GtkListItem* item, gpointer user_data) {
                     auto* holder = static_cast<ui::holders::NodeInfoHolder*>(gtk_list_item_get_item(item));
                     holder->info_updated.clear();
                   }),
                   self);

  gtk_list_view_set_factory(self->listview, factory);
  g_object_unref(factory);

  auto* selection = gtk_no_selection_new(G_LIST_MODEL(self->apps_model));
  gtk_list_view_set_model(self->listview, GTK_SELECTION_MODEL(selection));
  g_object_unref(selection);
}

void setup(AppsBox* self, app::Application* application, PipelineType pipeline_type, GtkIconTheme* icon_theme) {
  self->data->application = application;
  self->icon_theme = icon_theme;

  auto setup_pipeline = [&](auto* pm, const char* schema_id, const std::string& media_class) {
    self->settings = g_settings_new(schema_id);

    for (const auto& [serial, node] : pm->node_map) {
      if (node.media_class == media_class) {
        on_app_added(self, node);
      }
    }

    self->data->connections.push_back(pm->stream_added.connect([=](const NodeInfo info) { on_app_added(self, info); }));
    self->data->connections.push_back(pm->stream_removed.connect([=](const uint64_t serial) { on_app_removed(self, serial); }));
    self->data->connections.push_back(pm->stream_changed.connect([=](const NodeInfo node_info) { on_app_changed(self, node_info); }));
  };

  switch (pipeline_type) {
    case PipelineType::input:
      setup_pipeline(application->sie->pm, tags::schema::id_input, tags::pipewire::media_class::input_stream);
      break;
    case PipelineType::output:
      setup_pipeline(application->soe->pm, tags::schema::id_output, tags::pipewire::media_class::output_stream);
      break;
  }

  auto update_blocklist = [=](GSettings* settings, char* key) {
    const auto show_blocklisted_apps = g_settings_get_boolean(self->settings, "show-blocklisted-apps") != 0;
    g_list_store_remove_all(self->apps_model);

    for (guint n = 0U; n < g_list_model_get_n_items(G_LIST_MODEL(self->all_apps_model)); n++) {
      auto* holder = static_cast<ui::holders::NodeInfoHolder*>(g_list_model_get_item(G_LIST_MODEL(self->all_apps_model), n));
      const auto app_is_enabled = self->data->application->pm->stream_is_connected(holder->info->id, holder->info->media_class);

      if (app_is_blocklisted(self, holder->info->name)) {
        if (app_is_enabled) {
          disconnect_stream(self, holder->info->id, holder->info->media_class);
        }
        if (show_blocklisted_apps) {
          g_list_store_append(self->apps_model, holder);
        }
      } else {
        if (!app_is_enabled) {
          try {
            if (self->data->enabled_app_list.at(holder->info->id)) {
              connect_stream(self, holder->info->id, holder->info->media_class);
            }
          } catch (...) {
            connect_stream(self, holder->info->id, holder->info->media_class);
            util::warning("can't retrieve enabled state of node " + holder->info->name);
            self->data->enabled_app_list.insert({holder->info->id, true});
          }
        }
        g_list_store_append(self->apps_model, holder);
      }
      g_object_unref(holder);
    }
    update_empty_list_overlay(self);
  };

  self->data->gconnections.push_back(g_signal_connect(self->settings, "changed::blocklist", G_CALLBACK(update_blocklist), self));
  self->data->gconnections.push_back(g_signal_connect(self->settings, "changed::show-blocklisted-apps", G_CALLBACK(update_blocklist), self));
}

void dispose(GObject* object) {
  auto* self = EE_APPS_BOX(object);

  for (auto& c : self->data->connections) {
    c.disconnect();
  }

  for (auto& handler_id : self->data->gconnections) {
    g_signal_handler_disconnect(self->settings, handler_id);
  }

  self->data->connections.clear();
  self->data->gconnections.clear();

  g_object_unref(self->all_apps_model);  // do not do this to self->apps_model. It is owned by the listview
  g_object_unref(self->settings);
  g_object_unref(self->app_settings);

  util::debug("disposed");

  G_OBJECT_CLASS(apps_box_parent_class)->dispose(object);
}

void finalize(GObject* object) {
  auto* self = EE_APPS_BOX(object);
  delete self->data;
  util::debug("finalized");
  G_OBJECT_CLASS(apps_box_parent_class)->finalize(object);
}

void apps_box_class_init(AppsBoxClass* klass) {
  auto* object_class = G_OBJECT_CLASS(klass);
  auto* widget_class = GTK_WIDGET_CLASS(klass);

  object_class->dispose = dispose;
  object_class->finalize = finalize;

  gtk_widget_class_set_template_from_resource(widget_class, tags::resources::apps_box_ui);
  gtk_widget_class_bind_template_child(widget_class, AppsBox, overlay);
  gtk_widget_class_bind_template_child(widget_class, AppsBox, overlay_empty_list);
  gtk_widget_class_bind_template_child(widget_class, AppsBox, listview);
}

void apps_box_init(AppsBox* self) {
  gtk_widget_init_template(GTK_WIDGET(self));
  self->data = new Data();
  self->app_settings = g_settings_new(tags::app::id);
  self->apps_model = g_list_store_new(ui::holders::node_info_holder_get_type());
  self->all_apps_model = g_list_store_new(ui::holders::node_info_holder_get_type());
  gtk_overlay_set_clip_overlay(self->overlay, GTK_WIDGET(self->overlay_empty_list), 1);
  setup_listview(self);
}

auto create() -> AppsBox* {
  return static_cast<AppsBox*>(g_object_new(EE_TYPE_APPS_BOX, nullptr));
}

}  // namespace ui::apps_box

// By GST @2024/10/27