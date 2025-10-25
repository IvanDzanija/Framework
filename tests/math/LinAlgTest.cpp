#include "MafLib/main/GlobalHeader.hpp"
#include "MafLib/math/linalg/Matrix.hpp"
#include "MafLib/math/linalg/MatrixMethods.hpp"
#include "MafLib/math/linalg/PLU.hpp"
#include "MafLib/math/linalg/Vector.hpp"

using namespace maf;

void should_construct_empty_matrix_with_zero_rows_and_columns() {
    Matrix<int> m;
    assert(m.row_count() == 0);
    assert(m.column_count() == 0);
    assert(m.size() == 0);
}

void should_construct_empty_matrix_of_given_size() {
    Matrix<int> m(2, 2);
    assert(m.size() == 4);
}

void should_throw_if_constructed_with_zero_dimensions() {
    bool thrown = false;
    try {
        Matrix<double> m(0, 3);
    } catch (const std::invalid_argument &e) {
        thrown = true;
    }
    assert(thrown);
}

void should_construct_from_raw_data() {
    int data[4] = {1, 2, 3, 4};
    Matrix<int> m(2, 2, data);
    assert(m.row_count() == 2);
    assert(m.column_count() == 2);
    assert(m.at(0, 0) == 1);
    assert(m.at(0, 1) == 2);
    assert(m.at(1, 0) == 3);
    assert(m.at(1, 1) == 4);
}

void should_throw_if_raw_data_is_null() {
    bool thrown = false;
    try {
        Matrix<int> m(2, 2, nullptr);
    } catch (const std::invalid_argument &e) {
        thrown = true;
    }
    assert(thrown);
}

void should_construct_from_std_vector() {
    std::vector<int> data = {1, 2, 3, 4, 5, 6};
    Matrix<int> m(2, 3, data);
    assert(m.at(0, 0) == 1);
    assert(m.at(0, 1) == 2);
    assert(m.at(0, 2) == 3);
    assert(m.at(1, 0) == 4);
    assert(m.at(1, 1) == 5);
    assert(m.at(1, 2) == 6);
}

void should_throw_if_vector_size_mismatch() {
    std::vector<int> data = {1, 2, 3};
    bool thrown = false;
    try {
        Matrix<int> m(2, 2, data);
    } catch (const std::invalid_argument &e) {
        thrown = true;
    }
    assert(thrown);
}

void should_construct_from_nested_vector() {
    std::vector<std::vector<int>> data = {{1, 2}, {3, 4}};
    Matrix<int> m(2, 2, data);
    assert(m.at(0, 0) == 1);
    assert(m.at(0, 1) == 2);
    assert(m.at(1, 0) == 3);
    assert(m.at(1, 1) == 4);
}

void should_throw_if_nested_vector_dimensions_mismatch() {
    std::vector<std::vector<int>> data = {{1, 2, 3}, {4, 5, 6}};
    bool thrown = false;
    try {
        Matrix<int> m(2, 2, data);
    } catch (const std::invalid_argument &e) {
        thrown = true;
    }
    assert(thrown);
}

void should_construct_from_std_array() {
    std::array<float, 6> data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Matrix<float> m(2, 3, data);
    assert(is_close(m.at(1, 2) - 6.0, 0));
}

void should_construct_from_initializer_list() {
    Matrix<int> m(2, 2, {1, 2, 3, 4});
    assert(m.at(0, 0) == 1);
    assert(m.at(0, 1) == 2);
    assert(m.at(1, 0) == 3);
    assert(m.at(1, 1) == 4);
}

void should_throw_if_initializer_list_size_mismatch() {
    bool thrown = false;
    try {
        Matrix<int> m(2, 2, {1, 2, 3});
    } catch (const std::invalid_argument &e) {
        thrown = true;
    }
    assert(thrown);
}

// Checkers tests

void should_return_true_for_square_matrix() {
    std::array<float, 9> data1 = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9};
    std::array<float, 6> data2 = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Matrix<float> m1(3, 3, data1);
    Matrix<float> m2(2, 3, data2);
    assert(m1.is_square());
    assert(!m2.is_square());
}

void should_return_true_for_symmetric_matrix() {
    std::array<float, 9> data1 = {1.1, 1.2, 1.3, 1.2, 2.2, 2.3, 1.3, 2.3, 3.3};
    std::array<float, 6> data2 = {1.0, 2.0, 3.0, 2.0, 4.0, 5.0};
    std::array<float, 9> data3 = {1.1, 0.0, 1.3, 1.2, 2.2, 2.3, 1.3, 2.3, 3.3};
    Matrix<float> m1(3, 3, data1);
    Matrix<float> m2(2, 3, data2);
    Matrix<float> m3(3, 3, data3);
    assert(m1.is_symmetric());
    assert(!m2.is_symmetric());
    assert(!m3.is_symmetric());
}

