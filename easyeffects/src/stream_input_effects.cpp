// Filename: stream_input_effects.cpp
#include "stream_input_effects.hpp"
#include <STTypes.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
#include <pipewire/link.h>
#include <sigc++/functors/mem_fun.h>
#include <spa/utils/defs.h>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ranges>
#include <set>
#include <string>
#include <thread>
#include <vector>
#include "effects_base.hpp"
#include "pipe_manager.hpp"
#include "pipe_objects.hpp"
#include "tags_pipewire.hpp"
#include "tags_plugin_name.hpp"
#include "tags_schema.hpp"
#include "util.hpp"

StreamInputEffects::StreamInputEffects(PipeManager* pipe_manager)
    : EffectsBase("sie: ", tags::schema::id_input, pipe_manager, PipelineType::input) {
  auto* PULSE_SOURCE = std::getenv("PULSE_SOURCE");

  if (PULSE_SOURCE != nullptr && PULSE_SOURCE != tags::pipewire::ee_source_name) {
    for (const auto& [serial, node] : pm->node_map) {
      if (node.name == PULSE_SOURCE) {
        pm->input_device = node;
        g_settings_set_string(settings, "input-device", pm->input_device.name.c_str());
        break;
      }
    }
  }

  connections.push_back(pm->source_added.connect([this](const NodeInfo node) {
    if (node.name == util::gsettings_get_string(settings, "input-device")) {
      pm->input_device = node;
      if (g_settings_get_boolean(global_settings, "bypass") != 0) {
        g_settings_set_boolean(global_settings, "bypass", 0);
        return;  // filter connected through update_bypass_state
      }
      set_bypass(false);
    }
  }));

  connections.push_back(pm->source_removed.connect([this](const NodeInfo node) {
    if (g_settings_get_boolean(settings, "use-default-input-device") == 0) {
      if (node.name == util::gsettings_get_string(settings, "input-device")) {
        pm->input_device.id = SPA_ID_INVALID;
        pm->input_device.serial = SPA_ID_INVALID;
      }
    }
  }));

  connections.push_back(pm->stream_input_added.connect(sigc::mem_fun(*this, &StreamInputEffects::on_app_added)));
  connections.push_back(pm->link_changed.connect(sigc::mem_fun(*this, &StreamInputEffects::on_link_changed)));

  connect_filters();

  gconnections.push_back(g_signal_connect(settings, "changed::input-device",
                                          G_CALLBACK(+[](GSettings* settings, char* key, gpointer user_data) {
                                            auto* self = static_cast<StreamInputEffects*>(user_data);
                                            const auto name = util::gsettings_get_string(settings, key);
                                            if (name.empty()) {
                                              return;
                                            }
                                            for (const auto& [serial, node] : self->pm->node_map) {
                                              if (node.name == name) {
                                                self->pm->input_device = node;
                                                if (g_settings_get_boolean(self->global_settings, "bypass") != 0) {
                                                  g_settings_set_boolean(self->global_settings, "bypass", 0);
                                                  return;  // filter connected through update_bypass_state
                                                }
                                                self->set_bypass(false);
                                                break;
                                              }
                                            }
                                          }),
                                          this));

  gconnections.push_back(g_signal_connect(settings, "changed::plugins",
                                          G_CALLBACK(+[](GSettings* settings, char* key, gpointer user_data) {
                                            auto* self = static_cast<StreamInputEffects*>(user_data);
                                            if (g_settings_get_boolean(self->global_settings, "bypass") != 0) {
                                              g_settings_set_boolean(self->global_settings, "bypass", 0);
                                              return;  // filter connected through update_bypass_state
                                            }
                                            self->set_bypass(false);
                                          }),
                                          this));
}

StreamInputEffects::~StreamInputEffects() {
  disconnect_filters();
  util::debug("destroyed");
}

void StreamInputEffects::on_app_added(const NodeInfo node_info) {
  const auto blocklist = util::gchar_array_to_vector(g_settings_get_strv(settings, "blocklist"));
  auto is_blocklisted = std::ranges::find(blocklist, node_info.application_id) != blocklist.end();
  is_blocklisted = is_blocklisted || std::ranges::find(blocklist, node_info.name) != blocklisted.end();

  if (g_settings_get_boolean(global_settings, "process-all-inputs") != 0 && !is_blocklisted) {
    pm->connect_stream_input(node_info.id);
  }
}

auto StreamInputEffects::apps_want_to_play() -> bool {
  return std::ranges::any_of(pm->list_links, [&](const auto& link) {
    return (link.output_node_id == pm->ee_source_node.id) && (link.state == PW_LINK_STATE_ACTIVE);
  });
}

void StreamInputEffects::on_link_changed(const LinkInfo link_info) {
  if (link_info.state != PW_LINK_STATE_ACTIVE && link_info.state != PW_LINK_STATE_PAUSED) {
    return;
  }

  if (pm->default_input_device_name == pm->ee_source_node.name) {
    return;
  }

  if (bypass) {
    return;
  }

  if (apps_want_to_play()) {
    if (list_proxies.empty()) {
      util::debug("At least one app linked to our device wants to play. Linking our filters.");
      connect_filters();
    }
  } else {
    if (g_settings_get_boolean(global_settings, "inactivity-timer-enable")) {
      int inactivity_timeout = g_settings_get_int(global_settings, "inactivity-timeout");
      g_timeout_add_seconds(inactivity_timeout, GSourceFunc(+[](StreamInputEffects* self) {
                              if (!self->apps_want_to_play() && !self->list_proxies.empty()) {
                                util::debug("No app linked to our device wants to play. Unlinking our filters.");
                                self->disconnect_filters();
                              }
                              return G_SOURCE_REMOVE;
                            }),
                            this);
    } else {
      if (!list_proxies.empty()) {
        util::debug("No app linked to our device wants to play, but the inactivity timer is disabled. Leaving filters linked.");
      }
    }
  }
}

