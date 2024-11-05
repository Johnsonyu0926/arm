// Filename: filter.cpp
// Optimized on: 2024/10/27

#include "filter.hpp"
#include <algorithm>
#include <memory>
#include <span>
#include <string>
#include "lv2_wrapper.hpp"
#include "pipe_manager.hpp"
#include "plugin_base.hpp"
#include "tags_plugin_name.hpp"
#include "util.hpp"

Filter::Filter(const std::string& tag,
               const std::string& schema,
               const std::string& schema_path,
               PipeManager* pipe_manager,
               PipelineType pipe_type)
    : PluginBase(tag,
                 tags::plugin_name::filter,
                 tags::plugin_package::lsp,
                 schema,
                 schema_path,
                 pipe_manager,
                 pipe_type) {
  lv2_wrapper = std::make_unique<lv2::Lv2Wrapper>("http://lsp-plug.in/plugins/lv2/filter_stereo");
  package_installed = lv2_wrapper->found_plugin;

  if (!package_installed) {
    util::debug(log_tag + "http://lsp-plug.in/plugins/lv2/sc_gate_stereo is not installed");
  }

  lv2_wrapper->bind_key_double<"f", "frequency">(settings);
  lv2_wrapper->bind_key_double<"w", "width">(settings);
  lv2_wrapper->bind_key_double<"q", "quality">(settings);
  lv2_wrapper->bind_key_double<"bal", "balance">(settings);
  lv2_wrapper->bind_key_double_db<"g", "gain">(settings);
  lv2_wrapper->bind_key_enum<"ft", "type">(settings);
  lv2_wrapper->bind_key_enum<"fm", "mode">(settings);
  lv2_wrapper->bind_key_enum<"mode", "equal-mode">(settings);
  lv2_wrapper->bind_key_enum<"s", "slope">(settings);

  setup_input_output_gain();
}

Filter::~Filter() {
  if (connected_to_pw) {
    disconnect_from_pw();
  }
  util::debug(log_tag + name + " destroyed");
}

void Filter::setup() {
  if (!lv2_wrapper->found_plugin) {
    return;
  }
  lv2_wrapper->set_n_samples(n_samples);
  if (lv2_wrapper->get_rate() != rate) {
    lv2_wrapper->create_instance(rate);
  }
}

void Filter::process(std::span<float>& left_in,
                     std::span<float>& right_in,
                     std::span<float>& left_out,
                     std::span<float>& right_out) {
  if (!lv2_wrapper->found_plugin || !lv2_wrapper->has_instance() || bypass) {
    std::copy(left_in.begin(), left_in.end(), left_out.begin());
    std::copy(right_in.begin(), right_in.end(), right_out.begin());
    return;
  }

  if (input_gain != 1.0F) {
    apply_gain(left_in, right_in, input_gain);
  }

  lv2_wrapper->connect_data_ports(left_in, right_in, left_out, right_out);
  lv2_wrapper->run();

  if (output_gain != 1.0F) {
    apply_gain(left_out, right_out, output_gain);
  }

  if (post_messages) {
    get_peaks(left_in, right_in, left_out, right_out);
    if (send_notifications) {
      notify();
    }
  }
}

auto Filter::get_latency_seconds() -> float {
  return 0.0F;
}

// Optimized on: 2024/10/27