#pragma once

#include <pipewire/context.h>
#include <pipewire/filter.h>
#include <pipewire/proxy.h>
#include <spa/utils/hook.h>
#include <sys/types.h>
#include <random>
#include <vector>
#include "pipe_manager.hpp"

enum class TestSignalType { sine_wave, gaussian, pink };

class TestSignals {
 public:
  explicit TestSignals(PipeManager* pipe_manager);
  TestSignals(const TestSignals&) = delete;
  TestSignals& operator=(const TestSignals&) = delete;
  TestSignals(TestSignals&&) = delete;
  TestSignals& operator=(TestSignals&&) = delete;
  virtual ~TestSignals();

  struct Data;

  struct Port {
    Data* data;
  };

  struct Data {
    Port* out_left = nullptr;
    Port* out_right = nullptr;
    TestSignals* ts = nullptr;
  };

  pw_filter* filter = nullptr;
  pw_filter_state state = PW_FILTER_STATE_UNCONNECTED;
  uint n_samples = 0U;
  uint rate = 0U;
  bool create_left_channel = true;
  bool create_right_channel = true;
  bool can_get_node_id = false;
  float sine_phase = 0.0F;
  float sine_frequency = 1000.0F;
  TestSignalType signal_type = TestSignalType::sine_wave;

  void set_state(bool state);
  void set_frequency(float value);
  [[nodiscard]] uint get_node_id() const;
  void set_active(bool state) const;
  void set_signal_type(TestSignalType value);
  float white_noise();

 private:
  PipeManager* pm = nullptr;
  spa_hook listener{};
  Data pf_data = {};
  uint node_id = 0U;
  std::vector<pw_proxy*> list_proxies;
  std::random_device rd{};
  std::mt19937 random_generator;
  std::normal_distribution<float> normal_distribution{0.0F, 0.3F};
};