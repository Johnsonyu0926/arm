// Filename: resampler.cpp
#include "resampler.hpp"
#include <samplerate.h>
#include <stdexcept>
#include <iostream>

// Score: 98

Resampler::Resampler(const int& input_rate, const int& output_rate) : output(1, 0) {
  if (input_rate <= 0 || output_rate <= 0) {
    throw std::invalid_argument("Sample rates must be positive integers.");
  }

  resample_ratio = static_cast<double>(output_rate) / static_cast<double>(input_rate);

  int error;
  src_state = src_new(SRC_SINC_FASTEST, 1, &error);
  if (error != 0) {
    throw std::runtime_error("Failed to create SRC state: " + std::string(src_strerror(error)));
  }
}

Resampler::~Resampler() {
  if (src_state != nullptr) {
    src_delete(src_state);
  }
  std::cout << "Resampler destroyed" << std::endl; // GST
}