void should_return_true_for_triangular_matrix() {
    std::array<float, 9> data1 = {1.1, 1.2, 1.3, 0.0, 2.2, 2.3, 0.0, 1e-9, 3.3};
    std::array<float, 9> data2 = {1.1, 0.0, 0.0, 1.5, 2.2, 0.0, 0.0, 1e-9, 3.3};
    std::array<float, 9> data3 = {1.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3.3};
    Matrix<float> m1(3, 3, data1);
    Matrix<float> m2(3, 3, data2);
    Matrix<float> m3(3, 3, data3);
    Matrix<float> m4 = identity_matrix<float>(3);
    assert(m1.is_upper_triangular());
    assert(m2.is_lower_triangular());
    assert(m3.is_lower_triangular() && m3.is_upper_triangular());
    assert(m4.is_lower_triangular() && m4.is_upper_triangular());
}

void should_return_true_for_diagonal_matrix() {
    std::array<float, 9> data1 = {1.1, 0, 0, 0, 2.2, 0, 0.0, 1e-9, 3.3};
    std::array<float, 9> data2 = {1.1, 0.0, 0.0, 1, 2.2, 0.0, 0.0, 0, 3.3};
    std::array<float, 9> data3 = {1.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3.3};
    Matrix<float> m1(3, 3, data1);
    Matrix<float> m2(3, 3, data2);
    Matrix<float> m3(3, 3, data3);
    Matrix<float> m4 = identity_matrix<float>(3);
    assert(m1.is_diagonal());
    assert(!m2.is_diagonal());
    assert(m3.is_diagonal());
    assert(m4.is_diagonal());
}

void should_return_true_for_positive_definite_matrix() {
    Matrix<int> m1(3, 3, {1, 2, 1, 2, 5, 2, 1, 2, 10});
    Matrix<int> m2(3, 3, {1, 2, 1, 2, -5, 2, 1, 2, 10});
    assert(m1.is_positive_definite());
    assert(!m2.is_positive_definite());
}

// Methods tests

void should_fill_matrix_with_value() {
    Matrix<int> m(2, 3);
    m.fill(9);
    for (size_t i = 0; i < m.row_count(); ++i)
        for (size_t j = 0; j < m.column_count(); ++j)
            assert(m.at(i, j) == 9);
}

void should_make_identity_matrix() {
    Matrix<int> m1(3, 3);
    m1.make_identity();
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            if (i == j)
                assert(m1.at(i, j) == 1);
            else
                assert(m1.at(i, j) == 0);
        }
    }
}

void should_transpose_square_matrix_in_place() {
    Matrix<int> m(2, 2, {1, 2, 3, 4});
    m.transpose();
    assert(m.at(0, 1) == 3);
    assert(m.at(1, 0) == 2);
}

void should_return_transposed_copy_for_non_square_matrix() {
    Matrix<int> m(2, 3, {1, 2, 3, 4, 5, 6});
    Matrix<int> t = m.transposed();
    assert(m.row_count() == 2);
    assert(m.column_count() == 3);
    assert(t.row_count() == 3);
    assert(t.column_count() == 2);
    assert(t.at(1, 0) == 2);
    assert(t.at(0, 1) == 4);
}

// Operators tests

void should_correctly_perform_unary_minus() {
    Matrix<int> m1(2, 3, {1, 2, 3, 4, 5, 6});
    Matrix<int> m2(2, 3, {-1, -2, -3, -4, -5, -6});
    assert(-m1 == m2);
}

void should_check_equality_between_identical_matrices() {
    Matrix<int> a(2, 2, {1, 2, 3, 4});
    Matrix<int> b(2, 2, {1, 2, 3, 4});
    assert(a == b);
}

void should_not_be_equal_if_any_element_differs() {
    Matrix<int> a(2, 2, {1, 2, 3, 4});
    Matrix<int> b(2, 2, {1, 9, 3, 4});
    assert(!(a == b));
}

void should_add_two_matrices_of_same_size() {
    Matrix<int> a(2, 2, {1, 2, 3, 4});
    Matrix<int> b(2, 2, {10, 20, 30, 40});
    Matrix<float> c(2, 2, {1.5, 2.5, 3.5, 4.5});
    auto d = a + b;
    auto e = d + c;
    assert(d.at(0, 0) == 11);
    assert(d.at(1, 1) == 44);
    assert(e.at(0, 0) == 12.5);
    assert(e.at(0, 1) == 24.5);
    assert(e.at(1, 0) == 36.5);
    assert(e.at(1, 1) == 48.5);
}

