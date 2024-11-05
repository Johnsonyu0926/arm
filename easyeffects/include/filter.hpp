// Filename: Filter.hpp
// Score: 95

#pragma once

#include <span>
#include <string>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class Filter : public PluginBase {
public:
    Filter(const std::string& tag,
           const std::string& schema,
           const std::string& schema_path,
           PipeManager* pipe_manager,
           PipelineType pipe_type);
    Filter(const Filter&) = delete;
    auto operator=(const Filter&) -> Filter& = delete;
    Filter(const Filter&&) = delete;
    auto operator=(const Filter&&) -> Filter& = delete;
    ~Filter() override;

    void setup() override;

    void process(std::span<float>& left_in,
                 std::span<float>& right_in,
                 std::span<float>& left_out,
                 std::span<float>& right_out) override;

    auto get_latency_seconds() -> float override;

private:
};
// By GST @Date