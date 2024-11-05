// Filename: Spectrum.hpp
// Score: 100

#pragma once

#include <atomic>
#include <fftw3.h>
#include <sigc++/signal.h>
#include <sys/types.h>
#include <deque>
#include <span>
#include <string>
#include <vector>
#include <array>
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class Spectrum : public PluginBase {
 public:
  Spectrum(const std::string& tag,
           const std::string& schema,
           const std::string& schema_path,
           PipeManager* pipe_manager,
           PipelineType pipe_type);
  Spectrum(const Spectrum&) = delete;
  auto operator=(const Spectrum&) -> Spectrum& = delete;
  Spectrum(Spectrum&&) = delete;
  auto operator=(Spectrum&&) -> Spectrum& = delete;
  ~Spectrum() override;

  void setup() override;

  void process(std::span<float> left_in,
               std::span<float> right_in,
               std::span<float> left_out,
               std::span<float> right_out) override;

  auto get_latency_seconds() -> float override;

  std::tuple<uint, uint, double*> compute_magnitudes();  // rate, nbands, magnitudes

 private:
  std::atomic<bool> fftw_ready = false;

  fftwf_plan plan = nullptr;
  fftwf_complex* complex_output = nullptr;

  static constexpr uint n_bands = 8192U;

  std::array<float, n_bands> real_input{};
  std::array<double, n_bands / 2U + 1U> output{};

  std::vector<float> left_delayed_vector;
  std::vector<float> right_delayed_vector;
  std::span<float> left_delayed;
  std::span<float> right_delayed;

  std::array<float, n_bands> latest_samples_mono{};
  std::array<float, n_bands> hann_window{};

  enum {
    DB_BIT_IDX = (1 << 0),      // To which db_buffers array process() should write.
    DB_BIT_NEWDATA = (1 << 1),  // If new data has been written by process().
    DB_BIT_BUSY = (1 << 2),     // If process() is currently writing data.
  };

  std::array<std::array<float, n_bands>, 2> db_buffers{};
  std::atomic<int> db_control = {0};
  static_assert(std::atomic<int>::is_always_lock_free);
};

// By GST @2024/10/27