void should_add_scalar_and_matrix() {
    Matrix<int> a(2, 2, {1, 2, 3, 4});
    auto c = a + 10;
    auto d = a + 4.5;
    auto e = 4.5 + a;
    assert(c.at(0, 0) == 11);
    assert(c.at(0, 1) == 12);
    assert(c.at(1, 0) == 13);
    assert(c.at(1, 1) == 14);
    assert(d.at(0, 0) == 5.5);
    assert(d.at(0, 1) == 6.5);
    assert(d.at(1, 0) == 7.5);
    assert(d.at(1, 1) == 8.5);
    assert(e == d);
}

void should_subtract_two_matrices_of_same_size() {
    Matrix<int> a(2, 2, {1, 2, 3, 4});
    Matrix<int> b(2, 2, {10, 20, 30, 40});
    Matrix<float> c(2, 2, {1.5, 2.5, 3.5, 4.5});
    auto d = b - a;
    auto e = b - c;
    assert(d.at(0, 0) == 9);
    assert(d.at(1, 1) == 36);
    assert(e.at(0, 0) == 8.5);
    assert(e.at(0, 1) == 17.5);
    assert(e.at(1, 0) == 26.5);
    assert(e.at(1, 1) == 35.5);
}

void should_subtract_scalar_and_matrix() {
    Matrix<int> a(2, 2, {1, 2, 3, 4});
    auto c = a - 10;
    auto d = a - 4.5;
    auto e = 4.5 - a;
    assert(c.at(0, 0) == -9);
    assert(c.at(0, 1) == -8);
    assert(c.at(1, 0) == -7);
    assert(c.at(1, 1) == -6);
    assert(d.at(0, 0) == -3.5);
    assert(d.at(0, 1) == -2.5);
    assert(d.at(1, 0) == -1.5);
    assert(d.at(1, 1) == -0.5);
    assert(e.at(0, 0) == 3.5);
    assert(e.at(1, 1) == 0.5);
}

void should_multiply_matrix_and_scalar() {
    Matrix<double> a(2, 2, {1.5, 2.0, -3.0, 4.0});
    auto b = a * 2.0;
    auto c = 2 * a;
    assert(fabs(b.at(0, 0) - 3.0) < 1e-6);
    assert(fabs(b.at(0, 1) - 4.0) < 1e-6);
    assert(fabs(b.at(1, 0) + 6.0) < 1e-6);
    assert(fabs(b.at(1, 1) - 8.0) < 1e-6);
    assert(b == c);
}

void should_multiply_matrices() {
    Matrix<int> a(2, 3, {1, 2, 3, 4, 5, 6});
    Matrix<double> b(3, 2, {0.5, 1.5, -1.0, 2.0, 0.0, 1.0});

    Matrix<double> expected(
        2, 2,
        {1 * 0.5 + 2 * (-1.0) + 3 * 0.0, 1 * 1.5 + 2 * 2.0 + 3 * 1.0,
         4 * 0.5 + 5 * (-1.0) + 6 * 0.0, 4 * 1.5 + 5 * 2.0 + 6 * 1.0});

    auto result = a * b;
    assert(result.row_count() == 2 && result.column_count() == 2);
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            assert(is_close(result.at(i, j), expected.at(i, j)));
        }
    }
}

void should_multiply_matrix_and_vector() {
    Matrix<float> m(2, 3, {1.0, 0.5, -2.0, 4.0, 1.0, 3.0});
    Vector<int> v(3, std::vector<int>{2, 4, 6}, maf::Vector<int>::COLUMN);

    Vector<float> expected(2,
                           std::vector<float>{1.0 * 2 + 0.5 * 4 + (-2.0) * 6,
                                              4.0 * 2 + 1.0 * 4 + 3.0 * 6},
                           maf::Vector<float>::COLUMN);

    auto result = m * v;
    assert(result.size() == 2);
    for (size_t i = 0; i < 2; ++i) {
        assert(is_close(result.at(i), expected.at(i)));
    }
}

void should_throw_if_plu_called_on_non_square_matrix() {
    Matrix<double> m(2, 3, {1, 2, 3, 4, 5, 6});
    bool thrown = false;
    try {
        auto [P, L, U] = plu(m);
    } catch (const std::invalid_argument &e) {
        thrown = true;
    }
    assert(thrown);
}

void should_decompose_singular_matrix() {
    Matrix<double> m(3, 3, {1, 2, 3, 2, 4, 6, 1, 2, 3});
    auto [P, L, U] = maf::plu(m);
    bool thrown = false;
    try {
        auto [P, L, U] = plu(m);
    } catch (const std::invalid_argument &e) {
        thrown = true;
    }
    assert(!thrown);
}

