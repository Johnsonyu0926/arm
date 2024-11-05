#pragma once

#include <span>
#include <string>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class StereoTools : public PluginBase {
 public:
  StereoTools(const std::string& tag,
              const std::string& schema,
              const std::string& schema_path,
              PipeManager* pipe_manager,
              PipelineType pipe_type);
  StereoTools(const StereoTools&) = delete;
  auto operator=(const StereoTools&) -> StereoTools& = delete;
  StereoTools(StereoTools&&) = delete;
  auto operator=(StereoTools&&) -> StereoTools& = delete;
  ~StereoTools() override;

  void setup() override;

  void process(std::span<float> left_in,
               std::span<float> right_in,
               std::span<float> left_out,
               std::span<float> right_out) override;

  auto get_latency_seconds() -> float override;

  double correlation_port_value = 0.0;

 private:
  float dry = 0.0F, wet = 1.0F;
};

// By GST @2024/10/27