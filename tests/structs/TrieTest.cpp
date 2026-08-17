#include "MafLib/main/GlobalHeader.hpp"
#include "MafLib/structs/strings/Trie.hpp"

using namespace maf;

void should_return_true_if_inserted_word_exists() {
    Trie trie;
    trie.insert("hello");
    assert(trie.search("hello") == true);
}

void should_return_false_if_no_word_exists() {
    Trie trie;
    assert(trie.search("missing") == false);
}

void should_return_false_if_prefix_inserted_but_not_full_word() {
    Trie trie;
    trie.insert("catastrophe");
    assert(trie.search("cat") == false);
}

void should_return_true_for_multiple_inserted_words() {
    Trie trie;
    trie.insert("dog");
    trie.insert("door");
    trie.insert("dorm");
    assert(trie.search("dog") == true);
    assert(trie.search("door") == true);
    assert(trie.search("dorm") == true);
    assert(trie.search("do") == false);
}

void should_not_crash_or_throw_if_inserting_empty_string() {
    Trie trie;
    trie.insert("");
    assert(trie.search("") == true);
}

void should_remove_existing_word_and_return_true_if_word_exists() {
    Trie trie;
    trie.insert("data");
    assert(trie.search("data") == true);
    assert(trie.remove("data") == true);
    assert(trie.search("data") == false);
}

void should_return_false_if_removing_nonexistent_word() {
    Trie trie;
    assert(trie.remove("ghost") == false);
}

void should_not_affect_other_words_when_removing_one() {
    Trie trie;
    trie.insert("apple");
    trie.insert("app");
    trie.insert("ape");

    trie.remove("app");
    assert(trie.search("apple") == true);
    assert(trie.search("ape") == true);
    assert(trie.search("app") == false);
}

void should_handle_removal_until_trie_becomes_empty() {
    Trie trie;
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

void should_support_long_word_insertion_and_search() {
    Trie trie;
    std::string long_word(1000, 'x');
    trie.insert(long_word);
    assert(trie.search(long_word) == true);
}

void should_support_words_with_special_characters() {
    Trie trie;
    trie.insert("C++");
    trie.insert("Hello-World!");
    trie.insert("foo_bar123");
    assert(trie.search("C++"));
    assert(trie.search("Hello-World!"));
    assert(trie.search("foo_bar123"));
    assert(!trie.search("Hello"));
}

void should_return_false_if_word_removed_and_reinserted_partially() {
    Trie trie;
    trie.insert("code");
    assert(trie.search("code"));
    trie.remove("code");
    trie.insert("cod");
    assert(!trie.search("code"));
    assert(!trie.search("co"));
    assert(trie.search("cod"));
}

void should_allow_reinsertion_after_removal() {
    Trie trie;
    trie.insert("test");
    trie.remove("test");
    trie.insert("test");
    assert(trie.search("test"));
}

int main() {
    std::cout << "=== Running Trie tests ===" << std::endl;

    should_return_true_if_inserted_word_exists();
    should_return_false_if_no_word_exists();
    should_return_false_if_prefix_inserted_but_not_full_word();
    should_return_true_for_multiple_inserted_words();
    should_not_crash_or_throw_if_inserting_empty_string();

    should_remove_existing_word_and_return_true_if_word_exists();
    should_return_false_if_removing_nonexistent_word();
    should_not_affect_other_words_when_removing_one();
    should_handle_removal_until_trie_becomes_empty();

    should_support_long_word_insertion_and_search();
    should_support_words_with_special_characters();
    should_return_false_if_word_removed_and_reinserted_partially();
    should_allow_reinsertion_after_removal();

    std::cout << "=== All Trie tests passed ===" << std::endl;
    return 0;
}
