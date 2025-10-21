#ifndef GLOBALHEADER_HPP
#define GLOBALHEADER_HPP

#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <omp.h>
#include <optional>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <vector>

using int8 = int8_t;
using uint8 = uint8_t;
using int16 = int16_t;
using uint16 = uint16_t;
using int32 = int32_t;
using uint32 = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;

inline constexpr int8_t OUT_OF_BOUNDS = -1;
inline constexpr int8_t NOT_SQUARE = -2;
inline constexpr double EPSILON = 1e-6;

template <typename T, typename U>
[[nodiscard]] bool is_close(T v1, U v2, double epsilon = EPSILON) {
    return std::abs(v1 - v2) < epsilon;
}

#endif // GLOBALHEADER_HPP
