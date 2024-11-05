// Filename: ConvolverUtils.hpp
// Score: 95

#pragma once

#include <filesystem>
#include <string>
#include <tuple>
#include <vector>

namespace ui::convolver {

auto read_kernel(std::filesystem::path irs_dir, const std::string& irs_ext, const std::string& file_name)
    -> std::tuple<int, std::vector<float>, std::vector<float>>;

}  // namespace ui::convolver
// By GST @Date