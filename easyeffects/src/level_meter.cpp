// Filename: level_meter.hpp
#include "level_meter.hpp"
#include <ebur128.h>
#include <algorithm>
#include <cstddef>
#include <mutex>
#include <span>
#include <string>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"
#include "tags_plugin_name.hpp"
#include "util.hpp"

// By GST @2024/10/26
LevelMeter::LevelMeter(const std::string& tag,
                       const std::string& schema,
                       const std::string& schema_path,
                       PipeManager* pipe_manager,
                       PipelineType pipe_type)
    : PluginBase(tag,
                 tags::plugin_name::level_meter,
                 tags::plugin_package::ebur128,
                 schema,
                 schema_path,
                 pipe_manager,
                 pipe_type) {}

LevelMeter::~LevelMeter() {
  if (connected_to_pw) {
    disconnect_from_pw();
  }

  for (auto& t : mythreads) {
    t.join();
  }

  mythreads.clear();

  std::scoped_lock<std::mutex> lock(data_mutex);

  if (ebur_state != nullptr) {
    ebur128_destroy(&ebur_state);
  }

  util::debug(log_tag + name + " destroyed");
}

auto LevelMeter::init_ebur128() -> bool {
  if (n_samples == 0U || rate == 0U) {
    return false;
  }

  if (ebur_state != nullptr) {
    ebur128_destroy(&ebur_state);
    ebur_state = nullptr;
  }

  ebur_state = ebur128_init(
      2U, rate, EBUR128_MODE_S | EBUR128_MODE_I | EBUR128_MODE_LRA | EBUR128_MODE_TRUE_PEAK | EBUR128_MODE_HISTOGRAM);

  ebur128_set_channel(ebur_state, 0U, EBUR128_LEFT);
  ebur128_set_channel(ebur_state, 1U, EBUR128_RIGHT);

  return ebur_state != nullptr;
}

void LevelMeter::setup() {
  if (2U * static_cast<size_t>(n_samples) != data.size()) {
    data.resize(static_cast<size_t>(n_samples) * 2U);
  }

  if (rate != old_rate) {
    std::scoped_lock<std::mutex> lock(data_mutex);

    ebur128_ready = false;

    mythreads.emplace_back([this]() {
      if (ebur128_ready) {
        return;
      }

      old_rate = rate;
      bool status = init_ebur128();

      std::scoped_lock<std::mutex> lock(data_mutex);
      ebur128_ready = status;
    });
  }
}

void LevelMeter::process(std::span<float>& left_in,
                         std::span<float>& right_in,
                         std::span<float>& left_out,
                         std::span<float>& right_out) {
  std::scoped_lock<std::mutex> lock(data_mutex);

  std::copy(left_in.begin(), left_in.end(), left_out.begin());
  std::copy(right_in.begin(), right_in.end(), right_out.begin());

  if (bypass || !ebur128_ready) {
    return;
  }

  for (size_t n = 0U; n < n_samples; n++) {
    data[2U * n] = left_in[n];
    data[2U * n + 1U] = right_in[n];
  }

  ebur128_add_frames_float(ebur_state, data.data(), n_samples);

  if (EBUR128_SUCCESS != ebur128_loudness_momentary(ebur_state, &momentary)) {
    momentary = 0.0;
  }

  if (EBUR128_SUCCESS != ebur128_loudness_shortterm(ebur_state, &shortterm)) {
    shortterm = 0.0;
  }

  if (EBUR128_SUCCESS != ebur128_loudness_global(ebur_state, &global)) {
    global = 0.0;
  }

  if (EBUR128_SUCCESS != ebur128_relative_threshold(ebur_state, &relative)) {
    relative = 0.0;
  }

  if (EBUR128_SUCCESS != ebur128_loudness_range(ebur_state, &range)) {
    range = 0.0;
  }

  if (EBUR128_SUCCESS != ebur128_true_peak(ebur_state, 0U, &true_peak_L)) {
    true_peak_L = 0.0;
  }

  if (EBUR128_SUCCESS != ebur128_true_peak(ebur_state, 1U, &true_peak_R)) {
    true_peak_R = 0.0;
  }

  if (post_messages) {
    get_peaks(left_in, right_in, left_out, right_out);

    if (send_notifications) {
      results.emit(momentary, shortterm, global, relative, range, true_peak_L, true_peak_R);
      notify();
    }
  }
}

auto LevelMeter::get_latency_seconds() -> float {
  return 0.0F;
}

void LevelMeter::reset_history() {
  mythreads.emplace_back([this]() {
    std::scoped_lock<std::mutex> lock(data_mutex);

    ebur128_ready = false;

    bool status = init_ebur128();

    ebur128_ready = status;
  });
}