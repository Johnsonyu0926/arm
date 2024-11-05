// Filename: libportal.cpp
#include "libportal.hpp"
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
#include <glibconfig.h>
#include <gobject/gobject.h>
#include <gtk/gtk.h>
#include <string>
#include "tags_app.hpp"
#include "ui_helpers.hpp"
#include "util.hpp"

// By GST @2024/10/26
namespace {

bool resetting_shutdown = false, resetting_autostart = false;
GSettings* settings = nullptr;
GtkSwitch *enable_autostart = nullptr, *shutdown_on_window_close = nullptr;

}  // namespace

namespace libportal {

using namespace std::string_literals;

XdpPortal* portal = nullptr;

void on_request_background_called(GObject* source, GAsyncResult* result, gpointer widgets_ptr) {
  if (!enable_autostart || !shutdown_on_window_close) {
    util::error("Programming error. When using libportal its pointers to our settings widgets must be initialized.");
    return;
  }

  g_autoptr(GError) error = nullptr;

  if (xdp_portal_request_background_finish(portal, result, &error) == 0) {
    std::string reason, explanation, error_message;

    if (error) {
      error_message = error->message;
      if (error->code == 19) {
        reason = "Background access has been denied";
        explanation = "Please allow Easy Effects to ask again with Flatpak permission-reset "s + tags::app::id;
      } else {
        reason = "Generic error";
        explanation = "Please verify your system has a XDG Background Portal implementation running and working.";
      }
    } else {
      error_message = "unknown reason";
      reason = "Unknown error";
      explanation = "No explanation could be provided, error was null";
    }

    util::debug("A background request failed: " + error_message);
    util::warning(reason);
    util::warning(explanation);

    ui::show_simple_message_dialog(GTK_WIDGET(g_list_model_get_item(gtk_window_get_toplevels(), 1)),
                                   "Unable to get background access: " + reason, explanation);

    if (gtk_switch_get_active(enable_autostart)) {
      resetting_autostart = true;
      util::warning("due to error, setting autostart state and switch to false");
      gtk_switch_set_state(enable_autostart, 0);
      gtk_switch_set_active(enable_autostart, 0);
    }

    if (!gtk_switch_get_active(shutdown_on_window_close)) {
      resetting_shutdown = true;
      util::warning("due to error, setting shutdown on window close state and switch to true");
      gtk_switch_set_state(shutdown_on_window_close, 1);
      gtk_switch_set_active(shutdown_on_window_close, 1);
    }

    resetting_autostart = false;
    resetting_shutdown = false;
    return;
  }

  gtk_switch_set_state(enable_autostart, gtk_switch_get_active(enable_autostart));
  gtk_switch_set_state(shutdown_on_window_close, gtk_switch_get_active(shutdown_on_window_close));

  resetting_autostart = false;
  resetting_shutdown = false;

  util::debug("a background request successfully completed");
}

void update_background_portal(bool use_autostart) {
  XdpBackgroundFlags background_flags = XDP_BACKGROUND_FLAG_NONE;
  g_autoptr(GPtrArray) command_line = nullptr;

  if (use_autostart) {
    command_line = g_ptr_array_new_with_free_func(g_free);
    g_ptr_array_add(command_line, g_strdup("easyeffects"));
    g_ptr_array_add(command_line, g_strdup("--gapplication-service"));
    background_flags = XDP_BACKGROUND_FLAG_AUTOSTART;
  }

  auto* reason = g_strdup("Easy Effects Background Access");
  xdp_portal_request_background(portal, nullptr, reason, command_line, background_flags, nullptr,
                                on_request_background_called, nullptr);
  g_free(reason);
}

void on_enable_autostart(GtkSwitch* obj, gboolean state, gpointer user_data) {
  if (!resetting_autostart) {
    util::debug(state ? "requesting autostart file since autostart is enabled"
                      : "not requesting autostart file since autostart is disabled");
    update_background_portal(state);
  }
}

void on_shutdown_on_window_close(GtkSwitch* btn, gboolean state, gpointer user_data) {
  if (!resetting_shutdown) {
    if (gtk_switch_get_active(enable_autostart)) {
      util::debug(state ? "requesting both background access and autostart file since autostart is enabled"
                        : "requesting autostart access since autostart enabled");
      update_background_portal(true);
    } else {
      util::debug(state ? "not requesting any access since enabling shutdown on window close"
                        : "requesting only background access, not creating autostart file");
      update_background_portal(!state);
    }
  }
}

void init(GtkSwitch* g_enable_autostart, GtkSwitch* g_shutdown_on_window_close) {
  enable_autostart = g_enable_autostart;
  shutdown_on_window_close = g_shutdown_on_window_close;

  if (!enable_autostart || !shutdown_on_window_close) {
    util::error("Programming error. When using libportal its pointers to our settings widgets must be initialized.");
    return;
  }

  if (!portal) {
    portal = xdp_portal_new();
  }

  settings = g_settings_new("com.github.wwmm.easyeffects.libportal");

  ui::gsettings_bind_widget(settings, "enable-autostart", g_enable_autostart);

  g_signal_connect(enable_autostart, "state-set", G_CALLBACK(on_enable_autostart), nullptr);
  g_signal_connect(shutdown_on_window_close, "state-set", G_CALLBACK(on_shutdown_on_window_close), nullptr);

  auto enable_autostart_state = gtk_switch_get_active(enable_autostart);
  auto shutdown_on_window_close_state = gtk_switch_get_active(shutdown_on_window_close);

  if (!enable_autostart_state && !shutdown_on_window_close_state) {
    util::debug("doing portal sanity check, autostart and shutdown switches are disabled");
    update_background_portal(false);
  } else {
    util::debug("doing portal sanity check, autostart and/or shutdown switches are enabled");
    update_background_portal(enable_autostart_state);
  }
}

}  // namespace libportal