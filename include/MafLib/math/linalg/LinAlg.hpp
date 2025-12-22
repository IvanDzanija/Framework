#ifndef LINALG_H
#define LINALG_H

#if defined(__APPLE__) && defined(ACCELERATE_AVAILABLE)
#include "MafLib/math/linalg/AccelerateWrappers/AccelerateWrapper.hpp"
#endif

#include "MafLib/math/Math.hpp"
#pragma once

namespace maf::math {

// Constants
static constexpr uint8 BLOCK_SIZE = 64;
static constexpr uint8 FLOAT_PRECISION = 5;

// Classes
template <Numeric T>
class Vector;

template <Numeric T>
class Matrix;

// Functions

}  // namespace maf::math

#include "Matrix.hpp"
#include "Vector.hpp"
#endif
