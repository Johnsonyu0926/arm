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

class LevelMeter : public PluginBase {
 public:
  LevelMeter(const std::string& tag,
             const std::string& schema,
             const std::string& schema_path,
             PipeManager* pipe_manager,
             PipelineType pipe_type);
  LevelMeter(const LevelMeter&) = delete;
  auto operator=(const LevelMeter&) -> LevelMeter& = delete;
  LevelMeter(LevelMeter&&) = delete;
  auto operator=(LevelMeter&&) -> LevelMeter& = delete;
  ~LevelMeter() override;

  void setup() override;

  void process(std::span<float> left_in,
               std::span<float> right_in,
               std::span<float> left_out,
               std::span<float> right_out) override;

  auto get_latency_seconds() -> float override;

  void reset_history();

  sigc::signal<void(double,  // momentary
                    double,  // shortterm
                    double,  // integrated
                    double,  // relative
                    double,  // range
                    double,  // true_peak_L
                    double   // true_peak_R
                    )>
      results;  // range

 private:
  bool ebur128_ready = false;
  uint old_rate = 0U;

  double momentary = 0.0;
  double shortterm = 0.0;
  double global = 0.0;
  double relative = 0.0;
  double range = 0.0;
  double true_peak_L = 0.0;
  double true_peak_R = 0.0;

  std::vector<float> data;
  ebur128_state* ebur_state = nullptr;
  std::vector<std::thread> mythreads;

  auto init_ebur128() -> bool;
};

/*
 *  优化日期: 2023年10月27日

### 优化后的实现文件

#### `level_meter.cpp`
/*
 *  优化日期: 2023年10月27日
 */
/*
#include "level_meter.h"
#include <iostream>
#include <stdexcept>

LevelMeter::LevelMeter(const std::string& tag,
                       const std::string& schema,
                       const std::string& schema_path,
                       PipeManager* pipe_manager,
                       PipelineType pipe_type)
    : PluginBase(tag, schema, schema_path, pipe_manager, pipe_type), ebur128_ready(false), old_rate(0U) {
    if (!init_ebur128()) {
        throw std::runtime_error("Failed to initialize ebur128.");
    }
}

LevelMeter::~LevelMeter() {
    if (ebur_state) {
        ebur128_destroy(&ebur_state);
    }
    for (auto& thread : mythreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void LevelMeter::setup() {
    // 实现 setup 逻辑
}

void LevelMeter::process(std::span<float> left_in,
                         std::span<float> right_in,
                         std::span<float> left_out,
                         std::span<float> right_out) {
    // 实现 process 逻辑
    if (ebur128_ready) {
        // Processing with ebur128
        ebur128_loudness_momentary(ebur_state, left_in.data(), left_in.size(), &momentary);
        ebur128_loudness_shortterm(ebur_state, left_in.data(), left_in.size(), &shortterm);
        ebur128_loudness_global(ebur_state, &global);
        ebur128_loudness_range(ebur_state, &range);
        ebur128_true_peak(ebur_state, 0, &true_peak_L);
        ebur128_true_peak(ebur_state, 1, &true_peak_R);

        results.emit(momentary, shortterm, global, relative, range, true_peak_L, true_peak_R);
    }
}

auto LevelMeter::get_latency_seconds() -> float {
    return static_cast<float>(latency_n_frames) / sample_rate;
}

void LevelMeter::reset_history() {
    if (ebur_state) {
        ebur128_reset(ebur_state);
    }
}

auto LevelMeter::init_ebur128() -> bool {
    ebur_state = ebur128_init(2, sample_rate, EBUR128_MODE_M | EBUR128_MODE_S | EBUR128_MODE_I | EBUR128_MODE_LRA | EBUR128_MODE_TP);
    if (!ebur_state) {
        std::cerr << "Failed to initialize ebur128 state." << std::endl;
        return false;
    }
    ebur128_ready = true;
    return true;
}