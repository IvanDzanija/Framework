#include "MafLib/main/GlobalHeader.hpp"

namespace maf {

template <typename T> class DisjointSets {
  private:
    std::vector<T> _objects;
    std::vector<size_t> _links;
    std::vector<size_t> _sizes;

    void _ensure_exists(const T &x) {
        if (!_links.contains(x)) {
            _links.insert(std::make_pair(x, x));
            _links.at(x) = x;
            _sizes.at(x) = 1;
        }
    }

  public:
    /// Adds another set with given value.
    void make_set(const T &x) { _ensure_exists(x); }

    /// Unites the sets of the 2 elements if not part of the same set.
    void unite(const T &a, const T &b) {
        const T &_repr_a = find(a);
        const T &_repr_b = find(b);
        if (_repr_a == _repr_b) {
            return;
        }

        if (_sizes.at(_repr_a) < _sizes.at(b)) {
            _sizes.at(_repr_b) += _sizes.at(_repr_a);
            _links.at(_repr_a) = _repr_b;
        } else {
            _sizes.at(_repr_a) += _sizes.at(_repr_b);
            _links.at(_repr_b) = _repr_a;
        }
    }

    /// Checks if 2 elements are part of the same set.
    [[nodiscard]] bool united(const T &a, const T &b) const {
        return find(a) == find(b);
    }

    /// Finds the representative element of the set.
    /// @return Reference to the repr.
    [[nodiscard]] const T &find(T a) const {
        _ensure_exists(a);
        while (a != _links.at(a)) {
            a = _links.at(a);
        }
        return a;
    }

    /// Counts distinct sets.
    [[nodiscard]] size_t count_sets() const {
        size_t res = 0;
        for (const auto &[x, p] : _links) {
            if (x == p) {
                ++res;
            }
        }
        return res;
    }

    /// Returns the size of the set containing x.
    [[nodiscard]] size_t set_size(const T &x) {
        const T &_repr_x = find(x);
        return _sizes.at(_repr_x);
    }
};
} // namespace maf
