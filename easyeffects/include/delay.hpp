// Filename: Delay.hpp
// Score: 95

#pragma once

#include <sys/types.h>
#include <span>
#include <string>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class Delay : public PluginBase {
public:
    Delay(const std::string& tag,
          const std::string& schema,
          const std::string& schema_path,
          PipeManager* pipe_manager,
          PipelineType pipe_type);
    Delay(const Delay&) = delete;
    auto operator=(const Delay&) -> Delay& = delete;
    Delay(const Delay&&) = delete;
    auto operator=(const Delay&&) -> Delay& = delete;
    ~Delay() override;

    void setup() override;

    void process(std::span<float>& left_in,
                 std::span<float>& right_in,
                 std::span<float>& left_out,
                 std::span<float>& right_out) override;

    auto get_latency_seconds() -> float override;

private:
    uint latency_n_frames = 0U;
};
// By GST @Date