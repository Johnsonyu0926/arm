// Filename: ClientInfoHolder.hpp
// Score: 95

#pragma once

#include <glib-object.h>
#include <glibconfig.h>
#include <sigc++/signal.h>
#include "pipe_objects.hpp"

namespace ui::holders {

G_BEGIN_DECLS

#define EE_TYPE_CLIENT_INFO_HOLDER (client_info_holder_get_type())

G_DECLARE_FINAL_TYPE(ClientInfoHolder, client_info_holder, EE, CLIENT_INFO_HOLDER, GObject)

G_END_DECLS

struct _ClientInfoHolder {
    GObject parent_instance;

    ClientInfo* info;

    sigc::signal<void(const ClientInfo)> info_updated;
};

auto create(const ClientInfo& info) -> ClientInfoHolder*;

}  // namespace ui::holders
// By GST @Date