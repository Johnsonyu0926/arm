// Filename: DeepFilterNet.hpp
// Score: 95

#pragma once

#include <memory>
#include <span>
#include <string>
#include <vector>
#include "ladspa_wrapper.hpp"
#include "pipe_manager.hpp"
#include "plugin_base.hpp"
#include "resampler.hpp"

class DeepFilterNet : public PluginBase {
public:
    DeepFilterNet(const std::string& tag,
                  const std::string& schema,
                  const std::string& schema_path,
                  PipeManager* pipe_manager,
                  PipelineType pipe_type);
    DeepFilterNet(const DeepFilterNet&) = delete;
    auto operator=(const DeepFilterNet&) -> DeepFilterNet& = delete;
    DeepFilterNet(const DeepFilterNet&&) = delete;
    auto operator=(const DeepFilterNet&&) -> DeepFilterNet& = delete;
    ~DeepFilterNet() override;

    void setup() override;

    void process(std::span<float>& left_in,
                 std::span<float>& right_in,
                 std::span<float>& left_out,
                 std::span<float>& right_out) override;

    auto get_latency_seconds() -> float override;

private:
    std::unique_ptr<ladspa::LadspaWrapper> ladspa_wrapper;

    bool resample = false;
    bool resampler_ready = true;

    std::unique_ptr<Resampler> resampler_inL, resampler_outL;
    std::unique_ptr<Resampler> resampler_inR, resampler_outR;

    std::vector<float> resampled_outL, resampled_outR;
    std::vector<float> carryover_l, carryover_r;
};
// By GST @Date