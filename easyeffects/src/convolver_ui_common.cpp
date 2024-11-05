// Filename: convolver_ui_common.cpp
// 优化日期: 2024/10/26

#include "convolver_ui_common.hpp"
#include <cstddef>
#include <filesystem>
#include <sndfile.hh>
#include <string>
#include <tuple>
#include <vector>
#include "util.hpp"

namespace ui::convolver {

auto read_kernel(const std::filesystem::path& irs_dir, const std::string& irs_ext, const std::string& file_name)
    -> std::tuple<int, std::vector<float>, std::vector<float>> {
  int rate = 0;
  std::vector<float> buffer;
  std::vector<float> kernel_L;
  std::vector<float> kernel_R;

  auto file_path = irs_dir / std::filesystem::path{file_name};

  util::debug("reading the impulse file: " + file_path.string());

  if (file_path.extension() != irs_ext) {
    file_path += irs_ext;
  }

  if (!std::filesystem::exists(file_path)) {
    util::debug("file: " + file_path.string() + " does not exist");
    return std::make_tuple(rate, kernel_L, kernel_R);
  }

  auto sndfile = SndfileHandle(file_path.string());

  if (sndfile.channels() != 2 || sndfile.frames() == 0) {
    util::warning("Only stereo impulse responses are supported.");
    util::warning("The impulse file was not loaded!");
    return std::make_tuple(rate, kernel_L, kernel_R);
  }

  buffer.resize(sndfile.frames() * sndfile.channels());
  kernel_L.resize(sndfile.frames());
  kernel_R.resize(sndfile.frames());

  sndfile.readf(buffer.data(), sndfile.frames());

  for (size_t n = 0U; n < kernel_L.size(); n++) {
    kernel_L[n] = buffer[2U * n];
    kernel_R[n] = buffer[2U * n + 1U];
  }

  rate = sndfile.samplerate();

  return std::make_tuple(rate, kernel_L, kernel_R);
}

}  // namespace ui::convolver

// By GST @2024/10/26