#include "MafLib/main/GlobalHeader.hpp"
#include "MafLib/structs/DisjointSet.hpp"
using namespace maf;

struct MoveOnlyType {
    int id;
    std::unique_ptr<int> ptr;

    explicit MoveOnlyType(int i) : id(i), ptr(std::make_unique<int>(i)) {}

    MoveOnlyType(const MoveOnlyType &) = delete;
    MoveOnlyType &operator=(const MoveOnlyType &) = delete;

    MoveOnlyType(MoveOnlyType &&) = default;
    MoveOnlyType &operator=(MoveOnlyType &&) = default;
};

void should_be_empty_on_construction() {
    DisjointSet<int> dsu;
    assert(dsu.count_sets() == 0);
}

void should_add_new_sets_with_lvalues() {
    DisjointSet<std::string> dsu;
    std::string s1 = "hello";
    std::string s2 = "world";

    size_t h1 = dsu.add_set(s1);
    size_t h2 = dsu.add_set(s2);

    assert(h1 == 0);
    assert(h2 == 1);
    assert(dsu.count_sets() == 2);
    assert(dsu.get(h1) == "hello");
    assert(dsu.get(h2) == "world");
    assert(s1 == "hello");
}

void should_add_new_sets_with_rvalues() {
    DisjointSet<std::string> dsu;
    size_t h1 = dsu.add_set("hello");
    size_t h2 = dsu.add_set(std::string("world"));

    assert(h1 == 0);
    assert(h2 == 1);
    assert(dsu.count_sets() == 2);
    assert(dsu.get(h1) == "hello");
    assert(dsu.get(h2) == "world");
}

void should_add_move_only_types() {
    DisjointSet<MoveOnlyType> dsu;
    MoveOnlyType m1(100);

    size_t h1 = dsu.add_set(std::move(m1));
    size_t h2 = dsu.add_set(MoveOnlyType(200));

    assert(h1 == 0);
    assert(h2 == 1);
    assert(dsu.count_sets() == 2);
    assert(dsu.get(h1).id == 100);
    assert(*dsu.get(h1).ptr == 100);
    assert(dsu.get(h2).id == 200);
    assert(m1.ptr == nullptr);
}

void should_find_representative_of_singleton_set() {
    DisjointSet<int> dsu;
    size_t h1 = dsu.add_set(10);
    assert(dsu.find(h1) == h1);
    assert(dsu.united(h1, h1));
}

void should_unite_two_disjoint_sets() {
    DisjointSet<int> dsu;
    size_t h1 = dsu.add_set(10);
    size_t h2 = dsu.add_set(20);

    assert(dsu.count_sets() == 2);
    assert(!dsu.united(h1, h2));

    dsu.unite(h1, h2);

    assert(dsu.count_sets() == 1);
    assert(dsu.united(h1, h2));
    assert(dsu.find(h1) == dsu.find(h2));
}

void should_handle_uniting_already_united_sets() {
    DisjointSet<int> dsu;
    size_t h1 = dsu.add_set(10);
    size_t h2 = dsu.add_set(20);
    dsu.unite(h1, h2);

    assert(dsu.count_sets() == 1);
    size_t root1 = dsu.find(h1);

    // Root shouldn't change
    dsu.unite(h1, h2);
    assert(dsu.count_sets() == 1);
    assert(dsu.find(h1) == root1);

    // Root shouldn't change
    dsu.unite(h2, h1);
    assert(dsu.count_sets() == 1);
    assert(dsu.find(h1) == root1);
}

void should_correctly_track_set_size_after_unions() {
    DisjointSet<int> dsu;
    size_t h1 = dsu.add_set(10);
    size_t h2 = dsu.add_set(20);
    size_t h3 = dsu.add_set(30);

    assert(dsu.set_size(h1) == 1);
    assert(dsu.set_size(h2) == 1);
    assert(dsu.set_size(h3) == 1);

    dsu.unite(h1, h2);
    assert(dsu.set_size(h1) == 2);
    assert(dsu.set_size(h2) == 2);
    assert(dsu.set_size(h3) == 1);

    dsu.unite(h3, h2);
    assert(dsu.set_size(h1) == 3);
    assert(dsu.set_size(h2) == 3);
    assert(dsu.set_size(h3) == 3);
}

void should_follow_union_by_size_heuristic() {
    DisjointSet<int> dsu;
    size_t h1 = dsu.add_set(10);
    size_t h2 = dsu.add_set(20);
    size_t h3 = dsu.add_set(30);

    dsu.unite(h1, h2);
    assert(dsu.find(h1) == h1);
    assert(dsu.find(h2) == h1);
    assert(dsu.set_size(h1) == 2);

    dsu.unite(h3, h1);
    assert(dsu.find(h3) == h1);
    assert(dsu.find(h1) == h1);
    assert(dsu.set_size(h1) == 3);

    size_t h4 = dsu.add_set(40);
    size_t h5 = dsu.add_set(50);
    dsu.unite(h4, h5);
    assert(dsu.find(h5) == h4);

    dsu.unite(h1, h4);
    assert(dsu.find(h4) == h1);
    assert(dsu.find(h5) == h1);
    assert(dsu.set_size(h1) == 5);
    assert(dsu.set_size(h5) == 5);
}

