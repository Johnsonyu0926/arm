// File: source/router/sherpa-onnx/sherpa-onnx/csrc/alsa-play.h

// Copyright (c)  2022-2023  Xiaomi Corporation

#ifndef SHERPA_ONNX_CSRC_ALSA_PLAY_H_
#define SHERPA_ONNX_CSRC_ALSA_PLAY_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "alsa/asoundlib.h"
#include "sherpa-onnx/csrc/resample.h"

namespace sherpa_onnx {

class AlsaPlay {
 public:
  AlsaPlay(const char *device_name, int32_t sample_rate);
  ~AlsaPlay();
  void Play(const std::vector<float> &samples);

  // Wait for all the samples to be played
  void Drain();

 private:
  void SetParameters(int32_t sample_rate);

  snd_pcm_t *handle_ = nullptr;
  std::unique_ptr<LinearResample> resampler_;
  std::vector<int16_t> buf_;
};

}  // namespace sherpa_onnx

#endif  // SHERPA_ONNX_CSRC_ALSA_PLAY_H_

//By GST ARMV8 GCC13.2