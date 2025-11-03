#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#pragma once
#include "MafLib/main/GlobalHeader.hpp"

namespace maf::math {

template <typename T> class Polynomial {
  private:
    size_t _degree;
    std::vector<T> _coeffs;

  public:
    // --- Constructors and destructors ---

    /**
     * @brief Constructs an uninitialized polynomial of given degree.
     * @param degree Degree of polynomial.
     * @throws std::invalid_argument if dimensions are zero.
     */
    Polynomial(size_t degree);

    /**
     * @brief Constructs from a raw data pointer.
     * @param degree Degree of polynomial.
     * @param coeffs Pointer to a C-style array of size (degree).
     * The data is COPIED into the polynomial.
     * @throws std::invalid_argument if degree is zero or data is
     * nullptr.
     */
    Polynomial(size_t degree, T *data);

    /**
     * @brief Constructs from a std::vector.
     * @param degree Degree of polynomial.
     * @param coeffs A std::vector of size (degree).
     * @throws std::invalid_argument if degree is zero or vector size
     * does not match.
     */
    Polynomial(size_t degree, const std::vector<T> &coeffs);

    /**
     * @brief Constructs from a std::array
     * @tparam U Type in the array (allows implicit conversion).
     * @tparam N Size of the array.
     * @param degree Degree of polynomial.
     * @param coeffs A std::array<U, N> where N must equal (degree).
     * @throws std::invalid_argument if degree is zero or array size
     * does not match.
     */
    template <typename U, size_t N>
    Polynomial(size_t degree, const std::array<U, N> &coeffs);

    /**
     * @brief Constructs from a std::initializer_list
     * @tparam U Type in the list (allows implicit conversion).
     * @param degree Degree of polynomial.
     * @param list A std::initializer_list<U> of size (degree).
     * @throws std::invalid_argument if degree is zero or list size
     * does not match.
     */
    template <typename U>
    Polynomial(size_t degree, std::initializer_list<U> coeffs);

    // --- Getters and setters ---
};

} // namespace maf::math

#endif
