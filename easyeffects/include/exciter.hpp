// Filename: Exciter.hpp
// Score: 95

#pragma once

#include <sigc++/signal.h>
#include <span>
#include <string>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class Exciter : public PluginBase {
public:
    Exciter(const std::string& tag,
            const std::string& schema,
            const std::string& schema_path,
            PipeManager* pipe_manager,
            PipelineType pipe_type);
    Exciter(const Exciter&) = delete;
    auto operator=(const Exciter&) -> Exciter& = delete;
    Exciter(const Exciter&&) = delete;
    auto operator=(const Exciter&&) -> Exciter& = delete;
    ~Exciter() override;

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