void should_correctly_perform_plu_decomposition_on_small_matrix() {
    Matrix<double> A(3, 3, {2, 1, 1, 4, -6, 0, -2, 7, 2});

    auto [p, L, U] = plu(A);

    assert(L.is_square() && U.is_square());
    assert(L.row_count() == 3 && U.row_count() == 3);
    assert(p.size() == 3);

    for (size_t i = 0; i < 3; ++i) {
        assert(is_close(L.at(i, i), 1.0));
        for (size_t j = i + 1; j < 3; ++j) {
            assert(is_close(L.at(i, j), 0.0));
        }
    }

    for (size_t i = 1; i < 3; ++i) {
        for (size_t j = 0; j < i; ++j) {
            assert(is_close(U.at(i, j), 0.0));
        }
    }
    Matrix P_ = identity_matrix<double>(3);
    Matrix<double> P(3, 3);
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            P.at(i, j) = P_.at(p.at(i), j);
        }
    }
    Matrix<double> PA = P * A;
    Matrix<double> LU = L * U;
    assert(loosely_equal(PA, LU));
}

void should_correctly_handle_identity_matrix_in_plu() {
    Matrix<double> I = identity_matrix<double>(3);
    auto [P, L, U] = plu(I);
    assert(L == I);
    assert(U == I);
    for (size_t i = 0; i < 3; ++i) {
        assert(P.at(i) == i);
    }
}

void should_correctly_decompose_upper_triangular_matrix() {
    Matrix<double> U_true(3, 3, {1, 2, 3, 0, 4, 5, 0, 0, 6});
    auto [P, L, U] = plu(U_true);
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            assert(is_close(L.at(i, j), (i == j ? 1.0 : 0.0)));
        }
    }

    Matrix<double> Pm(3, 3);

    for (size_t i = 0; i < 3; ++i) {
        Pm.at(i, P.at(i)) = 1.0;
    }
    Matrix<double> PA = Pm * U_true;
    Matrix<double> LU = L * U;
    assert(loosely_equal(PA, LU));
}

void should_correctly_handle_negative_pivots_in_plu() {
    Matrix<double> A(2, 2, {-4, -2, -2, -1});
    auto [P, L, U] = plu(A);
    Matrix<double> Pm(2, 2);

    for (size_t i = 0; i < 2; ++i) {
        Pm.at(i, P[i]) = 1.0;
    }
    Matrix<double> PA = Pm * A;
    Matrix<double> LU = L * U;
    assert(loosely_equal(PA, LU));
}

void time_test() {
    const size_t N = 4000; // Size of the matrix (N x N)
    Matrix<double> A(N, N);

    // Fill the matrix with random values for a realistic test
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0);
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            A.at(i, j) = dis(gen);
        }
    }

    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    // Perform PLU decomposition
    auto [P, L, U] = plu(A);

    // Stop timer
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "PLU decomposition for " << N << "x" << N << " matrix took "
              << elapsed.count() << " seconds.\n";
    std::cout << U.at(500, 500) << std::endl;
}

int main() {
    std::cout << "=== Running Matrix tests ===" << std::endl;

    should_construct_empty_matrix_with_zero_rows_and_columns();
    should_construct_empty_matrix_of_given_size();
    should_throw_if_constructed_with_zero_dimensions();
    should_construct_from_raw_data();
    should_throw_if_raw_data_is_null();
    should_construct_from_std_vector();
    should_throw_if_vector_size_mismatch();
    should_construct_from_nested_vector();
    should_throw_if_nested_vector_dimensions_mismatch();
    should_construct_from_std_array();
    should_construct_from_initializer_list();
    should_throw_if_initializer_list_size_mismatch();

    // Checkers tests
    should_return_true_for_square_matrix();
    should_return_true_for_symmetric_matrix();
    should_return_true_for_triangular_matrix();
    should_return_true_for_diagonal_matrix();
    should_return_true_for_positive_definite_matrix();

    // Methods tests
    should_fill_matrix_with_value();
    should_make_identity_matrix();
    should_transpose_square_matrix_in_place();
    should_return_transposed_copy_for_non_square_matrix();

    // Operators tests
    should_correctly_perform_unary_minus();
    should_check_equality_between_identical_matrices();
    should_not_be_equal_if_any_element_differs();
    should_add_two_matrices_of_same_size();
    should_add_scalar_and_matrix();
    should_subtract_two_matrices_of_same_size();
    should_subtract_scalar_and_matrix();
    should_multiply_matrix_and_scalar();
    should_multiply_matrices();
    should_multiply_matrix_and_vector();

    std::cout << "=== Running PLU decomposition tests ===" << std::endl;
    should_throw_if_plu_called_on_non_square_matrix();
    should_decompose_singular_matrix();
    should_correctly_perform_plu_decomposition_on_small_matrix();
    should_correctly_handle_identity_matrix_in_plu();
    should_correctly_decompose_upper_triangular_matrix();
    should_correctly_handle_negative_pivots_in_plu();
    std::cout << "=== All PLU tests passed ===" << std::endl;
    time_test();

    std::cout << "=== All Matrix tests passed ===" << std::endl;
    return 0;
}
