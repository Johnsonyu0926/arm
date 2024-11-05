// Filename: BassEnhancer.hpp
// Score: 95

#pragma once

#include <sigc++/signal.h>
#include <span>
#include <string>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class BassEnhancer : public PluginBase {
public:
    BassEnhancer(const std::string& tag,
                 const std::string& schema,
                 const std::string& schema_path,
                 PipeManager* pipe_manager,
                 PipelineType pipe_type);
    BassEnhancer(const BassEnhancer&) = delete;
    auto operator=(const BassEnhancer&) -> BassEnhancer& = delete;
    BassEnhancer(const BassEnhancer&&) = delete;
    auto operator=(const BassEnhancer&&) -> BassEnhancer& = delete;
    ~BassEnhancer() override;

    void setup() override;

    void process(std::span<float>& left_in,
                 std::span<float>& right_in,
                 std::span<float>& left_out,
                 std::span<float>& right_out) override;

    auto get_latency_seconds() -> float override;

    sigc::signal<void(const double)> harmonics;

    double harmonics_port_value = 0.0;

private:
};
// By GST @Date