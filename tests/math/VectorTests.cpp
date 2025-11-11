#include "ITest.hpp"
#include "MafLib/main/GlobalHeader.hpp"
#include "MafLib/math/linalg/Matrix.hpp"
#include "MafLib/math/linalg/Vector.hpp"

using namespace maf;
class VectorTests : public ITest {
  private:
    //=============================================================================
    // VECTOR CONSTRUCTORS TESTS
    //=============================================================================
    void should_construct_empty_vector_with_zero_size() {
        math::Vector<int> v;
        ASSERT_TRUE(v.size() == 0);
        ASSERT_TRUE(v.orientation() == math::Vector<int>::COLUMN);
    }

    void should_construct_vector_of_given_size() {
        math::Vector<int> v_col(5);
        ASSERT_TRUE(v_col.size() == 5);
        ASSERT_TRUE(v_col.orientation() == math::Vector<int>::COLUMN);

        math::Vector<double> v_row(3, math::Vector<double>::ROW);
        ASSERT_TRUE(v_row.size() == 3);
        ASSERT_TRUE(v_row.orientation() == math::Vector<double>::ROW);
    }

    void should_throw_if_constructed_with_zero_size() {
        bool thrown = false;
        try {
            math::Vector<double> v(0);
        } catch (const std::invalid_argument &e) {
            thrown = true;
        }
        ASSERT_TRUE(thrown);
    }

    void should_construct_from_raw_data() {
        int data[3] = {10, 20, 30};
        math::Vector<int> v(3, data, math::Vector<int>::ROW);
        ASSERT_TRUE(v.size() == 3);
        ASSERT_TRUE(v.orientation() == math::Vector<int>::ROW);
        ASSERT_TRUE(v[0] == 10);
        ASSERT_TRUE(v[1] == 20);
        ASSERT_TRUE(v[2] == 30);
    }

    void should_throw_if_raw_data_is_null() {
        bool thrown = false;
        try {
            math::Vector<int> v(3, nullptr);
        } catch (const std::invalid_argument &e) {
            thrown = true;
        }
        ASSERT_TRUE(thrown);
    }

    void should_throw_if_raw_data_constructor_has_zero_size() {
        bool thrown = false;
        int data[3] = {1, 2, 3};
        try {
            math::Vector<int> v(0, data);
        } catch (const std::invalid_argument &e) {
            thrown = true;
        }
        ASSERT_TRUE(thrown);
    }

    void should_construct_from_std_vector_copy() {
        std::vector<int> data = {5, 10, 15};
        math::Vector<int> v(3, data);
        ASSERT_TRUE(v.size() == 3);
        ASSERT_TRUE(v[1] == 10);
        // Modify original to ensure it was a copy
        data[1] = 99;
        ASSERT_TRUE(v[1] == 10);
    }

    void should_throw_if_std_vector_copy_size_mismatch() {
        std::vector<int> data = {1, 2};
        bool thrown = false;
        try {
            math::Vector<int> v(3, data);
        } catch (const std::invalid_argument &e) {
            thrown = true;
        }
        ASSERT_TRUE(thrown);
    }

    void should_construct_from_std_vector_move() {
        std::vector<int> data = {5, 10, 15};
        math::Vector<int> v(3, std::move(data));
        ASSERT_TRUE(v.size() == 3);
        ASSERT_TRUE(v[1] == 10);
        // data is now in a valid but unspecified state
    }

    void should_throw_if_std_vector_move_size_mismatch() {
        std::vector<int> data = {1, 2};
        bool thrown = false;
        try {
            math::Vector<int> v(3, std::move(data));
        } catch (const std::invalid_argument &e) {
            thrown = true;
        }
        ASSERT_TRUE(thrown);
    }

    void should_construct_from_std_array() {
        std::array<float, 3> data = {1.1f, 2.2f, 3.3f};
        math::Vector<float> v(3, data, math::Vector<float>::ROW);
        ASSERT_TRUE(v.size() == 3);
        ASSERT_TRUE(v.orientation() == math::Vector<float>::ROW);
        ASSERT_TRUE(is_close(v[1], 2.2f));
    }

    void should_throw_if_std_array_size_mismatch() {
        std::array<int, 2> data = {1, 2};
        bool thrown = false;
        try {
            // Size template N=2, but size_t size=3
            math::Vector<int> v(3, data);
        } catch (const std::invalid_argument &e) {
            thrown = true;
        }
        ASSERT_TRUE(thrown);
    }

  public:
    int run_all_tests() override {
        std::cout << "=== Running Vector tests ===" << std::endl;

        std::cout << "=== Running constructors tests ===" << std::endl;
        should_construct_empty_vector_with_zero_size();
        should_construct_vector_of_given_size();
        should_throw_if_constructed_with_zero_size();
        should_construct_from_raw_data();
        should_throw_if_raw_data_is_null();
        should_throw_if_raw_data_constructor_has_zero_size();
        should_construct_from_std_vector_copy();
        should_throw_if_std_vector_copy_size_mismatch();
        should_construct_from_std_vector_move();
        should_throw_if_std_vector_move_size_mismatch();
        should_construct_from_std_array();
        should_throw_if_std_array_size_mismatch();
        std::cout << "=== All constructors tests passed ===" << std::endl;

        // Accessors and Iterators
        //    should_access_elements_with_at_and_operator();
        //    should_throw_on_out_of_bounds_access();
        //    should_iterate_over_elements();
        //    std::cout << "--- Accessor tests passed ---" << std::endl;
        //
        //    // Checkers
        //    should_check_if_vector_is_null();
        //    std::cout << "--- Checker tests passed ---" << std::endl;
        //
        //    // Methods
        //    should_fill_vector_with_value();
        //    should_calculate_l2_norm();
        //    should_normalize_vector_in_place();
        //    should_transpose_vector_in_place();
        //    should_return_transposed_copy();
        //    std::cout << "--- Method tests passed ---" << std::endl;
        //
        //    // Operators
        //    should_check_equality();
        //    should_perform_unary_minus();
        //    should_add_two_vectors();
        //    should_add_scalar_to_vector();
        //    should_subtract_two_vectors();
        //    should_subtract_scalar_from_vector();
        //    should_multiply_vector_by_scalar();
        //    should_calculate_dot_product();
        //    should_calculate_outer_product();
        //    should_multiply_row_vector_by_matrix();
        //    should_multiply_matrix_by_column_vector();
        //    should_multiply_row_vector_by_matrix_mixed_types();
        //    should_multiply_matrix_by_column_vector_mixed_types();
        //    std::cout << "--- Operator tests passed ---" << std::endl;

        std::cout << "=== All Vector tests passed ===" << std::endl;
        return 0;
    }
};
