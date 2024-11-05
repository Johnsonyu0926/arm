// Filename: pitch.hpp
// Score: 100



#pragma once

#include <STTypes.h>
#include <deque>
#include <span>
#include <string>
#include <vector>
#include "SoundTouch.h"
#include "pipe_manager.hpp"
#include "plugin_base.hpp"

class Pitch : public PluginBase {
 public:
  Pitch(const std::string& tag,
        const std::string& schema,
        const std::string& schema_path,
        PipeManager* pipe_manager,
        PipelineType pipe_type);
  Pitch(const Pitch&) = delete;
  auto operator=(const Pitch&) -> Pitch& = delete;
  Pitch(const Pitch&&) = delete;
  auto operator=(const Pitch&&) -> Pitch& = delete;
  ~Pitch() override;

  enum class Mode { speed, quality, consistency };
  enum class Formant { shifted, preserved };
  enum class Transients { crisp, mixed, smooth };
  enum class Detector { compound, percussive, soft };
  enum class Phase { laminar, independent };

  void setup() override;

  void process(std::span<float>& left_in,
               std::span<float>& right_in,
               std::span<float>& left_out,
               std::span<float>& right_out) override;

  auto get_latency_seconds() -> float override;

 private:
  bool soundtouch_ready = false;
  bool notify_latency = false;

  uint latency_n_frames = 0U;

  std::vector<float> data_L, data_R, data;

  std::deque<float> deque_out_L, deque_out_R;

  soundtouch::SoundTouch* snd_touch = nullptr;

  bool anti_alias = false;
  bool quick_seek = false;

  int sequence_length_ms = 40;
  int seek_window_ms = 15;
  int overlap_length_ms = 8;

  double semitones = 0.0;
  double tempo_difference = 0.0;
  double rate_difference = 0.0;

  void set_semitones();
  void set_sequence_length();
  void set_seek_window();
  void set_overlap_length();
  void set_quick_seek();
  void set_anti_alias();
  void set_tempo_difference();
  void set_rate_difference();
  void init_soundtouch();
};

// By GST @2024/10/27