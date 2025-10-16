#include "MafLib/main/GlobalHeader.hpp"
#include "MafLib/structs/strings/Trie.hpp"

void should_return_true_if_inserted_word_exists() {
    maf::Trie<char> trie;
    trie.insert("hello");
    assert(trie.search("hello") == true);
}

void should_return_false_if_no_word_exists() {
    maf::Trie<char> trie;
    assert(trie.search("missing") == false);
}

void should_return_false_if_prefix_inserted_but_not_full_word() {
    maf::Trie<char> trie;
    trie.insert("catastrophe");
    assert(trie.search("cat") == false);
}

void should_return_true_for_multiple_inserted_words() {
    maf::Trie<char> trie;
    trie.insert("dog");
    trie.insert("door");
    trie.insert("dorm");
    assert(trie.search("dog") == true);
    assert(trie.search("door") == true);
    assert(trie.search("dorm") == true);
    assert(trie.search("do") == false);
}

void should_not_crash_or_throw_if_inserting_empty_string() {
    maf::Trie<char> trie;
    trie.insert("");
    // Depending on design, empty string may or may not exist
    // We simply assert that search doesn't crash and returns false
    assert(trie.search("") == false);
}

// Remove behavior
void should_remove_existing_word_and_return_true() {
    maf::Trie<char> trie;
    trie.insert("data");
    assert(trie.search("data") == true);
    assert(trie.remove("data") == true);
    assert(trie.search("data") == false);
}

void should_return_false_if_removing_nonexistent_word() {
    maf::Trie<char> trie;
    assert(trie.remove("ghost") == false);
}

void should_not_affect_other_words_when_removing_one() {
    maf::Trie<char> trie;
    trie.insert("apple");
    trie.insert("app");
    trie.insert("ape");

    trie.remove("app");
    assert(trie.search("apple") == true);
    assert(trie.search("ape") == true);
    assert(trie.search("app") == false);
}

void should_handle_removal_until_trie_becomes_empty() {
    maf::Trie<char> trie;
    trie.insert("a");
    trie.insert("b");
    trie.insert("c");
    assert(trie.remove("a") == true);
    assert(trie.remove("b") == true);
    assert(trie.remove("c") == true);
    assert(trie.search("a") == false);
    assert(trie.search("b") == false);
    assert(trie.search("c") == false);
}

// Edge and stress cases
void should_support_long_word_insertion_and_search() {
    maf::Trie<char> trie;
    std::string long_word(1000, 'x');
    trie.insert(long_word);
    assert(trie.search(long_word) == true);
}

void should_support_words_with_special_characters() {
    maf::Trie<char> trie;
    trie.insert("C++");
    trie.insert("Hello-World!");
    trie.insert("foo_bar123");
    assert(trie.search("C++"));
    assert(trie.search("Hello-World!"));
    assert(trie.search("foo_bar123"));
    assert(!trie.search("Hello"));
}

void should_return_false_if_word_removed_and_reinserted_partially() {
    maf::Trie<char> trie;
    trie.insert("code");
    assert(trie.search("code"));
    trie.remove("code");
    trie.insert("cod"); // prefix only
    assert(!trie.search("code"));
    assert(!trie.search("co"));
    assert(trie.search("cod"));
}

void should_allow_reinsertion_after_removal() {
    maf::Trie<char> trie;
    trie.insert("test");
    trie.remove("test");
    trie.insert("test");
    assert(trie.search("test"));
}

// ---------------------------
// Main test runner
// ---------------------------
int main() {
    std::cout << "=== Running maf::Trie Tests ===" << std::endl;

    run_test("should_return_true_if_inserted_word_exists",
             should_return_true_if_inserted_word_exists);
    run_test("should_return_false_if_no_word_exists",
             should_return_false_if_no_word_exists);
    run_test("should_return_false_if_prefix_inserted_but_not_full_word",
             should_return_false_if_prefix_inserted_but_not_full_word);
    run_test("should_return_true_for_multiple_inserted_words",
             should_return_true_for_multiple_inserted_words);
    run_test("should_not_crash_or_throw_if_inserting_empty_string",
             should_not_crash_or_throw_if_inserting_empty_string);

    run_test("should_remove_existing_word_and_return_true",
             should_remove_existing_word_and_return_true);
    run_test("should_return_false_if_removing_nonexistent_word",
             should_return_false_if_removing_nonexistent_word);
    run_test("should_not_affect_other_words_when_removing_one",
             should_not_affect_other_words_when_removing_one);
    run_test("should_handle_removal_until_trie_becomes_empty",
             should_handle_removal_until_trie_becomes_empty);

    run_test("should_support_long_word_insertion_and_search",
             should_support_long_word_insertion_and_search);
    run_test("should_support_words_with_special_characters",
             should_support_words_with_special_characters);
    run_test("should_return_false_if_word_removed_and_reinserted_partially",
             should_return_false_if_word_removed_and_reinserted_partially);
    run_test("should_allow_reinsertion_after_removal",
             should_allow_reinsertion_after_removal);

    std::cout << "=== All tests passed ===" << std::endl;
    return 0;
}
