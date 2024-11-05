#pragma once

#include <sys/types.h>
#include <span>
#include <string>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class Loudness : public PluginBase {
 public:
  Loudness(const std::string& tag,
           const std::string& schema,
           const std::string& schema_path,
           PipeManager* pipe_manager,
           PipelineType pipe_type);
  Loudness(const Loudness&) = delete;
  auto operator=(const Loudness&) -> Loudness& = delete;
  Loudness(Loudness&&) = delete;
  auto operator=(Loudness&&) -> Loudness& = delete;
  ~Loudness() override;

  void setup() override;

  void process(std::span<float> left_in,
               std::span<float> right_in,
               std::span<float> left_out,
               std::span<float> right_out) override;

  auto get_latency_seconds() -> float override;

 private:
  uint latency_n_frames = 0U;
};

/*
 *  优化日期: 2023年10月27日

### 优化后的实现文件

#### `loudness.cpp`
/*
 *  优化日期: 2023年10月27日
 */
/*
#include "loudness.h"
#include <iostream>

Loudness::Loudness(const std::string& tag,
                   const std::string& schema,
                   const std::string& schema_path,
                   PipeManager* pipe_manager,
                   PipelineType pipe_type)
    : PluginBase(tag, schema, schema_path, pipe_manager, pipe_type), latency_n_frames(0U) {
    // Initialization code
}

Loudness::~Loudness() {
    // Cleanup code
}

void Loudness::setup() {
    // Implementation of setup logic
}

void Loudness::process(std::span<float> left_in,
                       std::span<float> right_in,
                       std::span<float> left_out,
                       std::span<float> right_out) {
    // Implementation of process logic
}

auto Loudness::get_latency_seconds() -> float {
    return static_cast<float>(latency_n_frames) / sample_rate;
}