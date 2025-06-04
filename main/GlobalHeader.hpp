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
#include <vector>

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

constexpr int8 OUT_OF_BOUNDS = -1;
constexpr int8 NOT_SQUARE = -2;

#endif // GLOBALHEADER_HPP
