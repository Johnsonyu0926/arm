// Filename: fir_filter_lowpass.hpp
#include "fir_filter_lowpass.hpp"
#include <string>
#include <utility>
#include "fir_filter_base.hpp"

// By GST @2024/10/26
FirFilterLowpass::FirFilterLowpass(std::string tag) : FirFilterBase(std::move(tag)) {}

FirFilterLowpass::~FirFilterLowpass() = default;

void FirFilterLowpass::setup() {
  kernel = create_lowpass_kernel(max_frequency, transition_band);
  delay = 0.5F * static_cast<float>(kernel.size() - 1U) / static_cast<float>(rate);
  setup_zita();
}