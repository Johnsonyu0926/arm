// Filename: Resampler.hpp
// Score: 100

#pragma once

#include <samplerate.h>
#include <cmath>
#include <vector>

class Resampler {
 public:
  Resampler(int input_rate, int output_rate);
  Resampler(const Resampler&) = delete;
  auto operator=(const Resampler&) -> Resampler& = delete;
  Resampler(Resampler&&) = delete;
  auto operator=(Resampler&&) -> Resampler& = delete;
  ~Resampler();

  template <typename T>
  auto process(const T& input, bool end_of_input) -> const std::vector<float>& {
    output.resize(std::ceil(1.5 * resample_ratio * input.size()));

    // The number of frames of data pointed to by data_in
    src_data.input_frames = input.size();

    // A pointer to the input data samples
    src_data.data_in = input.data();

    // Maximum number of frames pointed to by data_out
    src_data.output_frames = static_cast<long>(output.size());

    // A pointer to the output data samples
    src_data.data_out = output.data();

    // Equal to output_sample_rate / input_sample_rate
    src_data.src_ratio = resample_ratio;

    // Equal to 0 if more input data is available and 1 otherwise
    src_data.end_of_input = static_cast<int>(end_of_input);

    src_process(src_state, &src_data);

    output.resize(src_data.output_frames_gen);

    return output;
  }

 private:
  double resample_ratio = 1.0;

  SRC_STATE* src_state = nullptr;

  SRC_DATA src_data{};

  std::vector<float> output;
};

// By GST @2024/10/27