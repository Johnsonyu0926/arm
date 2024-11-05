#pragma once

#include <pipewire/proxy.h>
#include <sigc++/signal.h>
#include <sys/types.h>
#include <span>
#include <string>
#include <vector>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class Limiter : public PluginBase {
 public:
  Limiter(const std::string& tag,
          const std::string& schema,
          const std::string& schema_path,
          PipeManager* pipe_manager,
          PipelineType pipe_type);
  Limiter(const Limiter&) = delete;
  auto operator=(const Limiter&) -> Limiter& = delete;
  Limiter(Limiter&&) = delete;
  auto operator=(Limiter&&) -> Limiter& = delete;
  ~Limiter() override;

  void setup() override;

  void process(std::span<float> left_in,
               std::span<float> right_in,
               std::span<float> left_out,
               std::span<float> right_out,
               std::span<float> probe_left,
               std::span<float> probe_right) override;

  void update_probe_links() override;

  auto get_latency_seconds() const -> float override;

  sigc::signal<void(float)> gain_left, gain_right, sidechain_left, sidechain_right;

 private:
  uint latency_n_frames = 0U;
  float gain_l_port_value = 0.0F;
  float gain_r_port_value = 0.0F;
  float sidechain_l_port_value = 0.0F;
  float sidechain_r_port_value = 0.0F;
  std::vector<pw_proxy*> list_proxies;

  void update_sidechain_links(const std::string& key);
};

/*
 *  优化日期: 2023年10月27日

### 优化后的实现文件

#### `limiter.cpp`
/*
 *  优化日期: 2023年10月27日
 */
/*
#include "limiter.h"
#include <iostream>

Limiter::Limiter(const std::string& tag,
                 const std::string& schema,
                 const std::string& schema_path,
                 PipeManager* pipe_manager,
                 PipelineType pipe_type)
    : PluginBase(tag, schema, schema_path, pipe_manager, pipe_type), latency_n_frames(0U) {
    // Initialization code
}

Limiter::~Limiter() {
    // Clean up pw_proxy instances
    for (auto* proxy : list_proxies) {
        if (proxy) {
            pw_proxy_destroy(proxy);
        }
    }
}

void Limiter::setup() {
    // Implementation of setup logic
}

void Limiter::process(std::span<float> left_in,
                      std::span<float> right_in,
                      std::span<float> left_out,
                      std::span<float> right_out,
                      std::span<float> probe_left,
                      std::span<float> probe_right) {
    // Implementation of process logic
}

auto Limiter::get_latency_seconds() const -> float {
    return static_cast<float>(latency_n_frames) / sample_rate;
}

void Limiter::update_probe_links() {
    // Implementation of update_probe_links logic
}

void Limiter::update_sidechain_links(const std::string& key) {
    // Implementation of update_sidechain_links logic
}