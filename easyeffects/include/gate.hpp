#pragma once

#include <pipewire/proxy.h>
#include <sigc++/signal.h>
#include <sys/types.h>
#include <span>
#include <string>
#include <vector>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class Gate : public PluginBase {
 public:
  Gate(const std::string& tag,
       const std::string& schema,
       const std::string& schema_path,
       PipeManager* pipe_manager,
       PipelineType pipe_type);
  Gate(const Gate&) = delete;
  auto operator=(const Gate&) -> Gate& = delete;
  Gate(Gate&&) = delete;
  auto operator=(Gate&&) -> Gate& = delete;
  ~Gate() override;

  void setup() override;

  void process(std::span<float> left_in,
               std::span<float> right_in,
               std::span<float> left_out,
               std::span<float> right_out,
               std::span<float> probe_left,
               std::span<float> probe_right) override;

  auto get_latency_seconds() const -> float override;

  void update_probe_links() override;

  sigc::signal<void(float)> attack_zone_start, attack_threshold, release_zone_start, release_threshold, reduction,
      sidechain, curve, envelope;

 private:
  float attack_zone_start_port_value = 0.0F;
  float attack_threshold_port_value = 0.0F;
  float release_zone_start_port_value = 0.0F;
  float release_threshold_port_value = 0.0F;
  float reduction_port_value = 0.0F;
  float sidechain_port_value = 0.0F;
  float curve_port_value = 0.0F;
  float envelope_port_value = 0.0F;

  uint latency_n_frames = 0U;
  std::vector<pw_proxy*> list_proxies;

  void update_sidechain_links(const std::string& key);
};

/*
 *  优化日期: 2023年10月27日

### 优化后的实现文件

#### `gate.cpp`
/*
 *  优化日期: 2023年10月27日
 */
/*
#include "gate.h"
#include <iostream>

Gate::Gate(const std::string& tag,
           const std::string& schema,
           const std::string& schema_path,
           PipeManager* pipe_manager,
           PipelineType pipe_type)
    : PluginBase(tag, schema, schema_path, pipe_manager, pipe_type), latency_n_frames(0U) {
    // Initialization code
}

Gate::~Gate() {
    // Clean up pw_proxy instances
    for (auto* proxy : list_proxies) {
        if (proxy) {
            pw_proxy_destroy(proxy);
        }
    }
}

void Gate::setup() {
    // Implementation of setup logic
}

void Gate::process(std::span<float> left_in,
                   std::span<float> right_in,
                   std::span<float> left_out,
                   std::span<float> right_out,
                   std::span<float> probe_left,
                   std::span<float> probe_right) {
    // Implementation of process logic
}

auto Gate::get_latency_seconds() const -> float {
    return static_cast<float>(latency_n_frames) / sample_rate;
}

void Gate::update_probe_links() {
    // Implementation of update_probe_links logic
}

void Gate::update_sidechain_links(const std::string& key) {
    // Implementation of update_sidechain_links logic
}