void should_handle_complex_unions_and_finds() {
    const int n = 10;
    DisjointSet<int> dsu;
    std::vector<size_t> handles(n);

    for (int i = 0; i < n; ++i) {
        handles[i] = dsu.add_set(i);
    }
    assert(dsu.count_sets() == n);

    dsu.unite(handles[0], handles[1]);
    dsu.unite(handles[1], handles[2]);
    dsu.unite(handles[3], handles[4]);
    dsu.unite(handles[4], handles[5]);
    assert(dsu.count_sets() == n - 4);
    assert(dsu.united(handles[0], handles[2]));
    assert(dsu.united(handles[3], handles[5]));
    assert(!dsu.united(handles[0], handles[5]));

    dsu.unite(handles[0], handles[5]);
    assert(dsu.count_sets() == n - 5);
    assert(dsu.united(handles[2], handles[4]));
    assert(dsu.set_size(handles[0]) == 6);

    assert(dsu.united(handles[0], handles[1]));
    assert(dsu.united(handles[0], handles[2]));
    assert(dsu.united(handles[0], handles[3]));
    assert(dsu.united(handles[0], handles[4]));
    assert(dsu.united(handles[0], handles[5]));

    assert(!dsu.united(handles[0], handles[6]));
}

void should_get_and_modify_element_by_handle() {
    DisjointSet<std::string> dsu;
    size_t h1 = dsu.add_set("hello");
    assert(dsu.get(h1) == "hello");

    dsu.get(h1) = "world";
    assert(dsu.get(h1) == "world");
}

void should_get_and_modify_representative_element() {
    DisjointSet<std::string> dsu;
    size_t h_alice = dsu.add_set("Alice");
    size_t h_bob = dsu.add_set("Bob");
    size_t h_char = dsu.add_set("Charlie");

    dsu.unite(h_alice, h_bob);
    assert(dsu.find(h_bob) == h_alice);
    assert(dsu.get_representative(h_alice) == "Alice");
    assert(dsu.get_representative(h_bob) == "Alice");

    dsu.unite(h_char, h_bob);
    assert(dsu.find(h_char) == h_alice);
    assert(dsu.get_representative(h_char) == "Alice");

    dsu.get_representative(h_bob) = "Queen Alice";

    assert(dsu.get(h_alice) == "Queen Alice");
    assert(dsu.get(h_bob) == "Bob");
    assert(dsu.get(h_char) == "Charlie");
    assert(dsu.get_representative(h_char) == "Queen Alice");
}

void should_throw_on_invalid_handle_find() {
    DisjointSet<int> dsu;
    dsu.add_set(10);
    bool thrown = false;
    try {
        dsu.find(1);
    } catch (const std::out_of_range &e) {
        thrown = true;
    }
    assert(thrown);
}

void should_throw_on_invalid_handle_unite() {
    DisjointSet<int> dsu;
    size_t h1 = dsu.add_set(10);

    bool thrown_a = false;
    try {
        dsu.unite(h1, 100);
    } catch (const std::out_of_range &e) {
        thrown_a = true;
    }
    assert(thrown_a);

    bool thrown_b = false;
    try {
        dsu.unite(100, h1);
    } catch (const std::out_of_range &e) {
        thrown_b = true;
    }
    assert(thrown_b);
}

void should_throw_on_invalid_handle_united() {
    DisjointSet<int> dsu;
    size_t h1 = dsu.add_set(10);
    bool thrown = false;
    try {
        dsu.united(h1, 1);
    } catch (const std::out_of_range &e) {
        thrown = true;
    }
    assert(thrown);
}

void should_throw_on_invalid_handle_set_size() {
    DisjointSet<int> dsu;
    dsu.add_set(10);
    bool thrown = false;
    try {
        dsu.set_size(99);
    } catch (const std::out_of_range &e) {
        thrown = true;
    }
    assert(thrown);
}

void should_throw_on_invalid_handle_get() {
    DisjointSet<int> dsu;
    dsu.add_set(10);
    bool thrown = false;
    try {
        dsu.get(1);
    } catch (const std::out_of_range &e) {
        thrown = true;
    }
    assert(thrown);
}

void should_throw_on_invalid_handle_get_representative() {
    DisjointSet<int> dsu;
    dsu.add_set(10);
    bool thrown = false;
    try {
        dsu.get_representative(1);
    } catch (const std::out_of_range &e) {
        thrown = true;
    }
    assert(thrown);
}

int main() {
    std::cout << "=== Running DisjointSets tests ===" << std::endl;

    should_be_empty_on_construction();
    should_add_new_sets_with_lvalues();
    should_add_new_sets_with_rvalues();
    should_add_move_only_types();
    should_find_representative_of_singleton_set();
    should_unite_two_disjoint_sets();
    should_handle_uniting_already_united_sets();
    should_correctly_track_set_size_after_unions();
    should_follow_union_by_size_heuristic();
    should_handle_complex_unions_and_finds();
    should_get_and_modify_element_by_handle();
    should_get_and_modify_representative_element();

    // Exception tests
    should_throw_on_invalid_handle_find();
    should_throw_on_invalid_handle_unite();
    should_throw_on_invalid_handle_united();
    should_throw_on_invalid_handle_set_size();
    should_throw_on_invalid_handle_get();
    should_throw_on_invalid_handle_get_representative();

    std::cout << "=== All DisjointSets tests passed ===" << std::endl;
    return 0;
}
