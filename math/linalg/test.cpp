#include "Containers.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>

int main(void) {
    // Original test data
    int len = 12;
    double data[12] = {1.0, 2.0, 3.0, 4.0,  5.0,  6.0,
                       7.0, 8.0, 9.0, 10.0, 11.0, 12.0};
    std::vector<double> vtest(12);
    vtest.assign(data, data + len);
    std::array<double, 12> atest;
    std::copy(data, data + len, atest.begin());

    std::cout << "=== Original Constructor Tests ===" << std::endl;
    math::Matrix<double> m1(3, 4, atest);
    math::Matrix<double> m2(4, 3, vtest);
    math::Matrix<double> m3(3, 4, data);

    std::cout << "Matrix m1 (3x4 from array):" << std::endl;
    m1.print();
    std::cout << "Matrix m2 (4x3 from vector):" << std::endl;
    m2.print();
    std::cout << "Matrix m3 (3x4 from raw data):" << std::endl;
    m3.print();
    std::cout << std::endl;

    // Test assignment operator
    std::cout << "=== Assignment Operator Test ===" << std::endl;
    m2 = m1;
    std::cout << "m2 after assignment from m1:" << std::endl;
    m2.print();
    std::cout << std::endl;

    // Test default constructor and empty matrix
    std::cout << "=== Default Constructor Test ===" << std::endl;
    math::Matrix<double> empty_matrix;
    std::cout << "Empty matrix rows: " << empty_matrix.rowsSize()
              << ", cols: " << empty_matrix.colsSize() << std::endl;

    // Test size constructor with fill
    std::cout << "=== Size Constructor and Fill Test ===" << std::endl;
    math::Matrix<double> m4(2, 3);
    std::cout << "Matrix m4 (2x3 default initialized):" << std::endl;
    m4.print();

    m4.fill(5.5);
    std::cout << "Matrix m4 after fill(5.5):" << std::endl;
    m4.print();
    std::cout << std::endl;

    // Test copy constructor
    std::cout << "=== Copy Constructor Test ===" << std::endl;
    math::Matrix<double> m5(m1);
    std::cout << "Matrix m5 (copy of m1):" << std::endl;
    m5.print();
    std::cout << std::endl;

    // Test move constructor
    std::cout << "=== Move Constructor Test ===" << std::endl;
    math::Matrix<double> m6(std::move(m5));
    std::cout << "Matrix m6 (moved from m5):" << std::endl;
    m6.print();
    std::cout << "Matrix m5 after move (should be empty): rows="
              << m5.rowsSize() << ", cols=" << m5.colsSize() << std::endl;
    std::cout << std::endl;

    // Test element access
    std::cout << "=== Element Access Test ===" << std::endl;
    std::cout << "m1.at(1, 2) = " << m1.at(1, 2) << std::endl;
    m1.at(1, 2) = 99.9;
    std::cout << "After setting m1.at(1, 2) = 99.9:" << std::endl;
    m1.print();
    std::cout << std::endl;

    // Test equality operator
    std::cout << "=== Equality Test ===" << std::endl;
    math::Matrix<double> m7(m1);
    std::cout << "m1 == m7: " << (m1 == m7 ? "true" : "false") << std::endl;
    m7.at(0, 0) = 999.0;
    std::cout << "After modifying m7, m1 == m7: "
              << (m1 == m7 ? "true" : "false") << std::endl;
    std::cout << std::endl;

    // Test arithmetic operations
    std::cout << "=== Arithmetic Operations Test ===" << std::endl;

    // Create two compatible matrices for addition/subtraction
    double data1[6] = {1, 2, 3, 4, 5, 6};
    double data2[6] = {6, 5, 4, 3, 2, 1};
    math::Matrix<double> ma(2, 3, data1);
    math::Matrix<double> mb(2, 3, data2);

    std::cout << "Matrix ma:" << std::endl;
    ma.print();
    std::cout << "Matrix mb:" << std::endl;
    mb.print();

    // Matrix addition
    auto sum = ma + mb;
    std::cout << "ma + mb:" << std::endl;
    sum.print();

    // Matrix subtraction
    auto diff = ma - mb;
    std::cout << "ma - mb:" << std::endl;
    diff.print();

    // Scalar operations
    auto scalar_add = ma + 10.0;
    std::cout << "ma + 10.0:" << std::endl;
    scalar_add.print();

    auto scalar_mult = ma * 2.0;
    std::cout << "ma * 2.0:" << std::endl;
    scalar_mult.print();

    auto scalar_sub = ma - 1.0;
    std::cout << "ma - 1.0:" << std::endl;
    scalar_sub.print();

    // Friend function tests
    auto friend_add = 5.0 + ma;
    std::cout << "5.0 + ma:" << std::endl;
    friend_add.print();

    auto friend_sub = 5.0 - ma;
    std::cout << "5.0 - ma:" << std::endl;
    friend_sub.print();

    auto friend_mult = 3.0 * ma;
    std::cout << "3.0 * ma:" << std::endl;
    friend_mult.print();
    std::cout << std::endl;

    // Test matrix property checkers
    std::cout << "=== Matrix Property Tests ===" << std::endl;

    // Create a square matrix for property testing
    double square_data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    math::Matrix<double> square_matrix(3, 3, square_data);
    std::cout << "Square matrix:" << std::endl;
    square_matrix.print();
    std::cout << "Is square: " << (square_matrix.is_square() ? "true" : "false")
              << std::endl;
    std::cout << "Is symmetric: "
              << (square_matrix.is_symmetric() ? "true" : "false") << std::endl;

    // Create symmetric matrix
    double sym_data[9] = {1, 2, 3, 2, 4, 5, 3, 5, 6};
    math::Matrix<double> sym_matrix(3, 3, sym_data);
    std::cout << "\nSymmetric matrix:" << std::endl;
    sym_matrix.print();
    std::cout << "Is symmetric: "
              << (sym_matrix.is_symmetric() ? "true" : "false") << std::endl;

    // Create upper triangular matrix
    double upper_data[9] = {1, 2, 3, 0, 4, 5, 0, 0, 6};
    math::Matrix<double> upper_matrix(3, 3, upper_data);
    std::cout << "\nUpper triangular matrix:" << std::endl;
    upper_matrix.print();
    std::cout << "Is upper triangular: "
              << (upper_matrix.is_upper_triangular() ? "true" : "false")
              << std::endl;
    std::cout << "Is lower triangular: "
              << (upper_matrix.is_lower_triangular() ? "true" : "false")
              << std::endl;

    // Create lower triangular matrix
    double lower_data[9] = {1, 0, 0, 2, 4, 0, 3, 5, 6};
    math::Matrix<double> lower_matrix(3, 3, lower_data);
    std::cout << "\nLower triangular matrix:" << std::endl;
    lower_matrix.print();
    std::cout << "Is upper triangular: "
              << (lower_matrix.is_upper_triangular() ? "true" : "false")
              << std::endl;
    std::cout << "Is lower triangular: "
              << (lower_matrix.is_lower_triangular() ? "true" : "false")
              << std::endl;

    // Create diagonal matrix
    double diag_data[9] = {1, 0, 0, 0, 4, 0, 0, 0, 6};
    math::Matrix<double> diag_matrix(3, 3, diag_data);
    std::cout << "\nDiagonal matrix:" << std::endl;
    diag_matrix.print();
    std::cout << "Is diagonal: "
              << (diag_matrix.is_diagonal() ? "true" : "false") << std::endl;
    std::cout << std::endl;

    // Test transpose operations
    std::cout << "=== Transpose Tests ===" << std::endl;
    math::Matrix<double> transpose_test(square_matrix);
    std::cout << "Original matrix:" << std::endl;
    transpose_test.print();

    auto transposed_copy = transpose_test.transposed();
    std::cout << "Transposed copy of original:" << std::endl;
    transposed_copy.print();

    transpose_test.transpose();
    std::cout << "After in-place transpose:" << std::endl;
    transpose_test.print();
    std::cout << std::endl;

    // Test different data types
    std::cout << "=== Different Data Types Test ===" << std::endl;
    int int_data[6] = {1, 2, 3, 4, 5, 6};
    math::Matrix<int> int_matrix(2, 3, int_data);
    std::cout << "Integer matrix:" << std::endl;
    int_matrix.print();

    float float_data[4] = {1.1f, 2.2f, 3.3f, 4.4f};
    math::Matrix<float> float_matrix(2, 2, float_data);
    std::cout << "Float matrix:" << std::endl;
    float_matrix.print();
    std::cout << std::endl;

    // Test error conditions
    std::cout << "=== Error Handling Tests ===" << std::endl;

    try {
        math::Matrix<double> invalid(0, 5);
    } catch (const std::invalid_argument &e) {
        std::cout << "Caught expected error for zero rows: " << e.what()
                  << std::endl;
    }

    try {
        math::Matrix<double> invalid(5, 0);
    } catch (const std::invalid_argument &e) {
        std::cout << "Caught expected error for zero cols: " << e.what()
                  << std::endl;
    }

    try {
        square_matrix.at(10, 10);
    } catch (const std::out_of_range &e) {
        std::cout << "Caught expected error for out of bounds access: "
                  << e.what() << std::endl;
    }

    try {
        ma + square_matrix; // Different dimensions
    } catch (const std::invalid_argument &e) {
        std::cout << "Caught expected error for incompatible matrix addition: "
                  << e.what() << std::endl;
    }

    try {
        ma.is_symmetric(); // Non-square matrix
    } catch (const std::invalid_argument &e) {
        std::cout
            << "Caught expected error for symmetry check on non-square matrix: "
            << e.what() << std::endl;
    }

    try {
        ma.transpose(); // Non-square matrix
    } catch (const std::invalid_argument &e) {
        std::cout << "Caught expected error for in-place transpose on "
                     "non-square matrix: "
                  << e.what() << std::endl;
    }

    std::cout << "\n=== All tests completed successfully! ===" << std::endl;
    return 0;
}
