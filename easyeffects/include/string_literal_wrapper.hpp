// Filename: StringLiteralWrapper.hpp
// Score: 100

#pragma once

#include <array>
#include <cstddef>
#include <utility>

template <size_t N>
struct StringLiteralWrapper {
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    constexpr StringLiteralWrapper(const char (&str)[N]) : msg(std::to_array(str)) {}

    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    constexpr StringLiteralWrapper(std::array<char, N> arr) : msg(std::move(arr)) {}

    std::array<char, N> msg;
};

// By GST @2024/10/27