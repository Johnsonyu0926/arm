// Filename: bass_loudness.cpp
// 优化日期: 2024/10/26

#include "bass_loudness.hpp"
#include <algorithm>
#include <memory>
#include <span>
#include <string>
#include "lv2_wrapper.hpp"
#include "pipe_manager.hpp"
#include "plugin_base.hpp"
#include "tags_plugin_name.hpp"
#include "util.hpp"

BassLoudness::BassLoudness(const std::string& tag,
                           const std::string& schema,
                           const std::string& schema_path,
                           PipeManager* pipe_manager,
                           PipelineType pipe_type)
    : PluginBase(tag,
                 tags::plugin_name::bass_loudness,
                 tags::plugin_package::mda,
                 schema,
                 schema_path,
                 pipe_manager,
                 pipe_type) {
  lv2_wrapper = std::make_unique<lv2::Lv2Wrapper>("http://drobilla.net/plugins/mda/Loudness");

  package_installed = lv2_wrapper->found_plugin;

  if (!package_installed) {
    util::debug(log_tag + "http://drobilla.net/plugins/mda/Loudness is not installed");
  }

  lv2_wrapper->bind_key_double_db<"loudness", "loudness">(settings);
  lv2_wrapper->bind_key_double_db<"output", "output">(settings);
  lv2_wrapper->bind_key_double_db<"link", "link">(settings);

  setup_input_output_gain();
}

BassLoudness::~BassLoudness() {
  if (connected_to_pw) {
    disconnect_from_pw();
  }

  util::debug(log_tag + name + " destroyed");
}

void BassLoudness::setup() {
  if (!lv2_wrapper->found_plugin) {
    return;
  }

  lv2_wrapper->set_n_samples(n_samples);

  if (lv2_wrapper->get_rate() != rate) {
    lv2_wrapper->create_instance(rate);
  }
}

void BassLoudness::process(std::span<float>& left_in,
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

auto BassLoudness::get_latency_seconds() -> float {
  return 0.0F;
}

// By GST @2024/10/26