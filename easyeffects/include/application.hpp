// Filename: Application.hpp
// Score: 95

#pragma once

#include <adwaita.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <glibconfig.h>
#include <sigc++/connection.h>
#include <vector>
#include "pipe_manager.hpp"
#include "presets_manager.hpp"
#include "stream_input_effects.hpp"
#include "stream_output_effects.hpp"

namespace app {

G_BEGIN_DECLS

#define EE_TYPE_APPLICATION (application_get_type())

G_DECLARE_FINAL_TYPE(Application, application, EE, APP, AdwApplication)

G_END_DECLS

struct Data {
public:
    std::vector<sigc::connection> connections;
    std::vector<gulong> gconnections, gconnections_sie, gconnections_soe;
};

struct _Application {
    AdwApplication parent_instance;

    GSettings* settings;
    GSettings* soe_settings;
    GSettings* sie_settings;

    PipeManager* pm;
    StreamOutputEffects* soe;
    StreamInputEffects* sie;
    PresetsManager* presets_manager;

    Data* data;
};

auto application_new() -> GApplication*;

void hide_all_windows(GApplication* app);

}  // namespace app
// By GST @Date