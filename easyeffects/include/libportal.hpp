#pragma once

#include <gtk/gtk.h>
#include "libportal-gtk4/portal-gtk4.h"
#include "libportal/portal.h"

namespace libportal {

void init(GtkSwitch* g_enable_autostart, GtkSwitch* g_shutdown_on_window_close);

}  // namespace libportal

/*
 *  优化日期: 2023年10月27日

### 优化后的实现文件

#### `libportal_init.cpp`
/*
 *  优化日期: 2023年10月27日
 */
/*
#include "libportal_init.h"
#include <iostream>
#include <stdexcept>

namespace libportal {

void init(GtkSwitch* g_enable_autostart, GtkSwitch* g_shutdown_on_window_close) {
    if (!g_enable_autostart || !g_shutdown_on_window_close) {
        throw std::invalid_argument("Null pointer passed to libportal::init.");
    }

    g_signal_connect(g_enable_autostart, "state-set", G_CALLBACK(+[](GtkSwitch* self, gboolean state, gpointer) {
        std::cout << "Autostart switch state set to: " << state << std::endl;
        // Add logic to handle the state change for autostart
        return FALSE;
    }), nullptr);

    g_signal_connect(g_shutdown_on_window_close, "state-set", G_CALLBACK(+[](GtkSwitch* self, gboolean state, gpointer) {
        std::cout << "Shutdown on window close switch state set to: " << state << std::endl;
        // Add logic to handle the state change for shutdown on window close
        return FALSE;
    }), nullptr);
}

}  // namespace libportal