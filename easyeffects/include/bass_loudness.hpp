// Filename: BassLoudness.hpp
// Score: 95

#pragma once

#include <span>
#include <string>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class BassLoudness : public PluginBase {
public:
    BassLoudness(const std::string& tag,
                 const std::string& schema,
                 const std::string& schema_path,
                 PipeManager* pipe_manager,
                 PipelineType pipe_type);
    BassLoudness(const BassLoudness&) = delete;
    auto operator=(const BassLoudness&) -> BassLoudness& = delete;
    BassLoudness(const BassLoudness&&) = delete;
    auto operator=(const BassLoudness&&) -> BassLoudness& = delete;
    ~BassLoudness() override;

    void setup() override;

    void process(std::span<float>& left_in,
                 std::span<float>& right_in,
                 std::span<float>& left_out,
                 std::span<float>& right_out) override;

    auto get_latency_seconds() -> float override;

private:
};
// By GST @Date