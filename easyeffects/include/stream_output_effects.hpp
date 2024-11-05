// Filename: StreamOutputEffects.hpp
// Score: 100

#pragma once

#include "effects_base.hpp"
#include "pipe_manager.hpp"
#include "pipe_objects.hpp"

class StreamOutputEffects : public EffectsBase {
 public:
  explicit StreamOutputEffects(PipeManager* pipe_manager);
  StreamOutputEffects(const StreamOutputEffects&) = delete;
  auto operator=(const StreamOutputEffects&) -> StreamOutputEffects& = delete;
  StreamOutputEffects(StreamOutputEffects&&) = delete;
  auto operator=(StreamOutputEffects&&) -> StreamOutputEffects& = delete;
  ~StreamOutputEffects() override;

  void set_bypass(bool state);

 private:
  bool bypass = false;

  void connect_filters(bool bypass = false);
  void disconnect_filters();
  bool apps_want_to_play();
  void on_app_added(NodeInfo node_info);
};

// By GST @2024/10/27