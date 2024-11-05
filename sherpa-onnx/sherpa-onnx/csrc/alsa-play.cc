// File: source/router/sherpa-onnx/sherpa-onnx/csrc/alsa-play.cc

// Copyright (c)  2022-2023  Xiaomi Corporation

#ifdef SHERPA_ONNX_ENABLE_ALSA

#include "sherpa-onnx/csrc/alsa-play.h"

#include <algorithm>
#include <iostream>

namespace sherpa_onnx {

AlsaPlay::AlsaPlay(const char *device_name, int32_t sample_rate) {
  int32_t err = snd_pcm_open(&handle_, device_name, SND_PCM_STREAM_PLAYBACK, 0);

  if (err) {
    std::cerr << "Unable to open: " << device_name << ". " << snd_strerror(err) << std::endl;
    exit(EXIT_FAILURE);
  }

  SetParameters(sample_rate);
}

AlsaPlay::~AlsaPlay() {
  if (handle_) {
    int32_t err = snd_pcm_close(handle_);
    if (err < 0) {
      std::cerr << "Failed to close pcm: " << snd_strerror(err) << std::endl;
    }
  }
}

void AlsaPlay::SetParameters(int32_t sample_rate) {
  snd_pcm_hw_params_t *params;
  snd_pcm_hw_params_alloca(&params);
  snd_pcm_hw_params_any(handle_, params);

  int32_t err = snd_pcm_hw_params_set_access(handle_, params, SND_PCM_ACCESS_RW_INTERLEAVED);
  if (err < 0) {
    std::cerr << "SND_PCM_ACCESS_RW_INTERLEAVED is not supported: " << snd_strerror(err) << std::endl;
    exit(EXIT_FAILURE);
  }

  err = snd_pcm_hw_params_set_format(handle_, params, SND_PCM_FORMAT_S16_LE);
  if (err < 0) {
    std::cerr << "Can't set format to 16-bit: " << snd_strerror(err) << std::endl;
    exit(EXIT_FAILURE);
  }

  err = snd_pcm_hw_params_set_channels(handle_, params, 1);
  if (err < 0) {
    std::cerr << "Can't set channel number to 1: " << snd_strerror(err) << std::endl;
  }

  uint32_t rate = sample_rate;
  err = snd_pcm_hw_params_set_rate_near(handle_, params, &rate, 0);
  if (err < 0) {
    std::cerr << "Can't set rate to " << rate << ". " << snd_strerror(err) << std::endl;
  }

  err = snd_pcm_hw_params(handle_, params);
  if (err < 0) {
    std::cerr << "Can't set hardware parameters. " << snd_strerror(err) << std::endl;
    exit(EXIT_FAILURE);
  }

  uint32_t tmp;
  snd_pcm_hw_params_get_rate(params, &tmp, 0);
  int32_t actual_sample_rate = tmp;
  if (actual_sample_rate != sample_rate) {
    std::cerr << "Creating a resampler:\n"
              << "   in_sample_rate: " << sample_rate << "\n"
              << "   output_sample_rate: " << actual_sample_rate << std::endl;

    float min_freq = std::min(actual_sample_rate, sample_rate);
    float lowpass_cutoff = 0.99 * 0.5 * min_freq;

    int32_t lowpass_filter_width = 6;
    resampler_ = std::make_unique<LinearResample>(
        sample_rate, actual_sample_rate, lowpass_cutoff, lowpass_filter_width);
  }

  snd_pcm_uframes_t frames;
  snd_pcm_hw_params_get_period_size(params, &frames, 0);
  buf_.resize(frames);
}

void AlsaPlay::Play(const std::vector<float> &samples) {
  std::vector<float> tmp;
  const float *p = samples.data();
  int32_t num_samples = samples.size();
  if (resampler_) {
    resampler_->Resample(samples.data(), samples.size(), false, &tmp);
    p = tmp.data();
    num_samples = tmp.size();
  }

  int32_t frames = buf_.size();
  int32_t i = 0;
  for (; i + frames < num_samples; i += frames) {
    for (int32_t k = 0; k != frames; ++k) {
      buf_[k] = p[i + k] * 32767;
    }

    int32_t err = snd_pcm_writei(handle_, buf_.data(), frames);
    if (err == -EPIPE) {
      std::cerr << "XRUN." << std::endl;
      snd_pcm_prepare(handle_);
    } else if (err < 0) {
      std::cerr << "Can't write to PCM device: " << snd_strerror(err) << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  if (i < num_samples) {
    for (int32_t k = 0; k + i < num_samples; ++k) {
      buf_[k] = p[i + k] * 32767;
    }

    int32_t err = snd_pcm_writei(handle_, buf_.data(), num_samples - i);
    if (err == -EPIPE) {
      std::cerr << "XRUN." << std::endl;
      snd_pcm_prepare(handle_);
    } else if (err < 0) {
      std::cerr << "Can't write to PCM device: " << snd_strerror(err) << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

void AlsaPlay::Drain() {
  int32_t err = snd_pcm_drain(handle_);
  if (err < 0) {
    std::cerr << "Failed to drain pcm. " << snd_strerror(err) << std::endl;
  }
}

}  // namespace sherpa_onnx

#endif  // SHERPA_ONNX_ENABLE_ALSA

//By GST ARMV8 GCC13.2