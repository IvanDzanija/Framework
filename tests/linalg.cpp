#include "../math/linalg/Containers.hpp"

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
    std::cout << std::endl;

    std::cout << "=== Matrix Multiplication Tests ===" << std::endl;

    // Test 1: Basic 2x3 * 3x2 multiplication
    double data_a[6] = {1, 2, 3, 4, 5, 6};    // 2x3 matrix
    double data_b[6] = {7, 8, 9, 10, 11, 12}; // 3x2 matrix
    math::Matrix<double> mult_a(2, 3, data_a);
    math::Matrix<double> mult_b(3, 2, data_b);

    std::cout << "Matrix A (2x3):" << std::endl;
    mult_a.print();
    std::cout << "Matrix B (3x2):" << std::endl;
    mult_b.print();

    auto result_ab = mult_a * mult_b;
    std::cout << "A * B (2x2):" << std::endl;
    result_ab.print();

    // Verify the result manually:
    // [1*7+2*9+3*11, 1*8+2*10+3*12] = [58, 64]
    // [4*7+5*9+6*11, 4*8+5*10+6*12] = [139, 154]
    assert(result_ab.at(0, 0) == 58);
    assert(result_ab.at(0, 1) == 64);
    assert(result_ab.at(1, 0) == 139);
    assert(result_ab.at(1, 1) == 154);
    std::cout << "✓ Basic multiplication test passed" << std::endl;

    // Test 2: Square matrix multiplication
    double square_a[4] = {1, 2, 3, 4}; // 2x2
    double square_b[4] = {5, 6, 7, 8}; // 2x2
    math::Matrix<double> sq_a(2, 2, square_a);
    math::Matrix<double> sq_b(2, 2, square_b);

    std::cout << "\nSquare Matrix A:" << std::endl;
    sq_a.print();
    std::cout << "Square Matrix B:" << std::endl;
    sq_b.print();

    auto sq_result = sq_a * sq_b;
    std::cout << "A * B:" << std::endl;
    sq_result.print();

    // Verify: [1*5+2*7, 1*6+2*8] = [19, 22]
    //         [3*5+4*7, 3*6+4*8] = [43, 50]
    assert(sq_result.at(0, 0) == 19);
    assert(sq_result.at(0, 1) == 22);
    assert(sq_result.at(1, 0) == 43);
    assert(sq_result.at(1, 1) == 50);
    std::cout << "✓ Square matrix multiplication test passed" << std::endl;

    // Test 3: Identity matrix multiplication
    std::cout << "\n--- Identity Matrix Tests ---" << std::endl;
    double test_data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9}; // 3x3 test matrix
    math::Matrix<double> identity(3, 3);
    identity.make_identity();
    math::Matrix<double> test_matrix(3, 3, test_data);

    std::cout << "Identity matrix:" << std::endl;
    identity.print();
    std::cout << "Test matrix:" << std::endl;
    test_matrix.print();

    auto id_result1 = identity * test_matrix;
    auto id_result2 = test_matrix * identity;

    std::cout << "Identity * Test:" << std::endl;
    id_result1.print();
    std::cout << "Test * Identity:" << std::endl;
    id_result2.print();

    // Both should equal the original test matrix
    assert(id_result1 == test_matrix);
    assert(id_result2 == test_matrix);
    std::cout << "✓ Identity matrix multiplication test passed" << std::endl;

    // Test 4: Matrix chain multiplication (associativity)
    std::cout << "\n--- Chain Multiplication (Associativity) Test ---"
              << std::endl;
    double chain_a[6] = {1, 2, 3, 4, 5, 6};                       // 2x3
    double chain_b[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; // 3x4
    double chain_c[8] = {1, 2, 3, 4, 5, 6, 7, 8};                 // 4x2

    math::Matrix<double> chain_ma(2, 3, chain_a);
    math::Matrix<double> chain_mb(3, 4, chain_b);
    math::Matrix<double> chain_mc(4, 2, chain_c);

    // Test (A * B) * C vs A * (B * C)
    auto ab = chain_ma * chain_mb;
    auto abc_left = ab * chain_mc;

    auto bc = chain_mb * chain_mc;
    auto abc_right = chain_ma * bc;

    std::cout << "(A * B) * C:" << std::endl;
    abc_left.print();
    std::cout << "A * (B * C):" << std::endl;
    abc_right.print();

    assert(abc_left == abc_right);
    std::cout << "✓ Associativity test passed" << std::endl;

    // Test 5: Single element matrices
    std::cout << "\n--- Single Element Matrix Test ---" << std::endl;
    double single_a[1] = {5};
    double single_b[1] = {3};
    math::Matrix<double> single_ma(1, 1, single_a);
    math::Matrix<double> single_mb(1, 1, single_b);

    auto single_result = single_ma * single_mb;
    std::cout << "5 * 3 = " << single_result.at(0, 0) << std::endl;
    assert(single_result.at(0, 0) == 15);
    std::cout << "✓ Single element multiplication test passed" << std::endl;

    // Test 6: Vector-like matrices
    std::cout << "\n--- Vector-like Matrix Tests ---" << std::endl;
    double row_vec[3] = {1, 2, 3}; // 1x3 row vector
    double col_vec[3] = {4, 5, 6}; // 3x1 column vector
    math::Matrix<double> row_vector(1, 3, row_vec);
    math::Matrix<double> col_vector(3, 1, col_vec);

    std::cout << "Row vector (1x3):" << std::endl;
    row_vector.print();
    std::cout << "Column vector (3x1):" << std::endl;
    col_vector.print();

    // Dot product (1x3) * (3x1) = (1x1)
    auto dot_product = row_vector * col_vector;
    std::cout << "Dot product (1x1):" << std::endl;
    dot_product.print();
    assert(dot_product.at(0, 0) == 32); // 1*4 + 2*5 + 3*6 = 32

    // Outer product (3x1) * (1x3) = (3x3)
    auto outer_product = col_vector * row_vector;
    std::cout << "Outer product (3x3):" << std::endl;
    outer_product.print();
    std::cout << "✓ Vector-like matrix tests passed" << std::endl;

    // Test 7: Zero matrix multiplication
    std::cout << "\n--- Zero Matrix Test ---" << std::endl;
    math::Matrix<double> zero_matrix(2, 3);
    zero_matrix.fill(0.0);

    auto zero_result = zero_matrix * mult_b;
    std::cout << "Zero matrix * B:" << std::endl;
    zero_result.print();

    // All elements should be zero
    for (int i = 0; i < zero_result.rowsSize(); ++i) {
        for (int j = 0; j < zero_result.colsSize(); ++j) {
            assert(zero_result.at(i, j) == 0.0);
        }
    }
    std::cout << "✓ Zero matrix multiplication test passed" << std::endl;

    // Test 8: Large matrix multiplication performance test
    std::cout << "\n--- Performance Test (100x100 matrices) ---" << std::endl;
    const int size = 100;
    std::vector<double> large_data_a(size * size);
    std::vector<double> large_data_b(size * size);

    // Fill with random data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 10.0);

    for (int i = 0; i < size * size; ++i) {
        large_data_a[i] = dis(gen);
        large_data_b[i] = dis(gen);
    }

    math::Matrix<double> large_a(size, size, large_data_a);
    math::Matrix<double> large_b(size, size, large_data_b);

    auto start = std::chrono::high_resolution_clock::now();
    auto large_result = large_a * large_b;
    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "100x100 matrix multiplication completed in "
              << duration.count() << " ms" << std::endl;
    std::cout << "✓ Performance test completed" << std::endl;

    // Test 9: Different data types
    std::cout << "\n--- Different Data Types Test ---" << std::endl;
    int int_a[4] = {1, 2, 3, 4};
    int int_b[4] = {5, 6, 7, 8};
    math::Matrix<int> int_ma(2, 2, int_a);
    math::Matrix<int> int_mb(2, 2, int_b);

    auto int_result = int_ma * int_mb;
    std::cout << "Integer matrix multiplication:" << std::endl;
    int_result.print();
    assert(int_result.at(0, 0) == 19);
    assert(int_result.at(1, 1) == 50);
    std::cout << "✓ Integer matrix multiplication test passed" << std::endl;

    // Test 10: Error handling for incompatible dimensions
    std::cout << "\n--- Error Handling Tests ---" << std::endl;
    try {
        math::Matrix<double> incompatible_a(2, 3);
        math::Matrix<double> incompatible_b(4, 2); // 3 != 4, should fail
        auto should_fail = incompatible_a * incompatible_b;
        assert(false); // Should not reach here
    } catch (const std::invalid_argument &e) {
        std::cout << "✓ Caught expected error for incompatible dimensions: "
                  << e.what() << std::endl;
    }

    // Test 11: Matrix multiplication properties
    std::cout << "\n--- Matrix Properties Tests ---" << std::endl;

    // Test that (A + B) * C = A*C + B*C (distributivity)
    math::Matrix<double> prop_a(2, 2, square_a);
    math::Matrix<double> prop_b(2, 2, square_b);
    math::Matrix<double> prop_c(2, 2);
    prop_c.make_identity();

    auto left_dist = (prop_a + prop_b) * prop_c;
    auto right_dist = (prop_a * prop_c) + (prop_b * prop_c);

    std::cout << "(A + B) * C:" << std::endl;
    left_dist.print();
    std::cout << "A*C + B*C:" << std::endl;
    right_dist.print();

    assert(left_dist == right_dist);
    std::cout << "✓ Distributivity test passed" << std::endl;

    // Matrix multiplication performance test
    const int n = 1024;
    math::Matrix<double> mat1(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            mat1.at(i, j) = i + j + random() % 10;
        }
    }

    math::Matrix<double> mat2(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            mat2.at(i, j) = i + j + random() % 10;
        }
    }
    start = std::chrono::high_resolution_clock::now();

    math::Matrix<double> mat3 = mat1 * mat2;
    end = std::chrono::high_resolution_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " ms" << std::endl;

    std::cout
        << "\n=== All matrix multiplication tests completed successfully! ==="
        << std::endl;
    return 0;
}
