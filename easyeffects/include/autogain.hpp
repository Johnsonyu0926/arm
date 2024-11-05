// Filename: AutoGain.hpp
// Score: 95

#pragma once

#include <ebur128.h>
#include <sigc++/signal.h>
#include <sys/types.h>
#include <span>
#include <string>
#include <thread>
#include <vector>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class AutoGain : public PluginBase {
public:
    AutoGain(const std::string& tag,
             const std::string& schema,
             const std::string& schema_path,
             PipeManager* pipe_manager,
             PipelineType pipe_type);
    AutoGain(const AutoGain&) = delete;
    auto operator=(const AutoGain&) -> AutoGain& = delete;
    AutoGain(const AutoGain&&) = delete;
    auto operator=(const AutoGain&&) -> AutoGain& = delete;
    ~AutoGain() override;

    enum class Reference {
        momentary,
        shortterm,
        integrated,
        geometric_mean_msi,
        geometric_mean_ms,
        geometric_mean_mi,
        geometric_mean_si
    };

    void setup() override;

    void process(std::span<float>& left_in,
                 std::span<float>& right_in,
                 std::span<float>& left_out,
                 std::span<float>& right_out) override;

    auto get_latency_seconds() -> float override;

    sigc::signal<void(const double,  // loudness
                      const double,  // gain
                      const double,  // momentary
                      const double,  // shortterm
                      const double,  // integrated
                      const double,  // relative
                      const double)>
        results;  // range

    double momentary = 0.0;
    double shortterm = 0.0;
    double global = 0.0;
    double relative = 0.0;
    double range = 0.0;
    double loudness = 0.0;

private:
    bool ebur128_ready = false;

    uint old_rate = 0U;

    double target = -23.0;  // target loudness level
    double silence_threshold = -70.0;
    double internal_output_gain = 1.0;

    Reference reference = Reference::geometric_mean_msi;

    std::vector<float> data;

    ebur128_state* ebur_state = nullptr;

    std::vector<std::thread> mythreads;

    auto init_ebur128() -> bool;

    static auto parse_reference_key(const std::string& key) -> Reference;

    void set_maximum_history(const int& seconds);
};
// By GST @Date
