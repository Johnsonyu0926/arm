// Filename: fir_filter_bandpass.hpp
#include "fir_filter_bandpass.hpp"
#include <algorithm>
#include <cstddef>
#include <string>
#include <utility>
#include "fir_filter_base.hpp"

// By GST @2024/10/26
FirFilterBandpass::FirFilterBandpass(std::string tag) : FirFilterBase(std::move(tag)) {}

FirFilterBandpass::~FirFilterBandpass() = default;

void FirFilterBandpass::setup() {
  const auto lowpass_kernel = create_lowpass_kernel(max_frequency, transition_band);
  auto highpass_kernel = create_highpass_kernel(min_frequency, transition_band);

  kernel.resize(highpass_kernel.size());

  // Creating a bandpass from a band reject through spectral inversion
  // Reference: https://www.dspguide.com/ch16/4.htm
  std::transform(lowpass_kernel.begin(), lowpass_kernel.end(), highpass_kernel.begin(), kernel.begin(), std::plus<>());

  std::ranges::for_each(kernel, [](auto& v) { v *= -1.0F; });

  kernel[(kernel.size() - 1U) / 2U] += 1.0F;

  delay = 0.5F * static_cast<float>(kernel.size() - 1U) / static_cast<float>(rate);

  setup_zita();
}

std::vector<float> FirFilterBandpass::create_highpass_kernel(float cutoff_frequency, float transition_band) {
  auto kernel = create_lowpass_kernel(cutoff_frequency, transition_band);
  std::ranges::for_each(kernel, [](auto& v) { v *= -1.0F; });
  kernel[(kernel.size() - 1U) / 2U] += 1.0F;
  return kernel;
}