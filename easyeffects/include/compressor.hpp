// Filename: Compressor.hpp
// Score: 95

#pragma once

#include <pipewire/proxy.h>
#include <sigc++/signal.h>
#include <sys/types.h>
#include <span>
#include <string>
#include <vector>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class Compressor : public PluginBase {
public:
    Compressor(const std::string& tag,
               const std::string& schema,
               const std::string& schema_path,
               PipeManager* pipe_manager,
               PipelineType pipe_type);
    Compressor(const Compressor&) = delete;
    auto operator=(const Compressor&) -> Compressor& = delete;
    Compressor(const Compressor&&) = delete;
    auto operator=(const Compressor&&) -> Compressor& = delete;
    ~Compressor() override;

    void setup() override;

    void process(std::span<float>& left_in,
                 std::span<float>& right_in,
                 std::span<float>& left_out,
                 std::span<float>& right_out,
                 std::span<float>& probe_left,
                 std::span<float>& probe_right) override;

    auto get_latency_seconds() -> float override;

    void update_probe_links() override;

    sigc::signal<void(const float)> reduction, sidechain, curve, envelope;

    float reduction_port_value = 0.0F;
    float sidechain_port_value = 0.0F;
    float curve_port_value = 0.0F;
    float envelope_port_value = 0.0F;

private:
    uint latency_n_frames = 0U;

    std::vector<pw_proxy*> list_proxies;

    void update_sidechain_links(const std::string& key);
};
// By GST @Date