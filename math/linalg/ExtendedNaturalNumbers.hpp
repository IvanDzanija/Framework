#ifndef EXTENDEDINTEGER
#define EXTENDEDINTEGER

#pragma once
#include <variant>

/**
 * @brief An extended integer class that supports positive and negative infinity
 *
 * This class can represent regular integers as well as positive and negative
 * infinity values. It provides arithmetic operations that handle infinity
 * according to mathematical rules.
 *
 * @version 1.0
 * @since 2025
 */
class ExtendedInt {
  public:
    enum class InfinityType { PosInf, NegInf };

    // Factory methods
    static ExtendedInt posInf() { return ExtendedInt(InfinityType::PosInf); }
    static ExtendedInt negInf() { return ExtendedInt(InfinityType::NegInf); }

    // Constructors

    ExtendedInt() : _value(0) {} // Default constructor initilizes value to 0
    ExtendedInt(int value) : _value(value) {}
    ExtendedInt(InfinityType value) : _value(value) {}

    bool isInf() const { return std::holds_alternative<InfinityType>(_value); }
  bool isPositiveInf

      private : std::variant<int, InfinityType>
                    _value;
};

#endif
