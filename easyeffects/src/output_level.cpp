// Filename: output_level.cpp
// Optimized on: 2024/10/27

#include "output_level.hpp"
#include <algorithm>
#include <span>
#include <string>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"
#include "tags_plugin_name.hpp"
#include "util.hpp"

OutputLevel::OutputLevel(const std::string& tag,
                         const std::string& schema,
                         const std::string& schema_path,
                         PipeManager* pipe_manager,
                         PipelineType pipe_type)
    : PluginBase(tag, "output_level", tags::plugin_package::ee, schema, schema_path, pipe_manager, pipe_type) {}

OutputLevel::~OutputLevel() {
  if (connected_to_pw) {
    disconnect_from_pw();
  }
  util::debug(log_tag + name + " destroyed");
}

void OutputLevel::setup() {
  util::debug(log_tag + name + ": PipeWire blocksize: " + util::to_string(n_samples, ""));
  util::debug(log_tag + name + ": PipeWire sampling rate: " + util::to_string(rate, ""));
}

void OutputLevel::process(std::span<float>& left_in,
                          std::span<float>& right_in,
                          std::span<float>& left_out,
                          std::span<float>& right_out) {
  std::copy(left_in.begin(), left_in.end(), left_out.begin());
  std::copy(right_in.begin(), right_in.end(), right_out.begin());

  if (post_messages) {
    get_peaks(left_in, right_in, left_out, right_out);

    if (send_notifications) {
      notify();
    }
  }
}

auto OutputLevel::get_latency_seconds() -> float {
  return 0.0F;
}

// Optimized on: 2024/10/27