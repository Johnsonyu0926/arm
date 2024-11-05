// Filename: fir_filter_base.hpp
#include "fir_filter_base.hpp"
#include <sched.h>
#include <sys/types.h>
#include <zita-convolver.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <numbers>
#include <string>
#include <utility>
#include <vector>
#include "util.hpp"

namespace {

constexpr auto CONVPROC_SCHEDULER_PRIORITY = 0;
constexpr auto CONVPROC_SCHEDULER_CLASS = SCHED_FIFO;

}  // namespace

FirFilterBase::FirFilterBase(std::string tag) : log_tag(std::move(tag)) {}

FirFilterBase::~FirFilterBase() {
  zita_ready = false;

  if (conv != nullptr) {
    conv->stop_process();
    conv->cleanup();
    delete conv;
  }
}

void FirFilterBase::set_rate(const uint& value) {
  rate = value;
}

void FirFilterBase::set_n_samples(const uint& value) {
  n_samples = value;
}

void FirFilterBase::set_min_frequency(const float& value) {
  min_frequency = value;
}

void FirFilterBase::set_max_frequency(const float& value) {
  max_frequency = value;
}

void FirFilterBase::set_transition_band(const float& value) {
  transition_band = value;
}

void FirFilterBase::setup() {}

auto FirFilterBase::create_lowpass_kernel(const float& cutoff, const float& transition_band) const
    -> std::vector<float> {
  std::vector<float> output;

  if (rate == 0U) {
    return output;
  }

  const float b = transition_band / static_cast<float>(rate);
  auto M = static_cast<uint>(std::ceil(4.0F / b));
  M = (M % 2U == 0U) ? M : M + 1U;  // Ensure M is even

  output.resize(M + 1U);
  const float fc = cutoff / static_cast<float>(rate);
  float sum = 0.0F;

  for (size_t n = 0U; n < output.size(); n++) {
    if (n == M / 2U) {
      output[n] = 2.0F * std::numbers::pi_v<float> * fc;
    } else {
      output[n] = std::sin(2.0F * std::numbers::pi_v<float> * fc * static_cast<float>(n - static_cast<uint>(M / 2U))) /
                  static_cast<float>(n - static_cast<uint>(M / 2U));
    }

    const float w = 0.42F -
                    0.5F * std::cos(2.0F * std::numbers::pi_v<float> * static_cast<float>(n) / static_cast<float>(M)) +
                    0.08F * std::cos(4.0F * std::numbers::pi_v<float> * static_cast<float>(n) / static_cast<float>(M));

    output[n] *= w;
    sum += output[n];
  }

  std::ranges::for_each(output, [&](auto& v) { v /= sum; });

  return output;
}

void FirFilterBase::setup_zita() {
  zita_ready = false;

  if (n_samples == 0U || kernel.empty()) {
    return;
  }

  if (conv != nullptr) {
    conv->stop_process();
    conv->cleanup();
    delete conv;
  }

  conv = new Convproc();
  conv->set_options(0);

  int ret = conv->configure(2, 2, kernel.size(), n_samples, n_samples, n_samples, 0.0F);

  if (ret != 0) {
    util::warning(log_tag + "can't initialise zita-convolver engine: " + util::to_string(ret, ""));
    return;
  }

  ret = conv->impdata_create(0, 0, 1, kernel.data(), 0, static_cast<int>(kernel.size()));
  if (ret != 0) {
    util::warning(log_tag + "left impdata_create failed: " + util::to_string(ret, ""));
    return;
  }

  ret = conv->impdata_create(1, 1, 1, kernel.data(), 0, static_cast<int>(kernel.size()));
  if (ret != 0) {
    util::warning(log_tag + "right impdata_create failed: " + util::to_string(ret, ""));
    return;
  }

  ret = conv->start_process(CONVPROC_SCHEDULER_PRIORITY, CONVPROC_SCHEDULER_CLASS);
  if (ret != 0) {
    util::warning(log_tag + "start_process failed: " + util::to_string(ret, ""));
    conv->stop_process();
    conv->cleanup();
    return;
  }

  zita_ready = true;
}

void FirFilterBase::direct_conv(const std::vector<float>& a, const std::vector<float>& b, std::vector<float>& c) {
  const uint M = (c.size() + 1U) / 2U;

  for (uint n = 0U; n < c.size(); n++) {
    c[n] = 0.0F;

    for (uint m = 0U; m < M; m++) {
      if (n > m && n - m < M) {
        c[n] += a[n - m] * b[m];
      }
    }
  }
}

auto FirFilterBase::get_delay() const -> float {
  return delay;
}