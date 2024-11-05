// Filename: node_info_holder.hpp
// Score: 100


#pragma once

#include <glib-object.h>
#include <glibconfig.h>
#include <gobject/gobject.h>
#include <gtk/gtk.h>
#include <sigc++/signal.h>
#include <string>
#include "pipe_objects.hpp"

namespace ui::holders {

G_BEGIN_DECLS

#define EE_TYPE_NODE_INFO_HOLDER (node_info_holder_get_type())

G_DECLARE_FINAL_TYPE(NodeInfoHolder, node_info_holder, EE, NODE_INFO_HOLDER, GObject)

G_END_DECLS

struct _NodeInfoHolder {
  GObject parent_instance;

  NodeInfo* info;
  std::string icon_name;  // The name of the icon that will represent the node when we show it in a list

  sigc::signal<void(const NodeInfo)> info_updated;
};

auto create(const NodeInfo& info) -> NodeInfoHolder*;

}  // namespace ui::holders

// By GST @2024/10/27