void StreamInputEffects::connect_filters(const bool& bypass) {
  const auto input_device_name = util::gsettings_get_string(settings, "input-device");

  if (input_device_name.empty()) {
    util::debug("No input device set. Aborting the link");
    return;
  }

  bool dev_exists = false;

  for (const auto& [serial, node] : pm->node_map) {
    if (node.name == input_device_name) {
      dev_exists = true;
      pm->input_device = node;
      break;
    }
  }

  if (!dev_exists) {
    util::debug("The input device " + input_device_name + " is not available. Aborting the link");
    return;
  }

  const auto list = (bypass) ? std::vector<std::string>() : util::gchar_array_to_vector(g_settings_get_strv(settings, "plugins"));

  auto mic_linked = false;
  int timeout = 0;

  while (pm->count_node_ports(pm->input_device.id) < 1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    timeout++;
    if (timeout > 10000) {
      util::warning("Information about the ports of the input device " + pm->input_device.name + " with id " + util::to_string(pm->input_device.id) + " are taking to long to be available. Aborting the link");
      return;
    }
  }

  uint prev_node_id = pm->input_device.id;
  uint next_node_id = 0U;

  if (!list.empty()) {
    for (const auto& name : list) {
      if (!plugins.contains(name)) {
        continue;
      }

      if (!plugins[name]->connected_to_pw ? plugins[name]->connect_to_pw() : true) {
        next_node_id = plugins[name]->get_node_id();
        const auto links = pm->link_nodes(prev_node_id, next_node_id);

        for (auto* link : links) {
          list_proxies.push_back(link);
        }

        if (mic_linked && (links.size() == 2U)) {
          prev_node_id = next_node_id;
        } else if (!mic_linked && (!links.empty())) {
          prev_node_id = next_node_id;
          mic_linked = true;
        } else {
          util::warning(" link from node " + util::to_string(prev_node_id) + " to node " + util::to_string(next_node_id) + " failed");
        }
      }
    }

    for (const auto& name : list) {
      if (!plugins.contains(name)) {
        continue;
      }

      if (name.starts_with(tags::plugin_name::echo_canceller)) {
        if (plugins[name]->connected_to_pw) {
          for (const auto& link : pm->link_nodes(pm->output_device.id, plugins[name]->get_node_id(), true)) {
            list_proxies.push_back(link);
          }
        }
      }

      plugins[name]->update_probe_links();
    }
  }

  for (const auto node_id : {spectrum->get_node_id(), output_level->get_node_id(), pm->ee_source_node.id}) {
    next_node_id = node_id;
    const auto links = pm->link_nodes(prev_node_id, next_node_id);

    for (auto* link : links) {
      list_proxies.push_back(link);
    }

    if (mic_linked && (links.size() == 2U)) {
      prev_node_id = next_node_id;
    } else if (!mic_linked && (!links.empty())) {
      prev_node_id = next_node_id;
      mic_linked = true;
    } else {
      util::warning(" link from node " + util::to_string(prev_node_id) + " to node " + util::to_string(next_node_id) + " failed");
    }
  }
}

void StreamInputEffects::disconnect_filters() {
  std::set<uint> link_id_list;
  const auto selected_plugins_list = (bypass) ? std::vector<std::string>() : util::gchar_array_to_vector(g_settings_get_strv(settings, "plugins"));

  for (const auto& plugin : plugins | std::views::values) {
    for (const auto& link : pm->list_links) {
      if (link.input_node_id == plugin->get_node_id() || link.output_node_id == plugin->get_node_id()) {
        link_id_list.insert(link.id);
      }
    }

    if (plugin->connected_to_pw) {
      if (std::ranges::find(selected_plugins_list, plugin->name) == selected_plugins_list.end()) {
        util::debug("disconnecting the " + plugin->name + " filter from PipeWire");
        plugin->disconnect_from_pw();
      }
    }
  }

  for (const auto& link : pm->list_links) {
    if (link.input_node_id == spectrum->get_node_id() || link.output_node_id == spectrum->get_node_id() ||
        link.input_node_id == output_level->get_node_id() || link.output_node_id == output_level->get_node_id()) {
      link_id_list.insert(link.id);
    }
  }

  for (const auto& id : link_id_list) {
    pm->destroy_object(static_cast<int>(id));
  }

  pm->destroy_links(list_proxies);
  list_proxies.clear();
}

void StreamInputEffects::set_bypass(const bool& state) {
  bypass = state;
  disconnect_filters();
  connect_filters(state);
}

void StreamInputEffects::set_listen_to_mic(const bool& state) {
  if (state) {
    for (const auto& link : pm->link_nodes(pm->ee_source_node.id, pm->output_device.id, false, false)) {
      list_proxies_listen_mic.push_back(link);
    }
  } else {
    pm->destroy_links(list_proxies_listen_mic);
    list_proxies_listen_mic.clear();
  }
}

// By GST @2024/10/27