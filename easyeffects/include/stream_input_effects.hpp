#pragma once

#include "effects_base.hpp"
#include "pipe_manager.hpp"
#include "pipe_objects.hpp"

class StreamInputEffects : public EffectsBase {
 public:
  explicit StreamInputEffects(PipeManager* pipe_manager);
  StreamInputEffects(const StreamInputEffects&) = delete;
  auto operator=(const StreamInputEffects&) -> StreamInputEffects& = delete;
  StreamInputEffects(StreamInputEffects&&) = delete;
  auto operator=(StreamInputEffects&&) -> StreamInputEffects& = delete;
  ~StreamInputEffects() override;

  void set_bypass(bool state);
  void set_listen_to_mic(bool state);

 private:
  bool bypass = false;

  void connect_filters(bool bypass = false);
  void disconnect_filters();
  bool apps_want_to_play();
  void on_app_added(NodeInfo node_info);
  void on_link_changed(LinkInfo link_info);
};

// By GST @2024/10/27