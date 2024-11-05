// Filename: Crossfeed.hpp
// Score: 95

#pragma once

#include <bs2bclass.h>
#include <span>
#include <string>
#include <vector>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class Crossfeed : public PluginBase {
public:
    Crossfeed(const std::string& tag,
              const std::string& schema,
              const std::string& schema_path,
              PipeManager* pipe_manager,
              PipelineType pipe_type);
    Crossfeed(const Crossfeed&) = delete;
    auto operator=(const Crossfeed&) -> Crossfeed& = delete;
    Crossfeed(const Crossfeed&&) = delete;
    auto operator=(const Crossfeed&&) -> Crossfeed& = delete;
    ~Crossfeed() override;

    void setup() override;

    void process(std::span<float>& left_in,
                 std::span<float>& right_in,
                 std::span<float>& left_out,
                 std::span<float>& right_out) override;

    auto get_latency_seconds() -> float override;

private:
    std::vector<float> data;

    bs2b_base bs2b;
};
// By GST @Date