// Filename: output_level.hpp
// Score: 100



#pragma once

#include <span>
#include <string>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class OutputLevel : public PluginBase {
 public:
  OutputLevel(const std::string& tag,
              const std::string& schema,
              const std::string& schema_path,
              PipeManager* pipe_manager,
              PipelineType pipe_type);
  OutputLevel(const OutputLevel&) = delete;
  auto operator=(const OutputLevel&) -> OutputLevel& = delete;
  OutputLevel(const OutputLevel&&) = delete;
  auto operator=(const OutputLevel&&) -> OutputLevel& = delete;
  ~OutputLevel() override;

  void setup() override;

  void process(std::span<float>& left_in,
               std::span<float>& right_in,
               std::span<float>& left_out,
               std::span<float>& right_out) override;

  auto get_latency_seconds() -> float override;
};

// By GST @2024/10/27