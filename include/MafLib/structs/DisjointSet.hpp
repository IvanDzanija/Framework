#include "MafLib/main/GlobalHeader.hpp"

namespace maf {
/**
 * @brief Implements a Disjoint Set Union (DSU) data structure.
 *
 * This class tracks a collection of elements partitioned into a number of
 * disjoint (non-overlapping) sets. It is also known as a Union-Find
 * data structure.
 *
 * This implementation acts as a container, "owning" the elements added to it.
 * It provides efficient operations to add new sets, merge existing sets,
 * and determine if two elements are in the same set.
 *
 * Operations are performed using lightweight `size_t` handles, which are
 * returned when elements are added. This avoids copying or hashing the
 * elements `T` for DSU operations, making this implementation very
 * efficient for large or complex element types.
 *
 * It uses two optimizations for amortized O(α(n)) time complexity:
 * 1. Union by Size: Merges the smaller set into the larger one.
 * 2. Path Compression: Flattens the tree structure during `find`
 * operations.
 *
 * More information:
 * https://cp-algorithms.com/data_structures/disjoint_set_union.html
 *
 * @tparam T The type of the elements to be stored.
 *
 * @version 2.0 (Handle-based, Union-by-Size, Path Compression)
 * @since 2025
 */
template <typename T> class DisjointSet {
  private:
    std::vector<T> _objects;
    std::vector<size_t> _links; // _links[i] = parent of element i
    std::vector<size_t> _sizes; // _sizes[i] = size of set represented by i

    /**
     * @brief Checks if a handle is valid.
     * @throws std::out_of_range if the handle is out of bounds.
     */
    void _validate_handle(size_t handle) const {
        if (handle >= _links.size()) {
            throw std::out_of_range("Invalid handle provided to DisjointSets.");
        }
    }

  public:
    /**
     * @brief Creates a new singleton set containing a copy of the given
     * element.
     * @param x The element to add.
     * @return A `size_t` handle for the newly added element. This
     * handle is used for all other DSU operations.
     */
    size_t add_set(const T &x) {
        size_t new_id = _objects.size();
        _objects.push_back(x);
        _links.push_back(new_id);
        _sizes.push_back(1);
        return new_id;
    }

    /**
     * @brief Creates a new singleton set by moving the given element.
     * @param x The element to add, moved via `std::move`.
     * @return A `size_t` handle for the newly added element. This
     * handle is used for all other DSU operations.
     */
    size_t add_set(T &&x) {
        size_t new_id = _objects.size();
        _objects.push_back(std::move(x));
        _links.push_back(new_id);
        _sizes.push_back(1);
        return new_id;
    }

    /**
     * @brief Finds the representative handle of the set containing `a`.
     * @details Traverses the parent links from `a` up to the root.
     * Implements path compression (two-pass iterative) for
     * optimization.
     *
     * @param a The handle of the element to find.
     * @return The `size_t` handle of the representative element for the
     * set.
     * @throws std::out_of_range if `a` is an invalid handle.
     */
    [[nodiscard]] size_t find(size_t a) {
        _validate_handle(a);

        // 1. Find the root
        size_t root = a;
        while (root != _links[root]) {
            root = _links[root];
        }

        // 2. Path compression
        while (a != root) {
            size_t next = _links[a];
            _links[a] = root;
            a = next;
        }

        return root;
    }

    /**
     * @brief Merges the two sets containing elements `a` and `b`.
     * @details Finds the representatives of the sets for `a` and `b`. If they
     * are not already in the same set, the smaller set is merged
     * into the larger set (union by size heuristic).
     *
     * @param a Handle of an element from the first set to unite.
     * @param b Handle of an element from the second set to unite.
     * @throws std::out_of_range if `a` or `b` are invalid handles.
     */
    void unite(size_t a, size_t b) {
        size_t repr_a = find(a);
        size_t repr_b = find(b);

        if (repr_a == repr_b) {
            return;
        }

        if (_sizes[repr_a] < _sizes[repr_b]) {
            _sizes[repr_b] += _sizes[repr_a];
            _links[repr_a] = repr_b;
        } else {
            _sizes[repr_a] += _sizes[repr_b];
            _links[repr_b] = repr_a;
        }
    }

    /**
     * @brief Checks if two elements are in the same set.
     * @param a Handle of the first element.
     * @param b Handle of the second element.
     * @return `true` if `a` and `b` belong to the same set,
     * `false` otherwise.
     * @throws std::out_of_range if `a` or `b` are invalid handles.
     */
    [[nodiscard]] bool united(size_t a, size_t b) { return find(a) == find(b); }

    /**
     * @brief Counts the total number of distinct sets.
     * @details Iterates through all elements and counts how many
     * are their own representative (i.e., root nodes).
     * @return The total number of disjoint sets.
     */
    [[nodiscard]] size_t count_sets() const {
        size_t res = 0;
        for (size_t i = 0; i < _links.size(); ++i) {
            if (_links[i] == i) {
                ++res;
            }
        }
        return res;
    }

    /**
     * @brief Returns the size (number of elements) of the set containing `x`.
     * @param x The handle of the element to query.
     * @return The total number of elements in the set.
     * @throws std::out_of_range if `x` is an invalid handle.
     */
    [[nodiscard]] size_t set_size(size_t x) {
        size_t repr_x = find(x);
        return _sizes.at(repr_x);
    }

    /**
     * @brief Retrieves a mutable reference to an element by its handle.
     * @param handle The handle of the element to retrieve.
     * @return A mutable reference (`T&`) to the stored element.
     * @throws std::out_of_range if `handle` is invalid.
     */
    [[nodiscard]] T &get(size_t handle) {
        _validate_handle(handle);
        return _objects.at(handle);
    }

    /**
     * @brief Retrieves a const reference to an element by its handle.
     * @param handle The handle of the element to retrieve.
     * @return A const reference (`const T&`) to the stored element.
     * @throws std::out_of_range if `handle` is invalid.
     */
    [[nodiscard]] const T &get(size_t handle) const {
        _validate_handle(handle);
        return _objects.at(handle);
    }

    /**
     * @brief Retrieves a mutable reference to the representative element
     * of the set containing `handle`.
     * @param handle The handle of an element in the set.
     * @return A mutable reference (`T&`) to the representative
     * element.
     * @throws std::out_of_range if `handle` is invalid.
     */
    [[nodiscard]] T &get_representative(size_t handle) {
        size_t repr_handle = find(handle);
        return _objects[repr_handle];
    }
};

} // namespace maf
