// Filename: fir_filter_highpass.hpp
#include "fir_filter_highpass.hpp"
#include <algorithm>
#include <string>
#include <utility>
#include "fir_filter_base.hpp"

// By GST @2024/10/26
FirFilterHighpass::FirFilterHighpass(std::string tag) : FirFilterBase(std::move(tag)) {}

FirFilterHighpass::~FirFilterHighpass() = default;

void FirFilterHighpass::setup() {
  kernel = create_highpass_kernel(min_frequency, transition_band);
  delay = 0.5F * static_cast<float>(kernel.size() - 1U) / static_cast<float>(rate);
  setup_zita();
}

std::vector<float> FirFilterHighpass::create_highpass_kernel(float cutoff_frequency, float transition_band) {
  auto kernel = create_lowpass_kernel(cutoff_frequency, transition_band);
  std::ranges::for_each(kernel, [](auto& v) { v *= -1.0F; });
  kernel[(kernel.size() - 1U) / 2U] += 1.0F;
  return kernel;
}