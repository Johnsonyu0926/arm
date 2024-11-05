// sherpa-onnx/csrc/vad-model-config.h
//
// Copyright (c)  2023  Xiaomi Corporation
#ifndef SHERPA_ONNX_CSRC_VAD_MODEL_CONFIG_H_
#define SHERPA_ONNX_CSRC_VAD_MODEL_CONFIG_H_

#include <string>

#include "sherpa-onnx/csrc/parse-options.h"
#include "sherpa-onnx/csrc/silero-vad-model-config.h"

namespace sherpa_onnx {

struct VadModelConfig {
  SileroVadModelConfig silero_vad;  // Silero VAD 模型配置

  int32_t sample_rate = 16000;  // 采样率，默认16000
  int32_t num_threads = 1;  // 线程数，默认1
  std::string provider = "cpu";  // 提供者，默认cpu

  bool debug = false;  // 是否显示加载模型时的调试信息，默认false

  // 默认构造函数
  VadModelConfig() = default;

  // 带参数的构造函数
  VadModelConfig(const SileroVadModelConfig &silero_vad, int32_t sample_rate,
                 int32_t num_threads, const std::string &provider, bool debug)
      : silero_vad(silero_vad),
        sample_rate(sample_rate),
        num_threads(num_threads),
        provider(provider),
        debug(debug) {}

  // 注册解析选项
  void Register(ParseOptions *po);

  // 验证配置是否有效
  bool Validate() const;

  // 将配置转换为字符串
  std::string ToString() const;
};

}  // namespace sherpa_onnx

#endif  // SHERPA_ONNX_CSRC_VAD_MODEL_CONFIG_H_

//By GST ARMV8 GCC13.2