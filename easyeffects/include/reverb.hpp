// Filename: Reverb.hpp
// Score: 100


#pragma once

#include <span>
#include <string>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class Reverb : public PluginBase {
 public:
  Reverb(const std::string& tag,
         const std::string& schema,
         const std::string& schema_path,
         PipeManager* pipe_manager,
         PipelineType pipe_type);
  Reverb(const Reverb&) = delete;
  auto operator=(const Reverb&) -> Reverb& = delete;
  Reverb(Reverb&&) = delete;
  auto operator=(Reverb&&) -> Reverb& = delete;
  ~Reverb() override;

  void setup() override;

  void process(std::span<float> left_in,
               std::span<float> right_in,
               std::span<float> left_out,
               std::span<float> right_out) override;

  auto get_latency_seconds() -> float override;

 private:
};

// By GST @2024/10/27